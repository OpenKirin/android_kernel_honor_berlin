/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : xsmsimsainterface.h
  版 本 号   : 初稿
  作    者   : z00316370
  生成日期   : 2016年2月19日
  功能描述   : XSMS和IMSA接口头文件
  函数列表   :
  修改历史   :
  1.日    期   : 2016年2月19日
    作    者   : z00316370
    修改内容   : 创建文件

******************************************************************************/
#ifndef  XSMS_IMSA_INTERFACE_H
#define  XSMS_IMSA_INTERFACE_H

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "NasSms.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif


#pragma pack(4)

#define XSMS_IMSA_MAX_ADDR_LEN          (48)
#define XSMS_IMSA_MAX_USER_DATA_LEN     (255)
#define TAF_XSMS_IMSA_XSMS_BASE         (0)

/*****************************************************************************
  2 枚举定义
*****************************************************************************/
/*****************************************************************************
枚举名    : XSMS_IMSA_XSMS_TYPE_ENUM
结构说明  : XSMS和IMSA的之间的消息

  1.日    期   : 2016年2月19日
    作    者   : c00299064
    修改内容   : 定义XSMS和IMSA之间的接口消息
*****************************************************************************/

enum XSMS_IMSA_XSMS_TYPE_ENUM
{
    ID_XSMS_IMSA_TPDU_DATA_REQ      =   TAF_XSMS_IMSA_XSMS_BASE,            /* _H2ASN_XSMSChoice XSMS_IMSA_TPDU_DATA_REQ_STRU */
    ID_XSMS_IMSA_TL_ACK_REPORT_REQ,                                         /* _H2ASN_XSMSChoice XSMS_IMSA_TL_ACK_REPORT_REQ_STRU */

    ID_IMSA_XSMS_TPDU_DATA_IND      =   TAF_XSMS_IMSA_XSMS_BASE+0x100,      /* _H2ASN_XSMSChoice IMSA_XSMS_TPDU_DATA_IND_STRU */
    ID_IMSA_XSMS_TL_ACK_REPORT_IND,                                         /* _H2ASN_XSMSChoice IMSA_XSMS_TL_ACK_REPORT_IND_STRU */

    ID_IMSA_XSMS_TYPE_BUTT
};
typedef  VOS_UINT32  XSMS_IMSA_XSMS_TYPE_ENUM_UINT32;

/*****************************************************************************
枚举名    : XSMS_IMSA_TL_ACK_WAIT_NEED_ENUM
结构说明  : XSMS和IMSA的之间 MO 流程是否需要等待网侧的TL_ACK

  1.日    期   : 2016年2月19日
    作    者   : c00299064
    修改内容   :
*****************************************************************************/
enum XSMS_IMSA_TL_ACK_WAIT_NEED_ENUM
{
    XSMS_IMSA_TL_ACK_NOT_NEED_WAIT       = 0,
    XSMS_IMSA_TL_ACK_WAIT_NEEDED         = 1,

    XSMS_IMSA_TL_ACK_WAIT_NEED_BUTT

};

typedef  VOS_UINT8  XSMS_IMSA_TL_ACK_WAIT_NEED_ENUM_UINT8;

/*****************************************************************************
枚举名    : XSMS_IMSA_TL_ACK_WAIT_NEED_ENUM
结构说明  : XSMS和IMSA的之间 MT 流程是否需要給网侧的TL_ACK

  1.日    期   : 2016年2月19日
    作    者   : c00299064
    修改内容   :
*****************************************************************************/
enum XSMS_IMSA_TL_ACK_SND_NEED_ENUM
{
    XSMS_IMSA_TL_ACK_NOT_NEED_SND       = 0,
    XSMS_IMSA_TL_ACK_SND_NEEDED         = 1,

    XSMS_IMSA_TL_ACK_SND_NEED_BUTT

};

typedef  VOS_UINT8  XSMS_IMSA_TL_ACK_SND_NEED_ENUM_UINT8;

/*****************************************************************************
枚举名    : IMS_SMS_PROTOCOL_TYPE_ENUM
结构说明  : IMS 收到短信的协议格式

  1.日    期   : 2016年2月19日
    作    者   : c00299064
    修改内容   :
*****************************************************************************/
enum IMS_SMS_PROTOCOL_TYPE_ENUM
{
    IMS_SMS_PROTOCOL_TYPE_3GPP       = 0,
    IMS_SMS_PROTOCOL_TYPE_3GPP2      = 1,

    IMS_SMS_PROTOCOL_TYPE_BUTT

};

typedef  VOS_UINT8  IMS_SMS_PROTOCOL_TYPE_ENUM_UINT8;


/*****************************************************************************
枚举名    : IMSA_XSMS_SIP_CAUSE_CODE_ENUM
结构说明  : IMSA 上报的短信发送结果

  1.日    期   : 2016年7月12日
    作    者   : z00316370
    修改内容   :
*****************************************************************************/
enum IMSA_XSMS_SIP_CAUSE_CODE_ENUM
{
    IMSA_XSMS_SIP_CAUSE_TRYING                               = 100,
    IMSA_XSMS_SIP_CAUSE_CALL_IS_BEING_FORWARDED              = 181,      /* 被重新路由到另外一个目的地 */
    IMSA_XSMS_SIP_CAUSE_QUEUED                               = 182,      /* callee当前是不可获得的 */
    IMSA_XSMS_SIP_CAUSE_SESSION_PROGRESS                     = 183,      /* 警告频段错误 */
    IMSA_XSMS_SIP_CAUSE_SUCCESS                              = 200,      /* 成功 */
    IMSA_XSMS_SIP_CAUSE_MULTIPLE_CHOICES                     = 300,      /* 呼叫的地址被解析成多个地址 */
    IMSA_XSMS_SIP_CAUSE_MOVED_PERMANENTLY                    = 301,      /* 指定地址的用户已永远不可用 */
    IMSA_XSMS_SIP_CAUSE_MOVED_TEMPORARITY                    = 302,      /* 指定地址的用户暂时不可用 */
    IMSA_XSMS_SIP_CAUSE_USER_PROXY                           = 305,      /* caller 必须使用一个proxy来连续callee */
    IMSA_XSMS_SIP_CAUSE_ALTERNATIVE_SERVICE                  = 380,      /* caller 不可用，但是可选择其他的服务 */
    IMSA_XSMS_SIP_CAUSE_BAD_REQUEST                          = 400,      /* 非法格式，请求不能被理解 */
    IMSA_XSMS_SIP_CAUSE_UNAUTHORISED                         = 401,      /* 需要用户认证 */
    IMSA_XSMS_SIP_CAUSE_PAYMENT_REQUIRED                     = 402,      /* 完成会话需要用户付费 */
    IMSA_XSMS_SIP_CAUSE_FORBIDDEN                            = 403,      /* 服务被拒 */
    IMSA_XSMS_SIP_CAUSE_NOT_FOUND                            = 404,         /* 指定的域中用户不存在 */
    IMSA_XSMS_SIP_CAUSE_METHOD_NOT_ALLOWED                   = 405,         /* 请求中指定的方法时不被允许的，将返回一个允许的方法列表 */
    IMSA_XSMS_SIP_CAUSE_HEADER_NOT_ACCEPTABLE                = 406,         /* header 中指定的请求不接受 */
    IMSA_XSMS_SIP_CAUSE_PROXY_AUTHENTICATION_REQUIRED        = 407,         /* 需要再proxy上认证 */
    IMSA_XSMS_SIP_CAUSE_REQUEST_TIMEOUT                      = 408,         /* 在timeout过期前，server不能产生相应  */
    IMSA_XSMS_SIP_CAUSE_CONFLICT                             = 409,         /* 资源冲突，请求不能被处理 */
    IMSA_XSMS_SIP_CAUSE_GONE                                 = 410,         /* 请求资源不可用，也没有转发的地址 */
    IMSA_XSMS_SIP_CAUSE_LENGTH_REQUIRED                      = 411,         /* 用户拒绝没有定义content长度的请求 */
    IMSA_XSMS_SIP_CAUSE_REQUEST_ENTITY_TOO_LARGE             = 413,         /* 超过server能处理的大小  */
    IMSA_XSMS_SIP_CAUSE_REQUEST_URI_TOO_LONG                 = 414,         /* 请求的URI太长 */
    IMSA_XSMS_SIP_CAUSE_UNSUPPORTED_MEDIA                    = 415,         /* body格式不被目的终端支持 */
    IMSA_XSMS_SIP_CAUSE_BAD_EXTENSION                        = 420,         /* server不能理解header中的扩展协议 */
    IMSA_XSMS_SIP_CAUSE_TEMPORARILY_UNAVAILABLE              = 480,         /* callee已联系上，但暂时不可用 */
    IMSA_XSMS_SIP_CAUSE_CALL_NOT_EXIST                       = 481,         /* server正在忽略请求 */
    IMSA_XSMS_SIP_CAUSE_LOOP_DETECTED                        = 482,         /* 收到一个包含它自己路径的请求 */
    IMSA_XSMS_SIP_CAUSE_TOO_MANY_HOPSS                       = 483,         /* hop太大 */
    IMSA_XSMS_SIP_CAUSE_ADDRESS_INCOMPLETE                   = 484,         /* 地址不完整 */
    IMSA_XSMS_SIP_CAUSE_AMBIGUOUS                            = 485,         /* 地址不明确 */
    IMSA_XSMS_SIP_CAUSE_BUSY_HERE                            = 486,         /* 忙碌 */
    IMSA_XSMS_SIP_CAUSE_SERVER_INTERNAL_ERROR                = 500,         /* server或gateway发生意外 */
    IMSA_XSMS_SIP_CAUSE_NOT_IMPLEMENTED                      = 501,         /* 不支持 */
    IMSA_XSMS_SIP_CAUSE_BAD_GATEWAY                          = 502,         /* 非法响应 */
    IMSA_XSMS_SIP_CAUSE_SERVICE_UNAVAILABLE                  = 503,         /* 超负载 */
    IMSA_XSMS_SIP_CAUSE_GATEWAY_TIMEOUT                      = 504,         /* 没及时响应 */
    IMSA_XSMS_SIP_CAUSE_VERSION_NOT_SUPPORTED                = 505,         /* SIP版本不支持 */
    IMSA_XSMS_SIP_CAUSE_BUSY_EVERYWHERE                      = 600,         /* 忙碌中 */
    IMSA_XSMS_SIP_CAUSE_DECLINE                              = 603,         /* 不能加入CALL */
    IMSA_XSMS_SIP_CAUSE_DOES_NOT_EXIST_ANYWHERE              = 604,         /* CALLEE不在网络中 */
    IMSA_XSMS_SIP_CAUSE_SESSION_NOT_ACCEPTABLE               = 606,         /* session不被接受 */

    IMSA_XSMS_SIP_CAUSE_BUTT

};
typedef  VOS_UINT32  IMSA_XSMS_SIP_CAUSE_CODE_ENUM_UINT32;



/*****************************************************************************
  3 类型定义
*****************************************************************************/
/*****************************************************************************
  4 全局变量声明
*****************************************************************************/
/*****************************************************************************
  5 消息头定义
*****************************************************************************/
/*****************************************************************************
  6 消息定义
*****************************************************************************/
/*****************************************************************************
  7 STRUCT定义
*****************************************************************************/
typedef struct
{
    VOS_UINT32                          ulDataLen;                              /*用户数据的长度*/
    VOS_UINT8                           aucData[4];                           /*用户数据地址*/
}XSMS_IMSA_DATA_STRU;

/*****************************************************************************
 结构名  : XSMS_IMSA_TPDU_DATA_REQ_STRU,
 结构说明: MO的TL DATA消息

 修改历史      :
  1.日    期   : 2016年2月19日
    作    者   : c00299064
    修改内容   : 新增结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*消息头*/
    VOS_UINT32                                              ulMsgName;
    XSMS_IMSA_TL_ACK_WAIT_NEED_ENUM_UINT8                   enIsNwTlackNeeded;
    IMS_SMS_PROTOCOL_TYPE_ENUM_UINT8                        enProtocolType;
    VOS_UINT16                                              usDestAddrLen;      /* 目标地址的长度  */
    VOS_UINT8                                               aucDestAddr[XSMS_IMSA_MAX_ADDR_LEN];      /* 目标地址 ASCII码格式最大长度48 */
    XSMS_IMSA_DATA_STRU                                     stSmsData;
}XSMS_IMSA_TPDU_DATA_REQ_STRU;

/*****************************************************************************
 结构名  : XSMS_IMSA_REPORT_REQ_STRU
 结构说明: MT的TL_ACK确认消息

 修改历史      :
  1.日    期   : 2016年2月19日
    作    者   : c00299064
    修改内容   : 新增结构
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                                              /*消息头*/
    VOS_UINT32                                              ulMsgName;
    XSMS_IMSA_TL_ACK_SND_NEED_ENUM_UINT8                    enTlAckSndNeeded;
    IMS_SMS_PROTOCOL_TYPE_ENUM_UINT8                        enProtocolType;
    VOS_UINT16                                              usDestAddrLen;      /* 目标地址的长度  */
    VOS_UINT8                                               aucDestAddr[XSMS_IMSA_MAX_ADDR_LEN];      /* 目标地址 ASCII码格式最大长度48 */
    XSMS_IMSA_DATA_STRU                                     stSmsData;
} XSMS_IMSA_TL_ACK_REPORT_REQ_STRU;

/*****************************************************************************
 结构名  : IMSA_XSMS_DATA_IND_STRU
 结构说明: MT的TL DATA消息

 修改历史      :
  1.日    期   : 2016年2月19日
    作    者   : c00299064
    修改内容   : 新增结构
*****************************************************************************/
typedef SMT_SMR_DATA_STRU IMSA_XSMS_TPDU_DATA_IND_STRU;

/*****************************************************************************
 结构名    : IMSA_XSMS_TL_ACK_REPORT_IND_STRU
 结构说明  : IMSA给XSMS上报的短信发送结果消息结构，
             ulMsgName   消息名
             ulRslt      短信发送结果，VOS_TRUE 成功  VOS_FALSE 成功
  1.日    期   : 2016年7月12日
    作    者   : z00316370
    修改内容   : 新创建
*****************************************************************************/
typedef struct
{
    VOS_MSG_HEADER                                              /*消息头*/
    VOS_UINT32                                              ulMsgName;
    IMSA_XSMS_SIP_CAUSE_CODE_ENUM_UINT32                    enCauseCode;            /* 结果原因值 */
}IMSA_XSMS_TL_ACK_REPORT_IND_STRU;

/*****************************************************************************
  8 UNION定义
*****************************************************************************/
/*****************************************************************************
  9 OTHERS定义
*****************************************************************************/

/*****************************************************************************
  H2ASN顶级消息结构定义
*****************************************************************************/
typedef struct
{
    XSMS_IMSA_XSMS_TYPE_ENUM_UINT32     enMsgId;

    VOS_UINT8                           aucMsgBlock[4];
    /***************************************************************************
        _H2ASN_XSMSChoice_When_Comment          XSMS_IMSA_XSMS_TYPE_ENUM_UINT32
    ****************************************************************************/
}XSMS_IMSA_XSMS_DATA;
/*_H2ASN_Length UINT32*/

typedef struct
{
    VOS_MSG_HEADER
    XSMS_IMSA_XSMS_DATA                 stMsgData;
}XsmsImsaInterface_XSMS;


/*****************************************************************************
  10 函数声明
*****************************************************************************/


#if ((VOS_OS_VER == VOS_WIN32) || (VOS_OS_VER == VOS_NUCLEUS))
#pragma pack()
#else
#pragma pack(0)
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* XSMS_IMSA_INTERFACE_H */
