/******************************************************************************

                  版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名   : nv_common_interface
  版 本 号   : 初稿
  作    者   : s00313518
  生成日期   : 2016年4月7日
  最近修改   :
  功能描述   : common NV
  函数列表   :
  修改历史   :
  1.日    期   : 2016年4月7日
    作    者   : s00313518
    修改内容   : 创建文件

******************************************************************************/

/*****************************************************************************
  1 其他头文件包含
*****************************************************************************/
/*#include  ###*/
#include "type_define_data.h"

#ifndef __NV_COMMON_INTERFACE_H__
#define __NV_COMMON_INTERFACE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif



/*****************************************************************************
  2 宏定义
*****************************************************************************/
#define RX_ANTENNA_MIPI_NUM             ( 4 )
#define ANT_TUNER_MIPI_NUM              ( 2 )
#define TX_COUPLER_MIPI_NUM             ( 1 )
#define MAX_TRIG_NUM                    ( 4 )
#define TX_ANT_SEL_MIPI_NUM             ( 4 )
#define PA_MODE_MIPI_NUM                ( 4 )
#define PA_TRIG_NUM                     ( 2 )
#define LPHY_DL_PATH_NUM                     ( 8 )
#define LPHY_UL_PATH_NUM                     ( 8 )
#define DL_CA_BAND_NUM                  ( 5 )
#define UL_CA_BAND_NUM                  ( 2 )
#define CA_COM_ANT_SEL_MIPI_NUM         ( 4 )
#define CA_COM_ANT_TUNER_MIPI_NUM       ( 4 )
#define MAX_RFIC_NUM                    ( 4 )
#define MAX_RFIC_RX_CH_NUM              ( 3 )
#define MAX_RFIC_TX_CH_NUM              ( 2 )
#define MAX_BAND_NUM                    ( 32 )
#define RF_RX_MAX_FREQ_NUM              ( 32 )
#define AGC_MAX_GAIN_NUM                ( 8 )
#define AGC_SWITCH_THRESHOLD_NUM        ( AGC_MAX_GAIN_NUM - 1 )
#define AGC_MAX_TABLE_NUM               ( 10 )
#define MAX_VGA_GAIN_NUM                ( 16 )

#define LNA_SINGLE_GAIN_MIPI_NUM        ( 4 )
#define EXT_LNA_LEVEL_NUM               ( 4 )
#define EXT_LNA_BAND_SEL_NUM            ( 2 )
#define EXT_LNA_OFF_NUM                 ( 2 )
#define LNA_MIPI_TRIG_NUM               ( 8 )
#define EXT_LNA_GIAN_CAL_NUM           ( 2 )



#define MAX_IP2_CAL_FREQ_NUM            ( 3 )

#define RF_TX_GAIN_6363_NUM             ( 20 )
#define RFIC_INIT_REG_NUM               ( 256 )
#define BBGAIN_NUM_PER_RFGAIN           ( 2 )
#define RF_PA_GAIN_NUM                  ( 3 )
#define MAX_TX_ANT_NUM                  ( 2 )
#define MAX_RX_ANT_NUM                  ( 4 )
#define CAL_IP2_FREQ_NUM                ( 3 )

#define MAX_DL_CC_NUM                   ( 5 )
#define MAX_UL_CC_NUM                   ( 2 )
#define MAX_DL_BAND_NUM                 (MAX_DL_CC_NUM)
#define MAX_UL_BAND_NUM                 (MAX_UL_CC_NUM)

#define MAX_TEMP_DEFINE_NUM             (32)
/*****************************************************************************
  3 枚举定义
*****************************************************************************/


/*****************************************************************************
  4 消息头定义
*****************************************************************************/


/*****************************************************************************
  5 消息定义
*****************************************************************************/


/*****************************************************************************
  6 STRUCT定义
*****************************************************************************/

/****************************RF NV  定义*************************************/
/*****************************************************************************
 结构名    : PHY_MIPIDEV_CMD_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : MIPI命令控制结构体
*****************************************************************************/
typedef struct
{
    UINT32                              bitByteCnt                  :5;          /* 0表示该CMMAND无效，取值0~4 */
    UINT32                              bitMipiPortSel              :3;          /* mipi port 0~7 */
    UINT32                              bitSlaveID                  :4;          /* slave id 0~15 */
    UINT32                              bitRegAddr                  :16;         /* reg addr 0~65535 */
    UINT32                              bitRsv                      :4;          /* 四字节对齐 */

} PHY_MIPIDEV_CMD_STRU;

/*****************************************************************************
 结构名    : PHY_MIPIDEV_DATA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : MIPI数据结构体
*****************************************************************************/
typedef struct
{
    UINT32                              bitByte0                    :8;          /* 第一个byte */
    UINT32                              bitByte1                    :8;          /* 第二个byte */
    UINT32                              bitByte2                    :8;          /* 第三个byte */
    UINT32                              bitByte3                    :8;          /* 第四个byte */

} PHY_MIPIDEV_DATA_STRU;

/*****************************************************************************
 结构名    : PHY_MIPIDEV_UNIT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : MIPI命令最小单元
*****************************************************************************/
typedef struct
{
    PHY_MIPIDEV_CMD_STRU                stCmd;                                  /*控制域*/
    PHY_MIPIDEV_DATA_STRU               stData;                                 /*数据域,一个MIPI占用2个UINT32*/

} PHY_MIPIDEV_UNIT_STRU;

/*****************************************************************************
 结构名    : NV_MODEM_OPERATING_BAND_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 频段结构体
*****************************************************************************/
typedef struct
{
    UINT16                              bitBand                     :8;         /* 频段号，band1:1,band2:2, 依次类推 */
    UINT16                              bitBandExIdx                :2;         /* 举例：正常的Band，填0，Band 128填1 */
    UINT16                              bitLteNvValidFlag           :1;         /* only for Lte,判断NV是否生效 */
    UINT16                              bitRsv0                     :3;
    UINT16                              bitRsv1                     :2;

}NV_MODEM_OPERATING_BAND_STRU;

/*****************************************************************************
 结构名    : NV_BAND_DL_RFIC_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 下行RFIC配置,包括RFIC ID,通道号，输入的PORT口
*****************************************************************************/
typedef struct
{
    UINT32                              bitRficIdx                  :2;         /* RFIC ID：[0,2]分别表示RFIC1，RFIC2，RFIC3 */
    UINT32                              bitRficRxCh                 :2;         /* 下行RX通道号，每个RFIC有3个通道，分别是RX1，RX2，RX3; 对于下行4*4MIMO固定使用RX1A1B+RX3A3B,但是采用同一个TX_PLL1控制，因此MIMO场景不用选RX通道，但4收切2收时，
                                                                                只切向Main和Div Ant,RFIC的两收RxCh需要填写使用的通道号；如果支持2收场景，RxCh表示使用的Rx通道号;
                                                                                4R 和2R复用同一个NV */


    UINT32                              bitRfPortCrossFlag          :1;         /*主集天线的信号流接到了 RFIC DRX Port口，软件需要在CTU通道交叉到主集，只支持2收的交叉*/
    UINT32                              bitMainAntRxPortSel         :4;         /* 主集LNA的RFIC端口选择，即信号通过哪个端口输入到RFIC */
    UINT32                              bitDivAntRxPortSel          :4;         /* 分集LNA的RFIC端口选择 */
    UINT32                              bitAuxMainAntRxPortSel      :4;         /* 辅助主集LNA的RFIC端口选择，目前只有LTE使用，GUC不使用 */
    UINT32                              bitAuxDivAntRxPortSel       :4;         /* 辅助分集LNA的RFIC端口选择，目前只有LTE使用，GUC不使用 */
    UINT32                              bitRsv0                     :2;         /* 预留位 */
    UINT32                              bitRsv1                     :2;
    UINT32                              bitRsv2                     :2;
    UINT32                              bitRsv3                     :2;
    UINT32                              bitRsv4                     :3;

} NV_BAND_DL_RFIC_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_UL_RFIC_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 上行RFIC配置,包括RFIC ID,通道号，输出的PORT口
*****************************************************************************/
typedef struct
{
    UINT32                              bitRficIdx                 :2;          /* 上行通道所在的RFIC ID,支持收发分离 */
    UINT32                              bitRficTxCh                :2;          /* 上行通道号，上行MIMO固定使用Tx1Ch,因此当满足上行MIMO后，不取此通道；单发情况下，填写Tx1Iq1,Tx2Iq；上行2Tx转1Tx复用NV，策略和Rx一样*/
    UINT32                              bitMainAntRficTxPortSel    :4;          /* 上行PORT口选择 */
    UINT32                              bitDivAntRficTxPortSel     :4;
    UINT32                              bitMainAntRficTxMrxPortSel :1;           /*上行环回通道对Mrx通道的选择，0: MRX1 RF input selected  1: MRX2 RF input selected*/
    UINT32                              bitDivAntRficTxMrxPortSel  :1;
    UINT32                              bitRsv0                    :2;
    UINT32                              bitRsv1                    :2;
    UINT32                              bitRsv2                    :2;
    UINT32                              bitRsv3                    :2;
    UINT32                              bitRsv4                    :2;
    UINT32                              bitRsv5                    :4;
    UINT32                              bitRsv6                    :4;
}NV_BAND_UL_RFIC_PARA_STRU;

/*****************************************************************************
 结构名    : NV_DL_CONFIG_TIME_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 时序NV结构参数结构体定义
*****************************************************************************/
typedef struct
{

    UINT16                              usLnaBandSelTime;
    UINT16                              usLnaGainSwitchTime;
    UINT16                              usLnaOffTime;
    UINT16                              usLnaTimeRsv;
}NV_DL_CONFIG_TIME_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_DL_PATH_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 每个PATH的下行RFIC通道配置
*****************************************************************************/
typedef struct
{
    UINT16                              bitRxAntNum                 :3;         /* 接收天线数 */
    UINT16                              bitMainAntValidFlag         :1;         /* 2收case下，使用天线分布情况，用于校准 */
    UINT16                              bitDivAntValidFlag          :1;
    UINT16                              bitAuxMainAntValidFlag      :1;
    UINT16                              bitAuxDivAntValidFlag       :1;
    UINT16                              bitRsv0                     :2;
    UINT16                              bitRsv1                     :7;
    UINT16                              usDlFemPathIdx;                         /* 获取下行 FemPath NV的Idx号:0~255，FemPathNV 完全动态查询，包括 天线和外置LNA同时查询，二者Idx保持一致*/

    NV_BAND_DL_RFIC_PARA_STRU           stBandDlRficPara;                       /* 下行RFIC参数 */
    NV_DL_CONFIG_TIME_PARA_STRU         astLnaTimePara[MAX_RX_ANT_NUM];
} NV_BAND_DL_PATH_PARA_STRU;

/*****************************************************************************
 结构名    : NV_UL_CONFIG_TIME_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 上行时序配置
*****************************************************************************/
typedef struct
{
    UINT16                              usPaOpenTime;                           /* PA 打开时间配置时序 */
    UINT16                              usPaUpModeTime;                       /* PA 升增益配置时序 */
    UINT16                              usPaDownModeTime;                        /* PA 降增益配置时序 */
    UINT16                              usPaCloseTime;                          /* PA 关闭配置置时序 */
    UINT16                              usTxPchApcRficTime;                 /*最终需要根据逻辑定义修改*/
    UINT16                              usTxSrsApcRficTime;                 /*最终需要根据逻辑定义修改*/
}NV_UL_CONFIG_TIME_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_UL_PATH_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 每个PATH的上行RFIC通道配置
*****************************************************************************/
typedef struct
{
    UINT16                              bitTxAntNum                 :2;         /* 发射天线数 */
    UINT16                              bitMainAntValid             :1;         /* 2发case下，使用天线分布情况，用于校准 */
    UINT16                              bitDivAntValid              :1;
    UINT16                              bitAptPdm0PinSel            :3;
    UINT16                              bitAptPdm1PinSel            :3;         /* GUCT 模不关心 */
    UINT16                              bitPmuAuxSel                :3;
    UINT16                              bitRsv                      :3;
    UINT16                              usUlFemPathIdx;                         /* 获取上行 FemPath NV的Idx号:0~255，FemPathNV 完全动态查询 */

    NV_BAND_UL_RFIC_PARA_STRU           stBandUlRficPara;                       /* RFIC参数 */

    NV_UL_CONFIG_TIME_PARA_STRU         stUlCfgTime[MAX_TX_ANT_NUM];
}NV_BAND_UL_PATH_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_MODEM_USED_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 每个Band在单载波以及侦听使用的PathIdx,LTE包括主副卡；
*****************************************************************************/
typedef struct
{
    UINT32                              bitBand                     :8;         /* 频段号，band1:1,band2:2, 依次类推 */
    UINT32                              bitBandExIdx                :2;         /* 举例：正常的Band，填0，Band 128填1 */
    UINT32                              bitLteNvValidFlag           :1;         /* only for Lte,判断NV是否生效 */
    UINT32                              bitTASSupport               :1;         /* bitBlindSwitchFlag:是否支持TAS盲切,1表支持，需产线定制*/
    UINT32                              bitMASSupport               :2;         /* 支持能力 0-不支持 1-支持下天线MAS 2-支持上天线MAS 3都支持上下天线MAS*/
    UINT32                              bitRsv3                     :2;

    UINT32                              bitDlPathNum                :4;         /* 校准复用 ,最多支持8条,PathNum = 8,表示支持的个数，0表示不支持 */
    UINT32                              bit2AntSingleCcDlPathIdx    :3;         /* Modem0:单载波 2 收下行选择的path编号 :0~7*/
    UINT32                              bit4AntSingleCcDlPathIdx    :3;         /* Modem0:单载波 4 收下行选择的path编号 :0~7,当2收和4收Pathidx相同，则单载波校准只校准一次，且按4收校准 */
    UINT32                              bit8by8MimoAddPathIdx       :3;         /* 8收增加的额外Pathidx号 */
    UINT32                              bitListeningDlPathIdx       :3;         /* Modem0:侦听通道标识 */

    UINT32                              bitUlPathNum                :4;         /* Modem0:校准复用,最多支持8条,PathNum = 8,表示支持的个数，0表示不支持 */
    UINT32                              bit1AntSingleCcUlPathIdx    :3;         /* Modem0:单载波 单发上行选择的path编号:0~7 */
    UINT32                              bit2AntSingleCcUlPathIdx    :3;         /* Modem0:单载波 双发上行选择的path编号:0~7 */
    UINT32                              bitListeningUlPathIdx       :3;         /* Modem0:侦听通道标识*/
    UINT32                              bitAntNum                   :3;

    /* Modem1 used path information */
    UINT32                              bitModem1DlPathIdx          :3;         /* Modem1(副卡)使用的下行PathIdx :0~7,LTE使用GUC不关心 */
    UINT32                              bitModem1UlPathIdx          :3;         /* Modem1(副卡)使用的上行PathIdx :0~7,LTE使用GUC不关心 */
    UINT32                              bitModem1ListenDlPathIdx    :3;         /* Modem1(副卡)使用的下行侦听PathIdx :0~7,LTE使用GUC不关心 */
    UINT32                              bitModem1AntNum             :3;
    UINT32                              bitRsv2                     :4;
}NV_BAND_USED_PATH_STRU;
/*****************************************************************************
 结构名    : NV_MODEM_RF_BAND_CONFIG_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : LTE的RF BAND 配置，区分每个模式的每个频段，其中GSM最多支持9个频段
             WCDMA支持20个频段，LTE支持64-9-20=35个频段，目前该结构体LTE使用，
             GUC模根据自身情况定义
*****************************************************************************/
typedef struct
{
    NV_BAND_USED_PATH_STRU             stBandModemInfo;
    NV_BAND_DL_PATH_PARA_STRU           astBandDlPathPara[LPHY_DL_PATH_NUM];         /* 下行PATH编号，目前DL最多支持6，GUC模只需要1个path */
    NV_BAND_UL_PATH_PARA_STRU           astBandUlPathPara[LPHY_UL_PATH_NUM ];         /* 上行PATH编号，目前UL最多支持3，GUC模只需要1个path */

}NV_MODEM_RF_BAND_CONFIG_STRU;

/*****************************************************************************
 结构名    : NV_BAND_DL_GPIO_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 前端下行GPIO配置，包括天线开关，TUNER等，支持天线开关和TUNER单独配置
*****************************************************************************/
typedef struct
{
    UINT32                              ulMainRxAntSel;                         /*ASM通路上的切换开关,各天线开关或出去*/
    UINT32                              ulMainRxAntSelExt;                      /*ASM通路上的切换开关,各天线开关或出去*/
    UINT32                              ulDivRxAntSel;
    UINT32                              ulDivRxAntSelExt;
    UINT32                              ulAuxMainRxAntSel;                      /*ASM通路上的切换开关*/
    UINT32                              ulAuxMainRxAntSelExt;
    UINT32                              ulAuxDivRxAntSel;
    UINT32                              ulAuxDivRxAntSelExt;
    UINT32                              ulMainRxTunerSel;                       /*ASM通路上的切换开关*/
    UINT32                              ulMainRxTunerSelExt;
    UINT32                              ulDivRxTunerSel;
    UINT32                              ulDivRxTunerSelExt;
    UINT32                              ulAuxMainTunerRxSel;                    /*ASM通路上的切换开关*/
    UINT32                              ulAuxMainTunerRxSelExt;
    UINT32                              ulAuxDivRxTunerSel;
    UINT32                              ulAuxDivRxTunerSelExt;

} NV_BAND_DL_GPIO_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_DL_MIPI_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 前端下行MIPI配置，包括天线开关，TUNER等，支持天线开关和TUNER单独配置
*****************************************************************************/
typedef struct
{
    PHY_MIPIDEV_UNIT_STRU              astMipiMainAntRxSel[RX_ANTENNA_MIPI_NUM];            /*Ant Mipi num=4,需要射频评估*/
    PHY_MIPIDEV_UNIT_STRU              astMipiDivAntRxSel[RX_ANTENNA_MIPI_NUM ];
    PHY_MIPIDEV_UNIT_STRU              astMipiAuxMainAntRxSel[RX_ANTENNA_MIPI_NUM ];
    PHY_MIPIDEV_UNIT_STRU              astMipiAuxDivAntRxSel[RX_ANTENNA_MIPI_NUM ];
    PHY_MIPIDEV_UNIT_STRU              astMipiMainTunerRxSel[ANT_TUNER_MIPI_NUM ];
    PHY_MIPIDEV_UNIT_STRU              astMipiDivTunerRxSel[ANT_TUNER_MIPI_NUM ];           /*Tuner Mipi num=2,需要射频评估*/
    PHY_MIPIDEV_UNIT_STRU              astMipiAuxMainTunerRxSel[ANT_TUNER_MIPI_NUM ];
    PHY_MIPIDEV_UNIT_STRU              astMipiAuxDivTunerRxSel[ANT_TUNER_MIPI_NUM ];
    PHY_MIPIDEV_UNIT_STRU              astMipiAntTrig[MAX_RX_ANT_NUM ];                     /*Ant Mipi Trig*/
    PHY_MIPIDEV_UNIT_STRU              astMipiTunerTrig[MAX_RX_ANT_NUM ];                   /*Tuner Mipi Trig*/
} NV_BAND_DL_MIPI_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_DL_FEM_PATH_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 每条PATH的前端下行MIPI配置，包括天线开关，TUNER等，
             支持天线开关和TUNER单独配置
*****************************************************************************/
typedef struct
{

    NV_BAND_DL_GPIO_PARA_STRU           stBandDlGpioPara;                       /* 每条PATH下行GPIO配置 */
    NV_BAND_DL_MIPI_PARA_STRU           stBandDlMipiPara;                       /* 每条PATH下行MIPI配置 */

}NV_BAND_DL_FEM_PATH_PARA_STRU;

/*****************************************************************************
 结构名    : NV_MODEM_RF_DL_FEM_PATH_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 所有PATH的前端下行MIPI配置，包括天线开关，TUNER等，
             支持天线开关和TUNER单独配置
*****************************************************************************/
typedef struct
{
    NV_MODEM_OPERATING_BAND_STRU        stBand;
    UINT16                              usLteNvValidFalg;
    NV_BAND_DL_FEM_PATH_PARA_STRU       stBandDlFemPathPara;

}NV_MODEM_RF_DL_FEM_PATH_STRU;

/*****************************************************************************
 结构名    : NV_BAND_UL_GPIO_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 上行GPIO配置，包括天线开关，TUNER等，支持天线开关和TUNER单独配置
*****************************************************************************/
typedef struct
{
    UINT32                              ulTxAntSel;                             /* 上行天线开关 */
    UINT32                              ulTxAntSelExt;
    UINT32                              UlTxAntCrossSel;                        /* LTE基站选择调度UE 发射交叉到分集天线，GPIO配置;GUCT 不关心 */
    UINT32                              ulTxTuner;                              /* 上行Tuner开关 */
    UINT32                              ulTxTunerExt;
    UINT32                              ulPaEnCtrl;                             /* PA线控开关 */
    UINT32                              ulPaHighModeCtrl;                       /* PA高增益控制字 */
    UINT32                              ulPaMidModeCtrl;                        /* PA中增益控制字 */
    UINT32                              ulPaLowModeCtrl;                        /* PA低增益控制字 */
    UINT32                              ulPaUltraLowModeCtrl;                   /* utralow */

} NV_BAND_UL_GPIO_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_UL_MIPI_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 上行MIPI配置，包括天线开关，TUNER等，支持天线开关和TUNER单独配置
*****************************************************************************/
typedef struct
{
    PHY_MIPIDEV_UNIT_STRU               astMipiAntSelTx[TX_ANT_SEL_MIPI_NUM];   /* MIPI上行天线开关  TX_ANT_SEL_MIPI_NUM =4 */
    PHY_MIPIDEV_UNIT_STRU               astMipiTunerTx[ANT_TUNER_MIPI_NUM];     /* MIPI上行TUNER开关 支持收发分离的模式需要配置 */


    PHY_MIPIDEV_UNIT_STRU               stMipiPaOnCtrl;                         /* MIPI PA On控制 */
    PHY_MIPIDEV_UNIT_STRU               stMipiPaOffCtrl;                        /* MIPI PA OFF控制 */
    PHY_MIPIDEV_UNIT_STRU               astMipiPaHighMode[PA_MODE_MIPI_NUM];    /* MIPI PA高增益控制字 PA_MODE_MIPI_NUM=4 */
    PHY_MIPIDEV_UNIT_STRU               astMipiPaMidMode[PA_MODE_MIPI_NUM];     /* MIPI PA中增益控制字 PA_MODE_MIPI_NUM=4 */
    PHY_MIPIDEV_UNIT_STRU               astMipiPaLowMode[PA_MODE_MIPI_NUM];     /* MIPI PA低增益控制字 PA_MODE_MIPI_NUM=4 */
    PHY_MIPIDEV_UNIT_STRU               astMipiPaUltraLowMode[PA_MODE_MIPI_NUM];

} NV_BAND_UL_MIPI_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_UL_FEM_SINGLE_ANTENNA_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 前端上行配置
*****************************************************************************/
typedef struct
{
    NV_BAND_UL_GPIO_PARA_STRU           stBandUlGpioPara;                       /* 上行GPIO配置 */
    NV_BAND_UL_MIPI_PARA_STRU           stBandUlMipiPara;                       /* 上行MIPI配置 */

}NV_BAND_UL_FEM_SINGLE_ANTENNA_PARA_STRU;

/*****************************************************************************
 结构名    : NV_BAND_UL_FEM_PATH_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 前端上行配置，每个PATH的配置
*****************************************************************************/
typedef struct
{
    NV_BAND_UL_FEM_SINGLE_ANTENNA_PARA_STRU astBandUlFemPara[MAX_TX_ANT_NUM];   /*按照天线数定义上行FEM 结构*/
    PHY_MIPIDEV_UNIT_STRU                   astMipiAntTxTrig[MAX_TX_ANT_NUM];
    PHY_MIPIDEV_UNIT_STRU                   astMipiPaTrig[MAX_TX_ANT_NUM];

}NV_BAND_UL_FEM_PATH_PARA_STRU;

/*****************************************************************************
结构名    : NV_RF_UL_FEM_PATH_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 前端上行配置，所有PATH的配置
*****************************************************************************/
typedef struct
{
    NV_MODEM_OPERATING_BAND_STRU        stBand;
    UINT16                              bitLteNvValidFalg     :1;
    UINT16                              bitRsv0               :2;
    UINT16                              bitRsv1               :2;
    UINT16                              bitRsv2               :2;
    UINT16                              bitRsv3               :2;
    UINT16                              bitRsv4               :2;
    UINT16                              bitRsv5               :2;
    UINT16                              bitRsv6               :3;

    NV_BAND_UL_FEM_PATH_PARA_STRU       stBandUlFemPathPara;
}NV_RF_UL_FEM_PATH_STRU;                                                      /* L模:上行FEM Path,按照1个Path 1个NV ID，其它模 1个Path */

/*****************************************************************************
结构名    : NV_BAND_UL_FEM_PATH_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : LTE 上行FEM Path NV 软件加载构，不开放 NV,其它模不可见
*****************************************************************************/
typedef struct
{
    NV_MODEM_OPERATING_BAND_STRU        stBand;
    UINT16                              bitLteNvValidFalg     :1;
    UINT16                              bitRsv0               :2;
    UINT16                              bitRsv1               :2;
    UINT16                              bitRsv2               :2;
    UINT16                              bitRsv3               :2;
    UINT16                              bitRsv4               :2;
    UINT16                              bitRsv5               :2;
    UINT16                              bitRsv6               :3;

    NV_BAND_UL_FEM_PATH_PARA_STRU astBandUlFemPathPara[LPHY_UL_PATH_NUM ];
}NV_MODEM_RF_UL_FEM_PATH_STRU;                                                /* 软件使用，不开放成NV*/
/*****************************************************************************
 结构名    : NV_LNA_CTRL_ENUM
 协议表格  :
 ASN.1描述 :
 结构说明  : 外置LNA配置方式，GPIO、MIPI
*****************************************************************************/
typedef enum __NV_EXT_LNA_CTRL_ENUM__
{
    EXT_NA_CTRL_NOT                        = 0,
    EXT_LNA_GPIO_CTRL                       = 1,
    EXT_LNA_MIPI_CTRL                       = 2,
    EXT_LNA_GPIO_MIPI_CTRL                  = 4,
    EXT_LNA_CTRL_BUTT                       = 5

}NV_EXT_LNA_CTRL_ENUM;

typedef UINT16 NV_EXT_LNA_CTRL_ENUM_UINT16;

/*****************************************************************************
 结构名    : NV_EXT_LNA_ANTINFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : LNA相关配置
*****************************************************************************/
typedef struct
{

    NV_EXT_LNA_CTRL_ENUM_UINT16         enLnaCtrlInd;                           /* LNA控制方式 */
    UINT16                              usGpioLnaLevelNum;                      /* Lna支持的档位 */

    UINT32                              ulGpioLnaMask;                          /* LNA GPIO线控 */
    UINT32                              ulGpioLnaMaskExt;

    UINT32                              aulGpioLnaGain[EXT_LNA_LEVEL_NUM];      /* GPIO LNA增益 */
    UINT32                              aulGpioLnaGainExt[EXT_LNA_LEVEL_NUM];

    PHY_MIPIDEV_UNIT_STRU               astMipilnaBandSel[EXT_LNA_BAND_SEL_NUM];/* MIPI LAN band选择 */

    PHY_MIPIDEV_UNIT_STRU               astMipiLnaOff[EXT_LNA_OFF_NUM];         /* MIPI LAN OFF */

    PHY_MIPIDEV_UNIT_STRU               astMipiLnaGain0[LNA_SINGLE_GAIN_MIPI_NUM]; /* MIPI LAN GAIN0配置 */
    PHY_MIPIDEV_UNIT_STRU               astMipiLnaGain1[LNA_SINGLE_GAIN_MIPI_NUM]; /* MIPI LAN GAIN1配置 */
    PHY_MIPIDEV_UNIT_STRU               astMipiLnaGain2[LNA_SINGLE_GAIN_MIPI_NUM]; /* MIPI LAN GAIN2配置 */
    PHY_MIPIDEV_UNIT_STRU               astMipiLnaGain3[LNA_SINGLE_GAIN_MIPI_NUM]; /* MIPI LAN GAIN3配置 */

    UINT16                              ausNoblockLnaGainIdx[AGC_MAX_GAIN_NUM]; /* 实际的AGC NOBLOCK档位索引外置LNA的档位 */
    UINT16                              ausBlockLnaGainIdx[AGC_MAX_GAIN_NUM];   /* 实际的AGC BLOCK档位索引外置LNA的档位 */
    UINT16                              ausAcsLnaGainIdx[AGC_MAX_GAIN_NUM];     /* 实际的ACS NOBLOCK档位索引外置LNA的档位 */

}NV_EXT_LNA_ANTINFO_STRU;

/*****************************************************************************
 结构名    : NV_BAND_EXT_LNA_PATH_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 外置LNA按照天线数定义结构信息
*****************************************************************************/
typedef struct
{
    NV_EXT_LNA_ANTINFO_STRU                stMainAntLnaInfo;
    NV_EXT_LNA_ANTINFO_STRU                stDivAntLnaInfo;
    NV_EXT_LNA_ANTINFO_STRU                stAuxMainAntLnaInfo;
    NV_EXT_LNA_ANTINFO_STRU                stAuxDivAntLnaInfo;
    PHY_MIPIDEV_UNIT_STRU               astLnaTrig[LNA_MIPI_TRIG_NUM];          /*4RX 对应的Extra Lna MIPI控制同时生效*/

}NV_BAND_EXT_LNA_PATH_PARA_STRU;


/*****************************************************************************
 结构名    : NV_RF_EXT_LNA_PATH_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 外置LNA 单个 Path 总结构参数结构体定义，包括Band号，Path信息
*****************************************************************************/
typedef struct
{
    NV_MODEM_OPERATING_BAND_STRU        stBand;
    UINT16                              bitLteNvValidFalg     :1;
    UINT16                              bitRsv0               :2;
    UINT16                              bitRsv1               :2;
    UINT16                              bitRsv2               :2;
    UINT16                              bitRsv3               :2;
    UINT16                              bitRsv4               :2;
    UINT16                              bitRsv5               :2;
    UINT16                              bitRsv6               :3;

    NV_BAND_EXT_LNA_PATH_PARA_STRU      stExtLnaPathPara;
}NV_RF_EXT_LNA_PATH_STRU;                                                       /*LTE下行LNA6个path占用6个NV ID,其它模1个*/


/*****************************************************************************
 结构名    : NV_MODEM_RF_LNA_PATH_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 外置LNA L模按照6个 Path 总结构参数结构体定义，进行NV 加载
*****************************************************************************/
typedef struct
{
    NV_MODEM_OPERATING_BAND_STRU        stBand;                                 /* 软件使用，不开放成NV*/
    UINT16                              bitLteNvValidFalg     :1;
    UINT16                              bitRsv0               :2;
    UINT16                              bitRsv1               :2;
    UINT16                              bitRsv2               :2;
    UINT16                              bitRsv3               :2;
    UINT16                              bitRsv4               :2;
    UINT16                              bitRsv5               :2;
    UINT16                              bitRsv6               :3;

    NV_RF_EXT_LNA_PATH_STRU         astExtLnaPathPara[LPHY_DL_PATH_NUM];
}NV_MODEM_RF_LNA_PATH_STRU;

/*****************************************************************************
 结构名    : NV_BAND_DL_PATHUSED_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CA中下行Band号，以及Band使用的Path信息
*****************************************************************************/
typedef struct
{
    NV_MODEM_OPERATING_BAND_STRU        stBand;
    UINT16                              bitRxAntNum            :3;               /* 2:2天线；4:4天线 */
    UINT16                              bitDlPathIdx           :3;               /* 0~7*/
    UINT16                              bitCompFemPathValidFlg :1;               /* 是否使用在基准通道之上的补偿通道标记，装备需要根据此判断BT标示*/
    UINT16                              bitRsv0                :4;
    UINT16                              bitRsv1                :3;
    UINT16                              bitRsv2                :2;
    INT16                               sMainRxAntRfILComp;                     /* CA组合下可用于补偿DlPath插损耗,LTE用，GUC不用 */
    INT16                               sDivRxAntRfILComp;                      /* CA组合下可用于补偿DlPath插损耗,LTE用，GUC不用 */
    INT16                               sAuxMainRxAntRfILComp;                  /* CA组合下可用于补偿DlPath插损耗,LTE用，GUC不用 */
    INT16                               sAuxDivRxAntRfILComp;                   /* CA组合下可用于补偿DlPath插损耗,LTE用，GUC不用 */

}NV_BAND_DL_PATHUSED_INFO_STRU;


/*****************************************************************************
 结构名    : NV_BAND_UL_PATHUSED_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CA中上行Band号，以及Band使用的Path信息
*****************************************************************************/
typedef struct
{
    NV_MODEM_OPERATING_BAND_STRU        stBand;
    UINT16                              bitTxAntNum            :2;              /* 1:1天线；2:2天线;增加天线数用于CA组合判断
                                                                                       存在天线数不同使用PathIdx不同 */
    UINT16                              bitUlPathIdx           :3;              /* 0~7*/
    UINT16                              bitCompFemPathValidFlg :1;              /* 是否使用在基准通道之上的补偿通道标记，装备需要根据此判断BT标示*/
    UINT16                              bitRsv0                :4;
    UINT16                              bitRsv1                :3;
    UINT16                              bitRsv2                :3;
    INT16                               sMainTxAntRfILComp;                     /* CA组合下可用于计算DlPath插入损耗，补偿基准通道 */
    INT16                               sDivTxAntRfILComp;                      /* CA组合下可用于计算DlPath插入损耗，补偿基准通道 */
    PHY_MIPIDEV_UNIT_STRU               astInterSubFrmAntSel[RX_ANTENNA_MIPI_NUM];  /*异时隙配比开关配置*/

}NV_BAND_UL_PATHUSED_INFO_STRU;


/*****************************************************************************
 结构名    : NV_COMBAND_CC_LIST_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CA中CC 组合信息
*****************************************************************************/
typedef struct
{
    NV_MODEM_OPERATING_BAND_STRU        ausDlBandList[DL_CA_BAND_NUM];          /*DL BandList*/
    NV_MODEM_OPERATING_BAND_STRU        ausUlBandList[UL_CA_BAND_NUM];          /*UL BandList*/
    UINT16                              usRsv0;

}NV_COMBAND_CC_LIST_STRU;

/*****************************************************************************
 结构名    : NV_LTE_RF_CA_BAND_CONFIG_PATH_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : CA NV 总结构参数结构体定义，包括Band号，Path信息
*****************************************************************************/
typedef struct
{
    UINT16                              usDlBandNum;                            /*DL Band Num: CA>1,If BandNum=0,this NV is invalid.*/
    UINT16                              usUlBandNum;
    NV_COMBAND_CC_LIST_STRU             stComBandCcList;
    PHY_MIPIDEV_UNIT_STRU               astMipiCaTuner[ANT_TUNER_MIPI_NUM];
    PHY_MIPIDEV_UNIT_STRU               astCommDSubFrmAntSel[RX_ANTENNA_MIPI_NUM];
    PHY_MIPIDEV_UNIT_STRU               astCommUSubFrmAntSel[RX_ANTENNA_MIPI_NUM];
    NV_BAND_DL_PATHUSED_INFO_STRU       astBandDlPathUsedInfo[DL_CA_BAND_NUM];  /* DL_CA_BAND_NUM = 5*/
    NV_BAND_UL_PATHUSED_INFO_STRU       astBandUlPathUsedInfo[UL_CA_BAND_NUM];  /* UL_CA_BAND_NUM = 2*/

} NV_LTE_RF_CA_BAND_CONFIG_PATH_STRU;

/*****************************************************************************
 结构名    : NV_BAND_CONFIG_TIME_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 时序NV结构参数结构体定义
*****************************************************************************/
typedef struct
{
    UINT16 usBandNum;                                                           /*每个Path有效的 Band总数 */
    UINT16 usRsv;
    NV_UL_CONFIG_TIME_PARA_STRU         astTimePara[MAX_BAND_NUM];                      /*每个Path最大支持32个Band Time调试 */
}NV_BAND_CONFIG_TIME_PARA_STRU;


/*****************************************************************************
 结构名    : NV_LTE_RF_UL_BAND_CONFIG_TIME_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 时序NV结构参数结构体，NV显示结构
*****************************************************************************/
typedef struct
{

    NV_BAND_CONFIG_TIME_PARA_STRU       stFeTimePathPara;
}NV_LTE_RF_UL_BAND_CONFIG_TIME_STRU;

/*****************************************************************************
 结构名    : NV_MODEM_RF_UL_BAND_CONFIG_TIME_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 时序NV结构参数结构体，NV软件加载结构
*****************************************************************************/
typedef struct
{

    NV_LTE_RF_UL_BAND_CONFIG_TIME_STRU  astUlFeTimePathPara[LPHY_UL_PATH_NUM ];       /* 软件使用，不开放成NV*/
}NV_MODEM_RF_UL_BAND_CONFIG_TIME_STRU;

/*****************************************************************************
 结构名    : NV_MODEM_RF_DL_BAND_CONFIG_TIME_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 时序NV结构参数结构体，NV软件加载结构
*****************************************************************************/
typedef struct
{

    NV_DL_CONFIG_TIME_PARA_STRU  astDLFeTimePathPara[LPHY_DL_PATH_NUM];       /* 软件使用，不开放成NV*/
}NV_MODEM_RF_DL_BAND_CONFIG_TIME_STRU;

/*****************************************************************************
 结构名    : NV_RF_MIPI_INIT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : RFIC 初始化结构参数结构体定义
*****************************************************************************/
typedef struct
{


    UINT32                              bitSlaveId:8;
    UINT32                              bitRegAddr:16;                              /* Reg Addr: 0~65535 ,Extended Write Long MIPI*/
    UINT32                              bitRegData:8;

}NV_RF_MIPI_INIT_STRU;

/*****************************************************************************
 结构名    : NV_MODEM_RF_RFIC_INIT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : RFIC 初始化结构参数NV名称结构体定义
*****************************************************************************/
typedef struct
{
    UINT16 usRegNum;                                                            /* Single RFIC Init LUT Reg Num */
    UINT16 usRsv;
    NV_RF_MIPI_INIT_STRU                astUint[RFIC_INIT_REG_NUM];

}NV_MODEM_RF_RFIC_INIT_STRU;

/*****************************************************************************
 结构名    : NV_RFIC_CONNECT_ABB_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : RFIC和ABB的连接关系
*****************************************************************************/
typedef struct
{
    UINT16                              usRficChipId;                          /* 同一个MIPI控制2个RFIC，RFIC chip ID的值，0--chipId下拉，1--chipId上拉*/
    UINT16                              usRficMipiPortSel;                      /* MIPI Port选择 */
    UINT16                              ausRficRxCh[MAX_RFIC_RX_CH_NUM];        /* 表示每个RFIC的RX1,Rx2,RX3分别与ABB通道的连接情况,实际填写ABB的通道号[0,9] */
    UINT16                              ausRficABCrossFlag[MAX_RFIC_RX_CH_NUM]; /* RFIC（主分集）与ABB的通道主分集是否有交叉 */
    UINT16                              usMrxCh;                                /* RFIC MRX IQ和ABB IQ连接关系,填ABB MRX IQ号 */

    UINT16                              ausRficTxCh[MAX_RFIC_TX_CH_NUM];        /* 表示每个RFIC的TX1,TX2分别与ABB TX通道的连接情况，填写ABB的TX IQ通道号 */
    UINT16                              ausRficTxChCrossFlag[MAX_RFIC_TX_CH_NUM];/* RFIC TX IQ正负与ABB的 IQ正负是否有交叉,待确认? */
    UINT16                              usRsv;
}NV_RFIC_CONNECT_ABB_PARA_STRU;

/*****************************************************************************
 结构名    : NV_MODEM_RF_HW_BASIC_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : RFIC和ABB的连接关系
*****************************************************************************/
typedef struct
{
    UINT16                              usRifcNum;
    UINT16                              usRsv;
    NV_RFIC_CONNECT_ABB_PARA_STRU       astRficConnectAbb[MAX_RFIC_NUM];
}NV_MODEM_RF_HW_BASIC_INFO_STRU;

/*****************************************************************************
 结构名    : NV_RF_RX_CAL_FREQ_LIST_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 需要校准的频点列表，每个BAND最多支持32个频点，该NV区分BAND号
*****************************************************************************/
typedef struct
{
    UINT16                              usValidCalFreqNum;                      /* 需要校准的频点个数 */
    UINT16                              usRsv;
    UINT16                              aulRxCalFreqList[RF_RX_MAX_FREQ_NUM];   /* RF_RX_FREQ_NUM =32 ,GUTL单位100KHz,CDMA:1KHz*/

}NV_RF_RX_CAL_FREQ_LIST_STRU;


/*****************************************************************************
 结构名    : NV_RX_AGC_GAIN_THRESHOLD_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC切换门限，包括BLOCK、NOBLOCK和ACS
*****************************************************************************/
typedef struct
{
    INT16                               asNoBlockInc[AGC_MAX_GAIN_NUM];         /* NOBLOCK的上门限，AGC_MAX_GAIN_NUM = 8，单位：1dB */
    INT16                               asNoBlockDec[AGC_MAX_GAIN_NUM];         /* NOBLOCK的下门限，AGC_MAX_GAIN_NUM = 8，单位：1dB */
    INT16                               asBlockInc[AGC_MAX_GAIN_NUM];           /* BLOCK的上门限，BLOCK改为8档，AGC_MAX_GAIN_NUM = 8，单位：1dB */
    INT16                               asBlockDec[AGC_MAX_GAIN_NUM];           /* BLOCK的下门限，BLOCK改为8档，AGC_MAX_GAIN_NUM = 8，单位：1dB */
    INT16                               asAcsInc[AGC_MAX_GAIN_NUM];             /* ACS的上门限，GUC不用，AGC_MAX_GAIN_NUM = 8，单位：1dB */
    INT16                               asAcsDec[AGC_MAX_GAIN_NUM];             /* ACS的上门限，GUC不用，AGC_MAX_GAIN_NUM = 8，单位：1dB */
}NV_RX_AGC_GAIN_THRESHOLD_STRU;

/*****************************************************************************
 结构名    : NV_ALG_RX_AGC_SWITCH_THRESHOLD_TABLE_BANK_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC切换门限资源池，最多支持10张表，包含10种不同的AGC切换门限
*****************************************************************************/
typedef struct
{
    NV_RX_AGC_GAIN_THRESHOLD_STRU       astAgcSwitchTable[AGC_MAX_TABLE_NUM];   /*AGC_MAX_TABLE = 10*/

}NV_ALG_RX_AGC_SWITCH_THRESHOLD_TABLE_BANK_STRU;

/*****************************************************************************
 结构名    : NV_RX_AGC_GAIN_DEFAULT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC的默认GAIN值，包含BLOCK、NOBLOCK和ACS
*****************************************************************************/
typedef struct
{
    UINT16                              ausGainNBlock[AGC_MAX_GAIN_NUM];        /* NOBLOCK的默认AGC GAIN值，AGC_MAX_GAIN_NUM = 8，单位:1/8 dB */
    UINT16                              ausGainBlock[AGC_MAX_GAIN_NUM];         /* BLOCK的默认AGC GAIN值，支持8档AGC，AGC_MAX_GAIN_NUM = 8，单位:1/8 dB */
    UINT16                              ausGainAcs[AGC_MAX_GAIN_NUM];           /* ACS的默认AGC GAIN值，GUC不用，AGC_MAX_GAIN_NUM = 8，单位:1/8 dB */

}NV_RX_AGC_GAIN_DEFAULT_STRU;

/*****************************************************************************
 结构名    : NV_RF_RX_AGC_GAIN_TABLE_BANK_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC默认GAIN值的资源池，最多支持10张表，包含10种不同的AGC DEFAULT GAIN
*****************************************************************************/
typedef struct
{
    NV_RX_AGC_GAIN_DEFAULT_STRU         astAgcGainDefaultTable[AGC_MAX_TABLE_NUM];  /*AGC_MAX_TABLE = 10*/

}NV_RF_RX_AGC_GAIN_TABLE_BANK_STRU;

/*****************************************************************************
 结构名    : NV_RX_AGC_RFIC_TABLE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC的RFIC控制字，包含BLOCK、NOBLOCK和ACS
*****************************************************************************/
typedef struct
{
    UINT16                              ausRfTableNoBlok[AGC_MAX_GAIN_NUM];     /* NOBLOCK下AGC的RFIC控制字，AGC_MAX_GAIN_NUM = 8 */
    UINT16                              ausRfTableBlock[AGC_MAX_GAIN_NUM];      /* BLOCK下AGC的RFIC控制字，支持8档，AGC_MAX_GAIN_NUM = 8 */
    UINT16                              ausRfTableAcs[AGC_MAX_GAIN_NUM];        /* ACS下AGC的RFIC控制字，GUC不用，AGC_MAX_GAIN_NUM = 8 */

}NV_RX_AGC_RFIC_TABLE_STRU;

/*****************************************************************************
 结构名    : NV_RF_RX_AGC_RFIC_TABLE_BANK_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC的RFIC控制字资源池，最多支持10张表，包含10种不同的RFIC控制字
*****************************************************************************/
typedef struct
{
    NV_RX_AGC_RFIC_TABLE_STRU           astAgcRficTable[AGC_MAX_TABLE_NUM];     /*AGC_MAX_TABLE = 10*/

}NV_RF_RX_AGC_RFIC_TABLE_BANK_STRU;

/*****************************************************************************
 结构名    : NV_RX_AGC_TABLE_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 某一个PATH中每根天线所选择的AGC TABLE INDEX，根据INDEX去索引对应的
 AGC的切换门限、AGC DEFAULT GAIN和AGC的RFIC控制字
*****************************************************************************/
typedef struct
{
    UINT32                              bitMainAntRxAgcTableIdx         :4;     /* 主集天线对应的AGC TABLE INDEX */
    UINT32                              bitDivAntRxAgcTableIdx          :4;     /* 分集天线对应的AGC TABLE INDEX */
    UINT32                              bitAuxMainAntRxAgcTableIdx      :4;     /* GUC不用，辅助主集天线对应的AGC TABLE INDEX */
    UINT32                              bitAuxDivAntRxAgcTableIdx       :4;     /* GUC不用，辅助分集天线对应的AGC TABLE INDEX 理论上4bit可以满足，为了4字节对齐，采用了8bit*/
    UINT32                              bitRsv0                           :4;
    UINT32                              bitRsv1                           :4;
    UINT32                              bitRsv2                           :4;
    UINT32                              bitRsv3                           :4;
}NV_RX_AGC_TABLE_INFO_STRU;

/*****************************************************************************
 结构名    : NV_RF_RX_AGC_TABLE_USED_INFO_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC的TABLE INDEX，最多支持6个PATH，GUC只需要使用一个PATH，区分BAND
*****************************************************************************/
typedef struct
{
    NV_RX_AGC_TABLE_INFO_STRU           astAgcTableInfoDlPath[LPHY_DL_PATH_NUM];

}NV_RF_RX_AGC_TABLE_USED_INFO_STRU;


/*****************************************************************************
 结构名    : NV_RX_PATH_ILOSS_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 某一个PATH中每根天线所选择的插损值
*****************************************************************************/
typedef struct
{
    INT16                               usMainAntRffeIloss;                     /* 主集天线对应的插损值，单位:1db */
    INT16                               ustDivAntRffeIloss;                     /* 分集天线对应的插损值，单位:1db */
    INT16                               usAuxMainAntRffeIloss;                  /* GUC不用，辅助主集天线对应的插损值，单位:1db */
    INT16                               usAuxDivAntRffeIloss;                  /* GUC不用，辅助分集天线对应的插损值，单位:1db，理论上4bit可以满足，为了4字节对齐，采用了8bit*/

}NV_RX_PATH_ILOSS_STRU;

/*****************************************************************************
 结构名    : NV_RF_RX_RFFE_ILOSS_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 前端的插损值，最多支持6个PATH，GUC只用一个PATH，区分BAND
*****************************************************************************/
typedef struct
{
    NV_RX_PATH_ILOSS_STRU               astRffeIlossDlPath[LPHY_DL_PATH_NUM];

}NV_RF_RX_RFFE_ILOSS_STRU;

/*****************************************************************************
 结构名    : NV_RX_AGC_GAIN_LEVEL_CAL_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC的校准结果，包含插损值，但是不包含频补值
*****************************************************************************/
typedef struct
{
    UINT16                              ausGainLevelNoBlock[AGC_MAX_GAIN_NUM];  /* NOBLOCK的AGC校准值，单位:1/8dB，AGC_MAX_GAIN_NUM = 8 */
    UINT16                              ausGainLevelBlock[AGC_MAX_GAIN_NUM];    /* BLOCK的AGC校准值，单位:1/8dB，AGC_MAX_GAIN_NUM = 8 */
    UINT16                              ausGainLevelAcs[AGC_MAX_GAIN_NUM];      /* GUC不用，ACS的AGC校准值，单位:1/8dB，AGC_MAX_GAIN_NUM = 8 */

}NV_RX_AGC_GAIN_LEVEL_CAL_STRU;

/*****************************************************************************
 结构名    : NV_RX_AGC_FREQ_COMP_CAL_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC的频补校准结果
*****************************************************************************/
typedef struct
{
    INT16                               ausFreqComp[RF_RX_MAX_FREQ_NUM];        /* RF_RX_MAX_FREQ_NUM = 32 */

}NV_RX_AGC_FREQ_COMP_CAL_STRU;

/*****************************************************************************
 结构名    : NV_RX_AGC_CAL_RESULT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC某一根天线对应的总的校准结果
*****************************************************************************/
typedef struct
{
    NV_RX_AGC_GAIN_LEVEL_CAL_STRU       stGainLevelCalResult;                   /* 包含了AGC_GAIN_DEFAULT和插损，但是不含有频补值，频补需要再做插值计算*/
    NV_RX_AGC_FREQ_COMP_CAL_STRU        astFreqCalResult[EXT_LNA_GIAN_CAL_NUM]; /* AGC的频补校准值 */
    INT16                               sRffeIloss;                              /* 校准时使用的插损值 */
    INT16                               sRsv;

}NV_RX_AGC_CAL_RESULT_STRU;

/*****************************************************************************
 结构名    : NV_RX_AGC_CAL_RESULT_TABLE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : AGC某一个PATH对应的总的校准结果，GUC只用一个PATH，区分BAND
*****************************************************************************/
typedef struct
{
    NV_RX_AGC_CAL_RESULT_STRU           astMainAntAgcCalResult;                 /* 主集天线对应的AGC校准结果 */
    NV_RX_AGC_CAL_RESULT_STRU           astDivAntAgcCalResult;                  /* 分集天线对应的AGC校准结果 */
    NV_RX_AGC_CAL_RESULT_STRU           astAuxMainAntAgcCalResult;              /* GUC不用，辅助主集天线对应的AGC校准结果 */
    NV_RX_AGC_CAL_RESULT_STRU           astAuxDivAntAgcCalResult;               /* GUC不用，辅助分集天线对应的AGC校准结果 */

}NV_RX_AGC_CAL_RESULT_TABLE_STRU;

/*****************************************************************************
 结构名    : NV_RX_DCOC_COMP_VAlUE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : DCOC的校准码字和BBP上报的残余直流值
*****************************************************************************/
typedef struct
{
    UINT16                              usDciCode;                              /* 校准的I路的DCOC码字 */
    UINT16                              usDcqCode;                              /* 校准的Q路的DCOC码字 */
    INT16                               sRemainDigitalDci;                      /* 校准时BBP上报的I路的残余直流值 */
    INT16                               sRemainDigitalDcq;                      /* 校准时BBP上报的Q路的残余直流值 */

}NV_RX_DCOC_COMP_VAlUE_STRU;

/*****************************************************************************
 结构名    : NV_RX_DCOC_CAL_RESULT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 某一个PATH中每根天线的DCOC的校准结果，按照VGA档位校准的，最多包含16组
*****************************************************************************/
typedef struct
{
    NV_RX_DCOC_COMP_VAlUE_STRU          astDcocComp[MAX_VGA_GAIN_NUM];

}NV_RX_DCOC_CAL_RESULT_STRU;

/*****************************************************************************
 结构名    : NV_RX_DCOC_CAL_RESULT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  : 某一个PATH的DCOC的校准结果
*****************************************************************************/
typedef struct
{
    NV_RX_DCOC_CAL_RESULT_STRU          astMainAntCalResult;                    /* 主集的DCOC校准结果 */
    NV_RX_DCOC_CAL_RESULT_STRU          astDivAntCalResult;                     /* 分集的DCOC校准结果 */
    NV_RX_DCOC_CAL_RESULT_STRU          astAuxMainAntCalResult;                 /* GUC不用，辅助主集的DCOC校准结果 */
    NV_RX_DCOC_CAL_RESULT_STRU          astAuxDivAntCalResult;                  /* GUC不用，辅助分集的DCOC校准结果 */

}NV_RX_PATH_DCOC_CAL_RESULT_STRU;


/*****************************************************************************
 结构名    : NV_LTE_IP2_USE_APC_CODE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    UINT16                              usTxDefaultPowHIdx;                     /*由产品线填写校准默认功率的Index,物理层到默认的RFIC Table表中查对应使用的
                                                                                APC Code（default power与APC Code一一对应,需要加上APC表的起始点计算RFIC的APC code index）*/
    UINT16                              ausFreq[CAL_IP2_FREQ_NUM];

}NV_LTE_IP2_USE_APC_CODE_STRU;

/*****************************************************************************
 结构名    : NV_LTE_IP2_CAl_PATH_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    UINT08                              ucCalDlPathIdx;                         /* Ip2校准下行的pathIdx */
    UINT08                              ucUseUlPathIdx;                         /* 校准使用的上行的pathIdx */
    UINT08                              bitCalMainAntValidFlg       :1;
    UINT08                              bitCalDivAntValidFlg        :1;
    UINT08                              bitCalAuxMainAntValidFlg    :1;
    UINT08                              bitCalAuxDivAntValidFlg     :1;
    UINT08                              bitRsv0                     :4;
    UINT08                              ucRsv;
    NV_LTE_IP2_USE_APC_CODE_STRU        astRxIp2Para;

}NV_LTE_IP2_CAl_PATH_PARA_STRU;
/*****************************************************************************
 结构名    : NV_LTE_CAL_IP2_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    UINT08                              ucIp2CalDlPathNum;                      /* IP2校准Path数 */
    UINT08                              ucRsv0;
    UINT08                              ucRsv1;
    UINT08                              ucRsv2;
    NV_LTE_IP2_CAl_PATH_PARA_STRU       astRxIp2PathPara[LPHY_DL_PATH_NUM];

}NV_LTE_CAL_IP2_PARA_STRU;                                                    /* 单个NV较小，建议按照Band预留 */

/*****************************************************************************
 结构名    : NV_RX_IP2_IQREG_CODE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    INT16                               sIRegCode;                              /*需要确认下UMTS用INT16的原因，L在V7r5上用的是UINT16*/
    INT16                               sQRegCode;

}NV_RX_IP2_IQREG_CODE_STRU;

/*****************************************************************************
 结构名    : NV_RX_IP2_CAL_RESULT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    NV_RX_IP2_IQREG_CODE_STRU           stMainAntDivOn;
    NV_RX_IP2_IQREG_CODE_STRU           stMainAntDivOff;                        /*RFIC 确认不需要单独校准分集的Ip2,删除以前版本中关于校准分集的部分*/

}NV_RX_IP2_CAL_RESULT_STRU;

/*****************************************************************************
 结构名    : NV_RX_PATH_IP2_CAL_RESULT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    NV_RX_IP2_CAL_RESULT_STRU           astRxIp2CalResultFreq[MAX_IP2_CAL_FREQ_NUM];
}NV_RX_PATH_IP2_CAL_RESULT_STRU;

typedef struct
{
    NV_RX_PATH_IP2_CAL_RESULT_STRU      astIp2CalResultPath[LPHY_DL_PATH_NUM];
}NV_LTE_RX_IP2_CAL_RESULT_STRU;

/*****************************************************************************
 结构名    : NV_LTE_TXIQ_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    UINT16                              usTxDefaultPowHIdx;                     /*由产品线填写校准默认功率的Index,物理层到默认的RFIC Table表中查对应使用的
                                                                                APC Code（default power与APC Code一一对应,需要加上APC表的起始点计算RFIC
                                                                                的APC code index）*/
    UINT16                              usRficMrxGainIdx;                       /*填写Mrx实际对应的增益档位，默认值：0~F*/

}NV_LTE_TXIQ_PARA_STRU;

/*****************************************************************************
 结构名    : NV_ID_LTE_TX_CAL_TXIQ_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    NV_LTE_TXIQ_PARA_STRU               astTxiqParaPath[LPHY_UL_PATH_NUM ];

}NV_ID_LTE_TX_CAL_TXIQ_PARA_STRU;

/*****************************************************************************
 结构名    : NV_TX_CAL_DEFAULT_POWER_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    INT32                               bitFreqComDefaultPowerHIdx      :6;     /* 频补的校准目标功率默认从高功率中选,0~40 */
    INT32                               bitPowerLowDiff                 :6;     /* 每个path功率与基准目标功率固定差，-64~64，1/8 dB */
    INT32                               bitPowerMidDiff                 :6;
    INT32                               bitPowerHighDiff                :6;
    INT32                               bitRsv                          :8;

}NV_TX_CAL_DEFAULT_POWER_STRU;

/*****************************************************************************
 结构名    : NV_LTE_TX_CAL_DEFAULT_POWER_TABLE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{

    NV_TX_CAL_DEFAULT_POWER_STRU        astTxDefaultPowPath[LPHY_UL_PATH_NUM ];
    INT16                               asRfDefaultPowerLow[RF_TX_GAIN_6363_NUM][BBGAIN_NUM_PER_RFGAIN];/*上行校准天线口基准目标功率；RF_TX_GAIN_6363_NUM = 20,RF_BBGAIN_NUM_PER_RFGAIN = 2 */
    INT16                               asRfDefaultPowerMid[RF_TX_GAIN_6363_NUM][BBGAIN_NUM_PER_RFGAIN];
    INT16                               asRfDefaultPowerHigh[RF_TX_GAIN_6363_NUM][BBGAIN_NUM_PER_RFGAIN];

}NV_LTE_TX_CAL_DEFAULT_POWER_TABLE_STRU;

/*****************************************************************************
 结构名    : NV_LTE_APC_TABLE_PARA_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    UINT16                              ausApcTableStartPointEachPalevel[RF_PA_GAIN_NUM];
    UINT16                              ausGainNumsNeedCalEachPaLevel[RF_PA_GAIN_NUM];                  /* RF_PA_GAIN_NUM = 3 */
    INT16                               sDbbValue;                                              /* 校准时DBB 衰减值，单位: 1/16 */
    INT16                               sRsv;
}NV_LTE_APC_TABLE_PARA_STRU;

/*****************************************************************************
 结构名    : NV_LTE_TX_CAL_APC_TABLE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{

    NV_LTE_APC_TABLE_PARA_STRU          astTxApcParaPath[LPHY_UL_PATH_NUM ];
    UINT32                              aulLteApcTable[RF_TX_GAIN_6363_NUM][BBGAIN_NUM_PER_RFGAIN]; /* RFIC 校准APC 基准表，每个Band一个，Path共享 */
}NV_LTE_TX_CAL_APC_TABLE_STRU;

/*****************************************************************************
 结构名    : NV_LTE_TX_APC_GAIN_STATUS_POW_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
        INT16 sMaxPow;
        INT16 sMinPow;
}NV_LTE_TX_APC_GAIN_STATUS_POW_STRU;

/*****************************************************************************
 结构名    : NV_LTE_TX_CAL_RESULT_TABLE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{

   NV_LTE_TX_APC_GAIN_STATUS_POW_STRU   astTxPaPower[RF_PA_GAIN_NUM][RF_TX_GAIN_6363_NUM];

}NV_LTE_TX_CAL_APC_RESULT_STRU;
/*****************************************************************************
 结构名    : NV_LTE_TX_CAL_GAIN_COMP_RESULT_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{

    NV_LTE_TX_CAL_APC_RESULT_STRU     astTxCalResultPath[LPHY_UL_PATH_NUM ];

}NV_LTE_TX_CAL_COMP_RESULT_STRU;

/*****************************************************************************
 结构名    : NV_MODEM_TEMP_DEFINE_STRU
 协议表格  :
 ASN.1描述 :
 结构说明  :
*****************************************************************************/
typedef struct
{
    UINT16                            usTempNum;
    UINT16                            usRsv;
    INT16                             asTempList[MAX_TEMP_DEFINE_NUM];
}NV_MODEM_TEMP_DEFINE_STRU;

/*****************************************************************************
  7 UNION定义
*****************************************************************************/








#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif

#endif /* end of nv_common_interface */
