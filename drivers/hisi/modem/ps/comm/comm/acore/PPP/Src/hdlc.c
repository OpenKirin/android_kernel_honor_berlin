/*-
 * Copyright (c) 1996 - 2001 Brian Somers <brian@Awfulhak.org>
 *          based on work by Toshiharu OHNO <tony-o@iij.ad.jp>
 *                           Internet Initiative Japan, Inc (IIJ)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD: stable/9/usr.sbin/ppp/hdlc.c 134789 2004-09-05 01:46:52Z brian $
 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/******************************************************************************
   头文件包含
******************************************************************************/
#include "mdrv.h"
#include "TTFComm.h"
#include "soc_sctrl_interface.h"
#include "PPP/Inc/ppp_public.h"
#include "PPP/Inc/layer.h"
#include "PPP/Inc/ppp_mbuf.h"
#include "PPP/Inc/ppp_fsm.h"
#include "PPP/Inc/hdlc.h"
#include "PPP/Inc/throughput.h"
#include "PPP/Inc/lcp.h"
#include "PPP/Inc/async.h"
#include "PPP/Inc/auth.h"
#include "PPP/Inc/ipcp.h"
#include "PPP/Inc/pppid.h"
#include "PPP/Inc/link.h"
#include "PPP/Inc/ppp_init.h"
#if (FEATURE_ON == FEATURE_HARDWARE_HDLC_FUNC)
#include "PPP/Inc/hdlc_hardware.h"
#endif
	
/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_HDLC_C

#if(FEATURE_ON == FEATURE_PPP)

VOS_UINT16 const fcstab[256] = {
   /* 00 */ 0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
   /* 08 */ 0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
   /* 10 */ 0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
   /* 18 */ 0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
   /* 20 */ 0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
   /* 28 */ 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
   /* 30 */ 0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
   /* 38 */ 0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
   /* 40 */ 0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
   /* 48 */ 0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
   /* 50 */ 0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
   /* 58 */ 0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
   /* 60 */ 0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
   /* 68 */ 0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
   /* 70 */ 0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
   /* 78 */ 0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
   /* 80 */ 0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
   /* 88 */ 0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
   /* 90 */ 0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
   /* 98 */ 0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
   /* a0 */ 0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
   /* a8 */ 0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
   /* b0 */ 0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
   /* b8 */ 0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
   /* c0 */ 0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
   /* c8 */ 0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
   /* d0 */ 0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
   /* d8 */ 0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
   /* e0 */ 0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
   /* e8 */ 0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
   /* f0 */ 0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
   /* f8 */ 0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

#if (FEATURE_ON == FEATURE_HARDWARE_HDLC_FUNC)
VOS_UINT_PTR    g_ulPppVirtAddr         = 0;
VOS_UINT_PTR    g_ulPppPhyAddr          = 0;
VOS_UINT32      g_ulPppTotalBufLen      = 0;
#endif

/******************************************************************************
   5 函数实现
******************************************************************************/
void
hdlc_Init(struct hdlc *hdlc, struct lcp *lcp)
{
  PSACORE_MEM_SET(hdlc, sizeof(struct hdlc), '\0', sizeof(struct hdlc));
  hdlc->lqm.owner = lcp;
}

/*
 *  HDLC FCS computation. Read RFC 1171 Appendix B and CCITT X.25 section
 *  2.27 for further details.
 */
VOS_UINT16
hdlc_Fcs(VOS_CHAR *cp, VOS_UINT32 len)
{
  VOS_UINT16 fcs = INITFCS;

  while (len--)
    fcs = (fcs >> 8) ^ fcstab[(fcs ^ *cp++) & 0xff];

  return fcs;
}

VOS_UINT16
HdlcFcsBuf(VOS_UINT16 fcs, struct ppp_mbuf *m)
{
  VOS_INT32 len;
  VOS_CHAR *pos, *end;

  len = ppp_m_length(m);
  pos = PPP_MBUF_CTOP(m);
  end = pos + m->m_len;
  while (len--) {
    fcs = (fcs >> 8) ^ fcstab[(fcs ^ *pos++) & 0xff]; /* [false alarm]:移植开源代码 */
    if (pos == end && len) {
      m = m->m_next;
      pos = PPP_MBUF_CTOP(m);
      end = pos + m->m_len;
    }
  }
  return (fcs);
}

struct ppp_mbuf *
hdlc_LayerPush(struct link *l, struct ppp_mbuf *bp,
               VOS_INT32 pri, VOS_UINT16 *proto)
{
  return bp;
}

PPP_ZC_STRU *hdlc_LayerPull(struct link *l, PPP_ZC_STRU *pstMem, VOS_UINT16 *proto)
{
  VOS_UINT16    usFcs;
  VOS_UINT16    usLen;

  usLen = PPP_ZC_GET_DATA_LEN(pstMem);

  usFcs = hdlc_Fcs(PPP_ZC_GET_DATA_PTR(pstMem), usLen);

  /* PPP_MNTN_LOG1(PS_PID_APP_PPP, 0, PS_PRINT_NORMAL, "hdlc_LayerPull: fcs = <1>\r\n",usFcs); */

  l->hdlc.lqm.ifInOctets += usLen + 1;        /* plus 1 flag octet! */

  if (usFcs != GOODFCS)
  {
    l->hdlc.lqm.ifInErrors++;
    l->hdlc.stats.badfcs++;
    PPP_MNTN_LOG(PS_PID_APP_PPP, 0, PS_PRINT_WARNING, "bad hdlc fcs\r\n");
    PPP_MemFree(pstMem);

    return VOS_NULL_PTR;
  }

  /* Either done here or by the sync layer */
  l->hdlc.lqm.lqr.InGoodOctets += usLen + 1;  /* plus 1 flag octet! */
  l->hdlc.lqm.ifInUniPackets++;

  /* rfc1662 section 4.3 */
  if (usLen < 4)
  {
    PPP_MemFree(pstMem);

    return VOS_NULL_PTR;
  }

  PPP_MemCutTailData(&pstMem, (VOS_UINT8 *)(&usFcs), 2, PPP_ZC_UL_RESERVE_LEN);

  return pstMem;
}

struct layer hdlclayer = { LAYER_HDLC, "hdlc", hdlc_LayerPush, hdlc_LayerPull };

#if (FEATURE_ON == FEATURE_HARDWARE_HDLC_FUNC)
/*****************************************************************************
 函 数 名  : PPP_PHY_TO_VIRT
 功能描述  : 将PPP中的物理地址转换为虚地址
 输入参数  : unsigned int ulPAddr   需要转换的物理地址
             g_ulPppPhyAddr         PPP物理地址基地址
 输出参数  : 无
 返 回 值  : 对应的虚地址
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年9月25日
    作    者   : x59651
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT_PTR PPP_PHY_TO_VIRT(VOS_UINT_PTR ulPAddr)
{
    return (VOS_UINT_PTR)phys_to_virt(ulPAddr);
}

/*****************************************************************************
 函 数 名  : PPP_VIRT_TO_PHY
 功能描述  : 将PPP中的虚地址转换为物理地址
 输入参数  : VOS_UINT_PTR ulVAddr   需要转换的虚地址
             g_ulPppVirtAddr        PPP虚地址基地址
 输出参数  : 无
 返 回 值  : 对应的物理地址
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年9月25日
    作    者   : x59651
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT_PTR PPP_VIRT_TO_PHY(VOS_UINT_PTR ulVAddr)
{
    return (VOS_UINT_PTR)virt_to_phys((void *)ulVAddr);
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_NODE_PHY_TO_VIRT
 功能描述  : 将PPP中封装解封装的物理地址转换为虚地址
 输入参数  : unsigned int ulPAddr   需要转换的物理地址
 输出参数  : 无
 返 回 值  : 对应的虚地址
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年08月14日
    作    者   : g00178567
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT_PTR PPP_HDLC_NODE_PHY_TO_VIRT(VOS_UINT_PTR ulPAddr)
{
    return VOS_UncacheMemPhyToVirt((VOS_UINT8 *)ulPAddr, (VOS_UINT8 *)g_ulPppPhyAddr, (VOS_UINT8 *)g_ulPppVirtAddr, g_ulPppTotalBufLen);
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_NODE_VIRT_TO_PHY
 功能描述  : 将PPP中封装解封装的虚地址转换为物理地址
 输入参数  : unsigned int ulVAddr   需要转换的虚地址
 输出参数  : 无
 返 回 值  : 对应的物理地址
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年08月14日
    作    者   : g00178567
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT_PTR PPP_HDLC_NODE_VIRT_TO_PHY(VOS_UINT_PTR ulVAddr)
{
    return VOS_UncacheMemVirtToPhy((VOS_UINT8 *)ulVAddr, (VOS_UINT8 *)g_ulPppPhyAddr, (VOS_UINT8 *)g_ulPppVirtAddr, g_ulPppTotalBufLen);
}
/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_InitBuf
 功能描述  : 初始化HDLC需要的内存
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_InitBuf(VOS_VOID)
{
    /* 计算内存长度，实际使用长度为0x6140，申请按照0x8000(32K) */
    g_ulPppTotalBufLen  = PAGE_ALIGN(TTF_HDLC_MASTER_LINK_TOTAL_LEN + PAGE_SIZE);

    /* 调用系统接口dma_alloc_coherent申请uncache内存 */
    g_ulPppVirtAddr     = (VOS_UINT_PTR)VOS_UnCacheMemAlloc(g_ulPppTotalBufLen, &g_ulPppPhyAddr);
    if (VOS_NULL_PTR == g_ulPppVirtAddr)
    {
        PPP_HDLC_ERROR_LOG("PPP_HDLC_HARD_InitBuf, ERROR, VOS_UnCacheMemAlloc Fail\r\n");
        return VOS_ERR;
    }

    PSACORE_MEM_SET((void *)g_ulPppVirtAddr, g_ulPppTotalBufLen, 0, g_ulPppTotalBufLen);

    /* 根据TtfMemoryMap.h初始化HDLC所需内存 */
    g_pstHdlcDefBufInfo = (HDLC_DEF_BUFF_INFO_STRU *)g_ulPppVirtAddr;
    g_pstHdlcFrmBufInfo = (HDLC_FRM_BUFF_INFO_STRU *)(g_ulPppVirtAddr + sizeof(HDLC_DEF_BUFF_INFO_STRU));

    /* TTF_HDLC_MASTER_DEF_BUF_LEN必须与结构HDLC_DEF_BUFF_INFO_STRU的大小一致 */
    if (PPP_HEDL_HRPD_CHECK_STRU_SIZE(TTF_HDLC_MASTER_DEF_BUF_LEN , HDLC_DEF_BUFF_INFO_STRU))
    {
        PPP_HDLC_ERROR_LOG2("PPP_HDLC_HARD_InitBuf, ERROR, TTF_HDLC_MASTER_DEF_BUF_LEN %d sizeof(HDLC_DEF_BUFF_INFO_STRU) %d\r\n",
                      TTF_HDLC_MASTER_DEF_BUF_LEN, sizeof(HDLC_DEF_BUFF_INFO_STRU));
        return VOS_ERR;
    }

    /* TTF_HDLC_MASTER_FRM_BUF_LEN收必须与结构HDLC_FRM_BUFF_INFO_STRU的大小一致 */
    if (PPP_HEDL_HRPD_CHECK_STRU_SIZE(TTF_HDLC_MASTER_FRM_BUF_LEN, HDLC_FRM_BUFF_INFO_STRU))
    {
        PPP_HDLC_ERROR_LOG2("PPP_HDLC_HARD_InitBuf, ERROR, TTF_HDLC_MASTER_FRM_BUF_LEN %d sizeof(HDLC_FRM_BUFF_INFO_STRU) %d\r\n",
                      TTF_HDLC_MASTER_FRM_BUF_LEN, sizeof(HDLC_FRM_BUFF_INFO_STRU));
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_MntnDefTraceInput
 功能描述  : 勾取解封装输入参数链表和数据
 输入参数  : pstFrmBuffInfo -   封装使用的内存
             pstBuildInfo   -   存放输入数据
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_MntnDefTraceInput
(
    HDLC_DEF_BUFF_INFO_STRU         *pstDefBuffInfo,
    HDLC_PARA_LINK_BUILD_INFO_STRU  *pstBuildInfo
)
{
    VOS_UINT32                          ulNodeLoop;
    HDLC_PARA_LINK_NODE_STRU           *pstParaNode;

    if ((g_stHdlcConfigInfo.ulHdlcMntnTraceCfg & PPP_HDLC_MNTN_TRACE_PARA) != 0)
    {
        PPP_HDLC_HARD_MntnTraceInputParaLink(ID_HDLC_MNTN_DEF_INPUT_PARA,
                                             pstBuildInfo->ulInputLinkNodeCnt,
                                             pstBuildInfo->ulInputLinkTotalSize,
                                             &(pstDefBuffInfo->astInputParaLinkNodeBuf[0]));
    }

    if ((g_stHdlcConfigInfo.ulHdlcMntnTraceCfg & PPP_HDLC_MNTN_TRACE_DATA) != 0)
    {
        /* 记录并上报参数链表所有节点的数据内容，每个节点是一个IP包 */
        for ( ulNodeLoop = 0; ulNodeLoop < pstBuildInfo->ulInputLinkNodeCnt; ulNodeLoop++ )
        {
            pstParaNode = &(pstDefBuffInfo->astInputParaLinkNodeBuf[ulNodeLoop]);

            PPP_HDLC_HARD_MntnTraceSingleData(pstParaNode->usDataLen, (VOS_UINT8*)PPP_PHY_TO_VIRT((VOS_UINT32)(pstParaNode->pucDataAddr)),
                                              ID_HDLC_MNTN_DEF_INPUT_DATA, ulNodeLoop);

            /* 需要将数据写回DDR，HDLC从DDR中读数据 */
            PPP_HDLC_CACHE_FLUSH((VOS_UINT8 *)PPP_PHY_TO_VIRT((VOS_UINT32)(pstParaNode->pucDataAddr)), pstParaNode->usDataLen);
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_MntnDefTraceOutput
 功能描述  : 勾取解封装输出数据
 输入参数  : pstBuildInfo    -   存放输入数据
             usValidFrameNum -   有效帧个数
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_MntnDefTraceOutput
(
    VOS_UINT16                          usValidFrameNum,
    HDLC_DEF_BUFF_INFO_STRU            *pstDefBuffInfo
)
{
    HDLC_MNTN_DEF_OUTPUT_PARA_STRU     *pstOutputPara;
    VOS_UINT32                          ulDataLen;
    VOS_UINT32                          ulNodeLoop;
    HDLC_DEF_RPT_NODE_STRU             *pstRptNode;
    VOS_UINT16                          usMaxFrameNum;


    /* 解封装上报空间可维可测 */
    if ((g_stHdlcConfigInfo.ulHdlcMntnTraceCfg & PPP_HDLC_MNTN_TRACE_PARA) != 0)
    {
        usMaxFrameNum = (HDLC_MNTN_ALLOC_MEM_MAX_SIZE - sizeof(HDLC_MNTN_DEF_OUTPUT_PARA_STRU)) /
                         sizeof(HDLC_DEF_RPT_NODE_STRU);
        usMaxFrameNum = TTF_MIN(usMaxFrameNum, usValidFrameNum);

        ulDataLen     = sizeof(HDLC_MNTN_DEF_OUTPUT_PARA_STRU) + usMaxFrameNum * sizeof(HDLC_DEF_RPT_NODE_STRU);
        pstOutputPara = (HDLC_MNTN_DEF_OUTPUT_PARA_STRU *)PS_MEM_ALLOC(PS_PID_PPP_HDLC, ulDataLen);

        if (VOS_NULL_PTR == pstOutputPara)
        {
            PPP_HDLC_NORMAL_LOG1("PPP_HDLC_HARD_MntnDefTraceOutput, NORMAL, Alloc mem failed ulDataLen %!\r\n", ulDataLen);
            return;
        }

        pstOutputPara->usDefValidNum = usValidFrameNum;
        pstOutputPara->usTraceNum    = usMaxFrameNum;

        VOS_MemCpy_s((VOS_UINT8 *)(pstOutputPara + 1),
                    usMaxFrameNum * sizeof(HDLC_DEF_RPT_NODE_STRU),
                   (VOS_UINT8 *)(&(pstDefBuffInfo->astRptNodeBuf[0])),
                   usMaxFrameNum * sizeof(HDLC_DEF_RPT_NODE_STRU));

        PPP_HDLC_HARD_MntnTraceMsg((HDLC_MNTN_TRACE_HEAD_STRU *)pstOutputPara,
                                   ID_HDLC_MNTN_DEF_OUTPUT_PARA, ulDataLen);

        PS_MEM_FREE(PS_PID_PPP_HDLC, pstOutputPara);
    }

    /* 解封装目的空间中每个有效帧可维可测 */
    if ((g_stHdlcConfigInfo.ulHdlcMntnTraceCfg & PPP_HDLC_MNTN_TRACE_DATA) != 0)
    {
        for ( ulNodeLoop = 0; ulNodeLoop < usValidFrameNum; ulNodeLoop++ )
        {
            pstRptNode = &(pstDefBuffInfo->astRptNodeBuf[ulNodeLoop]);

            PPP_HDLC_HARD_MntnTraceSingleData(pstRptNode->usDefOutOneLen, (VOS_UINT8*)PPP_HDLC_NODE_PHY_TO_VIRT((VOS_UINT32)(pstRptNode->pucDefOutOneAddr)),
                                              ID_HDLC_MNTN_DEF_OUTPUT_DATA, ulNodeLoop);
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_MntnFrmTraceInput
 功能描述  : 勾取封装输入参数链表和数据
 输入参数  : pstFrmBuffInfo -   封装使用的内存
             pstBuildInfo   -   存放输入数据
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_MntnFrmTraceInput
(
    HDLC_FRM_BUFF_INFO_STRU         *pstFrmBuffInfo,
    HDLC_PARA_LINK_BUILD_INFO_STRU  *pstBuildInfo
)
{
    VOS_UINT32                          ulNodeLoop;
    HDLC_PARA_LINK_NODE_STRU           *pstParaNode;


    if ((g_stHdlcConfigInfo.ulHdlcMntnTraceCfg & PPP_HDLC_MNTN_TRACE_PARA) != 0)
    {
        PPP_HDLC_HARD_MntnTraceInputParaLink(ID_HDLC_MNTN_FRM_INPUT_PARA,
                                             pstBuildInfo->ulInputLinkNodeCnt,
                                             pstBuildInfo->ulInputLinkTotalSize,
                                             &(pstFrmBuffInfo->astInputParaLinkNodeBuf[0]));
    }

    if ((g_stHdlcConfigInfo.ulHdlcMntnTraceCfg & PPP_HDLC_MNTN_TRACE_DATA) != 0)
    {
        /* 记录并上报参数链表所有节点的数据内容，每个节点是一个IP包 */
        for ( ulNodeLoop = 0; ulNodeLoop < pstBuildInfo->ulInputLinkNodeCnt; ulNodeLoop++ )
        {
            pstParaNode = &(pstFrmBuffInfo->astInputParaLinkNodeBuf[ulNodeLoop]);

            PPP_HDLC_HARD_MntnTraceSingleData(pstParaNode->usDataLen, (VOS_UINT8*)PPP_PHY_TO_VIRT((VOS_UINT32)(pstParaNode->pucDataAddr)),
                                              ID_HDLC_MNTN_FRM_INPUT_DATA, ulNodeLoop);

            /* 需要将数据写回DDR，HDLC从DDR中读数据 */
            PPP_HDLC_CACHE_FLUSH((VOS_UINT8*)PPP_PHY_TO_VIRT((VOS_UINT32)(pstParaNode->pucDataAddr)), pstParaNode->usDataLen);
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_MntnFrmTraceOutput
 功能描述  : 勾取封装输出参数链表和数据
 输入参数  : ucFrmValidNum  -   有效帧个数
             usFrmOutSegNum -   有效帧分段个数
             pstFrmBuffInfo -   封装相关内存地址
             pstBuildInfo   -   存放输入数据
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_MntnFrmTraceOutput
(
    VOS_UINT8                           ucFrmValidNum,
    VOS_UINT16                          usFrmOutSegNum,
    HDLC_FRM_BUFF_INFO_STRU            *pstFrmBuffInfo,
    HDLC_PARA_LINK_BUILD_INFO_STRU     *pstBuildInfo
)
{
    HDLC_MNTN_FRM_OUTPUT_PARA_STRU      stOutputPara;
    HDLC_MNTN_FRM_OUTPUT_PARA_STRU     *pstOutputPara = &stOutputPara;
    VOS_UINT32                          ulDataLen;
    VOS_UINT32                          ulNodeLoop;
    HDLC_PARA_LINK_NODE_STRU           *pstParaNode;

    if ((g_stHdlcConfigInfo.ulHdlcMntnTraceCfg & PPP_HDLC_MNTN_TRACE_PARA) != 0)
    {
        ulDataLen     = sizeof(HDLC_MNTN_FRM_OUTPUT_PARA_STRU);

        pstOutputPara->ulOutputLinkNodeCnt   = pstBuildInfo->ulOutputLinkNodeCnt;
        pstOutputPara->ulOutputLinkTotalSize = pstBuildInfo->ulOutputLinkTotalSize;
        pstOutputPara->ucFrmValidNum         = ucFrmValidNum;
        pstOutputPara->usOutputNodeUsedCnt   = usFrmOutSegNum;

        VOS_MemCpy_s((VOS_UINT8 *)(&(pstOutputPara->astOutputParaLinkNodeBuf[0])),
                    TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM * sizeof(HDLC_PARA_LINK_NODE_STRU),
                   (VOS_UINT8 *)(&(pstFrmBuffInfo->astOutputParaLinkNodeBuf[0])),
                   TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM * sizeof(HDLC_PARA_LINK_NODE_STRU));

        VOS_MemCpy_s((VOS_UINT8 *)(&(pstOutputPara->astRptNodeBuf[0])),
                    TTF_HDLC_FRM_RPT_MAX_NUM * sizeof(HDLC_FRM_RPT_NODE_STRU),
                   (VOS_UINT8 *)(&(pstFrmBuffInfo->astRptNodeBuf[0])),
                   TTF_HDLC_FRM_RPT_MAX_NUM * sizeof(HDLC_FRM_RPT_NODE_STRU));

        PPP_HDLC_HARD_MntnTraceMsg((HDLC_MNTN_TRACE_HEAD_STRU *)pstOutputPara,
                                        ID_HDLC_MNTN_FRM_OUTPUT_PARA, ulDataLen);
    }

    if ((g_stHdlcConfigInfo.ulHdlcMntnTraceCfg & PPP_HDLC_MNTN_TRACE_DATA) != 0)
    {
        for ( ulNodeLoop = 0; ulNodeLoop < usFrmOutSegNum; ulNodeLoop++ )
        {
            pstParaNode = &(pstFrmBuffInfo->astOutputParaLinkNodeBuf[ulNodeLoop]);

            PPP_HDLC_HARD_MntnTraceSingleData(pstParaNode->usDataLen, (VOS_UINT8*)PPP_PHY_TO_VIRT((VOS_UINT32)(pstParaNode->pucDataAddr)),
                                              ID_HDLC_MNTN_FRM_OUTPUT_DATA, ulNodeLoop);
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_CommReleaseInputData
 功能描述  : 释放存储的PPP_ZC_STRU结构
 输入参数  : ppstLinkNode   -- 存储PPP_ZC_STRU指针的数组
             ulRelCnt       -- 待释放的个数
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_CommReleaseLink
(
    PPP_ZC_STRU     **ppstLinkNode,
    VOS_UINT32        ulRelCnt
)
{
    VOS_UINT32                          ulNodeLoop;


    for ( ulNodeLoop = 0; ulNodeLoop < ulRelCnt; ulNodeLoop++ )
    {
        PPP_MemFree(ppstLinkNode[ulNodeLoop]);

        ppstLinkNode[ulNodeLoop] = VOS_NULL_PTR;
    }

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefProcRptNode
 功能描述  : 根据上报信息地址和长度，申请零拷贝内存，并拷贝数据
 输入参数  : pstRptNode   -   上报信息
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
PPP_ZC_STRU * PPP_HDLC_HARD_DefProcRptNode
(
    HDLC_DEF_RPT_NODE_STRU             *pstRptNode
)
{
    PPP_ZC_STRU                        *pstMem;
    VOS_UINT32                          usFistSegLen;
    VOS_UINT8                          *pucDefOutOneAddr;


    if ( (0 == pstRptNode->usDefOutOneLen) || (HDLC_DEF_OUT_PER_MAX_CNT < pstRptNode->usDefOutOneLen) )
    {
        PPP_HDLC_WARNING_LOG1("PPP_HDLC_HARD_DefProcValidFrames, WARNING, invalid usDefOutOneLen %d\r\n",
                      pstRptNode->usDefOutOneLen);

        return VOS_NULL_PTR;
    }

    pstMem = PPP_MemAlloc(pstRptNode->usDefOutOneLen, PPP_ZC_UL_RESERVE_LEN);

    if (VOS_NULL_PTR == pstMem)
    {
        return VOS_NULL_PTR;
    }

    pucDefOutOneAddr    = (VOS_UINT8*)PPP_HDLC_NODE_PHY_TO_VIRT((VOS_UINT32)pstRptNode->pucDefOutOneAddr);
    /* 判断该帧起始加长度是否超过输出空间尾部，超出后按绕回处理 */
    if ((pucDefOutOneAddr + pstRptNode->usDefOutOneLen) >
        HDLC_DEF_OUTPUT_BUF_END_ADDR)
    {
        if (pucDefOutOneAddr <= HDLC_DEF_OUTPUT_BUF_END_ADDR)
        {
            usFistSegLen = (VOS_UINT32)(HDLC_DEF_OUTPUT_BUF_END_ADDR - pucDefOutOneAddr);

            /* 拷贝从起始地址至输出空间尾部的数据 */
            PPP_MemSingleCopy(PPP_ZC_GET_DATA_PTR(pstMem), pucDefOutOneAddr,
                              usFistSegLen);

            /* 拷贝在输出空间首部的剩余数据 */
            PPP_MemSingleCopy(PPP_ZC_GET_DATA_PTR(pstMem) + usFistSegLen, HDLC_DEF_OUTPUT_BUF_START_ADDR,
                              pstRptNode->usDefOutOneLen - usFistSegLen);
        }
        else
        {
            PPP_MemFree(pstMem);

            PPP_HDLC_ERROR_LOG2("\r\nPPP, PPP_HDLC_HARD_DefProcRptNode, ERROR, Error pucDefOutOneAddr.\r\n",
                    pucDefOutOneAddr, HDLC_DEF_OUTPUT_BUF_END_ADDR);

            PPP_HDLC_HARD_MntnShowDefReg();

            return VOS_NULL_PTR;
        }
    }
    else
    {
        PPP_MemSingleCopy(PPP_ZC_GET_DATA_PTR(pstMem), pucDefOutOneAddr,
                          pstRptNode->usDefOutOneLen);
    }

    /* 设置零拷贝数据长度 */
    PPP_ZC_SET_DATA_LEN(pstMem, pstRptNode->usDefOutOneLen);

    return pstMem;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefProcValidFrames
 功能描述  : 处理有效帧
 输入参数  : ulMode       -   模式，IP或PPP
             usPppId    -   PPP实体ID
             pstLink    -   PPP链路信息
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_DefProcValidFrames
(
    VOS_UINT32          ulMode,
        PPP_ID          usPppId,
    struct link        *pstLink
)
{
    VOS_UINT16                          usValidFrameNum;
    HDLC_DEF_BUFF_INFO_STRU            *pstDefBuffInfo;
    VOS_UINT32                          ulFrameLoop;
    HDLC_DEF_RPT_NODE_STRU             *pstRptNode;
    PPP_ZC_STRU                        *pstMem;


    pstDefBuffInfo  = HDLC_DEF_GET_BUF_INFO(usPppId);
    usValidFrameNum = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR), 8, 23);

    /* 上报解封装后数据可维可测:上报空间信息、输出内容 */
    PPP_HDLC_HARD_MntnDefTraceOutput(usValidFrameNum, pstDefBuffInfo);

    /* 有效帧数最大值检查 */
    if (TTF_HDLC_DEF_RPT_MAX_NUM < usValidFrameNum)
    {
        PPP_HDLC_WARNING_LOG2("PPP_HDLC_HARD_DefProcValidFrames, WARNING, usValidFrameNum = %d > TTF_HDLC_DEF_RPT_MAX_NUM = %d",
                      usValidFrameNum, TTF_HDLC_DEF_RPT_MAX_NUM);
        return;
    }

    g_PppHdlcHardStat.ulDefMaxValidCntOnce = TTF_MAX(g_PppHdlcHardStat.ulDefMaxValidCntOnce, usValidFrameNum);

    /* 将目的空间的有效帧数据拷贝至零拷贝内存，根据拨号类型调用上行发数接口 */
    for ( ulFrameLoop = 0 ; ulFrameLoop < usValidFrameNum; ulFrameLoop++ )
    {
        pstRptNode = &(pstDefBuffInfo->astRptNodeBuf[ulFrameLoop]);

        pstMem = PPP_HDLC_HARD_DefProcRptNode(pstRptNode);

        /* 申请不到内容或上报信息错误，丢弃该有效帧 */
        if (VOS_NULL_PTR == pstMem)
        {
            continue;
        }

        if (HDLC_IP_MODE == ulMode)
        {
            PPP_HDLC_ProcIpModeUlData(pstLink, pstMem, pstRptNode->usDefOutOnePro);
        }
        else
        {
            PPP_HDLC_ProcPppModeUlData(usPppId, pstMem);
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefProcErrorFrames
 功能描述  : 处理错误帧
 输入参数  : pstLink    -   PPP链路信息
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_DefProcErrorFrames
(
    struct link        *pstLink
)
{
    VOS_UINT8                           ucErrType;
    VOS_UINT8                           ucMask;
    VOS_UINT32                          ulErrTypeLoop;
    VOS_UINT8                           ucResult;
    HDLC_DEF_ERR_FRAMES_CNT_STRU        stErrCnt;


    /* 查询状态寄存器hdlc_def_status (0x88)的第24:30对应比特位为1表示有某种错误帧输出，
       为0表示无帧输出 */
    ucErrType = (VOS_UINT8)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR) , 24, 30);

    if (0 == ucErrType)
    {
        return;
    }

    /* get fcs error count */
    stErrCnt.usFCSErrCnt        = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_ERR_INFO_0_ADDR(HDLC_IP_BASE_ADDR), 0, 15);

    /* get frame too long error count */
    stErrCnt.usLenLongCnt       = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_ERR_INFO_0_ADDR(HDLC_IP_BASE_ADDR), 16, 31);

    /* get frame too short error count */
    stErrCnt.usLenShortCnt      = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_ERR_INFO_1_ADDR(HDLC_IP_BASE_ADDR), 0, 15);

    /* get error protocol count */
    stErrCnt.usErrProtocolCnt   = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_ERR_INFO_1_ADDR(HDLC_IP_BASE_ADDR), 16, 31);

    /* get error control count */
    stErrCnt.usErrCtrlCnt       = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_ERR_INFO_2_ADDR(HDLC_IP_BASE_ADDR), 0, 15);

    /* get error address count */
    stErrCnt.usErrAddrCnt       = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_ERR_INFO_2_ADDR(HDLC_IP_BASE_ADDR), 16, 31);

    /* get error flag position count */
    stErrCnt.usFlagPosErrCnt    = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_DEF_ERR_INFO_3_ADDR(HDLC_IP_BASE_ADDR), 0, 15);
    for (ulErrTypeLoop = 0UL; ulErrTypeLoop < HDLC_DEF_MAX_TYPE_CNT; ulErrTypeLoop++)
    {
        ucMask   = SET_BITS_VALUE_TO_BYTE(0x01, ulErrTypeLoop);    /* 构造掩码 */
        ucResult = (VOS_UINT8)GET_BITS_FROM_BYTE(ucErrType, ucMask);

        if (0 != ucResult)      /* 存在此类错误 */
        {
            if (0UL == ulErrTypeLoop)   /* 错误类型0: CRC校验错误 */
            {
                pstLink->hdlc.stats.badfcs       += stErrCnt.usFCSErrCnt;
                pstLink->hdlc.lqm.ifInErrors   += stErrCnt.usFCSErrCnt;
                PPP_HDLC_WARNING_LOG("bad hdlc fcs\r\n");
            }
            else if (1UL == ulErrTypeLoop)    /* 错误类型1: 解封装后帧字节数大于1502bytes */
            {
                pstLink->hdlc.lqm.ifInErrors += stErrCnt.usLenLongCnt;
                PPP_HDLC_WARNING_LOG("bad hdlc frame length too long\r\n");
            }
            else if (2UL == ulErrTypeLoop)    /* 错误类型2: 解封装后帧字节数小于4bytes */
            {
                pstLink->hdlc.lqm.ifInErrors += stErrCnt.usLenShortCnt;
                PPP_HDLC_WARNING_LOG("bad hdlc frame length too short\r\n");
            }
            else if (3UL == ulErrTypeLoop)    /* 错误类型3: 当P域需剥离时, 收到非法的Protocol域值(非*******0 *******1形式) */
            {
                pstLink->hdlc.lqm.ifInErrors += stErrCnt.usErrProtocolCnt;
                PPP_HDLC_WARNING_LOG("bad hdlc frame protocol\r\n");
            }
            else if (4UL == ulErrTypeLoop)    /* 错误类型4: 当AC域无压缩时, Control域值非0x03 */
            {
                pstLink->hdlc.lqm.ifInErrors += stErrCnt.usErrCtrlCnt;
                pstLink->hdlc.stats.badcommand += stErrCnt.usErrCtrlCnt;
                PPP_HDLC_NORMAL_LOG("bad hdlc frame control\r\n");
            }
            else if (5UL == ulErrTypeLoop)    /* 错误类型5: 当AC域无压缩时, Address域值非0xFF */
            {
                pstLink->hdlc.lqm.ifInErrors += stErrCnt.usErrAddrCnt;
                pstLink->hdlc.stats.badaddr    += stErrCnt.usErrAddrCnt;
                PPP_HDLC_NORMAL_LOG("bad hdlc frame address\r\n");
            }
            else if (6UL == ulErrTypeLoop)    /* 错误类型6: 转义字符0x7D后紧接一个Flag域 */
            {
                pstLink->hdlc.lqm.ifInErrors += stErrCnt.usFlagPosErrCnt;
                PPP_HDLC_NORMAL_LOG("bad hdlc frame flag position\r\n");
            }
            else
            {
                PPP_HDLC_NORMAL_LOG("link_HDLCDefDealErr err\r\n");
            }
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefCfgReg
 功能描述  : 配置解封装配置寄存器
 输入参数  : ulMode     -   IP或PPP模式
             pstLink    -   PPP链路信息
             pstDefUncompletedInfo  -   该路PPP链接上次解封装输出非完整帧信息
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_DefCfgReg
(
    VOS_UINT32                      ulMode,
    struct link                    *pstLink,
    HDLC_DEF_UNCOMPLETED_INFO_STRU *pstDefUncompletedInfo
)
{
    /*
        hdlc_def_cfg  (0x70)
         31                           4 3     2   1     0
        |-------------------------------|-------|-----|-----|
        |              Rsv              |  Pfc  | Acfc| ago |
        Reserved             [31:4]  28'b0   h/s R/W  保留位。读时返回0。写时无影响。
        def_pfc              [3:2]   2'b0    h/s R/W  P域压缩指示：00：P域无压缩，需剥离；01：P域压缩，需剥离；11：P域不剥离；其他：无效；
        def_acfc             [1]     1'b0    h/s R/W  AC域压缩指示：0：AC域无压缩；1：表示AC域压缩；
        def_uncompleted_ago  [0]     1'b0    h/s R/W  用于指示对应当前解封装输入链表，同一PPP/IP拨号的以前最近解封装输入链表是否有解出非完整帧，
                                                      为了支持多个PPP/IP拨号而增加的配置：0，没有；1，有
        */

    VOS_UINT32       ulDeframerCfg;
    VOS_UINT8        ucACComp;
    VOS_UINT8        ucPComp;
    VOS_UINT8        ucLowByte;
    VOS_UINT16       usLowWord;

    /* 1.根据ulMode、P域和AC域是否压缩配置hdlc_def_cfg (0x70) */

    /* 获取AC域压缩指示, P域压缩指示 */
    ucACComp = (1 == pstLink->lcp.want_acfcomp) ? 1 : 0;

    if (HDLC_IP_MODE == ulMode) /* IP模式: P域不合入 */
    {
        ucPComp = (1 == pstLink->lcp.want_protocomp)
                   ? HDLC_PROTOCOL_REMOVE_WITH_COMPRESS
                   : HDLC_PROTOCOL_REMOVE_WITHOUT_COMPRESS;
    }
    else                        /* PPP模式: P域合入 */
    {
        ucPComp = HDLC_PROTOCOL_NO_REMOVE;
    }

    /* 将ucPComp设置到一个字节的第2, 3位上 */
    ucLowByte = SET_BITS_VALUE_TO_BYTE(ucPComp, HDLC_DEF_PFC_BITPOS);

    if (1 == ucACComp)  /* AC域压缩 */
    {
        SET_BIT_TO_BYTE(ucLowByte, HDLC_DEF_ACFC_BITPOS);
    }

    /* 2.设置非完整帧相关信息 */
    if ( (VOS_NULL_PTR != pstDefUncompletedInfo) &&
        (HDLC_DEF_UNCOMPLETED_EXIST == pstDefUncompletedInfo->ucExistFlag) )
    {
        /* def_uncompleted_ago置1表示有上次输出的非完整帧参与本次解封装 */
        SET_BIT_TO_BYTE(ucLowByte, HDLC_DEF_IS_UNCOMPLETED_AGO_BITPOS);

        /*
        def_uncompleted_len  (0x74)
         31                 16  15                  0
        |---------------------|----------------------|
        |         Rsv         |         Len          |
        Reserved             [31:16] 16'b0   h/s R/W  保留位。读时返回0。写时无影响。
        def_uncompleted_len  [15:0]  16'b0   h/s R/W  对应当前解封装输入链表，同一PPP/IP拨号的以前最近解封装输入链表解出非完整帧的长度，为了支持多个PPP/IP拨号而增加的配置
        */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_LEN_ADDR(HDLC_IP_BASE_ADDR),
                        (VOS_UINT32)pstDefUncompletedInfo->usDefOutOneLen & 0xFFFF);

        /*
        def_uncompleted_pro  (0x78)
         31                 16  15                  0
        |---------------------|----------------------|
        |         Rsv         |         Pro          |
        Reserved             [31:16] 16'b0   h/s R/W  保留位。读时返回0。写时无影响。
        def_uncompleted_pro  [15:0]  16'b0   h/s R/W  对应当前解封装输入链表，同一PPP/IP拨号的以前最近解封装输入链表解出非完整帧的
                                                      协议，为了支持多个PPP/IP拨号而增加的配置（可能其中的0Byte、1Byte或2Byte有效）
        */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_PRO_ADDR(HDLC_IP_BASE_ADDR),
                        (VOS_UINT32)pstDefUncompletedInfo->usDefOutOnePro & 0xFFFF);

        /*
        def_uncompleted_addr  (0x7C)
         31                  0
        |----------------------|
        |         Addr         |
        def_uncompleted_addr [31:0]  32'b0   h/s R/W  对应当前解封装输入链表，同一PPP/IP拨号的以前最近解封装输入链表解出非完整帧的
                                                      外部存储起始地址，为了支持多个PPP/IP拨号而增加的配置（该地址可能是与原来上报不同的新地址）
        */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_ADDR(HDLC_IP_BASE_ADDR),
                        (VOS_UINT32)pstDefUncompletedInfo->pucDefOutOneAddr);

        /*
        def_uncomplet_st_ago  (0x80)
         31                  16 15             5 4     0
        |----------------------|----------------|-------|
        |         Ago          |       Rsv      |  Ago  |
        crc16_result_ago     [31:16] 16'b0   h/s R/W  与当前解封装输入链表同一PPP/IP拨号的以前最近那套解封装输入链表处理完解出非完整帧时的CRC校验值
        Reserved             [15:5]  11'b0   h/s R/W  保留位。读时返回0。写时无影响。
        def_data_st_curr_ago [4:0]   5'b0    h/s R/W  与当前解封装输入链表同一PPP/IP拨号的以前最近那套解封装输入链表处理完解出非完整帧时的数据状态机当前状态
        */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_ST_AGO_ADDR(HDLC_IP_BASE_ADDR),
                        (VOS_UINT32)pstDefUncompletedInfo->ulDefStAgo);

        /*
        def_info_frl_cnt_ago  (0xC0)
         31        27 26                 16 15   11 10              0
        |------------|---------------------|-------|-----------------|
        |    Rsv     |         Ago         |  Rsv  |       Ago       |
        Reserved             [31:27] 5'b0    h/s R/W  保留位。读时返回0。写时无影响。
        def_framel_cnt_ago   [26:16] 11'b0   h/s R/W  与当前解封装输入链表同一PPP/IP拨号的以前最近那套解封装输入链表处理完解出非完整帧时的帧长度
        Reserved             [15:11] 5'b0    h/s R/W  保留位。读时返回0。写时无影响。
        def_info_cnt_ago     [10:0]  11'b0   h/s R/W  与当前解封装输入链表同一PPP/IP拨号的以前最近那套解封装输入链表处理完解出非完整帧时的信息长度
        */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_INFO_FRL_CNT_AGO_ADDR(HDLC_IP_BASE_ADDR),
                        (VOS_UINT32)pstDefUncompletedInfo->ulDefInfoFrlCntAgo);
    }

    usLowWord     = HDLC_MAKE_WORD(0x00, ucLowByte);
    ulDeframerCfg = HDLC_MAKE_DWORD(0x00, usLowWord);

    /* 将配置结果写入寄存器 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_CFG_ADDR(HDLC_IP_BASE_ADDR),ulDeframerCfg);

    /* 设置输入数据最大单包长度 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_IN_PKT_LEN_MAX_ADDR(HDLC_IP_BASE_ADDR),
                    (VOS_UINT32)HDLC_DEF_IN_PER_MAX_CNT);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefSaveUncompletedInfo
 功能描述  : 根据非完整帧指示，保存非完整帧信息
 输入参数  : usPppId    -   PPP实体ID
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_DefSaveUncompletedInfo
(
    PPP_ID          usPppId
)
{
    HDLC_DEF_UNCOMPLETED_INFO_STRU     *pstUncompletedInfo;
    VOS_UINT32                          ulStatus;                /* 解封装状态 */
    HDLC_DEF_BUFF_INFO_STRU            *pstDefBuffInfo;
    VOS_UINT32                          ulValidNum;
    VOS_UINT8                          *pucDefOutOneAddr;
    HDLC_DEF_RPT_NODE_STRU             *pstRptNode;


    pstDefBuffInfo     = HDLC_DEF_GET_BUF_INFO(usPppId);
    pstUncompletedInfo = HDLC_DEF_GET_UNCOMPLETED_INFO(usPppId);

    ulStatus  =   PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_STATUS_ADDR(HDLC_IP_BASE_ADDR));

    /* 查询状态寄存器hdlc_def_status (0x88)的第2位
       为1表示本次解封装有非完整帧输出，
       为0表示无非完整帧输出 */
    if (0 == (ulStatus & 0x4))
    {
        pstUncompletedInfo->ucExistFlag = HDLC_DEF_UNCOMPLETED_NOT_EXIST;

        return;
    }

    g_PppHdlcHardStat.ulDefUncompleteCnt++;

    /* def_valid_num        [23:8]  16'b0   h/s RO   有帧上报时，有效帧数目；（不包括最后一个可能的非完整帧） */
    ulValidNum = (ulStatus & 0xFFFF00) >> 8;

    /* 非完整帧的上报信息在有效帧后面，但是不算在有效帧数目内 */
    if (TTF_HDLC_DEF_RPT_MAX_NUM <= ulValidNum)
    {
        PPP_HDLC_ERROR_LOG2("PPP_HDLC_HARD_DefSaveUncompletedInfo, ERROR, ulValidNum %d >= TTF_HDLC_DEF_RPT_MAX_NUM %d!\r\n",
                     ulValidNum, TTF_HDLC_DEF_RPT_MAX_NUM);

        pstUncompletedInfo->ucExistFlag = HDLC_DEF_UNCOMPLETED_NOT_EXIST;

        return;
    }

    /* 有非完整帧时需要读取并保存def_uncomplet_st_now(0x8C)、def_info_frl_cnt_now(0xC4)
       读取并保存上报空间有效帧之后的非完整帧长度、协议和数据地址 */
    pstUncompletedInfo->ucExistFlag = HDLC_DEF_UNCOMPLETED_EXIST;

    pstRptNode = &(pstDefBuffInfo->astRptNodeBuf[ulValidNum]);

    pucDefOutOneAddr = (VOS_UINT8*)PPP_HDLC_NODE_PHY_TO_VIRT((VOS_UINT32)pstRptNode->pucDefOutOneAddr);

    if (pucDefOutOneAddr != HDLC_DEF_OUTPUT_BUF_START_ADDR)
    {
        if ((pucDefOutOneAddr - HDLC_DEF_OUTPUT_BUF_START_ADDR) >= pstRptNode->usDefOutOneLen)
        {
            PSACORE_MEM_CPY(HDLC_DEF_OUTPUT_BUF_START_ADDR, pstRptNode->usDefOutOneLen, pucDefOutOneAddr, pstRptNode->usDefOutOneLen);
        }
        else
        {
            PSACORE_MEM_MOVE(HDLC_DEF_OUTPUT_BUF_START_ADDR, pstRptNode->usDefOutOneLen, pucDefOutOneAddr, pstRptNode->usDefOutOneLen);
        }
#ifndef PPP_ST_TEST
        pstRptNode->pucDefOutOneAddr = (VOS_UINT8*)PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)HDLC_DEF_OUTPUT_BUF_START_ADDR);
#endif
    }


    /* 非完整帧的协议、长度和在存储空间的地址，软件只是暂存这些信息，等下解封装的时候再原样配给HDLC */
    pstUncompletedInfo->usDefOutOnePro   = pstRptNode->usDefOutOnePro;
    pstUncompletedInfo->usDefOutOneLen   = pstRptNode->usDefOutOneLen;
    pstUncompletedInfo->pucDefOutOneAddr = pstRptNode->pucDefOutOneAddr;

    /* 软件只是暂存这些信息，等下解封装的时候再原样配给HDLC */
    pstUncompletedInfo->ulDefStAgo         = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_UNCOMPLETED_ST_NOW_ADDR(HDLC_IP_BASE_ADDR));
    pstUncompletedInfo->ulDefInfoFrlCntAgo = PPP_HDLC_READ_32REG(SOC_ARM_HDLC_DEF_INFO_FRL_CNT_NOW_ADDR(HDLC_IP_BASE_ADDR));

    PPP_HDLC_HARD_MntnDefTraceUncompleteInfo(pstUncompletedInfo);

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefWaitAndProc
 功能描述  : 等待解封装暂停或完成，然后处理输出数据，可能会有多次停等的过程
 输入参数  : ulMode     -   模式，IP或PPP
             ulEnableInterrupt  -   中断是否使能
             usPppId    -   PPP实体ID
             pstLink    -   PPP链路信息
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_DefWaitAndProc
(
    VOS_UINT32          ulMode,
    VOS_UINT32          ulEnableInterrupt,
    PPP_ID              usPppId,
    struct link        *pstLink
)
{
    VOS_UINT32                          ulDefStatus;
    VOS_UINT32                          ulContinue;

    for (; ;)
    {
        /* 使能中断，则等待中断辅程序释放信号量；否则轮询解封装状态寄存器 */
        ulDefStatus = PPP_HDLC_HARD_DefWaitResult(ulEnableInterrupt);

        switch ( ulDefStatus )
        {
            case HDLC_DEF_STATUS_PAUSE_RPT_SPACE_FULL :
            case HDLC_DEF_STATUS_PAUSE_OUTPUT_SPACE_FULL :
                /* 处理有效帧，配置GO_ON寄存器 */
                PPP_HDLC_HARD_DefProcValidFrames(ulMode, usPppId, pstLink);
                PPP_HDLC_HARD_DefCfgGoOnReg(ulDefStatus);

                ulContinue = VOS_TRUE;
                g_PppHdlcHardStat.ulDefFullPauseCnt++;
                break;
            case HDLC_DEF_STATUS_PAUSE_LCP :
                /* 处理有效帧和LCP帧，更新配置寄存器，配置GO_ON寄存器 */
                PPP_HDLC_HARD_DefProcValidFrames(ulMode, usPppId, pstLink);
                PPP_HDLC_HARD_DefCfgReg(ulMode, pstLink, VOS_NULL_PTR);
                PPP_HDLC_HARD_DefCfgGoOnReg(ulDefStatus);

                ulContinue = VOS_TRUE;
                g_PppHdlcHardStat.ulDefLcpPauseCnt++;
                break;
            case HDLC_DEF_STATUS_DONE_WITHOUT_FRAMES :
                /* 根据非完整帧指示，保存非完整帧信息 */
                PPP_HDLC_HARD_DefSaveUncompletedInfo(usPppId);

                ulContinue = VOS_FALSE;
                break;
            case HDLC_DEF_STATUS_DONE_WITH_FRAMES :
                /* 处理有效帧、错误帧和LCP帧(可能是最后一帧)，根据非完整帧指示，保存非完整帧信息 */
                PPP_HDLC_HARD_DefProcValidFrames(ulMode, usPppId, pstLink);
                PPP_HDLC_HARD_DefProcErrorFrames(pstLink);
                PPP_HDLC_HARD_DefSaveUncompletedInfo(usPppId);
                ulContinue = VOS_FALSE;
                break;
            case HDLC_DEF_STATUS_DOING :
            default:
                /* 打印异常日志，挂起PPP任务 */
                PPP_HDLC_HARD_DefProcException(ulDefStatus, ulEnableInterrupt);

                ulContinue = VOS_FALSE;
                break;
        }

        /* 暂停状态需要继续处理，其他状态解封装完成退出 */
        if (VOS_TRUE != ulContinue)
        {
            break;
        }
    }

    g_stHdlcRegSaveInfo.ulHdlcDefRawInt = 0xFFFFFFFFU;
    g_stHdlcRegSaveInfo.ulHdlcDefStatus = 0xFFFFFFFFU;

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefCheckPara
 功能描述  : 检查构造输入参数链表相关参数是否合法
 输入参数  : pstMem     -   待封装数据包
             ulDataLen  -   待封装数据包长度
             ucDataType -   待封装数据包类型
 输出参数  : 无
 返 回 值  : PPP_HDLC_PARA_CHECK_RESULT_ENUM_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
PPP_HDLC_PARA_CHECK_RESULT_ENUM_UINT32 PPP_HDLC_HARD_DefCheckPara
(
    PPP_ZC_STRU                 *pstMem,
    VOS_UINT32                   ulDataLen,
    PPP_DATA_TYPE_ENUM_UINT8     ucDataType
)
{
    PPP_DATA_TYPE_ENUM_UINT8     ucCurrDataType;


    /* 获取待封装数据包类型 */
    ucCurrDataType = (PPP_ZC_GET_DATA_APP(pstMem) & 0x00FF);

    if ( ucDataType != ucCurrDataType )
    {
        return PPP_HDLC_PARA_CHECK_FAIL_KEEP;
    }

    /* 解封装最大输入长度是内存模块允许的最大长度，目前是1536B */
    /* 规避HDLC Bug内存可能扩大1 */
    if ( (0 == ulDataLen) || (HDLC_DEF_IN_PER_MAX_CNT < ulDataLen) )
    {
        PPP_HDLC_NORMAL_LOG1("PPP_HDLC_HARD_DefCheckPara, NORMAL, invalid data length %d\r\n", ulDataLen);

        return PPP_HDLC_PARA_CHECK_FAIL_DISCARD;
    }

    return PPP_HDLC_PARA_CHECK_PASS;
}


/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_ForbiddenHdlcBug
 功能描述  : 规避HDLC硬件Bug,具体参见问题单DTS2014060907737
 输入参数  : ppstMem     -   待解封装数据包指针

 输出参数  : ppstMem  规避之后数据
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年06月17日
    作    者   : t00148005
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_ForbiddenHdlcBug(PPP_ZC_STRU **ppstMem)
{
    PPP_ZC_STRU *pstMem;
    PPP_ZC_STRU *pstTmpMem;
    VOS_UINT8   *pstData;
    VOS_UINT16   usLen;
    const VOS_UINT8    ucEndByte = 0x7e;

    pstMem  = *ppstMem;

    if (VOS_NULL_PTR == pstMem)
    {
        return;
    }

    usLen   = PPP_ZC_GET_DATA_LEN(pstMem);
    pstData = PPP_ZC_GET_DATA_PTR(pstMem);

    if (usLen <= 1)
    {
        return;
    }

    if (ucEndByte != pstData[usLen - 2])
    {
        /* 倒数第二个字节不为7e不用处理 */
        return;
    }
    else if ((usLen >= 3) && (ucEndByte == pstData[usLen - 3]))
    {
        /* 倒数第三个字节为7e不用处理 */
        return;
    }
    else
    {
        /*
            倒数第二个字节为7e,倒数第三个字节不为7e的场景
            将最后一个字节后向移一个字节,同时内存的长度+1

            skb申请的内存结构如下
           |--------data(uslen)--------|----32bytes align(0~31)-----|--skb_share_info(256)----|
        */
        if (PPP_ZC_GET_RESERVE_ROOM(pstMem) > 0)
        {
            INSERT_BYTE_LAST(pstMem, pstData, usLen, ucEndByte);
            g_PppHdlcHardStat.ulForbidHdlcBugNoCpy++;
        }
        else
        {
            /* 申请一个保留1字节的内存 */

            pstTmpMem = PPP_MemAlloc(usLen + 1, 0);
            if (VOS_NULL_PTR != pstTmpMem)
            {
                PPP_MemWriteData(pstTmpMem, pstData, usLen);

                pstData = PPP_ZC_GET_DATA_PTR(pstTmpMem);

                INSERT_BYTE_LAST(pstTmpMem, pstData, usLen, ucEndByte);
                g_PppHdlcHardStat.ulForbidHdlcBugCpy++;
            }

            /* 不管新申请的内存是否为空都传出去,在外面处理 */
            *ppstMem = pstTmpMem;

            /* 将原内存释放 */
            PPP_MemFree(pstMem);
        }
    }
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefBuildInputParaLink
 功能描述  : 构造解封装输入参数链表
 输入参数  : pstBuildPara   -   构造链表所需的参数
 输出参数  : pstBuildInfo   -   构造链表的结果信息，保存输入链表长度、节点个数、零拷贝内存指针
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_DefBuildInputParaLink
(
    HDLC_PARA_LINK_BUILD_PARA_STRU  *pstBuildPara,
    HDLC_PARA_LINK_BUILD_INFO_STRU  *pstBuildInfo
)
{
    VOS_UINT32                              ulLinkLoop;
    PPP_ZC_STRU                            *pstMem;
    PPP_DATA_TYPE_ENUM_UINT8                ucDataType;
    PPP_HDLC_PARA_CHECK_RESULT_ENUM_UINT32  enCheckResult;
    VOS_UINT16                              usDataLen;
    HDLC_PARA_LINK_NODE_STRU               *pstParaNode;
    VOS_UINT32                              ulNodeIndex;
    HDLC_DEF_BUFF_INFO_STRU                *pstDefBuffInfo;
    HDLC_DEF_UNCOMPLETED_INFO_STRU         *pstUncompletedInfo;
    VOS_UINT32                              ulMaxDataLen1Time = TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_SIZE;

    /* 清空输出信息 */
    VOS_MemSet_s(pstBuildInfo, sizeof(HDLC_PARA_LINK_BUILD_INFO_STRU),
                 0, sizeof(HDLC_PARA_LINK_BUILD_INFO_STRU));

    pstUncompletedInfo = HDLC_DEF_GET_UNCOMPLETED_INFO(pstBuildPara->usPppId);

    if (HDLC_DEF_UNCOMPLETED_EXIST == pstUncompletedInfo->ucExistFlag)
    {
        ulMaxDataLen1Time = TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_SIZE - pstUncompletedInfo->usDefOutOneLen;
    }


    /* 根据usPppId找到对应的内存  */
    pstDefBuffInfo = HDLC_DEF_GET_BUF_INFO(pstBuildPara->usPppId);
    ucDataType     = pstBuildPara->ucDataType;

    for ( ulLinkLoop = 0 ; ulLinkLoop < TTF_HDLC_DEF_INPUT_PARA_LINK_MAX_NUM; ulLinkLoop++ )
    {
        pstMem     = (PPP_ZC_STRU *)PPP_ZC_PEEK_QUEUE_HEAD(pstBuildPara->pstDataQ);

        /* 还没有到最大节点数，但是队列中已经没有数据 */
        if( VOS_NULL_PTR == pstMem )
        {
            break;
        }

        usDataLen     = PPP_ZC_GET_DATA_LEN(pstMem);
        enCheckResult = PPP_HDLC_HARD_DefCheckPara(pstMem, usDataLen, ucDataType);

        if( PPP_HDLC_PARA_CHECK_FAIL_DISCARD == enCheckResult )
        {
            pstMem = PPP_ZC_DEQUEUE_HEAD(pstBuildPara->pstDataQ);
            PPP_MemFree(pstMem);
            pstBuildInfo->ulDealCnt++;

            g_PppHdlcHardStat.ulDefInputDiscardCnt++;
            continue;
        }

        if( PPP_HDLC_PARA_CHECK_FAIL_KEEP == enCheckResult )
        {
            break;
        }

        /* 从PPP队列取出头结点，并插入输入队列 pstBuildInfo->apstInputLinkNode */
        pstMem  = PPP_ZC_DEQUEUE_HEAD(pstBuildPara->pstDataQ);

        /* 规避HDLC BUG,在发现HDLC BUG场景下对原有数据进行替换,
           后面的pstMem为替换后的内存指针,原始内存指针可能已经被释放
         */
        PPP_HDLC_HARD_ForbiddenHdlcBug(&pstMem);

        /* 替换内存的时候申请内存失败,直接处理下一块数据  */
        if (VOS_NULL_PTR == pstMem)
        {
            continue;
        }

        /* 内存可能发生替换,重新取长度 */
        usDataLen = PPP_ZC_GET_DATA_LEN(pstMem);

        /* 一次配链表过程中待封装数据包总长不能超过15KB */
        /* 规避HDLC BUG,后面数据被修改过,可能扩大了1字节,需要放到后面判断 */
        if( pstBuildInfo->ulInputLinkTotalSize + usDataLen  > ulMaxDataLen1Time )
        {
            /* 内存放回队列,下次再处理 */
            PPP_ZC_ENQUEUE_HEAD(pstBuildPara->pstDataQ, pstMem);

            break;
        }

        /* 获取当前要添加节点的下标 */
        ulNodeIndex = pstBuildInfo->ulInputLinkNodeCnt;

        /* 本次要构造的参数节点 */
        pstParaNode = &(pstDefBuffInfo->astInputParaLinkNodeBuf[ulNodeIndex]);

        /* 把节点组成链表 */
        if( 0 != ulNodeIndex )
        {
            pstDefBuffInfo->astInputParaLinkNodeBuf[ulNodeIndex - 1].pstNextNode = (HDLC_PARA_LINK_NODE_STRU *)PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)pstParaNode);
        }

        /* 填写输入参数链表结点相关域 */
        pstParaNode->pucDataAddr = (VOS_UINT8 *)PPP_VIRT_TO_PHY((VOS_UINT_PTR)PPP_ZC_GET_DATA_PTR(pstMem));
        pstParaNode->usDataLen   = usDataLen;
        pstParaNode->pstNextNode = VOS_NULL_PTR;

        pstBuildInfo->apstInputLinkNode[ulNodeIndex] = pstMem;
        pstBuildInfo->ulInputLinkTotalSize          += (VOS_UINT32)usDataLen;
        pstBuildInfo->ulInputLinkNodeCnt++;
        pstBuildInfo->ulDealCnt++;

        /* 需要将数据写回DDR，HDLC从DDR中读数据 */
        PPP_HDLC_CACHE_FLUSH(PPP_ZC_GET_DATA_PTR(pstMem), PPP_ZC_GET_DATA_LEN(pstMem));
    }

    if (0 == pstBuildInfo->ulInputLinkNodeCnt)
    {
        return VOS_ERR;
    }

    /* 上报输入参数链表内容可维可测 */
    PPP_HDLC_HARD_MntnDefTraceInput(pstDefBuffInfo, pstBuildInfo);

    return VOS_OK;
}

/************************************************************,*****************
 函 数 名  : PPP_HDLC_HARD_DefCfgBufReg
 功能描述  : 配置解封装使用的内存至相关寄存器
 输入参数  : pstDefBuffInfo    -   解封装使用的内存信息
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_DefCfgBufReg(HDLC_DEF_BUFF_INFO_STRU *pstDefBuffInfo)
{
    /* 将封装输入参数链表的起始地址配置给寄存器def_in_lli_addr(0x90) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_IN_LLI_ADDR(HDLC_IP_BASE_ADDR),
                    PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)&(pstDefBuffInfo->astInputParaLinkNodeBuf[0])));

    /* 将封装输出参数链表的起始地址配置给寄存器def_out_spc_addr(0xA0) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_OUT_SPC_ADDR(HDLC_IP_BASE_ADDR),
                    PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)&(pstDefBuffInfo->aucOutputDataBuf[0])));

    /* 将封装输出参数链表的起始地址配置给寄存器def_out_space_dep(0xA4)低16位 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_OUT_SPACE_DEP_ADDR(HDLC_IP_BASE_ADDR),
                    (VOS_UINT32)TTF_HDLC_DEF_OUTPUT_DATA_BUF_LEN & 0xFFFF);

    /* 将封装有效帧结果信息上报空间起始地址配置给寄存器def_rpt_addr(0xA8) */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_RPT_ADDR(HDLC_IP_BASE_ADDR),
                    PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)&(pstDefBuffInfo->astRptNodeBuf[0])));

    /* 将封装有效帧结果信息上报空间深度配置给寄存器def_rpt_dep (0xAC)低16位 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_DEF_RPT_DEP_ADDR(HDLC_IP_BASE_ADDR),
                    (VOS_UINT32)TTF_HDLC_DEF_RPT_BUF_LEN & 0xFFFF);

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefPacket
 功能描述  : 硬件方式IP类型解封装
 输入参数  : usPppId    -   PPP实体ID
             pstLink    -   PPP链路信息
             pstDataQ   -   PPP数据队列
             ulDealCnt  -   本次处理数据包个数
 输出参数  : 参见PPP_HDLC_RESULT_TYPE_ENUM_UINT32
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_DefPacket
(
    PPP_ID              usPppId,
    struct link        *pstLink,
    PPP_ZC_QUEUE_STRU  *pstDataQ,
    VOS_UINT32         *pulDealCnt
)
{
    VOS_UINT32                          ulEnableInterrupt;
    HDLC_PARA_LINK_BUILD_INFO_STRU      stBuildInfo;
    HDLC_PARA_LINK_BUILD_PARA_STRU      stBuildPara;
    VOS_UINT32                          ulBuildResult;
    HDLC_DEF_BUFF_INFO_STRU            *pstDefBuffInfo;

    pstDefBuffInfo = HDLC_DEF_GET_BUF_INFO(usPppId);

    stBuildPara.ucDataType = PPP_PULL_PACKET_TYPE;
    stBuildPara.usProtocol = PROTO_IP;
    stBuildPara.pstDataQ   = pstDataQ;
    stBuildPara.usPppId    = usPppId;

    /* 构造输入参数链表，并上报链表内存可维可测 */
    ulBuildResult = PPP_HDLC_HARD_DefBuildInputParaLink(&stBuildPara, &stBuildInfo);

    *pulDealCnt = stBuildInfo.ulDealCnt;

    if (VOS_OK != ulBuildResult)
    {
        PPP_HDLC_WARNING_LOG("PPP_HDLC_HARD_DefPacket, WARNING, BuildParaLink Fail.\r\n");
        return ulBuildResult;
    }

    g_PppHdlcHardStat.ulDefMaxInputCntOnce  = TTF_MAX(g_PppHdlcHardStat.ulDefMaxInputCntOnce, stBuildInfo.ulInputLinkNodeCnt);
    g_PppHdlcHardStat.ulDefMaxInputSizeOnce = TTF_MAX(g_PppHdlcHardStat.ulDefMaxInputSizeOnce, stBuildInfo.ulInputLinkTotalSize);

    /* 配置封装、解封装通用寄存器 */
    PPP_HDLC_HARD_CommCfgReg();

    /* 配置内存相关寄存器 */
    PPP_HDLC_HARD_DefCfgBufReg(pstDefBuffInfo);

    /* 配置压缩指示、非完整帧相关信息寄存器 */
    PPP_HDLC_HARD_DefCfgReg(HDLC_IP_MODE, pstLink, HDLC_DEF_GET_UNCOMPLETED_INFO(usPppId));

    /* 配置使能寄存器，并上报使能前寄存器可维可测 */
    ulEnableInterrupt = PPP_HDLC_HARD_DefCfgEnReg(stBuildInfo.ulInputLinkTotalSize);

    /* 等待解封装暂停或完成，然后处理输出数据，可能会有多次停等的过程 */
    PPP_HDLC_HARD_DefWaitAndProc(HDLC_IP_MODE, ulEnableInterrupt, usPppId, pstLink);

    /* 释放已解封装完成的数据 */
    PPP_HDLC_HARD_CommReleaseLink(stBuildInfo.apstInputLinkNode, stBuildInfo.ulInputLinkNodeCnt);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_DefRawData
 功能描述  : 硬件方式PPP类型解封装
 输入参数  : usPppId    -   PPP实体ID
             pstLink    -   PPP链路信息
             pstDataQ   -   PPP数据队列
             ulDealCnt  -   本次处理数据包个数
 输出参数  : 参见PPP_HDLC_RESULT_TYPE_ENUM_UINT32
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_DefRawData
(
    PPP_ID              usPppId,
    struct link        *pstLink,
    PPP_ZC_QUEUE_STRU  *pstDataQ,
    VOS_UINT32         *pulDealCnt
)
{
    VOS_UINT32                          ulEnableInterrupt;
    HDLC_PARA_LINK_BUILD_INFO_STRU      stBuildInfo;
    HDLC_PARA_LINK_BUILD_PARA_STRU      stBuildPara;
    VOS_UINT32                          ulBuildResult;
    HDLC_DEF_BUFF_INFO_STRU            *pstDefBuffInfo;

    pstDefBuffInfo = HDLC_DEF_GET_BUF_INFO(usPppId);

    VOS_MemSet_s(&stBuildInfo, sizeof(HDLC_PARA_LINK_BUILD_INFO_STRU),
                 0, sizeof(HDLC_PARA_LINK_BUILD_INFO_STRU));

    stBuildPara.ucDataType = PPP_PULL_RAW_DATA_TYPE;
    stBuildPara.usProtocol = PROTO_IP;
    stBuildPara.pstDataQ   = pstDataQ;
    stBuildPara.usPppId    = usPppId;

    /* 构造输入参数链表，并上报链表内存可维可测 */
    ulBuildResult = PPP_HDLC_HARD_DefBuildInputParaLink(&stBuildPara, &stBuildInfo);

    *pulDealCnt = stBuildInfo.ulDealCnt;

    if (VOS_OK != ulBuildResult)
    {
        return ulBuildResult;
    }

    g_PppHdlcHardStat.ulDefMaxInputCntOnce  = TTF_MAX(g_PppHdlcHardStat.ulDefMaxInputCntOnce, stBuildInfo.ulInputLinkNodeCnt);
    g_PppHdlcHardStat.ulDefMaxInputSizeOnce = TTF_MAX(g_PppHdlcHardStat.ulDefMaxInputSizeOnce, stBuildInfo.ulInputLinkTotalSize);

    /* 配置封装、解封装通用寄存器 */
    PPP_HDLC_HARD_CommCfgReg();

    /* 配置内存相关寄存器 */
    PPP_HDLC_HARD_DefCfgBufReg(pstDefBuffInfo);

    /* 配置压缩指示、非完整帧相关信息寄存器 */
    PPP_HDLC_HARD_DefCfgReg(HDLC_PPP_MODE, pstLink, HDLC_DEF_GET_UNCOMPLETED_INFO(usPppId));

    /* 配置使能寄存器，并上报使能前寄存器可维可测 */
    ulEnableInterrupt = PPP_HDLC_HARD_DefCfgEnReg(stBuildInfo.ulInputLinkTotalSize);

    /* 等待解封装暂停或完成，然后处理输出数据，可能会有多次停等的过程 */
    PPP_HDLC_HARD_DefWaitAndProc(HDLC_PPP_MODE, ulEnableInterrupt, usPppId, pstLink);

    /* 释放已解封装完成的数据 */
    PPP_HDLC_HARD_CommReleaseLink(stBuildInfo.apstInputLinkNode, stBuildInfo.ulInputLinkNodeCnt);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmUpdateLink
 功能描述  : 申请一块内存后更新链表相关结构信息
 输入参数  : ulAllocMemCnt      - 本次申请的内存块个数
             aulAllocLen        - 记录本次申请的各个内存块的长度
             papstAllocedMem    - 记录本次申请的各个内存块控制结构指针
             pstBuildPara       -   构造链表所需的参数
 输出参数  : pstBuildInfo       -   构造链表的结果信息，保存输入链表长度、节点个数、零拷贝内存指针
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_FrmUpdateLink
(
    VOS_UINT32                      ulAllocMemCnt,
    VOS_UINT32 *                    aulAllocLen,
    PPP_ZC_STRU                   **ppstAllocedMem,
    HDLC_PARA_LINK_BUILD_PARA_STRU *pstBuildPara,
    HDLC_PARA_LINK_BUILD_INFO_STRU *pstBuildInfo
)
{
    VOS_UINT32                          ulMemLoop;
    VOS_UINT32                          ulNodeIndex;
    HDLC_PARA_LINK_NODE_STRU           *pstOutputParaNode;
    HDLC_FRM_BUFF_INFO_STRU            *pstFrmBuffInfo;


    /* 根据usPppId找到对应的内存  */
    pstFrmBuffInfo          = HDLC_FRM_GET_BUF_INFO(pstBuildPara->usPppId);

    if( (pstBuildInfo->ulOutputLinkNodeCnt + ulAllocMemCnt) > TTF_HDLC_FRM_OUTPUT_PARA_LINK_MAX_NUM )
    {
        g_PppHdlcHardStat.ulFrmOutputLinkFullCnt++;

        PPP_HDLC_HARD_CommReleaseLink(ppstAllocedMem, ulAllocMemCnt);

        return VOS_ERR;
    }

    /* 更新目的空间数组 apstOutputLinkNode，用于释放目的空间 */
    VOS_MemCpy_s(&(pstBuildInfo->apstOutputLinkNode[pstBuildInfo->ulOutputLinkNodeCnt]),
               ulAllocMemCnt * sizeof(PPP_ZC_STRU *),
               &(ppstAllocedMem[0]),
               ulAllocMemCnt * sizeof(PPP_ZC_STRU *));

    /* 给封装输出参数链表结点的各个成员赋值 */
    for ( ulMemLoop = 0; ulMemLoop < ulAllocMemCnt; ulMemLoop++ )
    {
        ulNodeIndex = pstBuildInfo->ulOutputLinkNodeCnt;

        pstOutputParaNode = &(pstFrmBuffInfo->astOutputParaLinkNodeBuf[ulNodeIndex]);

        /* 更新上个输入参数链表结点的pstNextNode域 */
        if (0 != ulNodeIndex)
        {
            pstFrmBuffInfo->astOutputParaLinkNodeBuf[ulNodeIndex - 1].pstNextNode = (HDLC_PARA_LINK_NODE_STRU*)PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)pstOutputParaNode);
        }

        pstOutputParaNode->pucDataAddr = (VOS_UINT8 *)PPP_VIRT_TO_PHY((VOS_UINT_PTR)PPP_ZC_GET_DATA_PTR(ppstAllocedMem[ulMemLoop]));
        pstOutputParaNode->usDataLen   = (VOS_UINT16)aulAllocLen[ulMemLoop];
        pstOutputParaNode->pstNextNode = VOS_NULL_PTR;

        pstBuildInfo->ulOutputLinkNodeCnt++;
        pstBuildInfo->ulOutputLinkTotalSize += aulAllocLen[ulMemLoop];

        PPP_HDLC_CACHE_INVALID((VOS_UINT8 *)PPP_PHY_TO_VIRT((VOS_UINT32)(pstOutputParaNode->pucDataAddr)), pstOutputParaNode->usDataLen);
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_OutputMemAlloc
 功能描述  : 封装输出目的空间申请
 输入参数  : usLen          -   待申请的内存长度
             pstBuildPara   -   构造链表所需的参数
 输出参数  : pstBuildInfo   -   构造链表的结果信息，保存输入链表长度、节点个数、零拷贝内存指针
 返 回 值  : VOS_OK/VOS_ERR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_FrmOutputMemAlloc
(
    VOS_UINT16                           usLen,
    HDLC_PARA_LINK_BUILD_PARA_STRU      *pstBuildPara,
    HDLC_PARA_LINK_BUILD_INFO_STRU      *pstBuildInfo
)
{
    VOS_UINT16                  usFrmedMaxLen;                                  /* 封装后的可能的最大长度 */
    VOS_UINT16                  usAllocLen;
    VOS_UINT32                  ulAllocLoop;
    VOS_UINT32                  ulLoopCnt;
    VOS_UINT32                  ulAllocMemCnt;                                  /* 本次申请的内存块数 */
    VOS_UINT32                  aulAllocLen[HDLC_OUTPUT_PARA_LINK_MAX_SIZE];    /* 记录本次申请的各个内存块长度 */
    PPP_ZC_STRU *               apstAllocedMem[HDLC_OUTPUT_PARA_LINK_MAX_SIZE]; /* 记录本次申请的各个内存块指针 */
    PPP_ZC_STRU                *pstMem;


    /* 封装后可能的最大数据长度(2*原始数据长度+13B) */
    usFrmedMaxLen = (VOS_UINT16)HDLC_FRM_GET_MAX_FRAMED_LEN(usLen);
    ulAllocMemCnt = 0;
    ulLoopCnt     = TTF_CEIL(usFrmedMaxLen, PPP_ZC_MAX_DATA_LEN);

    for ( ulAllocLoop = 0 ; ulAllocLoop < ulLoopCnt; ulAllocLoop++ )
    {
        if (ulAllocLoop != (ulLoopCnt - 1) )
        {
            usAllocLen = PPP_ZC_MAX_DATA_LEN;
        }
        else
        {
            usAllocLen = (VOS_UINT16)(usFrmedMaxLen - ulAllocLoop*PPP_ZC_MAX_DATA_LEN);
        }

        pstMem = PPP_MemAlloc(usAllocLen, PPP_ZC_DL_RESERVE_LEN);

        if (VOS_NULL_PTR == pstMem)
        {
            break;
        }

        ulAllocMemCnt++;
        aulAllocLen[ulAllocLoop]    = usAllocLen;
        apstAllocedMem[ulAllocLoop] = pstMem;
    }

    /* 如果有申请内存失败的情况，则释放本次已经申请的内存 */
    if (ulLoopCnt > ulAllocMemCnt)
    {
        PPP_HDLC_HARD_CommReleaseLink(&(apstAllocedMem[0]), ulAllocMemCnt);
        return VOS_ERR;
    }

    return PPP_HDLC_HARD_FrmUpdateLink(ulAllocMemCnt,
                                       &(aulAllocLen[0]), &(apstAllocedMem[0]),
                                       pstBuildPara, pstBuildInfo);
}


/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmGetProtocol
 功能描述  : PPP模式下获取待封装数据包协议值
 输入参数  : pstMem         -   待封装数据包
 输出参数  : pusProtocol    -   待封装数据包协议值
 返 回 值  : VOS_OK/VOS_ERR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_FrmGetProtocol
(
    PPP_ZC_STRU     *pstMem,
    VOS_UINT16      *pusProtocol
)
{
    VOS_UINT8           aucProto[2] = {0, 0};
    VOS_UINT32          ulRtn;


    ulRtn = PPP_MemGet(pstMem, 0, aucProto, 2);

    if ( PS_FAIL == ulRtn )
    {
        return VOS_ERR;
    }

    if ( 0x01 == GET_BITS_FROM_BYTE(aucProto[0], 0x01) )
    {
        *pusProtocol = HDLC_MAKE_WORD(0x00, aucProto[0]);
    }
    else
    {
        if ( 0x01 == GET_BITS_FROM_BYTE(aucProto[1], 0x01) )
        {
            *pusProtocol = HDLC_MAKE_WORD(aucProto[0], aucProto[1]);
        }
        else
        {
            PPP_HDLC_WARNING_LOG("PPP_HDLC_HARD_FrmGetProtocol, Warning, PPP mode framer data protocol error\r\n");
            return VOS_ERR;
        }
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmBuildParaLink
 功能描述  : 检查构造输入输出参数链表相关参数是否合法
 输入参数  : pstMem     -   待封装数据包
             ulDataLen  -   待封装数据包长度
             ucDataType -   待封装数据包类型
             usProtocol -   待封装数据包协议类型
 输出参数  : 无
 返 回 值  : PPP_HDLC_PARA_CHECK_RESULT_ENUM_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
PPP_HDLC_PARA_CHECK_RESULT_ENUM_UINT32 PPP_HDLC_HARD_FrmCheckPara
(
    PPP_ZC_STRU                 *pstMem,
    VOS_UINT32                   ulDataLen,
    PPP_DATA_TYPE_ENUM_UINT8     ucDataType,
    VOS_UINT16                   usProtocol
)
{
    PPP_DATA_TYPE_ENUM_UINT8     ucCurrDataType;
    VOS_UINT32                   ulRlst;
    VOS_UINT16                   usCurrProtocol;


    /* 获取待封装数据包类型 */
    ucCurrDataType = (PPP_ZC_GET_DATA_APP(pstMem) & 0x00FF);

    if ( ucDataType != ucCurrDataType )
    {
        return PPP_HDLC_PARA_CHECK_FAIL_KEEP;
    }

    /* 待封装数据包长度异常 */
    if ( (0 == ulDataLen) || (HDLC_FRM_IN_PER_MAX_CNT < ulDataLen) )
    {
        PPP_HDLC_NORMAL_LOG1("PPP_HDLC_HARD_FrmCheckPara,IP mode framer data len wrong %d\r\n", ulDataLen);

        return PPP_HDLC_PARA_CHECK_FAIL_DISCARD;
    }

    /* 如果不是PPP模式 */
    if( PPP_PUSH_RAW_DATA_TYPE != ucCurrDataType )
    {
        return PPP_HDLC_PARA_CHECK_PASS;
    }

    ulRlst  = PPP_HDLC_HARD_FrmGetProtocol(pstMem, &usCurrProtocol);

    if( VOS_OK != ulRlst )
    {
        PPP_HDLC_WARNING_LOG("PPP_HDLC_HARD_FrmCheckPara, Warning, PPP mode get framer data protocol error\r\n");
        return PPP_HDLC_PARA_CHECK_FAIL_DISCARD;
    }

    /* 保证一次链表配置过程中封装协议值一致 */
    if( usProtocol != usCurrProtocol )
    {
        return PPP_HDLC_PARA_CHECK_FAIL_KEEP;
    }

    return PPP_HDLC_PARA_CHECK_PASS;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmStartTimer
 功能描述  : 申请封装目的空间失败开启定时器
 输入参数  : pstBuildPara   -   构造链表所需的参数
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_FrmStartTimer
(
    HDLC_PARA_LINK_BUILD_PARA_STRU      *pstBuildPara
)
{
    if( VOS_NULL_PTR != g_stHdlcConfigInfo.stHdlcFrmTimerHandle )
    {
        return;
    }

    if (VOS_OK != VOS_StartRelTimer(&g_stHdlcConfigInfo.stHdlcFrmTimerHandle, PS_PID_APP_PPP,
                     HDLC_FRM_TIME_INTERVAL,
                     TIMER_HDLC_FRM_OUTPUT_SPACE_ALLOC_FAIL,
                     (VOS_UINT32)pstBuildPara->usPppId,
                     VOS_RELTIMER_NOLOOP, VOS_TIMER_PRECISION_0))
    {
        PPP_HDLC_WARNING_LOG("PPP_HDLC_HARD_FrmStartTimer, Warning, start reltimer error\r\n");
    }

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmBuildParaLink
 功能描述  : 构造输入输出参数链表，并上报链表内存可维可测
 输入参数  : pstBuildPara   -   构造链表所需的参数
 输出参数  : pstBuildInfo   -   构造链表的结果信息，保存输入链表长度、节点个数、零拷贝内存指针
 返 回 值  : VOS_OK/VOS_ERR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_FrmBuildParaLink
(
 HDLC_PARA_LINK_BUILD_PARA_STRU      *pstBuildPara,
 HDLC_PARA_LINK_BUILD_INFO_STRU      *pstBuildInfo
 )
{
    PPP_ZC_STRU                             *pstMem;
    VOS_UINT32                               ulLoop;
    VOS_UINT16                               usUnFrmLen;
    VOS_UINT32                               ulAllocResult;
    HDLC_PARA_LINK_NODE_STRU                *pstParaNode;
    VOS_UINT32                               ulNodeIndex;
    PPP_HDLC_PARA_CHECK_RESULT_ENUM_UINT32   enCheckResult;
    VOS_UINT32                               ulAllocMemFail;
    HDLC_FRM_BUFF_INFO_STRU                 *pstFrmBuffInfo;


    VOS_MemSet_s(pstBuildInfo, sizeof(HDLC_PARA_LINK_BUILD_INFO_STRU),
                 0, sizeof(HDLC_PARA_LINK_BUILD_INFO_STRU));

    /* 根据usPppId找到对应的内存  */
    pstFrmBuffInfo = HDLC_FRM_GET_BUF_INFO(pstBuildPara->usPppId);
    ulAllocMemFail = VOS_FALSE;

    for(ulLoop = 0; ulLoop < TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_NUM; ulLoop++)
    {
        pstMem = (PPP_ZC_STRU *)PPP_ZC_PEEK_QUEUE_HEAD(pstBuildPara->pstDataQ);

        if( VOS_NULL_PTR == pstMem )
        {
            break;
        }

        usUnFrmLen    = PPP_ZC_GET_DATA_LEN(pstMem);
        enCheckResult = PPP_HDLC_HARD_FrmCheckPara(pstMem, usUnFrmLen,
                                                   pstBuildPara->ucDataType,
                                                   pstBuildPara->usProtocol);

        if( PPP_HDLC_PARA_CHECK_FAIL_DISCARD == enCheckResult )
        {
            pstMem = PPP_ZC_DEQUEUE_HEAD(pstBuildPara->pstDataQ);
            PPP_MemFree(pstMem);
            pstBuildInfo->ulDealCnt++;

            g_PppHdlcHardStat.ulFrmInputDiscardCnt++;
            continue;
        }

        if( PPP_HDLC_PARA_CHECK_FAIL_KEEP == enCheckResult )
        {
            break;
        }

        /* 一次配链表过程中待封装数据包总长不能超过15KB */
        if( pstBuildInfo->ulInputLinkTotalSize + usUnFrmLen > TTF_HDLC_FRM_INPUT_PARA_LINK_MAX_SIZE )
        {
            break;
        }

        /* 申请封装目的空间 */
        ulAllocResult = PPP_HDLC_HARD_FrmOutputMemAlloc(usUnFrmLen, pstBuildPara, pstBuildInfo);

        if( VOS_OK != ulAllocResult )
        {
            ulAllocMemFail = VOS_TRUE;

            g_PppHdlcHardStat.ulFrmAllocOutputMemFailCnt++;
            break;
        }

        /* 从PPP队列取出头结点，并插入输入队列 pstBuildInfo->apstInputLinkNode */
        pstMem = PPP_ZC_DEQUEUE_HEAD(pstBuildPara->pstDataQ);

        /* 获取当前要添加节点的下标 */
        ulNodeIndex = pstBuildInfo->ulInputLinkNodeCnt;

        /* 本次要构造的参数节点 */
        pstParaNode = &(pstFrmBuffInfo->astInputParaLinkNodeBuf[ulNodeIndex]);

        /* 填写输入参数链表结点相关域 */
        if( 0 != ulNodeIndex )
        {
            pstFrmBuffInfo->astInputParaLinkNodeBuf[ulNodeIndex - 1].pstNextNode = (HDLC_PARA_LINK_NODE_STRU*)PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)pstParaNode);
        }

        pstParaNode->pucDataAddr = (VOS_UINT8*)PPP_VIRT_TO_PHY((VOS_UINT_PTR)PPP_ZC_GET_DATA_PTR(pstMem));
        pstParaNode->usDataLen   = usUnFrmLen;
        pstParaNode->pstNextNode = VOS_NULL_PTR;

        pstBuildInfo->apstInputLinkNode[ulNodeIndex] = pstMem;
        pstBuildInfo->ulInputLinkTotalSize          += (VOS_UINT32)usUnFrmLen;
        pstBuildInfo->ulInputLinkNodeCnt++;
        pstBuildInfo->ulDealCnt++;

        /* 需要将数据写回DDR，HDLC从DDR中读数据 */
        PPP_HDLC_CACHE_FLUSH(PPP_ZC_GET_DATA_PTR(pstMem), PPP_ZC_GET_DATA_LEN(pstMem));
    }

    if (0 == pstBuildInfo->ulInputLinkNodeCnt)
    {
        if (VOS_TRUE == ulAllocMemFail)
        {
            /* 等待一段时间后，重新尝试申请内存再封装 */
            PPP_HDLC_HARD_FrmStartTimer(pstBuildPara);

            g_PppHdlcHardStat.ulFrmAllocFirstMemFailCnt++;
        }

        return VOS_ERR;
    }

    /* 报输入输出参数链表内容可维可测 */
    PPP_HDLC_HARD_MntnFrmTraceInput(pstFrmBuffInfo, pstBuildInfo);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmCfgBufReg
 功能描述  : 配置封装使用的内存至相关寄存器
 输入参数  : pstFrmBuffInfo -   封装使用的内存信息
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_FrmCfgBufReg(HDLC_FRM_BUFF_INFO_STRU *pstFrmBuffInfo)
{
    /* 将封装输入参数链表的起始地址配置给寄存器frm_in_lli_addr */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_IN_LLI_ADDR(HDLC_IP_BASE_ADDR),
                    PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)&(pstFrmBuffInfo->astInputParaLinkNodeBuf[0])));

    /* 将封装输出参数链表的起始地址配置给寄存器frm_out_lli_addr */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_OUT_LLI_ADDR(HDLC_IP_BASE_ADDR),
                    PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)&(pstFrmBuffInfo->astOutputParaLinkNodeBuf[0])));

    /* 将封装有效帧结果信息上报空间起始地址配置给寄存器frm_rpt_addr */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_RPT_ADDR(HDLC_IP_BASE_ADDR),
                    PPP_HDLC_NODE_VIRT_TO_PHY((VOS_UINT32)&(pstFrmBuffInfo->astRptNodeBuf[0])));

    /* 将封装有效帧结果信息上报空间深度配置给寄存器frm_rpt_dep的[15:0]位 */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_RPT_DEP_ADDR(HDLC_IP_BASE_ADDR),
                    (VOS_UINT32)TTF_HDLC_FRM_RPT_BUF_LEN & 0xFFFF);

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmCfgReg
 功能描述  : IP模式下配置封装相关寄存器
 输入参数  : pstLink    - PPP链路信息
             usProtocol - 待封装的协议值
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_FrmCfgReg
(
    struct link        *pstLink,
    VOS_UINT16          usProtocol
)
{
    /*
    hdlc_frm_cfg   (0x20)
      31              16 15           4 3      2   1      0
    |-------------------|--------------|--------|------|------|
    |      protocol     |      Rsv     |   Pfc  | Acfc |1dor2d|

    frm_protocol         [31:16] 16'b0   h/s R/W  封装协议值。有效协议值规定参见规格列表。
    Reserved             [15:4]  12'b0   h/s R/W  保留位。读时返回0。写时无影响。
    frm_pfc              [3:2]   2'b0    h/s R/W  P域压缩指示：00：硬件模块添加P域，P域无压缩;
                                                               01：硬件模块添加P域，P域压缩;
                                                               11：硬件模块不添加P域;
                                                               其他：无效;
    frm_acfc             [1]     1'b0    h/s R/W  AC域压缩指示：0：AC域无压缩;1：表示AC域压缩;
    frm_in_lli_1dor2d    [0]     1'b0    h/s R/W  封装输入一维或二维链表选择指示寄存器：
                                                                0为一维;1为二维;

    IP模式一定添加P域,PPP模式一定不添加P域
      LCP帧: AC域不压缩，P域不压缩
    */

    VOS_UINT32              ulFrmCfg;


    /* 将寄存器hdlc_frm_cfg的[0]位frm_in_lli_1dor2d配置为0 */
    ulFrmCfg = 0x0;

    /* 配置hdlc_frm_cfg的 P域和 AC域 */
    if (PROTO_LCP != usProtocol)
    {
        if ( 1 == pstLink->lcp.his_acfcomp )
        {
            ulFrmCfg |= (1 << HDLC_FRM_ACFC_BITPOS);
        }

        if ( 1 == pstLink->lcp.his_protocomp )
        {
            ulFrmCfg |= (1 << HDLC_FRM_PFC_BITPOS);
        }

        /* 配置hdlc_frm_accm */
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_ACCM_ADDR(HDLC_IP_BASE_ADDR),
                            pstLink->lcp.his_accmap);
    }
    else
    {
        PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_ACCM_ADDR(HDLC_IP_BASE_ADDR), 0xFFFFFFFFU);
    }

    /* 配置寄存器hdlc_frm_cfg的[31:16]位frm_protocol为usProtocol */
    ulFrmCfg |= ( ((VOS_UINT32)usProtocol) << 16 );

    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_CFG_ADDR(HDLC_IP_BASE_ADDR), ulFrmCfg);

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmCfgReg
 功能描述  : PPP模式下配置封装相关寄存器
 输入参数  : pstLink    - PPP链路信息
             usProtocol - 待封装的协议值
 输出参数  :
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_FrmRawCfgReg
(
    struct link        *pstLink,
    VOS_UINT16          usProtocol
)
{
    /*
    hdlc_frm_cfg   (0x20)
      31              16 15           4 3      2   1      0
    |-------------------|--------------|--------|------|------|
    |      protocol     |      Rsv     |   Pfc  | Acfc |1dor2d|

    frm_protocol         [31:16] 16'b0   h/s R/W  封装协议值。有效协议值规定参见规格列表。
    Reserved             [15:4]  12'b0   h/s R/W  保留位。读时返回0。写时无影响。
    frm_pfc              [3:2]   2'b0    h/s R/W  P域压缩指示：00：硬件模块添加P域，P域无压缩;
                                                               01：硬件模块添加P域，P域压缩;
                                                               11：硬件模块不添加P域;
                                                               其他：无效;
    frm_acfc             [1]     1'b0    h/s R/W  AC域压缩指示：0：AC域无压缩;1：表示AC域压缩;
    frm_in_lli_1dor2d    [0]     1'b0    h/s R/W  封装输入一维或二维链表选择指示寄存器：
                                                                0为一维;1为二维;

    IP模式一定添加P域,PPP模式一定不添加P域
      LCP帧: AC域不压缩，P域不压缩
    */

    VOS_UINT32              ulFrmCfg;
    /*PS_BOOL_ENUM_UINT8      enPComp;       是否压缩协议字段, 是 - PS_TRUE */
    PS_BOOL_ENUM_UINT8      enACComp;     /* 是否压缩地址和控制字段, 是 - PS_TRUE */
    VOS_UINT32              ulACCM;
    VOS_UINT8               ucACComp;
    VOS_UINT8               ucPComp;
    VOS_UINT8               ucLowByte;
    VOS_UINT16              usLowWord;


    /* 将寄存器hdlc_frm_cfg的[0]位frm_in_lli_1dor2d配置为0 */
    ulFrmCfg    = 0x0;

    if (PROTO_LCP == usProtocol)
    {
        /* LCP帧P不压缩, AC不压缩, ACCM为全转义 */
        /*enPComp     = PS_FALSE; */
        enACComp    = PS_FALSE;
        ulACCM      = 0xFFFFFFFFU;
    }
    else
    {
        /* 使用默认值 */
        /*enPComp     = (1 == pstLink->lcp.his_protocomp) ? PS_TRUE : PS_FALSE; */
        enACComp    = (1 == pstLink->lcp.his_acfcomp) ? PS_TRUE : PS_FALSE;
        /* 由于PPP拨号不知道ACCM, 故设置为协议默认值 */
        ulACCM      = 0xFFFFFFFFU;
    }

    /* 获取AC域压缩指示, P域压缩指示, 协议值, ACCM */
    ucACComp = (PS_TRUE == enACComp) ? 1 : 0;
    ucPComp = HDLC_PROTOCOL_NO_ADD;

    /* 设置accm */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_ACCM_ADDR(HDLC_IP_BASE_ADDR),ulACCM);

    /* 将ucPComp设置到一个字节的第2, 3位上 */
    ucLowByte = SET_BITS_VALUE_TO_BYTE(ucPComp, HDLC_FRM_PFC_BITPOS);

    if (1 == ucACComp)  /* AC域压缩 */
    {
        SET_BIT_TO_BYTE(ucLowByte, HDLC_FRM_ACFC_BITPOS);
    }
    else
    {
        CLEAR_BIT_TO_BYTE(ucLowByte, HDLC_FRM_ACFC_BITPOS);
    }

    usLowWord   = HDLC_MAKE_WORD(0x00, ucLowByte);
    ulFrmCfg    = HDLC_MAKE_DWORD(usProtocol, usLowWord);

    /* 设置AC域压缩指示, P域压缩指示, 协议值和使能封装标记, accm */
    PPP_HDLC_WRITE_32REG(SOC_ARM_HDLC_FRM_CFG_ADDR(HDLC_IP_BASE_ADDR),ulFrmCfg);

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmProcValidFram
 功能描述  : 处理有效帧，并释放剩余内存
 输入参数  : usPppId        -   PPP实体ID
 输出参数  : pstBuildInfo   -   构造链表的结果信息，保存输入链表长度、节点个数、零拷贝内存指针
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_FrmProcValidFrames
(
    PPP_ID                               usPppId,
    HDLC_PARA_LINK_BUILD_INFO_STRU      *pstBuildInfo
)
{
    PPP_ZC_STRU                        *pstMem;
    VOS_UINT16                          usFrmOutSegNum;
    VOS_UINT8                           ucFrmValidNum;
    VOS_UINT32                          ulFrmOutSpaceCnt;
    HDLC_FRM_BUFF_INFO_STRU            *pstFrmBuffInfo;
    HDLC_PARA_LINK_NODE_STRU           *pstOutputParaLink;
    HDLC_FRM_RPT_NODE_STRU             *pstFrmRptNodeStru;
    PPP_ZC_STRU                       **ppstOutputLinkNode;
    VOS_UINT32                          ucRptSpaceIndex;
    VOS_UINT32                          ulOutputLinkIndex;
    VOS_UINT8                          *pucFrmOutAddr;
    VOS_UINT16                          usFrmOutLen;
    VOS_UINT16                          usDataLen;


    pstFrmBuffInfo     = HDLC_FRM_GET_BUF_INFO(usPppId);                   /* 根据usPppId找到对应的内存  */
    pstFrmRptNodeStru  = &(pstFrmBuffInfo->astRptNodeBuf[0]);              /* 封装上报空间首地址 */
    pstOutputParaLink  = &(pstFrmBuffInfo->astOutputParaLinkNodeBuf[0]);   /* 封装输出链表首地址 */
    ppstOutputLinkNode = &(pstBuildInfo->apstOutputLinkNode[0]);           /* 封装输出链表节点对应零拷贝内存链表首地址 */
    ulFrmOutSpaceCnt   = pstBuildInfo->ulOutputLinkNodeCnt;                /* 封装输出链表节点的个数 */

    /* 有效帧存放占用片段个数usFrmOutSegNum= hdlc_frm_status寄存器[31:16]位的值 */
    usFrmOutSegNum = (VOS_UINT16)TTF_Read32RegByBit(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR), 16, 31);

    /* 有效帧个数usFrmValidNum= hdlc_frm_status寄存器[15:8]位的值 */
    ucFrmValidNum  = (VOS_UINT8)TTF_Read32RegByBit(SOC_ARM_HDLC_FRM_STATUS_ADDR(HDLC_IP_BASE_ADDR), 8, 15);

    /* 上报封装后数据可维可测:上报空间信息、输出链表内容 */
    PPP_HDLC_HARD_MntnFrmTraceOutput(ucFrmValidNum, usFrmOutSegNum, pstFrmBuffInfo, pstBuildInfo);

    /* ucFrmValidNum肯定要小于等于使用的内存块数usFrmOutSegNum */
    if( ucFrmValidNum > usFrmOutSegNum )
    {
        PPP_HDLC_ERROR_LOG2("PPP_HDLC_HARD_FrmProcValidFram, ERROR, frm_valid_num = %d > usFrmOutSegNum = %d",
                      ucFrmValidNum, usFrmOutSegNum);
        PPP_HDLC_HARD_CommReleaseLink(ppstOutputLinkNode, ulFrmOutSpaceCnt);
        return;
    }

    /* ucFrmValidNum应该与输入数据相同ulInputLinkNodeCnt，如果解封装出错可能会比后者小 */
    if( ucFrmValidNum > pstBuildInfo->ulInputLinkNodeCnt )
    {
        PPP_HDLC_ERROR_LOG2("PPP_HDLC_HARD_FrmProcValidFram, ERROR, frm_valid_num = %d > InputLinkNodeCnt = %d",
                      ucFrmValidNum, pstBuildInfo->ulInputLinkNodeCnt);
        PPP_HDLC_HARD_CommReleaseLink(ppstOutputLinkNode, ulFrmOutSpaceCnt);
        return;
    }

    /* usFrmOutSegNum使用的内存块数肯定小于等于ulOutputLinkNodeCnt */
    if( usFrmOutSegNum  > ulFrmOutSpaceCnt )
    {
        PPP_HDLC_ERROR_LOG2("PPP_HDLC_HARD_FrmProcValidFram, ERROR, frm_out_seg_num = %d > OutputLinkNodeCnt = %d",
                      usFrmOutSegNum, ulFrmOutSpaceCnt);
        PPP_HDLC_HARD_CommReleaseLink(ppstOutputLinkNode, ulFrmOutSpaceCnt);
        return;
    }

    ucRptSpaceIndex   = 0;
    ulOutputLinkIndex = 0;

    while( ucRptSpaceIndex < ucFrmValidNum )
    {
        pucFrmOutAddr    = (VOS_UINT8*)PPP_PHY_TO_VIRT((VOS_UINT32)pstFrmRptNodeStru[ucRptSpaceIndex].pucFrmOutOneAddr);
        usFrmOutLen      = pstFrmRptNodeStru[ucRptSpaceIndex].usFrmOutOneLen;

        if ( (0 == usFrmOutLen) || (HDLC_FRM_OUT_PER_MAX_CNT < usFrmOutLen) )
        {
            PPP_HDLC_ERROR_LOG1("PPP_HDLC_HARD_FrmProcValidFrames, ERROR, invalid usFrmOutOneLen %d\r\n", usFrmOutLen);

            /* 释放申请的目的空间 */
            PPP_HDLC_HARD_CommReleaseLink(ppstOutputLinkNode + ulOutputLinkIndex,
                                          ulFrmOutSpaceCnt - ulOutputLinkIndex);
            return;
        }

        pstMem      = ppstOutputLinkNode[ulOutputLinkIndex];

        if( pucFrmOutAddr != PPP_ZC_GET_DATA_PTR(pstMem) )
        {
            PPP_HDLC_ERROR_LOG("PPP_HDLC_HARD_FrmProcValidFram, ERROR, SOC copy error!\r\n");

            /* 释放申请的目的空间 */
            PPP_HDLC_HARD_CommReleaseLink(ppstOutputLinkNode + ulOutputLinkIndex,
                                          ulFrmOutSpaceCnt - ulOutputLinkIndex);
            return;
        }

        while( usFrmOutLen > 0 )
        {
            /* 从pstBuildInfo->apstOutputLinkNode取出头结点 pstMem */
            pstMem    = ppstOutputLinkNode[ulOutputLinkIndex];

            /* 单个输出链表结点所指目的空间的大小 */
            usDataLen = pstOutputParaLink[ulOutputLinkIndex].usDataLen;

            if( usFrmOutLen > usDataLen )
            {
                PPP_ZC_SET_DATA_LEN(pstMem, usDataLen);
                usFrmOutLen -= usDataLen;
            }
            else
            {
                PPP_ZC_SET_DATA_LEN(pstMem, usFrmOutLen);
                usFrmOutLen  = 0;
            }

            PPP_HDLC_ProcDlData(usPppId, pstMem);

            g_PppDataQCtrl.stStat.ulDownlinkSndDataCnt++;

            /* 将发送完的数据包从apstOutputLinkNode中清空 */
            ppstOutputLinkNode[ulOutputLinkIndex] = VOS_NULL_PTR;

            ulOutputLinkIndex++;
        }

        ucRptSpaceIndex++;
    }

    /* 释放剩余未利用的目的空间 */
    PPP_HDLC_HARD_CommReleaseLink(ppstOutputLinkNode + ulOutputLinkIndex,
                                  ulFrmOutSpaceCnt - ulOutputLinkIndex);

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmWaitAndProc
 功能描述  : 等待封装完成，然后处理输出数据,释放多余的目的空间内存
 输入参数  : ulEnableInterrupt  -   中断是否使能
             usPppId    -   PPP实体ID
             pstLink    -   PPP链路信息
 输出参数  : pstBuildInfo   -   构造链表的结果信息，保存输入链表长度、节点个数、零拷贝内存指针
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_FrmWaitAndProc
(
    VOS_UINT32                           ulEnableInterrupt,
    PPP_ID                               usPppId,
    struct link                         *pstLink,
    HDLC_PARA_LINK_BUILD_INFO_STRU      *pstBuildInfo
)
{
    VOS_UINT32                          ulFrmStatus;


    /* 使能中断，则等待中断辅程序释放信号量；否则轮询解封装状态寄存器 */
    ulFrmStatus = PPP_HDLC_HARD_FrmWaitResult(ulEnableInterrupt);

    if ( HDLC_FRM_ALL_PKT_DONE == ulFrmStatus )
    {
        /* 处理有效帧，并释放剩余内存 */
        PPP_HDLC_HARD_FrmProcValidFrames(usPppId, pstBuildInfo);
    }
    else
    {
        /* 释放申请的目的空间 */
        PPP_HDLC_HARD_CommReleaseLink(pstBuildInfo->apstOutputLinkNode, pstBuildInfo->ulOutputLinkNodeCnt);

        /* 打印异常日志，挂起PPP任务 */
        PPP_HDLC_HARD_FrmProcException(ulFrmStatus, ulEnableInterrupt);
    }

    g_stHdlcRegSaveInfo.ulHdlcFrmRawInt = 0xFFFFFFFFU;
    g_stHdlcRegSaveInfo.ulHdlcFrmStatus = 0xFFFFFFFFU;

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmPacket
 功能描述  : IP模式下行封装处理流程
 输入参数  : usPppId        -   PPP实体ID
             usProtocol     -   待封装协议
             pstLink        -   PPP链路信息
             pstDataQ       -   PPP数据队列
 输出参数  : *pulDealCurCnt  -  指向当前处理的PPP队列中的结点个数
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_FrmPacket
(
    PPP_ID              usPppId,
    VOS_UINT16          usProtocol,
    struct link        *pstLink,
    PPP_ZC_QUEUE_STRU  *pstDataQ,
    VOS_UINT32         *pulDealCurCnt
)
{
    VOS_UINT32                          ulEnableInterrupt;
    HDLC_PARA_LINK_BUILD_INFO_STRU      stBuildInfo;
    HDLC_PARA_LINK_BUILD_PARA_STRU      stBuildPara;
    VOS_UINT32                          ulBuildResult;
    HDLC_FRM_BUFF_INFO_STRU            *pstFrmBuffInfo;


    stBuildPara.ucDataType = PPP_PUSH_PACKET_TYPE;
    stBuildPara.usProtocol = usProtocol;
    stBuildPara.usPppId    = usPppId;
    stBuildPara.pstDataQ   = pstDataQ;

    /* 根据usPppId找到对应的内存 */
    pstFrmBuffInfo = HDLC_FRM_GET_BUF_INFO(usPppId);

    /* 构造输入输出参数链表，并上报链表内存可维可测 */
    ulBuildResult  = PPP_HDLC_HARD_FrmBuildParaLink(&stBuildPara, &stBuildInfo);

    *pulDealCurCnt = stBuildInfo.ulDealCnt;

    if (VOS_OK != ulBuildResult)
    {
        PPP_HDLC_WARNING_LOG("PPP_HDLC_HARD_FrmPacket, WARNING, BuildParaLink Fail.\r\n");
        return ulBuildResult;
    }

    g_PppHdlcHardStat.ulFrmMaxInputCntOnce   = TTF_MAX(g_PppHdlcHardStat.ulFrmMaxInputCntOnce, stBuildInfo.ulInputLinkNodeCnt);
    g_PppHdlcHardStat.ulFrmMaxInputSizeOnce  = TTF_MAX(g_PppHdlcHardStat.ulFrmMaxInputSizeOnce, stBuildInfo.ulInputLinkTotalSize);
    g_PppHdlcHardStat.ulFrmMaxOutputCntOnce  = TTF_MAX(g_PppHdlcHardStat.ulFrmMaxOutputCntOnce, stBuildInfo.ulOutputLinkNodeCnt);
    g_PppHdlcHardStat.ulFrmMaxOutputSizeOnce = TTF_MAX(g_PppHdlcHardStat.ulFrmMaxOutputSizeOnce, stBuildInfo.ulOutputLinkTotalSize);

    /* 配置封装、解封装通用寄存器 */
    PPP_HDLC_HARD_CommCfgReg();

    /* 配置内存相关寄存器 */
    PPP_HDLC_HARD_FrmCfgBufReg(pstFrmBuffInfo);

    /* 配置封装相关寄存器 */
    PPP_HDLC_HARD_FrmCfgReg(pstLink, usProtocol);

    /* 配置使能寄存器，并上报使能前寄存器可维可测 */
    ulEnableInterrupt = PPP_HDLC_HARD_FrmCfgEnReg(stBuildInfo.ulInputLinkTotalSize);

    /* 等待封装完成，然后处理输出数据,释放多余的目的空间内存 */
    PPP_HDLC_HARD_FrmWaitAndProc(ulEnableInterrupt, usPppId, pstLink, &stBuildInfo);

    /* 释放pstBuildInfo->apstInputLinkNode中的结点内存 */
    PPP_HDLC_HARD_CommReleaseLink(stBuildInfo.apstInputLinkNode, stBuildInfo.ulInputLinkNodeCnt);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_FrmRawPacket
 功能描述  : PPP模式下行封装处理流程
 输入参数  : usPppId        -   PPP实体ID
             usProtocol     -   待封装数据包协议
             pstLink        -   PPP链路信息
             pstDataQ       -   PPP数据队列
 输出参数  : *pulDealCurCnt  -  指向当前处理的PPP队列中的结点个数
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月20日
    作    者   : c00191211
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_FrmRawData
(
    PPP_ID              usPppId,
    VOS_UINT16          usProtocol,
    struct link        *pstLink,
    PPP_ZC_QUEUE_STRU  *pstDataQ,
    VOS_UINT32         *pulDealCurCnt
)
{
    VOS_UINT32                          ulEnableInterrupt;
    HDLC_PARA_LINK_BUILD_INFO_STRU      stBuildInfo;
    HDLC_PARA_LINK_BUILD_PARA_STRU      stBuildPara;
    VOS_UINT32                          ulBuildResult;
    HDLC_FRM_BUFF_INFO_STRU            *pstFrmBuffInfo;


    stBuildPara.ucDataType = PPP_PUSH_RAW_DATA_TYPE;
    stBuildPara.usProtocol = usProtocol;
    stBuildPara.usPppId    = usPppId;
    stBuildPara.pstDataQ   = pstDataQ;

    /* 根据usPppId找到对应的内存  */
    pstFrmBuffInfo         = HDLC_FRM_GET_BUF_INFO(usPppId);

    /* 构造输入输出参数链表，并上报链表内存可维可测 */
    ulBuildResult  = PPP_HDLC_HARD_FrmBuildParaLink(&stBuildPara, &stBuildInfo);

    *pulDealCurCnt = stBuildInfo.ulDealCnt;

    if (VOS_OK != ulBuildResult)
    {
        return ulBuildResult;
    }


    g_PppHdlcHardStat.ulFrmMaxInputCntOnce   = TTF_MAX(g_PppHdlcHardStat.ulFrmMaxInputCntOnce, stBuildInfo.ulInputLinkNodeCnt);
    g_PppHdlcHardStat.ulFrmMaxInputSizeOnce  = TTF_MAX(g_PppHdlcHardStat.ulFrmMaxInputSizeOnce, stBuildInfo.ulInputLinkTotalSize);
    g_PppHdlcHardStat.ulFrmMaxOutputCntOnce  = TTF_MAX(g_PppHdlcHardStat.ulFrmMaxOutputCntOnce, stBuildInfo.ulOutputLinkNodeCnt);
    g_PppHdlcHardStat.ulFrmMaxOutputSizeOnce = TTF_MAX(g_PppHdlcHardStat.ulFrmMaxOutputSizeOnce, stBuildInfo.ulOutputLinkTotalSize);

    /* 配置封装、解封装通用寄存器 */
    PPP_HDLC_HARD_CommCfgReg();

    /* 配置内存相关寄存器 */
    PPP_HDLC_HARD_FrmCfgBufReg(pstFrmBuffInfo);

    /* 配置封装相关寄存器 */
    PPP_HDLC_HARD_FrmRawCfgReg(pstLink, usProtocol);

    /* 配置使能寄存器，并上报使能前寄存器可维可测 */
    ulEnableInterrupt = PPP_HDLC_HARD_FrmCfgEnReg(stBuildInfo.ulInputLinkTotalSize);

    /* 等待封装完成，然后处理输出数据,释放多余的目的空间内存 */
    PPP_HDLC_HARD_FrmWaitAndProc(ulEnableInterrupt, usPppId, pstLink, &stBuildInfo);

    /* 释放pstBuildInfo->apstInputLinkNode中的结点内存 */
    PPP_HDLC_HARD_CommReleaseLink(stBuildInfo.apstInputLinkNode, stBuildInfo.ulInputLinkNodeCnt);

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_PROC_FUNC
 功能描述  : 硬件HDLC处理函数，在创建PPP实体的时候，根据需要可以灵活地将
             软件或硬件的实现与该实体关联
 输入参数  : usPppId    -   PPP实体ID
             pstLink    -   PPP链路信息
             pstDataQ   -   PPP数据队列
 输出参数  :
 返 回 值  : PPP_HDLC_RESULT_TYPE_ENUM_UINT32
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
PPP_HDLC_RESULT_TYPE_ENUM_UINT32 PPP_HDLC_HARD_ProcData
(
    PPP_ID              usPppId,
    struct link        *pstLink,
    PPP_ZC_QUEUE_STRU  *pstDataQ
)
{
    PPP_ZC_STRU                        *pstMem;
    VOS_UINT32                          ulDealTotalCnt;
    VOS_UINT32                          ulDealCurCnt;
    VOS_UINT32                          ulDealResult;
    VOS_UINT16                          usProtocol;
    PPP_DATA_TYPE_ENUM_UINT8            ucDataType;


    g_PppHdlcHardStat.ulHdlcHardProcCnt++;

    ulDealTotalCnt  = 0;
    ulDealCurCnt    = 0;

    for (;;)
    {
        pstMem  = (PPP_ZC_STRU *)PPP_ZC_PEEK_QUEUE_HEAD(pstDataQ);

        /* 队列为空的时候返回空指针 */
        if ( VOS_NULL_PTR == pstMem )
        {
            return PPP_HDLC_RESULT_COMM_FINISH;
        }

        /*如果循环处理的结点个数超出了队列一次允许处理最大结点数，
          则退出循环并发送PPP_DATA_PROC_NOTIFY消息*/
        if ( (ulDealTotalCnt >= PPP_ONCE_DEAL_MAX_CNT) )
        {
            g_PppHdlcHardStat.ulContinueCnt++;
            return PPP_HDLC_RESULT_COMM_CONTINUE;
        }

        /*处理该结点(结点的释放动作已经在各处理函数内部完成，无需再释放结点)*/
        ucDataType   = (PPP_ZC_GET_DATA_APP(pstMem) & 0x00FF);
        ulDealResult = VOS_OK;
        ulDealCurCnt = 0;

        switch ( ucDataType )
        {
            case PPP_PULL_PACKET_TYPE:

                ulDealResult = PPP_HDLC_HARD_DefPacket(usPppId, pstLink, pstDataQ, &ulDealCurCnt);

                g_PppHdlcHardStat.ulDefIpDataProcCnt += ulDealCurCnt;

                LcpEchoAdjust(pstLink);

                break;
            case PPP_PUSH_PACKET_TYPE:

                ulDealResult = PPP_HDLC_HARD_FrmPacket(usPppId, PROTO_IP, pstLink, pstDataQ, &ulDealCurCnt);

                g_PppHdlcHardStat.ulFrmIpDataProcCnt += ulDealCurCnt;
                break;

            case PPP_PULL_RAW_DATA_TYPE:

                ulDealResult = PPP_HDLC_HARD_DefRawData(usPppId, pstLink, pstDataQ, &ulDealCurCnt);

                g_PppHdlcHardStat.ulDefIpDataProcCnt += ulDealCurCnt;

                LcpEchoAdjust(pstLink);

                break;

            case PPP_PUSH_RAW_DATA_TYPE:

                /* 获取待封装数据包的协议值usProtocol */
                if (VOS_OK == PPP_HDLC_HARD_FrmGetProtocol(pstMem, &usProtocol))
                {
                    ulDealResult = PPP_HDLC_HARD_FrmRawData(usPppId, usProtocol, pstLink, pstDataQ, &ulDealCurCnt);

                    g_PppHdlcHardStat.ulFrmPppDataProcCnt += ulDealCurCnt;
                }
                else
                {
                    pstMem  = (PPP_ZC_STRU *)PPP_ZC_DEQUEUE_HEAD(pstDataQ);
                    PPP_MemFree(pstMem);
                }
                break;

            default:
                pstMem  = (PPP_ZC_STRU *)PPP_ZC_DEQUEUE_HEAD(pstDataQ);
                PPP_MemFree(pstMem);
                PPP_HDLC_WARNING_LOG1("PPP_HDLC_HARD_ProcData, WARNING, ucDataType %d is Abnormal!", ucDataType);
                break;
        }

        /*统计*/
        ulDealTotalCnt += ulDealCurCnt;

        if ( ulDealTotalCnt > g_PppHdlcHardStat.ulMaxCntOnce )
        {
            g_PppHdlcHardStat.ulMaxCntOnce = ulDealTotalCnt;
        }

        if (VOS_OK != ulDealResult)
        {
            PPP_HDLC_WARNING_LOG("PPP_HDLC_HARD_ProcData, WARNING, ulDealResult = ERROR!");
            break;
        }
    } /* for (;;) */

    return PPP_HDLC_RESULT_COMM_ERROR;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_SendAsFrmPacketMsg
 功能描述  : 消息发送函数
 输入参数  : usPppId        -- PPP实体ID
             usProtocol     -- 待封装协议值
             pstMem         -- 待封装数据包
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_UINT32 PPP_HDLC_HARD_SendAsFrmPacketMsg
(
    VOS_UINT16       usPppId,
    VOS_UINT16       usProtocol,
    PPP_ZC_STRU     *pstMem
)
{
    HDLC_PROC_AS_FRM_PACKET_IND_MSG_STRU    *pstMsg;
    VOS_UINT32                               ulLength;


    ulLength    = sizeof(HDLC_PROC_AS_FRM_PACKET_IND_MSG_STRU) - VOS_MSG_HEAD_LENGTH;
    pstMsg      = (HDLC_PROC_AS_FRM_PACKET_IND_MSG_STRU *)PS_ALLOC_MSG(PS_PID_PPP_HDLC, ulLength);

    if (VOS_NULL_PTR == pstMsg)
    {
        /*打印出错信息---申请消息包失败:*/
        PPP_HDLC_ERROR_LOG("PPP_HDLC_HARD_SendMsg, ERROR: PS_ALLOC_MSG Failed!\r\n");
        return VOS_ERR;
    }

    /*填写消息头:*/
    pstMsg->MsgHeader.ulSenderCpuId   = VOS_LOCAL_CPUID;
    pstMsg->MsgHeader.ulSenderPid     = PS_PID_PPP_HDLC;
    pstMsg->MsgHeader.ulReceiverCpuId = VOS_LOCAL_CPUID;
    pstMsg->MsgHeader.ulReceiverPid   = PS_PID_PPP_HDLC;
    pstMsg->MsgHeader.ulLength        = ulLength;

    pstMsg->MsgHeader.ulMsgName       = PPP_HDLC_PROC_AS_FRM_PACKET_IND;
    /*填写消息体:*/
    pstMsg->usPppId                   = usPppId;
    pstMsg->usProtocol                = usProtocol;
    pstMsg->pstMem                    = pstMem;

    /*发送该消息*/
    if (VOS_OK != PS_SEND_MSG(PS_PID_PPP_HDLC, pstMsg))
    {
        /*打印警告信息---发送消息失败:*/
        PPP_HDLC_ERROR_LOG("PPP_HDLC_HARD_SendAsFrmPacketMsg, ERROR : PS_SEND_MSG Failed!");
        PPP_MemFree(pstMem);
        return VOS_ERR;
    }

    return VOS_OK;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_ProcProtocolPacket
 功能描述  : 当做下行封装处理
 输入参数  : usPppId        -- PPP实体ID
             usProtocol     -- 待封装协议值
             pstMem         -- 待封装数据包
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_ProcAsFrmPacket
(
    VOS_UINT16       usPppId,
    VOS_UINT16       usProtocol,
    PPP_ZC_STRU     *pstMem
)
{
    VOS_UINT32              ulDealCurCnt;
    PPP_ZC_QUEUE_STRU       stDataQ;
    struct link            *pstLink;
    VOS_UINT32              ulDealResult;

    pstLink = PPP_LINK(usPppId);

    ulDealCurCnt = 0;

    /* 封装成下行IP类型数据包 */
    PPP_ZC_SET_DATA_APP(pstMem, (VOS_UINT16)(usPppId << 8) | (VOS_UINT16)PPP_PUSH_PACKET_TYPE);

    /* 初始化队列stDataQ, 并将 pstMem 入队 */
    PPP_ZC_QUEUE_INIT(&stDataQ);
    PPP_ZC_ENQUEUE_TAIL(&stDataQ, pstMem);

    /* 直接将该数据包封装后发送给PC */
    ulDealResult = PPP_HDLC_HARD_FrmPacket(usPppId, usProtocol, pstLink, &stDataQ, &ulDealCurCnt);

    if ((VOS_OK != ulDealResult) || (1 != ulDealCurCnt))
    {
        PPP_HDLC_ERROR_LOG2("PPP_HDLC_HARD_MakeFrmPacket, ERROR, ulDealResult %d ulDealCurCnt %d!\r\n",
                      ulDealResult, ulDealCurCnt);

        /* 说明因某种错误导致数据没有从队列中移出，需要释放 */
        if (0 < PPP_ZC_GET_QUEUE_LEN(&stDataQ))
        {
            PPP_MemFree(pstMem);
        }

        return;
    }

    g_PppHdlcHardStat.ulFrmIpDataProcCnt++;

    return;
}

/*****************************************************************************
 函 数 名  : PPP_HDLC_HARD_ProcProtocolPacket
 功能描述  : 硬件封装PPP协议栈输出的协议包
 输入参数  : pstLink    -   PPP链路信息
             pstMbuf    -   PPP协议栈输出的协议包
             ulPri      -   优先级
             usProto    -   数据包对应的协议
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年4月10日
    作    者   : l00164359
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_ProcProtocolPacket
(
    struct link     *pstLink,
    struct ppp_mbuf *pstMbuf,
    VOS_INT32        ulPri,
    VOS_UINT16       usProtocol
)
{
    PPP_ZC_STRU            *pstMem;
    VOS_UINT16              usPppId;
    VOS_UINT32              ulFrmResult;
    VOS_UINT32              ulDefResult;


    usPppId = (VOS_UINT16)PPP_LINK_TO_ID(pstLink);

    pstMem  = ppp_m_alloc_ttfmem_from_mbuf(pstMbuf);

    ppp_m_freem(pstMbuf);
    pstMbuf  = VOS_NULL_PTR;

    /* for lint 438 */
    (VOS_VOID)pstMbuf;

    if (VOS_NULL_PTR == pstMem)
    {
        PPP_HDLC_WARNING_LOG("PPP_HDLC_HARD_ProcProtocolPacket, WARNING, pstMem is NULL!\r\n");
        return;
    }

    /* PPP协议栈回复的协商包需要封装后发给PC，确认此时是否封装与解封装都已经完成 */
    ulFrmResult     = PPP_HDLC_HARD_FrmIsEnabled();
    ulDefResult     = PPP_HDLC_HARD_DefIsEnabled();

    /* 此函数在上行解出协商包时，PPP协议栈产生应答，这个时候封装不应该使能 */
    if( (VOS_TRUE == ulFrmResult) || (VOS_TRUE == ulDefResult) )
    {
        PPP_HDLC_WARNING_LOG2("PPP_HDLC_HARD_ProcProtocolPacket, WARNING, ulFrmResult %d, ulDefResult %d!\r\n",
                      ulFrmResult, ulDefResult);

        PPP_HDLC_HARD_SendAsFrmPacketMsg(usPppId, usProtocol, pstMem);

        return;
    }

    /* 下次PPP任务调度的时候再封装此协商，那时封装与解封装都未使能 */
    PPP_HDLC_HARD_ProcAsFrmPacket(usPppId, usProtocol, pstMem);

    return;
}

/*****************************************************************************
 函 数 名  : PPP_Help
 功能描述  : 帮助打印
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 无
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年9月25日
    作    者   : x59651
    修改内容   : 新生成函数

*****************************************************************************/
VOS_VOID PPP_HDLC_HARD_Help(VOS_VOID)
{
    vos_printf("********************PPP软调信息************************\n");
    vos_printf("PPP_HDLC_HARD_MntnShowStatInfo      打印统计信息\n");
    vos_printf("PPP_INPUT_ShowStatInfo              打印 g_PppDataQCtrl信息\n");
    vos_printf("PPP_HDLC_HARD_MntnSetConfig         设置可维可测等级:\n");
    vos_printf("                                    1--参数；2--寄存器；4--数据\n");
    vos_printf("PPP_HDLC_HARD_MntnShowDefReg        打印解封装寄存器信息\n");
    vos_printf("PPP_HDLC_HARD_MntnShowFrmReg        打印封装寄存器信息\n");
    vos_printf("PPP_HDLC_HARD_MntnSetDefIntLimit    设置解封装中断水线\n");
    vos_printf("PPP_HDLC_HARD_MntnSetFrmIntLimit    设置封装中断水线\n");
    vos_printf("PPP_HDLC_HARD_MntnShowConfigInfo    打印配置信息\n");

    vos_printf("PPP_Info                            虚地址  :0x%x\n", g_ulPppVirtAddr);
    vos_printf("PPP_Info                            物理地址:0x%x\n", g_ulPppPhyAddr);
    vos_printf("PPP_Info                            内存长度:0x%x\n", g_ulPppTotalBufLen);

    return;
}

#endif  /* #if(FEATURE_ON == FEATURE_HARDWARE_HDLC_FUNC) */

#endif /* #if(FEATURE_ON == FEATURE_PPP) */

#ifdef __cplusplus
    #if __cplusplus
        }
    #endif
#endif
