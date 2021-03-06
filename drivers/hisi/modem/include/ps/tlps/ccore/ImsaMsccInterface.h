/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : ImsaMsccInterface.h
  版 本 号   : 初稿
  作    者   : yanglinping y00245242
  生成日期   : 2015年1月22日
  功能描述   : IMSA和MSCC接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2015年1月22日
    作    者   : yanglinping y00245242
    修改内容   : 创建文件

******************************************************************************/


#ifndef __IMSAMSCCINTERFACE_H__
#define __IMSAMSCCINTERFACE_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/

#include "vos.h"

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
#define MSCC_IMSA_MAX_IMSI_LEN          (9)

/*****************************************************************************
  3 Massage Declare
*****************************************************************************/

/*****************************************************************************
 枚举名    : IMSA_CONTROL_CONN_MSG_ID_ENUM
 协议表格  :
 ASN.1描述 :
 枚举说明  : 内部消息ID定义
*****************************************************************************/
enum MSCC_IMSA_MSG_ID_ENUM
{
    /* MSCC发给IMSA的消息原语 */
    ID_MSCC_IMSA_START_REQ              = 0x00,                                 /* _H2ASN_MsgChoice MSCC_IMSA_START_REQ_STRU */
    ID_MSCC_IMSA_STOP_REQ               ,                                       /* _H2ASN_MsgChoice MSCC_IMSA_STOP_REQ_STRU */
    ID_MSCC_IMSA_CELLULAR_DEREG_REQ              ,                                       /* _H2ASN_MsgChoice MSCC_IMSA_DEREGISTER_REQ_STRU */
    ID_MSCC_IMSA_SERVICE_CHANGE_IND     ,                                       /* _H2ASN_MsgChoice MSCC_IMSA_SERVICE_CHANGE_IND_STRU */
    ID_MSCC_IMSA_CAMP_INFO_CHANGE_IND   ,                                       /* _H2ASN_MsgChoice MSCC_IMSA_CAMP_INFO_CHANGE_IND_STRU */
    ID_MSCC_IMSA_VOICE_DOMAIN_CHANGE_IND,                                       /* _H2ASN_MsgChoice MSCC_IMSA_VOICE_DOMAIN_CHANGE_IND_STRU */
    ID_MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ,                                        /* _H2ASN_MsgChoice MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ_STRU */

    ID_MSCC_IMSA_ROAM_IMS_SUPPORT_SET_REQ,                                      /* _H2ASN_MsgChoice MSCC_IMSA_ROAM_IMS_SUPPORT_SET_REQ_STRU */
    ID_MSCC_IMSA_AREA_LOST_IND,                                                 /* _H2ASN_MsgChoice MSCC_IMSA_AREA_LOST_IND_STRU */

    ID_MSCC_IMSA_CELLULAR_REG_REQ,                                                       /* _H2ASN_MsgChoice MSCC_IMSA_REG_REQ_STRU */

    /* IMSA发给MSCC的消息原语 */
    ID_IMSA_MSCC_START_CNF              = 0x40,                                 /* _H2ASN_MsgChoice IMSA_MSCC_START_CNF_STRU */
    ID_IMSA_MSCC_STOP_CNF               ,                                       /* _H2ASN_MsgChoice IMSA_MSCC_STOP_CNF_STRU */
    ID_IMSA_MSCC_CELLULAR_DEREG_CNF              ,                                       /* _H2ASN_MsgChoice IMSA_MSCC_DEREGISTER_CNF_STRU */
    ID_IMSA_MSCC_IMS_VOICE_CAP_NOTIFY   ,                                       /* _H2ASN_MsgChoice IMSA_MSCC_IMS_VOICE_CAP_NOTIFY_STRU */
    ID_IMSA_MSCC_IMS_DOMAIN_CFG_SET_CNF ,                                       /* _H2ASN_MsgChoice IMSA_MSCC_IMS_DOMAIN_CFG_SET_CNF_STRU */

    ID_IMSA_MSCC_ROAM_IMS_SUPPORT_SET_CNF,                                      /* _H2ASN_MsgChoice IMSA_MSCC_ROAM_IMS_SUPPORT_SET_CNF_STRU */

    ID_IMSA_MSCC_CELLULAR_REG_CNF,                                                       /* _H2ASN_MsgChoice MSCC_IMSA_REG_CNF_STRU */

    ID_IMSA_MSCC_MSG_ID_ENUM_BUTT
};
typedef VOS_UINT32 MSCC_IMSA_MSG_ID_ENUM_UINT32;


/*****************************************************************************
  4 Enum
*****************************************************************************/


enum MSCC_IMSA_IMS_VOPS_INDICATOR_ENUM
{
    MSCC_IMSA_IMS_VOPS_NOT_SUPPORT      =0,
    MSCC_IMSA_IMS_VOPS_SUPPORT,
    MSCC_IMSA_IMS_VOPS_BUTT
};
typedef VOS_UINT8 MSCC_IMSA_IMS_VOPS_INDICATOR_ENUM_UINT8;

enum MSCC_IMSA_EMS_INDICATOR_ENUM
{
    MSCC_IMSA_EMS_NOT_SUPPORT           =0,
    MSCC_IMSA_EMS_SUPPORT,
    MSCC_IMSA_EMS_BUTT
};
typedef VOS_UINT8 MSCC_IMSA_EMS_INDICATOR_ENUM_UINT8;

enum MSCC_IMSA_RAT_TYPE_ENUM
{
    MSCC_IMSA_RAT_TYPE_GSM              =0,
    MSCC_IMSA_RAT_TYPE_UTRAN,
    MSCC_IMSA_RAT_TYPE_LTE,
    MSCC_IMSA_RAT_TYPE_BUTT
};
typedef VOS_UINT8 MSCC_IMSA_RAT_TYPE_ENUM_UINT8;

enum MSCC_IMSA_SERVICE_STATUS_ENUM
{
    MSCC_IMSA_NORMAL_SERVICE            =0,
    MSCC_IMSA_LIMITED_SERVICE,
    MSCC_IMSA_NO_SERVICE,
    MSCC_IMSA_SERVICE_STATUS_BUTT
};
typedef VOS_UINT8 MSCC_IMSA_SERVICE_STATUS_ENUM_UINT8;

enum MSCC_IMSA_ACCESS_TYPE_ENUM
{
    MSCC_IMSA_ACCESS_TYPE_UTRAN_TDD,
    MSCC_IMSA_ACCESS_TYPE_UTRAN_FDD,
    MSCC_IMSA_ACCESS_TYPE_EUTRAN_TDD,
    MSCC_IMSA_ACCESS_TYPE_EUTRAN_FDD,
    MSCC_IMSA_ACCESS_TYPE_GERAN,

    MSCC_IMSA_ACCESS_TYPE_BUTT
};
typedef VOS_UINT8 MSCC_IMSA_ACCESS_TYPE_ENUM_UINT8;

enum MSCC_IMSA_IMS_VOICE_CAP_ENUM
{
    MSCC_IMSA_IMS_VOICE_CAP_UNAVAILABLE = 0,
    MSCC_IMSA_IMS_VOICE_CAP_AVAILABLE,

    MSCC_IMSA_IMS_VOICE_CAP_BUTT
};
typedef VOS_UINT8 MSCC_IMSA_IMS_VOICE_CAP_ENUM_UINT8;

/*****************************************************************************
 结构名    : MSCC_IMSA_VOICE_DOMAIN_ENUM_UINT32
 结构说明  : VOIDE DOMAIN类型
  1.日    期   : 2015年04月13日
    作    者   : f00179208
    修改内容   : 新建
 *****************************************************************************/
enum MSCC_IMSA_VOICE_DOMAIN_ENUM
{
    MSCC_IMSA_VOICE_DOMAIN_CS_ONLY            = 0,    /* CS voice only */
    MSCC_IMSA_VOICE_DOMAIN_IMS_PS_ONLY        = 1,    /* IMS PS voice only */
    MSCC_IMSA_VOICE_DOMAIN_CS_PREFERRED       = 2,    /* CS vocie preferred, IMS PS voice as secondary */
    MSCC_IMSA_VOICE_DOMAIN_IMS_PS_PREFERRED   = 3,    /* IMS PS voice preferred, CS vocie as secondary */

    MSCC_IMSA_VOICE_DOMAIN_BUTT
};
typedef VOS_UINT32  MSCC_IMSA_VOICE_DOMAIN_ENUM_UINT32;

/*****************************************************************************
 结构名    : MSCC_IMSA_IMS_DOMAIN_ENUM
 结构说明  : IMS域类型
  1.日    期   : 2015年10月07日
    作    者   : f00179208
    修改内容   : 新建
 *****************************************************************************/
enum MSCC_IMSA_IMS_DOMAIN_ENUM
{
    MSCC_IMSA_IMS_DOMAIN_WIFI_PREFEER           = 0x00,
    MSCC_IMSA_IMS_DOMAIN_CELLULAR_PREFEER       = 0x01,
    MSCC_IMSA_IMS_DOMAIN_WIFI_ONLY              = 0x02,
    MSCC_IMSA_IMS_DOMAIN_BUTT
};
typedef VOS_UINT32  MSCC_IMSA_IMS_DOMAIN_ENUM_UINT32;

/*****************************************************************************
 结构名    : IMSA_MSCC_SET_IMS_DOMAIN_RESULT_ENUM
 结构说明  : 设置IMS域的结果类型
  1.日    期   : 2015年10月07日
    作    者   : f00179208
    修改内容   : 新建
 *****************************************************************************/
enum IMSA_MSCC_SET_IMS_DOMAIN_RESULT_ENUM
{
    IMSA_MSCC_SET_IMS_DOMAIN_RESULT_SUCC        = 0x00,
    IMSA_MSCC_SET_IMS_DOMAIN_RESULT_FAIL        = 0x01,
    IMSA_MSCC_SET_IMS_DOMAIN_RESULT_BUTT
};
typedef VOS_UINT32  IMSA_MSCC_SET_IMS_DOMAIN_RESULT_ENUM_UINT32;

/*****************************************************************************
 结构名    : MSCC_IMSA_SET_ROAM_IMS_SUPPORT_ENUM_UINT32
 结构说明  : 漫游下IMS是否支持类型
  1.日    期   : 2016年3月9日
    作    者   : w00316404
    修改内容   : 新建
 *****************************************************************************/
enum MSCC_IMSA_SET_ROAM_IMS_SUPPORT_ENUM
{
    MSCC_IMSA_SET_ROAM_IMS_UNSUPPORT             = 0,
    MSCC_IMSA_SET_ROAM_IMS_SUPPORT               = 1,

    MSCC_IMSA_SET_ROAM_IMS_BUTT
};
typedef VOS_UINT32  MSCC_IMSA_SET_ROAM_IMS_SUPPORT_ENUM_UINT32;

/*****************************************************************************
 结构名    : IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_ENUM_UINT32
 结构说明  : 漫游下IMS是否支持设置请求返回结果
  1.日    期   : 2016年3月9日
    作    者   : w00316404
    修改内容   : 新建
 *****************************************************************************/
enum IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_ENUM
{
    IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_SUCCESS  = 0,
    IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_FAILED   = 1,

    IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_BUTT
};
typedef VOS_UINT32 IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_ENUM_UINT32;

/* Added by z00359541 for MSCC_IMSA_SERVICE_CHANGE_IND 接口调整, 2016-4-6, begin */
/*****************************************************************************
 结构名    : IMSA_MSCC_REG_SUCC_PROC_ENUM_UINT32
 结构说明  : 当前注册成功流程的类型
  1.日    期   : 2016年4月6日
    作    者   : z00359541
    修改内容   : 新建
 *****************************************************************************/
enum IMSA_MSCC_REG_SUCC_PROC_ENUM
{
    IMSA_MSCC_ATT_REAL_PROC        = 0,                     /* ATTACH真流程，与网侧发生交互 */
    IMSA_MSCC_TAU_REAL_PROC        = 1,                     /* TAU真流程，与网侧发生交互 */
    IMSA_MSCC_TAU_UNREAL_PROC      = 2,                     /* TAU假流程，没有与网侧发生交互 */
    IMSA_MSCC_REG_PROC_BUTT
};
typedef VOS_UINT32 IMSA_MSCC_REG_SUCC_PROC_ENUM_UINT32;
/* Added by z00359541 for MSCC_IMSA_SERVICE_CHANGE_IND 接口调整, 2016-4-6, end */

/*****************************************************************************
 结构名    : MSCC_IMSA_USIM_STATUS_ENUM_UINT32
 结构说明  : 卡状态信息结构体
  1.日    期   : 2016年8月18日
    作    者   : w00167002
    修改内容   : 卡存在状态信息
*****************************************************************************/
enum MSCC_IMSA_USIM_STATUS_ENUM
{
    MSCC_IMSA_USIM_VALID                  = 0x00000000,                         /* 有效卡 */
    MSCC_IMSA_USIM_INVALID,                                                     /* 无卡或卡无效 */

    MSCC_IMSA_USIM_STATUS_BUTT
};
typedef VOS_UINT32 MSCC_IMSA_USIM_STATUS_ENUM_UINT32;


/*****************************************************************************
  5 STRUCT
*****************************************************************************/
typedef struct
{
    /* 逻辑从右往左，ulMcc第0字节表示MCC digit1， ulMcc第1字节表示MCC digit2，
       ulMcc第2字节表示MCC digit3，ulMcc第3字节无效；
       逻辑从右往左，ulMnc第0字节表示MNC digit1， ulMnc第1字节表示MNC digit2，
       ulMnc第2字节表示MNC digit3，ulMnc第3字节无效；
       例如MCC+MNC为46001，则ulMcc为0x00000604,ulMnc为0x000f0100*/
    VOS_UINT32                          ulMcc;
    VOS_UINT32                          ulMnc;
}MSCC_IMSA_PLMN_ID_STRU;

/* Added by z00359541 for MSCC_IMSA_SERVICE_CHANGE_IND 接口调整, 2016-4-5, begin */
/*****************************************************************************
 结构名    : MSCC_IMSA_CAMPED_CELL_INFO_STRU
 结构说明  : 当前驻留小区信息的结构体
  1.日    期   : 2016年4月6日
    作    者   : z00359541
    修改内容   : 新建
*****************************************************************************/
typedef struct
{
    MSCC_IMSA_PLMN_ID_STRU              stPlmnId;
    VOS_UINT16                          usTac;
    VOS_UINT16                          usLac;
    VOS_UINT32                          ulCellId;
}MSCC_IMSA_CAMPED_CELL_INFO_STRU;
/* Added by z00359541 for MSCC_IMSA_SERVICE_CHANGE_IND 接口调整, 2016-4-5, end */



/*****************************************************************************
 结构名    : MSCC_IMSA_CARD_STATUS_INFO_STRU
 结构说明  : 卡状态信息结构体
  1.日    期   : 2016年8月12日
    作    者   : w00167002
    修改内容   : 增加卡状态信息通知
 *****************************************************************************/
typedef struct
{
    MSCC_IMSA_USIM_STATUS_ENUM_UINT32   ulUsimState;                            /* 卡是否存在状态 */
    VOS_UINT8                           ucCardChangedStatus;                    /* vos_False:表示卡存在且卡状态没有改变 */
    VOS_UINT8                           aucGuImsi[MSCC_IMSA_MAX_IMSI_LEN];      /* 卡存在时携带IMSI内容 */
    VOS_UINT8                           aucReserved2[2];
}MSCC_IMSA_CARD_STATUS_INFO_STRU;


/*****************************************************************************
 结构名    : MSCC_IMSA_START_REQ_STRU
 结构说明  : MSCC通知IMSA开机
  1.日    期   : 2016年8月12日
    作    者   : w00167002
    修改内容   : 增加卡状态信息通知
 *****************************************************************************/

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/

    MSCC_IMSA_CARD_STATUS_INFO_STRU     stCardStatusInfo;
}MSCC_IMSA_START_REQ_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT8                           aucReserve[4];
}IMSA_MSCC_START_CNF_STRU;


/*****************************************************************************
 结构名    : IMSA_CALL_RESULT_ACTION_IND_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : IMSA_CALL_RESULT_ACTION_IND_STRU消息结构定义
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
}MSCC_IMSA_STOP_REQ_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT8                           aucReserve[4];
}IMSA_MSCC_STOP_CNF_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT8                           aucReserve[4];
}MSCC_IMSA_DEREGISTER_REQ_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT8                           aucReserve[4];
}IMSA_MSCC_DEREGISTER_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT8                           aucReserve[4];
}MSCC_IMSA_REG_REQ_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT8                           aucReserve[4];
}IMSA_MSCC_REG_CNF_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32                            ulMsgId;            /*_H2ASN_Skip*/
    VOS_UINT32                                              bitRoamingValid : 1;
    /* Modified by z00359541 for MSCC_IMSA_SERVICE_CHANGE_IND 接口调整, 2016-4-6, begin */
    VOS_UINT32                                              bitRegSuccProcTypeValid : 1;
    VOS_UINT32                                              bitSpare        : 30;
    /* Modified by z00359541 for MSCC_IMSA_SERVICE_CHANGE_IND 接口调整, 2016-4-6, end */

    MSCC_IMSA_IMS_VOPS_INDICATOR_ENUM_UINT8                 enImsVoPsInd;
    MSCC_IMSA_EMS_INDICATOR_ENUM_UINT8                      enEmsInd;
    MSCC_IMSA_SERVICE_STATUS_ENUM_UINT8                     enPsServiceStatus;
    VOS_UINT8                                               ucPsSimValid;       /* VOS_TRUE :有效，VOS_FALSE :无效*/
    MSCC_IMSA_RAT_TYPE_ENUM_UINT8                           enRat;              /* 当前接入技术 */
    VOS_UINT8                                               ucRoamingFlg;       /* VOS_TRUE :漫游，VOS_FALSE :非漫游*/
    VOS_UINT8                                               aucReserve[2];
    /* Added by z00359541 for MSCC_IMSA_SERVICE_CHANGE_IND 接口调整, 2016-4-5, begin */
    MSCC_IMSA_CAMPED_CELL_INFO_STRU                         stCampedCellInfo;
    IMSA_MSCC_REG_SUCC_PROC_ENUM_UINT32                     enRegSuccProcType;
    /* Added by z00359541 for MSCC_IMSA_SERVICE_CHANGE_IND 接口调整, 2016-4-5, end */
}MSCC_IMSA_SERVICE_CHANGE_IND_STRU;


typedef struct
{
    VOS_MSG_HEADER                                                  /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;                    /*_H2ASN_Skip*/

    MSCC_IMSA_ACCESS_TYPE_ENUM_UINT8    enAccessType;
    VOS_UINT8                           aucReserved[3];
    VOS_UINT16                          usLac;
    VOS_UINT16                          usTac;
    MSCC_IMSA_PLMN_ID_STRU              stPlmnId;
    VOS_UINT32                          ulCellId;
}MSCC_IMSA_CAMP_INFO_CHANGE_IND_STRU;

typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
    MSCC_IMSA_IMS_VOICE_CAP_ENUM_UINT8  enImsVoiceCap;
    VOS_UINT8                           ucIsExistPersistentBearer; /* 0:表示不存在1:表示存在*/
    VOS_UINT8                           ucRcvServiceChangeIndFlag; /* 0:表示没有收到 1:表示收到 */
    VOS_UINT8                           aucRsv[1];
}IMSA_MSCC_IMS_VOICE_CAP_NOTIFY_STRU;

typedef struct
{
    VOS_MSG_HEADER                                                  /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;                    /*_H2ASN_Skip*/
    MSCC_IMSA_VOICE_DOMAIN_ENUM_UINT32  enVoiceDomain;              /* 0:cs only 1:ps only 2:cs prefer 3:ps prefer */
}MSCC_IMSA_VOICE_DOMAIN_CHANGE_IND_STRU;

/*****************************************************************************
 结构名  : MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ_STRU
 结构说明: IMS域设置请求

 修改历史      :
  1.日    期   : 2015年10月07日
    作    者   : f00179208
    修改内容   : 新增结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
    MSCC_IMSA_IMS_DOMAIN_ENUM_UINT32    enImsDomain;
}MSCC_IMSA_IMS_DOMAIN_CFG_SET_REQ_STRU;

/*****************************************************************************
 结构名  : IMSA_MSCC_IMS_DOMAIN_CFG_SET_CNF_STRU
 结构说明: IMS域设置回复

 修改历史      :
  1.日    期   : 2015年10月07日
    作    者   : f00179208
    修改内容   : 新增结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                              /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32                    ulMsgId;    /*_H2ASN_Skip*/
    IMSA_MSCC_SET_IMS_DOMAIN_RESULT_ENUM_UINT32     enResult;
}IMSA_MSCC_IMS_DOMAIN_CFG_SET_CNF_STRU;

/*****************************************************************************
 结构名  : MSCC_IMSA_AREA_LOST_IND_STRU
 结构说明: 丢网指示

 修改历史      :
  1.日    期   : 2015年10月07日
    作    者   : n00269697
    修改内容   : 新增结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                          /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_Skip*/
}MSCC_IMSA_AREA_LOST_IND_STRU;

/*****************************************************************************
 结构名  : MSCC_IMSA_ROAM_IMS_SUPPORT_SET_REQ_STRU
 结构说明: 漫游下IMS是否支持设置请求

 修改历史      :
  1.日    期   : 2016年3月9日
    作    者   : w00316404
    修改内容   : 新增结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32                  ulMsgId;                      /*_H2ASN_Skip*/
    MSCC_IMSA_SET_ROAM_IMS_SUPPORT_ENUM_UINT32    enRoamImsSupport;
}MSCC_IMSA_ROAM_IMS_SUPPORT_SET_REQ_STRU;

/*****************************************************************************
 结构名  : IMSA_MSCC_ROAM_IMS_SUPPORT_SET_CNF_STRU
 结构说明: 漫游下IMS是否支持设置回复

 修改历史      :
  1.日    期   : 2016年3月9日
    作    者   : w00316404
    修改内容   : 新增结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*_H2ASN_Skip*/
    MSCC_IMSA_MSG_ID_ENUM_UINT32                          ulMsgId;              /*_H2ASN_Skip*/
    IMSA_MSCC_SET_ROAM_IMS_SUPPORT_RESULT_ENUM_UINT32     enResult;
}IMSA_MSCC_ROAM_IMS_SUPPORT_SET_CNF_STRU;

typedef struct
{
    MSCC_IMSA_MSG_ID_ENUM_UINT32        ulMsgId;            /*_H2ASN_MsgChoice_Export MSCC_IMSA_MSG_ID_ENUM_UINT32*/
    VOS_UINT8                           aucMsg[4];
    /***************************************************************************
        _H2ASN_MsgChoice_When_Comment          MSCC_IMSA_MSG_ID_ENUM_UINT32
    ****************************************************************************/
}MSCC_IMSA_MSG_DATA;
/*_H2ASN_Length UINT32*/


typedef struct
{
    VOS_MSG_HEADER
    MSCC_IMSA_MSG_DATA                  stMsgData;
} ImsaMsccInterface_MSG;


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

#endif /* end of ImsaMsccInterface.h */

