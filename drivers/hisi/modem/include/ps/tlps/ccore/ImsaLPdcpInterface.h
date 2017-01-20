/******************************************************************************


        @(#)Copyright(C)2013,Hisilicon Co. LTD.

 ******************************************************************************
    File name   : ImsaLPdcpInterface.h
    Description : 内部模块间接口头文件
    History     :
      1.leixiantiao 00258641    2016-07-02  Draft Enact


******************************************************************************/

#ifndef __IMSALPDCPINTERFACE_H__
#define __IMSALPDCPINTERFACE_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/
#include    "vos.h"


/*****************************************************************************
  1.1 Cplusplus Announce
*****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*****************************************************************************
#pragma pack(*)    设置字节对齐方式
*****************************************************************************/
#if (VOS_OS_VER != VOS_WIN32)
#pragma pack(4)
#else
#pragma pack(push, 4)
#endif

/*****************************************************************************
  2 macro
*****************************************************************************/


#define IMSA_LPDCP_MSG_ID_HEADER        0
#define LPDCP_IMSA_MSG_ID_HEADER        10

#define IMSA_LPDCP_MAX_IMS_BEARER_NUM     (8)


/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

/*****************************************************************************
 枚举名    : IMSA_LPDCP_MSG_ID_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : IMSA与LPDCP之间的消息
*****************************************************************************/
enum    IMSA_LPDCP_MSG_ID_ENUM
{
    /* IMSA发给LPDCP的消息原语 */
    ID_IMSA_LPDCP_VOLTE_BEARER_NOTIFY           = 0x00 + IMSA_LPDCP_MSG_ID_HEADER,    /* _H2ASN_MsgChoice IMSA_LPDCP_VOLTE_BEARER_NOTIFY_STRU */
    ID_IMSA_LPDCP_CALL_STATUS_NOTIFY            = 0x01 + IMSA_LPDCP_MSG_ID_HEADER,    /* _H2ASN_MsgChoice IMSA_LPDCP_CALL_STATUS_NOTIFY_STRU */

    ID_IMSA_LPDCP_MSG_ID_ENUM_BUTT
};
typedef VOS_UINT32 IMSA_LPDCP_MSG_ID_ENUM_UINT32;


/*****************************************************************************
  4 Enum
*****************************************************************************/
/*****************************************************************************
 枚举名    : IMSA_LPDCP_IMS_BEARER_TYPE_ENUM
 枚举说明  : 承载类型枚举
*****************************************************************************/
enum IMSA_LPDCP_IMS_BEARER_TYPE_ENUM
{
    IMSA_LPDCP_IMS_BEARER_TYPE_SIGNAL           = 0x00, /* IMS信令承载 */
    IMSA_LPDCP_IMS_BEARER_TYPE_VOICE            = 0x01, /* IMS语音承载 */
    IMSA_LPDCP_IMS_BEARER_TYPE_VIDEO            = 0x02, /* IMS视频承载 */

    IMSA_LPDCP_IMS_BEARER_TYPE_BUTT             = 0xFF
};
typedef VOS_UINT8 IMSA_LPDCP_IMS_BEARER_TYPE_ENUM_UINT8;

/*****************************************************************************
 枚举名    : IMSA_LPDCP_CALL_STATUS_ENUM
 枚举说明  : 电话启停状态枚举
*****************************************************************************/
enum IMSA_LPDCP_CALL_STATUS_ENUM
{
    IMSA_LPDCP_CALL_STATUS_INIT                 = 0x00, /* 电话初始:起呼*/
    IMSA_LPDCP_CALL_STATUS_START                = 0x01, /* 电话启动:振铃*/
    IMSA_LPDCP_CALL_STATUS_ACTIVE               = 0x02, /* 电话接通 */
    IMSA_LPDCP_CALL_STATUS_END                  = 0x03, /* 电话停止 */

    IMSA_LPDCP_CALL_STATUS_BUTT
};
typedef VOS_UINT8 IMSA_LPDCP_CALL_STATUS_ENUM_UINT8;

/*****************************************************************************
  5 STRUCT
*****************************************************************************/
/*****************************************************************************
 结构名    : IMSA_LPDCP_IMS_BEARER_STRU
 结构说明  : IMS承载结构
*****************************************************************************/
typedef struct
{
    VOS_UINT8                               ucEpsbId;
    IMSA_LPDCP_IMS_BEARER_TYPE_ENUM_UINT8   enBearerType;
    VOS_UINT8                               aucReserved[2];
} IMSA_LPDCP_IMS_BEARER_STRU;

/*****************************************************************************
 结构名    : IMSA_LPDCP_IMS_BEARER_NOTIFY_STRU
 结构说明  : IMSA通知LPDCP承载信息结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                              ulMsgId;
    VOS_UINT32                              ulImsBearerNum;
    IMSA_LPDCP_IMS_BEARER_STRU              astImsBearerArray[IMSA_LPDCP_MAX_IMS_BEARER_NUM];
} IMSA_LPDCP_VOLTE_BEARER_NOTIFY_STRU;

/*****************************************************************************
 结构名    : IMSA_LPDCP_CALL_STATUS_NOTIFY_STRU
 结构说明  : IMSA通知LPDCP电话启停(启动时只在第一路电话时通知,结束时只在所有电话挂断时通知)
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER
    VOS_UINT32                              ulMsgId;
    IMSA_LPDCP_CALL_STATUS_ENUM_UINT8       enCallStatus;
    VOS_UINT8                               auRsv[3];
}IMSA_LPDCP_CALL_STATUS_NOTIFY_STRU;

typedef struct
{
    IMSA_LPDCP_MSG_ID_ENUM_UINT32         enMsgId;            /*_H2ASN_MsgChoice_Export IMSA_LPDCP_MSG_ID_ENUM_UINT32*/
    VOS_UINT8                             aucMsg[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          IMSA_LPDCP_MSG_ID_ENUM_UINT32
    ****************************************************************************/
}IMSA_LPDCP_INTERFACE_MSG_DATA;
/*_H2ASN_Length UINT32*/

typedef struct
{
    VOS_MSG_HEADER
    IMSA_LPDCP_INTERFACE_MSG_DATA         stMsgData;
} ImsaLPDCPInterface_MSG;



/*****************************************************************************
  6 UNION
*****************************************************************************/


/*****************************************************************************
  7 Extern Global Variable
*****************************************************************************/

/*****************************************************************************
  8 Fuction Extern
*****************************************************************************/


/*****************************************************************************
  9 OTHERS
*****************************************************************************/


#if (VOS_OS_VER != VOS_WIN32)
#pragma pack()
#else
#pragma pack(pop)
#endif




#ifdef __cplusplus
    #if __cplusplus
            }
    #endif
#endif

#endif /* end of ImsaLPDCPInterface.h */

