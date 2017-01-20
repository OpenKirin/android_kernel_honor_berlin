


/******************************************************************************
   1 头文件包含
******************************************************************************/
#include "PsLogFilter.h"
#include "msp_diag_comm.h"
#include "PsLib.h"

#ifdef  __cplusplus
    #if  __cplusplus
        extern "C" {
    #endif
#endif

#define    THIS_FILE_ID        PS_FILE_ID_LOG_FILTER_C

/******************************************************************************
   2 外部函数变量声明
******************************************************************************/
extern VOS_UINT32 GUNAS_OM_LayerMsgFilter(
    const VOS_VOID                     *pstMsg
);

extern VOS_UINT32 PAM_OM_LayerMsgFilter(
    const VOS_VOID                      *pMsg
);

extern VOS_UINT32 TLPS_OM_LayerMsgFilter(
    const VOS_VOID                      *pMsg
);

/******************************************************************************
   3 私有定义
******************************************************************************/


/******************************************************************************
   4 全局变量定义
******************************************************************************/
#if (OSA_CPU_CCPU == VOS_OSA_CPU)

PS_OM_LAYER_MSG_MATCH_CTRL_STRU         g_stLayerMsgMatchCtrl =
    {0, {VOS_NULL_PTR, VOS_NULL_PTR, VOS_NULL_PTR, VOS_NULL_PTR}};

#endif
/******************************************************************************
   5 函数实现
******************************************************************************/
/* C核过滤函数 */
#if (OSA_CPU_CCPU == VOS_OSA_CPU)
/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgMatch_Ccpu
 功能描述  : 层间消息替换接口
 输入参数  : VOS_VOID                           *pstMsg
 输出参数  : 无
 返 回 值  : VOS_VOID*
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年9月5日
    作    者   : z00311339
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID* PS_OM_LayerMsgMatch_Ccpu
(
    VOS_VOID                           *pMsg
)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT32                          ulEntryCnt;
    VOS_VOID                           *pResult = pMsg;
    PS_OM_LAYER_MSG_MATCH_CTRL_STRU    *pstMsgMatchCtrl;

    pstMsgMatchCtrl = &g_stLayerMsgMatchCtrl;
    ulEntryCnt      = PS_MIN((pstMsgMatchCtrl->ulRegCnt), PS_OM_LAYER_MSG_MATCH_ITEM_MAX_CNT);

    for (ulIndex = 0; ulIndex < ulEntryCnt; ulIndex++)
    {
        if (VOS_NULL != pstMsgMatchCtrl->apfuncMatchEntry[ulIndex])
        {
            pResult = pstMsgMatchCtrl->apfuncMatchEntry[ulIndex]((MsgBlock*)pMsg);
            if (pMsg != pResult)
            {
                break;
            }
        }
    }

    return pResult;
}

/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgMatchInit
 功能描述  : 初始化MsgMatch
 输入参数  : VOS_VOID
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年9月3日
    作    者   : z00311339
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PS_OM_LayerMsgMatchInit(VOS_VOID)
{
    (VOS_VOID)DIAG_TraceMatchFuncReg(PS_OM_LayerMsgMatch_Ccpu);

    return;
}

/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgMatchFuncReg_Ccpu
 功能描述  : 注册MsgMatch回调接口
 输入参数  : PS_OM_LAYER_MSG_MATCH_PFUNC          pFunc
             输入参数回调接口规则:
             1.如果注册回调内部没有对消息进行处理，则需要将入参指针返回，否则
               本模块不知道是否需要将此消息传递给下一个注册回调进行处理
             2.如果注册回调内部对消息进行了处理，则返回值能够实现两个功能:
               ①返回VOS_NULL，则将此消息进行完全过滤，不会再勾取出来
               ②返回与入参指针不同的另一个指针，则勾取的消息将会使用返回的指
                 针内容替换原消息的内容。另本模块不负责对替换的内存进行释放，
                 替换原消息使用的内存请各模块自行管理。
 输出参数  : 无
 返 回 值  : VOS_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年9月5日
    作    者   : z00311339
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PS_OM_LayerMsgMatchFuncReg
(
    PS_OM_LAYER_MSG_MATCH_PFUNC         pFunc
)
{
    PS_OM_LAYER_MSG_MATCH_CTRL_STRU    *pstMsgMatchCtrl;
    VOS_UINT32                          ulIndex     = 0;
    VOS_UINT32                          ulLoop      = 0;
    VOS_UINT32                          ulMaxLoop   = 0;

    pstMsgMatchCtrl = &g_stLayerMsgMatchCtrl;
    ulIndex = pstMsgMatchCtrl->ulRegCnt;

    ulMaxLoop   = PS_MIN(ulIndex, PS_OM_LAYER_MSG_MATCH_ITEM_MAX_CNT);

    for (ulLoop = 0; ulLoop < ulMaxLoop; ulLoop++)
    {
        if (pstMsgMatchCtrl->apfuncMatchEntry[ulLoop] == pFunc)
        {
            return VOS_OK;
        }
    }

    if ((VOS_NULL_PTR != pFunc) && (PS_OM_LAYER_MSG_MATCH_ITEM_MAX_CNT > ulIndex))
    {
        pstMsgMatchCtrl->ulRegCnt   += 1;
        pstMsgMatchCtrl->apfuncMatchEntry[ulIndex] = pFunc;

        return VOS_OK;
    }

    return VOS_ERR;
}

/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgFilter_Ccpu
 功能描述  : 用于向DIAG组件进行注册的公共的LOG过滤函数
             其他组件可将自己的过滤函数添加至本函数内，
             统一向DIAG组件进行注册
 输入参数  : const VOID *pMsg:消息指针
 输出参数  : 无
 返 回 值  : VOS_UINT32:
                返回VOS_TRUE:  表示该消息需要进行过滤
                返回VOS_FALSE: 表示该消息无需进行过滤
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2015年09月28日
    作    者   : h00313353
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 PS_OM_LayerMsgFilter_Ccpu(
    const VOS_VOID                      *pMsg
)
{
    /* GUNAS */
    if (VOS_TRUE == GUNAS_OM_LayerMsgFilter(pMsg))
    {
        return VOS_TRUE;
    }

    /* LNAS */
    if (VOS_TRUE == TLPS_OM_LayerMsgFilter(pMsg))
    {
        return VOS_TRUE;
    }

    if (VOS_TRUE == PAM_OM_LayerMsgFilter(pMsg))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
#endif

/* A核过滤函数 */
#if (OSA_CPU_ACPU == VOS_OSA_CPU)
/*****************************************************************************
 函 数 名  : PS_OM_LayerMsgFilter_Acpu
 功能描述  : 用于向DIAG组件进行注册的公共的LOG过滤函数
             其他组件可将自己的过滤函数添加至本函数内，
             统一向DIAG组件进行注册
             (本函数在A核任务中注册，用于过滤A核内部层间消息)
 输入参数  : const VOID *pMsg:消息指针
 输出参数  : 无
 返 回 值  : VOS_UINT32:
                返回VOS_TRUE:  表示该消息需要进行过滤
                返回VOS_FALSE: 表示该消息无需进行过滤
 调用函数  :
 被调函数  :
 修改历史      :
  1.日    期   : 2015年09月28日
    作    者   : h00313353
    修改内容   : 新生成函数
*****************************************************************************/
VOS_UINT32 PS_OM_LayerMsgFilter_Acpu(
    const VOS_VOID                      *pMsg
)
{
    /* GUNAS */
    if (VOS_TRUE == GUNAS_OM_LayerMsgFilter(pMsg))
    {
        return VOS_TRUE;
    }

    if (VOS_TRUE == PAM_OM_LayerMsgFilter(pMsg))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;
}
#endif

#ifdef  __cplusplus
    #if  __cplusplus
        }
    #endif
#endif
