


#include "diag_common.h"
#include "diag_msgmsp.h"
#include "diag_msgbbp.h"
#include "diag_msgps.h"
#include "diag_cfg.h"
#include "msp_errno.h"
#include "diag_debug.h"
#include "diag_api.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define    THIS_FILE_ID        MSP_FILE_ID_DIAG_MSGMSP_C

typedef VOS_UINT32 (*DIAG_MSGMSP_PROC_FUNC)(VOS_UINT8* pstReq);

typedef struct
{
    VOS_UINT32              ulCmdId;
    DIAG_MSGMSP_PROC_FUNC   pfnProc;
}DIAG_MSGMSP_PROC_STRU;


DIAG_MSGMSP_PROC_STRU g_astMsgMsp[] =
{
    {DIAG_CMD_LOG_CAT_PRINT,        diag_PrintCfgProc},
    {DIAG_CMD_LOG_CAT_LAYER,        diag_LayerCfgProc},
    {DIAG_CMD_LOG_CAT_AIR,          diag_AirCfgProc},
    {DIAG_CMD_LOG_CAT_EVENT,        diag_EventCfgProc},
    {DIAG_CMD_LOG_CAT_MSG,          diag_MsgCfgProc},

    {DIAG_CMD_GTR_SET,              diag_GtrProcEntry},
    {DIAG_CMD_GU_GTR_SET,           diag_GuGtrProcEntry},

    {DIAG_CMD_HOST_DISCONNECT,      diag_DisConnProc},

#if (VOS_OS_VER == VOS_LINUX)
    {DIAG_CMD_HOST_CONNECT,         diag_ConnProc},
    {DIAG_CMD_HOST_CONNECT_AGENT,   diag_ConnProc},
    {DIAG_CMD_GET_TIMESTAMP_VALUE,  diag_GetTimeStampInitValue},
    {DIAG_CMD_GET_MODEM_NUM,        diag_GetModemNum},
    {DIAG_CMD_PID_TABLE_MSG,        diag_GetPidTable},
#endif

};

VOS_UINT32 diag_MspMsgProc(DIAG_FRAME_INFO_STRU *pData);

#if (VOS_OS_VER == VOS_LINUX)
/*****************************************************************************
 Function Name   : diag_MspMsgInit
 Description     : MSP诊断命令初始化接口

 History         :
    1.c64416         2014-11-18  Draft Enact

*****************************************************************************/
void diag_MspMsgInit(void)
{
    VOS_UINT32 ulRet;

    ulRet = DIAG_MsgProcReg(DIAG_MSG_TYPE_MSP, diag_MspMsgProc);
    if(VOS_OK != ulRet)
    {
        diag_printf("diag_MspMsgInit DIAG_MsgProcReg failed.\n");
    }
}

/*****************************************************************************
 Function Name   : diag_GetModemNum
 Description     : 获取modem num
 Input           : pstReq 待处理数据
 Output          : None
 Return          : VOS_UINT32

    1.c00326366      2012-11-22  Draft Enact
*****************************************************************************/
VOS_UINT32 diag_GetModemNum(VOS_UINT8* pstReq)
{
    VOS_UINT ret = ERR_MSP_SUCCESS;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = {0};
    DIAG_CMD_GET_MODEM_NUM_CNF_STRU stModemNum = {0};
    DIAG_FRAME_INFO_STRU *pstDiagHead = NULL;

    pstDiagHead = (DIAG_FRAME_INFO_STRU*)(pstReq);

    DIAG_MSG_COMMON_PROC(stDiagInfo, stModemNum, pstDiagHead);

    stDiagInfo.ulMsgType = DIAG_MSG_TYPE_MSP;

#if ( FEATURE_MULTI_MODEM == FEATURE_ON )
#if (MULTI_MODEM_NUMBER == 3)
    stModemNum.ulNum = 3;
#else
    stModemNum.ulNum = 2;
#endif
#else
    stModemNum.ulNum = 1;
#endif

    stModemNum.ulRc  = ERR_MSP_SUCCESS;

    /*组包给FW回复*/
    ret = DIAG_MsgReport(&stDiagInfo, &stModemNum, sizeof(stModemNum));

    return (VOS_UINT32)ret;
}


/*****************************************************************************
 Function Name   : diag_GetPidTable
 Description     : 获取PID列表
 Input           : pstReq 待处理数据
 Output          : None
 Return          : VOS_UINT32

    1.c00326366      2016-02-14  Draft Enact
*****************************************************************************/
VOS_UINT32 diag_GetPidTable(VOS_UINT8* pstReq)
{
    VOS_UINT32 i, num, ulRc, len, ret;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = {0};
    DIAG_CMD_PID_TABLE_CNF_STRU *pstPidTable = VOS_NULL;
    DIAG_FRAME_INFO_STRU *pstDiagHead = VOS_NULL;
    VOS_UINT32 *pulPid = VOS_NULL;

    num = (VOS_CPU_ID_0_PID_BUTT - VOS_PID_CPU_ID_0_DOPRAEND) + (VOS_CPU_ID_1_PID_BUTT - VOS_PID_CPU_ID_1_DOPRAEND);

    pulPid = (VOS_UINT32 *)VOS_MemAlloc(DIAG_AGENT_PID, DYNAMIC_MEM_PT, (num * sizeof(VOS_UINT32)));

    num  = 0;
    ulRc = ERR_MSP_SUCCESS;

    if(VOS_NULL == pulPid)
    {
        return ERR_MSP_FAILURE;
    }
    else
    {
        /* 获取A核PID table */
        for(i = 0; i < (VOS_CPU_ID_1_PID_BUTT - VOS_PID_CPU_ID_1_DOPRAEND); i++)
        {
            if(VOS_PID_AVAILABLE == VOS_CheckPidValidity(VOS_PID_CPU_ID_1_DOPRAEND + i))
            {
                pulPid[num++] = (VOS_PID_CPU_ID_1_DOPRAEND + i);
            }
        }

        /* 获取C核PID table */
        for(i = 0; i < (VOS_CPU_ID_0_PID_BUTT - VOS_PID_CPU_ID_0_DOPRAEND); i++)
        {
            if(VOS_PID_AVAILABLE == VOS_CheckPidValidity(VOS_PID_CPU_ID_0_DOPRAEND + i))
            {
                pulPid[num++] = (VOS_PID_CPU_ID_0_DOPRAEND + i);
            }
        }
    }

    len = sizeof(DIAG_CMD_PID_TABLE_CNF_STRU) + (num * sizeof(VOS_UINT32));
    pstPidTable = (DIAG_CMD_PID_TABLE_CNF_STRU *)VOS_MemAlloc(DIAG_AGENT_PID, DYNAMIC_MEM_PT, len);
    if(VOS_NULL == pstPidTable)
    {
        VOS_MemFree(DIAG_AGENT_PID, pulPid);
        return ERR_MSP_FAILURE;
    }

    pstDiagHead = (DIAG_FRAME_INFO_STRU*)(pstReq);

    DIAG_MSG_COMMON_PROC(stDiagInfo, (*pstPidTable), pstDiagHead);

    stDiagInfo.ulMsgType    = DIAG_MSG_TYPE_MSP;

    pstPidTable->ulRc       = ulRc;
    pstPidTable->ulPidNum   = num;

    VOS_MemCpy_s(pstPidTable->aulPid, (VOS_UINT32)(num*sizeof(VOS_UINT32)), pulPid, (VOS_UINT32)(num*sizeof(VOS_UINT32)));

    ret = DIAG_MsgReport(&stDiagInfo, pstPidTable, len);

    VOS_MemFree(DIAG_AGENT_PID, pulPid);
    VOS_MemFree(DIAG_AGENT_PID, pstPidTable);

    return (VOS_UINT32)ret;
}

#endif

#if ((VOS_OS_VER == VOS_RTOSCK) || (VOS_OS_VER == VOS_VXWORKS))
/*****************************************************************************
 Function Name   : diag_AppTransMspProc
 Description     : A核发过来的诊断命令处理
 Input           : pMsgBlock    诊断命令请求的内容
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.c64416      2014-11-18  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_AppTransMspProc(MsgBlock* pMsgBlock)
{
    DIAG_MSG_A_TRANS_C_STRU *pstInfo;

    pstInfo = (DIAG_MSG_A_TRANS_C_STRU *)pMsgBlock;

    return diag_MspMsgProc(&pstInfo->stInfo);
}

DIAG_GTR_DATA_RCV_PFN g_pfnDiagGtrCallBack = VOS_NULL;

/*****************************************************************************
 Function Name   : DIAG_GtrRcvCallBackReg
 Description     : GTR回调注册
 Input           :DIAG_GTR_DATA_RCV_PFN pfnGtrRcv
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.w00182550      2012-12-26  Draft Enact

*****************************************************************************/
VOS_VOID DIAG_GtrRcvCallBackReg(DIAG_GTR_DATA_RCV_PFN pfnGtrRcv)
{
    g_pfnDiagGtrCallBack = pfnGtrRcv;
}

#endif


/*****************************************************************************
 Function Name   : diag_GuGtrProcEntry
 Description     : GU的RTT测试命令处理，只需要透传，不需要回复
                    1. 原值透传，不改变senderpid
                    2. 由于是GU协议栈的任务，需要在C核去申请消息发送消息
                       (senderpid不能是A核的PID，否则申请消息会失败)
                    3. 不需要回复，GTR通过层间消息的勾包码流判断成功失败
                    4. GU的RTT测试不分正常版本和RTT版本(版本归一)

 History         :
    1.c00326366      2015-9-6  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_GuGtrProcEntry(VOS_UINT8* pstReq)
{
    VOS_UINT32 ulRet                    = ERR_MSP_SUCCESS;
    DIAG_FRAME_INFO_STRU *pstDiagHead   = NULL;

#if(VOS_OS_VER == VOS_LINUX)
    VOS_UINT32 ulLen;
    DIAG_MSG_A_TRANS_C_STRU *pstInfo;
#else
    DIAG_OSA_MSG_STRU *pstMsg           = NULL;
    DIAG_OSA_MSG_STRU *pstVosMsg        = NULL;
#endif

    pstDiagHead = (DIAG_FRAME_INFO_STRU*)(pstReq);

#if(VOS_OS_VER == VOS_LINUX)
    DIAG_MSG_ACORE_CFG_PROC(ulLen, pstDiagHead, pstInfo, ulRet);
    return ulRet;
#else

    /* coverity[Event self_assign] */
    pstMsg = pstMsg;
    /* coverity[self_assign] */
    pstVosMsg = pstVosMsg;


    pstMsg = (DIAG_OSA_MSG_STRU *)(pstDiagHead->aucData + sizeof(MSP_DIAG_DATA_REQ_STRU));

    pstVosMsg = (DIAG_OSA_MSG_STRU *)VOS_AllocMsg(pstMsg->ulSenderPid, pstMsg->ulLength);

    if (pstVosMsg != NULL)
    {
        pstVosMsg->ulReceiverPid  = pstMsg->ulReceiverPid;

        (VOS_VOID)VOS_MemCpy_s(&pstVosMsg->ulMsgId, pstMsg->ulLength, &pstMsg->ulMsgId, pstMsg->ulLength);

        ulRet = VOS_SendMsg(pstMsg->ulSenderPid, pstVosMsg);
        if (ulRet != VOS_OK)
        {
            diag_printf("diag_GuGtrProcEntry VOS_SendMsg failed!\n");
        }
    }

    return ulRet;
#endif

#if(VOS_OS_VER == VOS_LINUX)
DIAG_ERROR:
    /* 不需要回复失败 */
    return ERR_MSP_FAILURE;
#endif
}


/*****************************************************************************
 Function Name   : diag_GtrProcEntry
 Description     : GTR命令处理接口入口
                    此功能只在回片前RTT联调时使用，会单独编译RTT联调版本
                    由PS的打桩接口注册到MSP，MSP在从工具侧接收到GTR命令时回调PS
                    的打桩接口，PS通过邮箱把消息转发给DSP
                    PS接收到DSP的回复后，调用(DIAG_TransReport)接口把应答结果上
                    报到工具侧
                    GTR测试软件-->HIDS-->MSP-->PS-->DSP-->PS-->MSP-->HIDS-->GTR

 History         :
    1.w00182550      2012-12-26  Draft Enact
    2.c64416         2014-11-18  适配新的诊断架构

*****************************************************************************/
VOS_UINT32 diag_GtrProcEntry(VOS_UINT8* pstReq)
{
    DIAG_CMD_GTR_SET_CNF_STRU stGtrCnf = {0};
    VOS_UINT32 ret = ERR_MSP_SUCCESS;
    MSP_DIAG_CNF_INFO_STRU stDiagInfo = {0};
    DIAG_FRAME_INFO_STRU *pstDiagHead = NULL;

#if(VOS_OS_VER == VOS_LINUX)
    VOS_UINT32 ulLen;
    DIAG_MSG_A_TRANS_C_STRU *pstInfo;
#else
    DIAG_CMD_GTR_SET_REQ_STRU* pstGtrReq = NULL;
#endif

    pstDiagHead = (DIAG_FRAME_INFO_STRU*)(pstReq);

#if(VOS_OS_VER == VOS_LINUX)
    DIAG_MSG_ACORE_CFG_PROC(ulLen, pstDiagHead, pstInfo, ret);
    return ret;
    
DIAG_ERROR:
#else

    pstGtrReq = (DIAG_CMD_GTR_SET_REQ_STRU*)(pstReq + DIAG_MESSAGE_DATA_HEADER_LEN);

    if (pstGtrReq->ulGtrDtaSize <= DIAG_CMD_DATA_MAX_LEN)
    {
        if (g_pfnDiagGtrCallBack != NULL)
        {
            ret = g_pfnDiagGtrCallBack(pstGtrReq->ulGtrDtaSize, pstGtrReq->aucDta);
            
            DIAG_MSG_COMMON_PROC(stDiagInfo, stGtrCnf, pstDiagHead);
            
            stGtrCnf.ulRc = ret;
            
            return DIAG_MsgReport(&stDiagInfo, &stGtrCnf, sizeof(stGtrCnf));
        }
    }

#endif

    DIAG_MSG_COMMON_PROC(stDiagInfo, stGtrCnf, pstDiagHead);

    stDiagInfo.ulMsgType    = DIAG_MSG_TYPE_MSP;

    stGtrCnf.ulRc = ERR_HIDS_CORE_ERROR;

    ret = DIAG_MsgReport(&stDiagInfo, &stGtrCnf, sizeof(stGtrCnf));

    return ret;
}


/*****************************************************************************
 Function Name   : diag_MspMsgProc
 Description     : MSP的诊断命令处理
 Input           : pData    诊断命令请求的内容
 Output          : None
 Return          : VOS_UINT32

 History         :
    1.c64416      2014-11-18  Draft Enact

*****************************************************************************/
VOS_UINT32 diag_MspMsgProc(DIAG_FRAME_INFO_STRU *pData)
{
    VOS_UINT32 ret = ERR_MSP_FAILURE;
    VOS_UINT32 i;

    if(NULL == pData)
    {
        return ret;
    }

#if(VOS_OS_VER == VOS_LINUX)
    diag_PTR(EN_DIAG_PTR_MSGMSP_IN);
#endif

    if(DIAG_MSG_TYPE_MSP != pData->stID.pri4b)
    {
        return ret;
    }

    for(i = 0; i < sizeof(g_astMsgMsp)/sizeof(DIAG_MSGMSP_PROC_STRU); i++)
    {
        if(pData->ulCmdId == g_astMsgMsp[i].ulCmdId)
        {
            return g_astMsgMsp[i].pfnProc((VOS_UINT8*)pData);
        }
    }

    (VOS_VOID)diag_FailedCmdCnf(pData, ERR_MSP_DIAG_INVALID_CMD);

    return ERR_MSP_SUCCESS;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


