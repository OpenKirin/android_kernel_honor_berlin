/******************************************************************************


        @(#)Copyright(C)2015,Hisilicon Co. LTD.

 ******************************************************************************
    File name   : ImsaWifiInterface.h
    Description : IMSA与Wifi的消息头文件，通过AT接口封装
    History     :
      1.sunbing 00265702    2015-09-08  Draft Enact


******************************************************************************/


#ifndef __IMSA_WIFI_INTERFACE_H__
#define __IMSA_WIFI_INTERFACE_H__

/*****************************************************************************
  1 Include Headfile
*****************************************************************************/


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

#define IMSA_WIFI_MAX_APN_LEN           (99)

#define IMSA_WIFI_IPV4_ADDR_LEN         (4)
#define IMSA_WIFI_IPV6_ADDR_LEN         (16)

#define IMSA_WIFI_NONCID_SCHEME_URI_LEN 128
#define IMSA_WIFI_CIVICLOC_COUNTRY_LEN  4
#define IMSA_WIFI_CIVICLOC_COUNTRY_A1_LEN  48
#define IMSA_WIFI_CIVICLOC_COUNTRY_A2_LEN  48
#define IMSA_WIFI_CIVICLOC_COUNTRY_A3_LEN  48
#define IMSA_WIFI_CIVICLOC_COUNTRY_A4_LEN  48
#define IMSA_WIFI_CIVICLOC_COUNTRY_A5_LEN  48
#define IMSA_WIFI_CIVICLOC_ROADSTREET_LEN  100
#define IMSA_WIFI_CIVICLOC_OTHER_ADDRESS_LEN  100
#define IMSA_WIFI_COORDINATE_LATITUDE_LEN  16
#define IMSA_WIFI_COORDINATE_LONGITUDE_LEN 16
#define IMSA_WIFI_COORDINATE_ALTITUDE_LEN  16
#define IMSA_WIFI_TIMESTAMP_LEN              24


/*****************************************************************************
  3 Massage Declare
*****************************************************************************/



/*****************************************************************************
  4 Enum
*****************************************************************************/


enum WIFI_IMSA_MSG_ID_ENUM
{
    /* WIFI发给IMSA的消息 */
    ID_WIFI_IMSA_POWERON_REQ            = 0x0000,           /* _H2ASN_MsgChoice WIFI_IMSA_POWERON_REQ_STRU */
    ID_WIFI_IMSA_POWEROFF_REQ           = 0x0001,           /* _H2ASN_MsgChoice WIFI_IMSA_POWEROFF_REQ_STRU */
    ID_WIFI_IMSA_IMS_PDN_ACTIVATE_CNF   = 0x0002,           /* _H2ASN_MsgChoice WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU*/
    ID_WIFI_IMSA_IMS_PDN_READY_IND      = 0x0003,           /* _H2ASN_MsgChoice WIFI_IMSA_IMS_PDN_READY_IND_STRU */
    ID_WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF = 0x0004,           /* _H2ASN_MsgChoice WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF_STRU */
    ID_WIFI_IMSA_ERROR_IND              = 0x0005,           /* _H2ASN_MsgChoice WIFI_IMSA_ERROR_IND_STRU */
    ID_WIFI_IMSA_STATE_IND              = 0x0006,           /* _H2ASN_MsgChoice WIFI_IMSA_STATE_IND_STRU */
    ID_WIFI_IMSA_IMS_PDN_DEACTIVATE_IND = 0x0007,           /* _H2ASN_MsgChoice WIFI_IMSA_IMS_PDN_DEACTIVATE_IND_STRU */
    ID_IMSA_WIFI_LOCATION_UPDATE_IND    = 0x0008,           /* _H2ASN_MsgChoice WIFI_IMSA_LOCATION_UPDATE_IND_STRU */

    /* IMSA发给WIFI的消息 */
    ID_IMSA_WIFI_POWERON_CNF            = 0x0100,           /* _H2ASN_MsgChoice IMSA_WIFI_POWERON_CNF_STRU */
    ID_IMSA_WIFI_POWEROFF_CNF           = 0x0101,           /* _H2ASN_MsgChoice IMSA_WIFI_POWEROFF_CNF_STRU */
    ID_IMSA_WIFI_IMS_PDN_ACTIVATE_REQ   = 0x0102,           /* _H2ASN_MsgChoice IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU */
    ID_IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ = 0x0103,           /* _H2ASN_MsgChoice IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU */
    ID_IMSA_WIFI_IMS_AUDIO_INFO_IND     = 0x0104,           /* _H2ASN_MsgChoice IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU */
    ID_IMSA_WIFI_IMS_VEDIO_INFO_IND     = 0x0105,           /* _H2ASN_MsgChoice IMSA_WIFI_IMS_VEDIO_INFO_IND_STRU */

    ID_WIFI_IMSA_MSG_BUTT
};
typedef VOS_INT32 WIFI_IMSA_MSG_ID_ENUM_INT32;

/*****************************************************************************
 枚举名称: IMSA_WIFI_IP_TYPE_ENUM
 枚举说明: PDP类型
*****************************************************************************/
enum IMSA_WIFI_IP_TYPE_ENUM
{
    IMSA_WIFI_IP_IPV4                   = 0x01,
    IMSA_WIFI_IP_IPV6                   = 0x02,
    IMSA_WIFI_IP_IPV4V6                 = 0x03,

    IMSA_WIFI_IP_TYPE_BUTT
};
typedef VOS_INT8 IMSA_WIFI_IP_TYPE_ENUM_INT8;

/*****************************************************************************
 枚举名称: IMSA_WIFI_PDN_RELEASE_TYPE_ENUM
 枚举说明: PDP类型
*****************************************************************************/
enum IMSA_WIFI_PDN_RELEASE_TYPE_ENUM
{
    IMSA_WIFI_PDN_RELEASE_TYPE_NW_CMD   = 0x00,
    IMSA_WIFI_PDN_RELEASE_TYPE_LOCAL    = 0x01,

    IMSA_WIFI_PDN_RELEASE_TYPE_BUTT
};
typedef VOS_INT8 IMSA_WIFI_PDN_RELEASE_TYPE_ENUM_INT8;

/*****************************************************************************
 枚举名称: IMSA_WIFI_PDN_RELEASE_TYPE_ENUM
 枚举说明: PDP类型
*****************************************************************************/
enum IMSA_WIFI_SERVICE_TYPE_ENUM
{
    IMSA_WIFI_NORMAL_SERVICE_IN_NORMAL_PDN   = 0x00,
    IMSA_WIFI_EMC_SERVICE_IN_NORMAL_APN,
    IMSA_WIFI_EMC_SERVICE_IN_EMERGECY_APN,

    IMSA_WIFI_SERVICE_TYPE_BUTT
};
typedef VOS_INT8 IMSA_WIFI_SERVICE_TYPE_ENUM_INT8;


/*****************************************************************************
    枚举名    : WIFI_QUALITY_LEVEL_ENUM
    枚举说明  : WIFI质量等级枚举
*****************************************************************************/
enum WIFI_QUALITY_LEVEL_ENUM
{
    WIFI_IMSA_QUALITY_LEVEL_BAD         = 0x00,
    WIFI_IMSA_QUALITY_LEVEL_NORMAL      = 0x01,
    WIFI_IMSA_QUALITY_LEVEL_GOOD        = 0x02,

    WIFI_IMSA_QUALITY_LEVEL_BUTT
};
typedef VOS_INT8 WIFI_IMSA_QUALITY_LEVEL_ENUM_INT8;

/*****************************************************************************
    枚举名    : WIFI_IMSA_SERVICE_STATUS_ENUM
    枚举说明  : WIFI上报的AP服务状态
*****************************************************************************/
enum WIFI_IMSA_SERVICE_STATUS_ENUM
{
    WIFI_IMSA_SERVICE_STATUS_NORMAL_SEVICE         = 0x00,
    WIFI_IMSA_SERVICE_STATUS_LIMITED_SERVICE       = 0x01,
    WIFI_IMSA_SERVICE_STATUS_NO_SERVICE            = 0x02,

    WIFI_IMSA_SERVICE_STATUS_BUTT
};
typedef VOS_INT8 WIFI_IMSA_SERVICE_STATUS_ENUM_INT8;


/*****************************************************************************
    枚举名    : WIFI_IMSA_PDN_ACT_RESULT_ENUM
    枚举说明  : IMS PDN激活结果枚举
*****************************************************************************/
enum WIFI_IMSA_PDN_ACT_RESULT_ENUM
{
    WIFI_IMSA_PDN_ACT_RESULT_SUCCESS    = 0x00,
    WIFI_IMSA_PDN_ACT_RESULT_PARAM_ERR,                       /* 指定参数错误 */
    WIFI_IMSA_PDN_ACT_RESULT_LINKLOST,                        /* 链路丢失 */
    WIFI_IMSA_PDN_ACT_RESULT_TIMEOUT,                         /* 建立超时 */
    WIFI_IMSA_PDN_ACT_RESULT_MAPCON_STATE_ERR,                /* MAPCON状态错误 */
    WIFI_IMSA_PDN_ACT_RESULT_OTHER_FAILURE,                 /* 其他错误*/
    WIFI_IMSA_PDN_ACT_RESULT_SIM_AUTH_NO_RESP = 0x06,
    WIFI_IMSA_PDN_ACT_RESULT_AUTH_NETWORK_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_PARSE_NETWORK_PAYLOAD_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_ADD_SA_AP_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_CLIENT_PAYLOAD_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_MSG_NETWORK_OVERTIME_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_ENC_ALG_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_APN_ID_ERROR,

    WIFI_IMSA_PDN_ACT_RESULT_NETWORK_AUTH_SIM_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_STATUS_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_SA_INIT_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_NON_3GPP_ACCESS_TO_EPC_NOT_ALLOWED,
	WIFI_IMSA_PDN_ACT_RESULT_USER_UNKNOWN  = 0x12,
	WIFI_IMSA_PDN_ACT_RESULT_PLMN_NOT_ALLOWED,
	WIFI_IMSA_PDN_ACT_RESULT_AUTHORIZATION_REJECTED,
	WIFI_IMSA_PDN_ACT_RESULT_RAT_TYPE_NOT_ALLOWED,

	WIFI_IMSA_PDN_ACT_RESULT_NETWORK_FAILURE,
	WIFI_IMSA_PDN_ACT_RESULT_NO_APN_SUBSCRIPTION,
	WIFI_IMSA_PDN_ACT_RESULT_PDN_CONNECTION_REJECTION,
	WIFI_IMSA_PDN_ACT_RESULT_CONNECTION_REACHED,
	WIFI_IMSA_PDN_ACT_RESULT_PROTECLE_ERROR,
	WIFI_IMSA_PDN_ACT_RESULT_ROAM_FORBID = 0x32,
	WIFI_IMSA_PDN_ACT_RESULT_HIFI_STATUS_OVERTIME = 0x33,

    WIFI_IMSA_PDN_ACT_RESULT_BUTT
};
typedef VOS_INT8 WIFI_IMSA_PDN_ACT_RESULT_ENUM_INT8;



/*****************************************************************************
    枚举名    : WIFI_IMSA_PDN_REACT_ACTION_ENUM
    枚举说明  : IMS PDN激活结果枚举
*****************************************************************************/
enum WIFI_IMSA_PDN_REACT_ACTION_ENUM
{
    WIFI_IMSA_PDN_REACT_ACTION_NO    = 0x00,               /* PDN建立异常, MAPCON没有指定后续行为 */
    WIFI_IMSA_PDN_REACT_ACTION_RETRY_NODELAY,              /* PDN建立异常, MAPCON指定立刻重试 */
    WIFI_IMSA_PDN_REACT_ACTION_RETRY_DELAY,                /* PDN建立异常, MAPCON指定延迟重试,如果ReactTimerLen指出时间,则IMSA使用这个时间长度 */
    WIFI_IMSA_PDN_REACT_ACTION_PERM_REJ,                   /* PDN建立异常, MAPCON指定永远不要尝试 */

    WIFI_IMSA_PDN_REACT_ACTION_BUTT
};
typedef VOS_INT8 WIFI_IMSA_PDN_REACT_ACTION_ENUM_INT8;

/*****************************************************************************
    枚举名    : WIFI_IMS_ERROR_CAUSE_ENUM
    枚举说明  : IMS PDN激活结果枚举
*****************************************************************************/
enum WIFI_IMSA_ERROR_CAUSE_ENUM
{
    /* 指示WIFI IMS通道出错 */
    WIFI_IMSA_ERROR_CAUSE_LINK_LOST         =0x00,          /* WIFI链路丢失 */
    WIFI_IMSA_ERROR_CAUSE_SIP_PORT_FAILURE,                 /* SIP PORT绑定失败*/
    WIFI_IMSA_ERROR_CAUSE_AUDIO_PORT_FAILURE,               /* MEDIA PORT绑定失败 */
    WIFI_IMSA_ERROR_CAUSE_MAPCON_RESET_FAILURE,              /* MAPCON异常*/

    WIFI_IMSA_ERROR_CAUSE_BUTT
};
typedef VOS_INT8 WIFI_IMSA_ERROR_CAUSE_ENUM_INT8;

/*****************************************************************************
    枚举名    : WIFI_IMS_ACCESS_TYPE_ENUM
    枚举说明  : WIFI接入类型枚举
*****************************************************************************/
enum WIFI_IMS_ACCESS_TYPE_ENUM
{
    WIFI_IMSA_ACCESS_TYPE_802_11    = 0,
    WIFI_IMSA_ACCESS_TYPE_802_11_a  = 1,
    WIFI_IMSA_ACCESS_TYPE_802_11_b  = 2,
    WIFI_IMSA_ACCESS_TYPE_802_11_g  = 3,
    WIFI_IMSA_ACCESS_TYPE_802_11_n  = 4,
    WIFI_IMSA_ACCESS_TYPE_802_11_ac = 5,
    WIFI_IMSA_ACCESS_TYPE_802_11_ad = 6,
    WIFI_IMSA_ACCESS_TYPE_802_11_ah = 7,

    WIFI_IMSA_ACCESS_TYPE_BUTT
};
typedef VOS_INT32 WIFI_IMS_ACCESS_TYPE_ENUM_INT32;


/*****************************************************************************
    枚举名    : IMSA_WIFI_POWEROFF_REASON_ENUM
    枚举说明  : WIFI释放原因类型枚举
*****************************************************************************/
enum IMSA_WIFI_POWEROFF_REASON_ENUM
{
    IMSA_WIFI_POWEROFF_REASON_WIFI_POWEROFF    = 0,
    IMSA_WIFI_POWEROFF_REASON_SYSTEM_DOWN      = 1,
    IMSA_WIFI_POWEROFF_REASON_VOWIFI_STOP      = 2,
    IMSA_WIFI_POWEROFF_REASON_ROAMING_FORB     = 3,
    IMSA_WIFI_POWEROFF_REASON_SIM_DISABLE      = 4,

    IMSA_WIFI_POWEROFF_REASON_BUTT
};
typedef VOS_INT8 IMSA_WIFI_POWEROFF_REASON_ENUM_INT8;


/*****************************************************************************
  5 STRUCT
*****************************************************************************/
/*****************************************************************************
 结构名    : IMSA_WIFI_IP_ADDRESS_STRU
 结构说明  : IP地址的结构体
*****************************************************************************/
typedef struct
{
    IMSA_WIFI_IP_TYPE_ENUM_INT8        enIpType;
    VOS_INT8                           ucReserved[3];
    VOS_INT8                           aucIpV4Addr[IMSA_WIFI_IPV4_ADDR_LEN];
    VOS_INT8                           aucIpV6Addr[IMSA_WIFI_IPV6_ADDR_LEN];
}IMSA_WIFI_IP_ADDRESS_STRU;

/*****************************************************************************
 结构名    : IMSA_WIFI_MEDIA_INFO_STRU
 结构说明  : media信息的结构体
*****************************************************************************/
typedef struct
{
    VOS_INT32                          ulChannelId;
    VOS_INT32                          usLocalRTPPort;
    VOS_INT32                          usLocalRTCPPort;
    VOS_INT32                          usRemoteRTPPort;       /* 0表示尚未获得 */
    VOS_INT32                          usRemoteRTCPPort;
    IMSA_WIFI_IP_ADDRESS_STRU          stRemoteAddr;
} IMSA_WIFI_MEDIA_INFO_STRU;


/*****************************************************************************
 结构名称: IMSA_WIFI_PDP_APN_STRU
 结构说明: APN结构
*****************************************************************************/
typedef struct
{
    VOS_UINT8                           ucLength;
    VOS_UINT8                           aucValue[IMSA_WIFI_MAX_APN_LEN];
} IMSA_WIFI_PDP_APN_STRU;


/*****************************************************************************
 结构名称: IMSA_WIFI_PDP_IPV4_DNS_STRU
 结构说明: IPv4 DNS地址结构体
*****************************************************************************/
typedef struct
{
    VOS_INT8                            cOpPriDns;
    VOS_INT8                            cOpSecDns;
    VOS_INT8                            cOpSpare[2];

    VOS_UINT8                           aucPriDns[IMSA_WIFI_IPV4_ADDR_LEN];
    VOS_UINT8                           aucSecDns[IMSA_WIFI_IPV4_ADDR_LEN];
}IMSA_WIFI_PDP_IPV4_DNS_STRU;

/*****************************************************************************
 结构名称: IMSA_WIFI_PDP_IPV4_PCSCF_STRU
 结构说明: IPv4 P-CSCF地址结构体
*****************************************************************************/
typedef struct
{
    VOS_INT8                             cOpPrimPcscfAddr;
    VOS_INT8                             cOpSecPcscfAddr;
    VOS_INT8                             cOpThiPcscfAddr;
    VOS_INT8                             cOpSpare;

    VOS_UINT8                           aucPrimPcscfAddr[IMSA_WIFI_IPV4_ADDR_LEN];
    VOS_UINT8                           aucSecPcscfAddr[IMSA_WIFI_IPV4_ADDR_LEN];
    VOS_UINT8                           aucThiPcscfAddr[IMSA_WIFI_IPV4_ADDR_LEN];
} IMSA_WIFI_PDP_IPV4_PCSCF_STRU;

/*****************************************************************************
 结构名称   : IMSA_WIFI_PDP_IPV6_DNS_STRU
 结构说明   : IPv6 DNS地址结构体
*****************************************************************************/
typedef struct
{
    VOS_INT8                            cOpPriDns;
    VOS_INT8                            cOpSecDns;
    VOS_INT8                            cOpSpare[2];

    VOS_UINT8                           aucPriDns[IMSA_WIFI_IPV6_ADDR_LEN];
    VOS_UINT8                           aucSecDns[IMSA_WIFI_IPV6_ADDR_LEN];
} IMSA_WIFI_PDP_IPV6_DNS_STRU;

/*****************************************************************************
 结构名称: IMSA_WIFI_PDP_IPV6_PCSCF_STRU
 结构说明: IPv6 P-CSCF地址结构体
*****************************************************************************/
typedef struct
{
    VOS_INT8                            cOpPrimPcscfAddr;
    VOS_INT8                            cOpSecPcscfAddr;
    VOS_INT8                            cOpThiPcscfAddr;
    VOS_INT8                            cOpSpare;

    VOS_UINT8                           aucPrimPcscfAddr[IMSA_WIFI_IPV6_ADDR_LEN];
    VOS_UINT8                           aucSecPcscfAddr[IMSA_WIFI_IPV6_ADDR_LEN];
    VOS_UINT8                           aucThiPcscfAddr[IMSA_WIFI_IPV6_ADDR_LEN];
} IMSA_WIFI_PDP_IPV6_PCSCF_STRU;

/*****************************************************************************
 结构名称: IMSA_WIFI_AP_MAC_STRU
 结构说明: WIFI AP MAC结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMS_ACCESS_TYPE_ENUM_INT32     enAccessType;
    VOS_CHAR                            acApMac[16];
}IMSA_WIFI_AP_MAC_STRU;

/*****************************************************************************
 结构名    : WIFI_IMSA_POWERON_REQ_STRU
 结构说明  : WIFI发给IMSA的开机的结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                            acReserve[4];
} WIFI_IMSA_POWERON_REQ_STRU;

/*****************************************************************************
 结构名    : WIFI_IMSA_POWERON_CNF_STRU
 结构说明  : IMSA回复WIFI的开机的结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                           acReserve[4];
} IMSA_WIFI_POWERON_CNF_STRU;

/*****************************************************************************
 结构名    : WIFI_IMSA_POWEROFF_REQ_STRU
 结构说明  : WIFI发给IMSA关机的结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    IMSA_WIFI_POWEROFF_REASON_ENUM_INT8 enReason;         /* 0:WiFi关闭；1：系统关闭；2: VoWiFi关闭；3:漫游禁止关闭 */
    VOS_INT8                           acReserve[3];
} WIFI_IMSA_POWEROFF_REQ_STRU;

/*****************************************************************************
 结构名    : WIFI_IMSA_POWEROFF_CNF_STRU
 结构说明  : WIFI发给IMSA关机回复的结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;            /*_H2ASN_Skip*/
    VOS_INT8                           acReserve[4];
} IMSA_WIFI_POWEROFF_CNF_STRU;



/*****************************************************************************
 结构名称: IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU
 结构说明: 激活请求结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;            /*_H2ASN_Skip*/

    IMSA_WIFI_IP_TYPE_ENUM_INT8         enIpType;
    VOS_INT8                            cIsHandover;       /* 1：Handover，0：Initial */
    VOS_INT8                            cServiceType;      /* 0：普通APN上的普通业务，1：普通APN上的紧急业务, 2: 紧急APN上的紧急业务 */
    VOS_INT8                            acReServe;
    IMSA_WIFI_PDP_APN_STRU              stApn;
    IMSA_WIFI_IP_ADDRESS_STRU           stPdpAddr;
    IMSA_WIFI_PDP_IPV4_DNS_STRU         stDns;
    IMSA_WIFI_PDP_IPV4_PCSCF_STRU       stPcscf;
    IMSA_WIFI_PDP_IPV6_DNS_STRU         stIPv6Dns;
    IMSA_WIFI_PDP_IPV6_PCSCF_STRU       stIPv6Pcscf;
} IMSA_WIFI_IMS_PDN_ACTIVATE_REQ_STRU;

/*****************************************************************************
 结构名称: WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU
 结构说明: WIFI PDN激活结果结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    IMSA_WIFI_IP_TYPE_ENUM_INT8         enPdnType;
    VOS_INT8                            cServiceType;      /* 0：普通APN上的普通业务，1：普通APN上的紧急业务, 2: 紧急APN上的紧急业务 */
    WIFI_IMSA_PDN_ACT_RESULT_ENUM_INT8  enCause;
    WIFI_IMSA_PDN_REACT_ACTION_ENUM_INT8  enRetryAction;

    IMSA_WIFI_PDP_APN_STRU              stApn;
    IMSA_WIFI_IP_ADDRESS_STRU           stPdpAddr;
    IMSA_WIFI_PDP_IPV4_DNS_STRU         stDns;
    IMSA_WIFI_PDP_IPV4_PCSCF_STRU       stPcscf;
    IMSA_WIFI_PDP_IPV6_DNS_STRU         stIPv6Dns;
    IMSA_WIFI_PDP_IPV6_PCSCF_STRU       stIPv6Pcscf;
    IMSA_WIFI_AP_MAC_STRU               stApMac;
    VOS_INT32                           lPdnReActTimerLen; /* 以秒为单位的重新发起WifiPdn建立过程的定时器 */

} WIFI_IMSA_IMS_PDN_ACTIVATE_CNF_STRU;

/*****************************************************************************
 结构名称: WIFI_IMSA_IMS_PDN_READY_IND_STRU
 结构说明: WIFI链路建立成功结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32                     lMsgId;
    VOS_INT8                                        cServiceType;      /* 0：普通APN上的普通业务，1：普通APN上的紧急业务, 2: 紧急APN上的紧急业务 */
    VOS_INT8                                        acReserve[3];
}WIFI_IMSA_IMS_PDN_READY_IND_STRU;

/*****************************************************************************
 结构名称: IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU
 结构说明: WIFI PDN去激活请求结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    IMSA_WIFI_PDP_APN_STRU              stApn;
    VOS_INT8                            cIsLocal;          /* 1：本地释放，0：与网侧交互 */
    VOS_INT8                            cServiceType;      /* 0：普通APN上的普通业务，1：普通APN上的紧急业务, 2: 紧急APN上的紧急业务 */
    VOS_INT8                            acReserve[2];
} IMSA_WIFI_IMS_PDN_DEACTIVATE_REQ_STRU;


/*****************************************************************************
 结构名称: WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF_STRU
 结构说明: WIFI PDN去激活结果结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    VOS_INT8                            cServiceType;      /* 0：普通APN上的普通业务，1：普通APN上的紧急业务, 2: 紧急APN上的紧急业务 */
    VOS_INT8                            acReserve[3];
} WIFI_IMSA_IMS_PDN_DEACTIVATE_CNF_STRU;

/*****************************************************************************
 结构名称: WIFI_IMSA_IMS_PDN_DEACTIVATE_IND_STRU
 结构说明: WIFI PDN去激活结果结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    IMSA_WIFI_PDP_APN_STRU              stApn;
    VOS_INT8                            cServiceType;      /* 0：普通APN上的普通业务，1：普通APN上的紧急业务, 2: 紧急APN上的紧急业务 */
    VOS_INT8                            enCause;           /* 0：普通APN上的普通业务，1：普通APN上的紧急业务, 2: 紧急APN上的紧急业务 */
    VOS_INT8                            acReserve[2];
} WIFI_IMSA_IMS_PDN_DEACTIVATE_IND_STRU;


/*****************************************************************************
 结构名称: WIFI_IMSA_STATE_IND_STRU
 结构说明: WIFI状态结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    WIFI_IMSA_QUALITY_LEVEL_ENUM_INT8  enWifiQuality;
    WIFI_IMSA_SERVICE_STATUS_ENUM_INT8 enServiceStatus;
    VOS_INT8                           acReserve[2];
    IMSA_WIFI_AP_MAC_STRU              stApMac;
} WIFI_IMSA_STATE_IND_STRU;

/*****************************************************************************
 结构名称: WIFI_IMSA_ERROR_IND_STRU
 结构说明: WIFI错误指示结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    WIFI_IMSA_ERROR_CAUSE_ENUM_INT8    enCause;
    VOS_INT8                           cServiceType;      /* 0：普通APN上的普通业务，1：普通APN上的紧急业务, 2: 紧急APN上的紧急业务 */
    VOS_INT8                           acReserve[2];
} WIFI_IMSA_ERROR_IND_STRU;

/*****************************************************************************
 结构名称: IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU
 结构说明: IMS audio信息结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    VOS_INT8                           cAudioInfoCnt;
    VOS_INT8                           acReserved[3];
    IMSA_WIFI_MEDIA_INFO_STRU          stMediaInfo[4];
} IMSA_WIFI_IMS_AUDIO_INFO_IND_STRU;


/*****************************************************************************
 结构名称: IMSA_WIFI_IMS_VIDEO_INFO_IND_STRU
 结构说明: IMS video信息结构体
*****************************************************************************/
typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32        lMsgId;
    VOS_INT8                           cVideoInfoCnt;
    VOS_INT8                           acReserved[3];
    IMSA_WIFI_MEDIA_INFO_STRU          stMediaInfo[2];
} IMSA_WIFI_IMS_VIDEO_INFO_IND_STRU;

/*****************************************************************************
 结构名称: WIFI_IMSA_LOCATION_UPDATE_IND_STRU
 结构说明: IMS位置信息信息结构体
*****************************************************************************/

typedef struct
{
   VOS_CHAR acCountry[IMSA_WIFI_CIVICLOC_COUNTRY_LEN];
   VOS_CHAR acCountryA1[IMSA_WIFI_CIVICLOC_COUNTRY_A1_LEN];
   VOS_CHAR acCountryA2[IMSA_WIFI_CIVICLOC_COUNTRY_A2_LEN];
   VOS_CHAR acCountryA3[IMSA_WIFI_CIVICLOC_COUNTRY_A3_LEN];
   VOS_CHAR acCountryA4[IMSA_WIFI_CIVICLOC_COUNTRY_A4_LEN];
   VOS_CHAR acCountryA5[IMSA_WIFI_CIVICLOC_COUNTRY_A5_LEN];
   VOS_CHAR acRoadStreet[IMSA_WIFI_CIVICLOC_ROADSTREET_LEN];
   VOS_CHAR acOtherAddress[IMSA_WIFI_CIVICLOC_OTHER_ADDRESS_LEN];
}stCivicLocInfo;

typedef struct
{
   VOS_CHAR acLatitude[IMSA_WIFI_COORDINATE_LATITUDE_LEN];
   VOS_CHAR acLongitude[IMSA_WIFI_COORDINATE_LONGITUDE_LEN];
   VOS_CHAR acAltitude[IMSA_WIFI_COORDINATE_ALTITUDE_LEN];
}stCoordinateLocInfo;


typedef struct
{
    VOS_UINT32                       ulMsgId;
    VOS_INT8                         cIsCivicLocInfoExist;
    VOS_INT8                         cIsCoordinateLocInfoExist;
    VOS_INT8                         acReserved[2];
    stCivicLocInfo                   stCivicLocInfo;
    stCoordinateLocInfo              stCoordinateLocInfo;
    VOS_CHAR                         acRetentionExpiry[IMSA_WIFI_TIMESTAMP_LEN];
    VOS_CHAR                         acTimeStamp[IMSA_WIFI_TIMESTAMP_LEN];
} WIFI_IMSA_LOCATION_UPDATE_IND_STRU;

/*****************************************************************************
 结构名称: WIFI_IMSA_MSG_STRU
 结构说明: WIFI-IMSA间的结构体
*****************************************************************************/

typedef struct
{
    WIFI_IMSA_MSG_ID_ENUM_INT32         lMsgId;
    VOS_INT8                            acReserve[4];
} WIFI_IMSA_MSG_STRU;


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


#endif //__IMSA_WIFI_INTERFACE_H__
