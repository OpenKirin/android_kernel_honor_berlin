

#include <linux/kernel.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/mm.h>
#include "product_config.h"
#include "hi_efuse.h"
#include "bsp_efuse.h"

#include "bsp_trace.h"
#include "bsp_shared_ddr.h"

#include "mdrv_efuse.h"

#ifdef HI_K3_EFUSE
#include <mdrv.h>
#include "../../adrv/adrv.h"
#endif

#include <soc_onchiprom.h>

#ifdef CONFIG_EFUSE

int mdrv_efuse_write_huk(char *pBuf,unsigned int len)
{
    return EFUSE_OK;
}

int mdrv_efuse_check_huk_valid(void)
{
    return EFUSE_OK;
}

/*************************************************
 函 数 名   : DRV_GET_DIEID
 功能描述   : DRV_GET_DIEID
 输入参数   : buf: data buffer
              len: length of the group
 输出参数   :
 返 回 值   : OK                (0)
              BUF_ERROR         (-55)
              LEN_ERROR         (-56)
              READ_EFUSE_ERROR  (-57)
 调用函数   :
 被调函数   :

 修改历史   :
   日    期 : 2013年8月12日
   作    者 : l00225826
   修改内容 : 新生成函数

*************************************************/
int mdrv_efuse_get_dieid(unsigned char* buf,int length)
{
#ifdef HI_K3_EFUSE
    return get_efuse_dieid_value(buf, (unsigned int)length, 0);
#else
    unsigned int i = 0;
    int ret = 0;

#if (EFUSE_DIEID_BIT < 32)
    u32 *buf_die_id = (u32*)buf;
#endif

    if (NULL == buf) {
        efuse_print_error("die id buf is error.\n");
        return EFUSE_ERROR_ARGS;
    }

    if (length < EFUSE_DIEID_LEN) {
        efuse_print_error("die id lenth is error.\n");
        return EFUSE_ERROR_ARGS;
    }

    memset(buf, 0, EFUSE_DIEID_LEN);

    pr_info("efuse read start group %d length %d.\n", EFUSE_GRP_DIEID, EFUSE_DIEID_SIZE);

    ret = bsp_efuse_read((u32*)buf, EFUSE_GRP_DIEID, EFUSE_DIEID_SIZE);
    if (EFUSE_OK != ret) {
        efuse_print_error("die id read efuse error.\n");
        return ret;
    }

    for(i = 0;i < EFUSE_DIEID_SIZE;i++) {
        pr_info("efuse end buf[%d] is 0x%x.\n", i, buf[i]);
    }
    pr_info("efuse read end ret %d\n", ret);

#if (EFUSE_DIEID_BIT < 32)
    buf_die_id[EFUSE_DIEID_SIZE-1] &= (((u32)0x1 << EFUSE_DIEID_BIT) - 1);
#endif

    return EFUSE_OK;
#endif
}


/*************************************************
 函 数 名   : DRV_GET_CHIPID
 功能描述   : DRV_GET_CHIPID
 输入参数   : buf: data buffer
              len: length of the group
 输出参数   :
 返 回 值   : OK                (0)
              BUF_ERROR         (-55)
              LEN_ERROR         (-56)
              READ_EFUSE_ERROR  (-57)
 调用函数   :
 被调函数   :

 修改历史   :
   日    期 : 2013年8月12日
   作    者 : l00225826
   修改内容 : 新生成函数

*************************************************/
#ifdef HI_K3_EFUSE
int mdrv_efuse_get_chipid(unsigned char* buf,int length)
{
    return get_efuse_chipid_value(buf, (unsigned int)length, 0);
}
int mdrv_efuse_ioctl(int cmd, int arg, unsigned char* buf, int len)
{
    efuse_print_error("It's only a stub");

    return EFUSE_OK;
}
#else
int mdrv_efuse_get_chipid(unsigned char* buf,int length)
{
    return EFUSE_OK;
}

#ifdef DX_SEC_BOOT

int bsp_efuse_ioctl_get_socid(unsigned char* buf, int len)
{
    if ((!buf) || (len < (SHM_SIZE_SEC_SOC_ID/4))) {
        efuse_print_error("bsp_efuse_ioctl_get_socid fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    memcpy((void*)buf, (void*)(SHM_BASE_ADDR+SHM_OFFSET_SEC_SOC_ID), SHM_SIZE_SEC_SOC_ID);

    return EFUSE_OK;
}

int bsp_efuse_ioctl_set_kce(unsigned char* buf, int len)
{
    int ret = 0;

    if ((!buf) || (len != EFUSE_LAYOUT_KCE_LENGTH)) {
        efuse_print_error("bsp_efuse_ioctl_set_kce fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_write((u32*)buf, EFUSE_LAYOUT_KCE_OFFSET, EFUSE_LAYOUT_KCE_LENGTH);

    return ret;
}


#define DX_SECURESTATE_SECURE   (1)
#define DX_SECURESTATE_RMA      (2)

int bsp_efuse_ioctl_get_securestate(void)
{
    int ret = 0;
    u32 value[2] = {0, 0};

    ret = bsp_efuse_read(&value[0], EFUSE_LAYOUT_MP_FLAG_OFFSET, 2);
    if (ret)
        return ret;

    if (value[0] & (0x1U << EFUSE_LAYOUT_MP_FLAG_RMA_BIT_OFFSET))
        return DX_SECURESTATE_RMA;

    if (((value[0] >> EFUSE_LAYOUT_MP_FLAG_LCS_BIT_OFFSET) & 0x0F) == 0x03) {
        if (value[1])
            return DX_SECURESTATE_SECURE;
        else
            return 0;
    }

    return 0;
}

int bsp_efuse_ioctl_set_securestate(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_securestate();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    if (arg == DX_SECURESTATE_SECURE) {
#ifdef CONFIG_TRUSTZONE
        value |= (0x3U << EFUSE_LAYOUT_MP_FLAG_LCS_BIT_OFFSET);
#else
        efuse_print_error("bsp_efuse_ioctl_set_securestate fail. trustzone is not supported.\n");
        return EFUSE_ERROR_ARGS;
#endif
    }

    if (arg == DX_SECURESTATE_RMA)
        value |= (0x1U << EFUSE_LAYOUT_MP_FLAG_RMA_BIT_OFFSET);

    ret = bsp_efuse_write(&value, EFUSE_LAYOUT_MP_FLAG_OFFSET, 1);

    return ret;
}

int bsp_efuse_ioctl_get_securedebug(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_BIT_OFFSET)) & 0x3U);
    return ret;
}

int bsp_efuse_ioctl_set_securedebug(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_securedebug();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x3) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_dbgen(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_DBGEN_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_DBGEN_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_dbgen(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_dbgen();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_DBGEN_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_DBGEN_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_niden(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_NIDEN_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_NIDEN_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_niden(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_niden();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_NIDEN_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_NIDEN_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_spiden(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_SPIDEN_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_SPIDEN_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_spiden(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_spiden();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_SPIDEN_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_SPIDEN_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_spniden(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_SPNIDEN_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_SPNIDEN_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_spniden(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_spniden();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_ARM_DBG_CTRL_SPNIDEN_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_ARM_DBG_CTRL_SPNIDEN_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_hifidbgen(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_HIFI_DBG_CTRL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_HIFI_DBG_CTRL_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_hifidbgen(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_hifidbgen();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_HIFI_DBG_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_HIFI_DBG_CTRL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_bbe16dbgen(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_bbe16dbgen(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_bbe16dbgen();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_BBE16_DBG_CTRL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_csdeviceen(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_CS_DEVICE_CTRL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_CS_DEVICE_CTRL_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_csdeviceen(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_csdeviceen();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_CS_DEVICE_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_CS_DEVICE_CTRL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_jtagen(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_JTAGEN_CTRL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_JTAGEN_CTRL_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_jtagen(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_jtagen();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_JTAGEN_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_JTAGEN_CTRL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_secdbgreset(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_SEC_DBG_RST_CTRL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_SEC_DBG_RST_CTRL_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_secdbgreset(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_secdbgreset();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_SEC_DBG_RST_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_SEC_DBG_RST_CTRL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_csreset(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_CORESIGHT_RST_CTRL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_CORESIGHT_RST_CTRL_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_csreset(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_csreset();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_CORESIGHT_RST_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_CORESIGHT_RST_CTRL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_dftsel(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_DFT_DISABLE_SEL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_DFT_DISABLE_SEL_BIT_OFFSET)) & 0x3U);
    return ret;
}

int bsp_efuse_ioctl_set_dftsel(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_dftsel();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x3) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_DFT_DISABLE_SEL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_DFT_DISABLE_SEL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_authkey(unsigned char* buf, int len)
{
    int ret = 0;
    if ((!buf) || (len < EFUSE_LAYOUT_DFT_AUTH_KEY_LENGTH)) {
        efuse_print_error("bsp_efuse_ioctl_get_authkey fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_read((u32*)buf, EFUSE_LAYOUT_DFT_AUTH_KEY_OFFSET, EFUSE_LAYOUT_DFT_AUTH_KEY_LENGTH);
    if (ret)
        return ret;

    return ret;
}

int bsp_efuse_ioctl_set_authkey(unsigned char* buf, int len)
{
    u32 i = 0;
    int ret = 0;
    u32 value[EFUSE_LAYOUT_DFT_AUTH_KEY_LENGTH];

    if ((!buf) || (len < EFUSE_LAYOUT_DFT_AUTH_KEY_LENGTH)) {
        efuse_print_error("bsp_efuse_ioctl_set_authkey fail. error args.\n");
        return EFUSE_ERROR_ARGS;
    }

    ret = bsp_efuse_ioctl_get_authkey((unsigned char*)&value[0], len);
    if (ret < 0)
        return ret;

    for (i = 0; i < sizeof(value)/sizeof(value[0]); i++) {
        if (value[i])
            return 1;
    }

    ret = bsp_efuse_write((u32*)buf, EFUSE_LAYOUT_DFT_AUTH_KEY_OFFSET, EFUSE_LAYOUT_DFT_AUTH_KEY_LENGTH);

    return ret;
}

int bsp_efuse_ioctl_get_authkeyrd(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_DFT_AUTH_KEY_RD_CTRL_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_DFT_AUTH_KEY_RD_CTRL_BIT_OFFSET)) & 0x1U);
    return ret;
}

int bsp_efuse_ioctl_set_authkeyrd(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_authkeyrd();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x1U) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_DFT_AUTH_KEY_RD_CTRL_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_DFT_AUTH_KEY_RD_CTRL_BIT_OFFSET), 1);

    return ret;
}

int bsp_efuse_ioctl_get_nsiverify(void)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_read(&value, GROUP_INDEX(EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET), 1);
    if (ret)
        return ret;

    ret = (int)((value >> BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET)) & 0x3U);
    return ret;
}

int bsp_efuse_ioctl_set_nsiverify(int arg)
{
    int ret = 0;
    u32 value = 0;

    ret = bsp_efuse_ioctl_get_nsiverify();
    if (ret < 0)
        return ret;

    if (ret > 0)
        return 1;

    value = (arg & 0x3) << BIT_INDEX_IN_GROUP(EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET);

    ret = bsp_efuse_write(&value, GROUP_INDEX(EFUSE_LAYOUT_NS_VERIFY_BIT_OFFSET), 1);

    return ret;
}

int mdrv_efuse_ioctl(int cmd, int arg, unsigned char* buf, int len)
{
    int ret = 0;

    switch (cmd) {
        case MDRV_EFUSE_IOCTL_CMD_GET_SOCID:
            ret = bsp_efuse_ioctl_get_socid(buf, len);
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_KCE:
            ret = bsp_efuse_ioctl_set_kce(buf, len);
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_SECURESTATE:
            ret = bsp_efuse_ioctl_set_securestate(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE:
            ret = bsp_efuse_ioctl_get_securestate();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_SECUREDEBUG:
            ret = bsp_efuse_ioctl_set_securedebug(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_SECUREDEBUG:
            ret = bsp_efuse_ioctl_get_securedebug();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_DBGEN:
            ret = bsp_efuse_ioctl_set_dbgen(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_DBGEN:
            ret = bsp_efuse_ioctl_get_dbgen();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_NIDEN:
            ret = bsp_efuse_ioctl_set_niden(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_NIDEN:
            ret = bsp_efuse_ioctl_get_niden();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_SPIDEN:
            ret = bsp_efuse_ioctl_set_spiden(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_SPIDEN:
            ret = bsp_efuse_ioctl_get_spiden();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_SPNIDEN:
            ret = bsp_efuse_ioctl_set_spniden(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_SPNIDEN:
            ret = bsp_efuse_ioctl_get_spniden();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_HIFIDBGEN:
            ret = bsp_efuse_ioctl_set_hifidbgen(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_HIFIDBGEN:
            ret = bsp_efuse_ioctl_get_hifidbgen();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_BBE16DBGEN:
            ret = bsp_efuse_ioctl_set_bbe16dbgen(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_BBE16DBGEN:
            ret = bsp_efuse_ioctl_get_bbe16dbgen();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_CSDEVICEEN:
            ret = bsp_efuse_ioctl_set_csdeviceen(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_CSDEVICEEN:
            ret = bsp_efuse_ioctl_get_csdeviceen();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_JTAGEN:
            ret = bsp_efuse_ioctl_set_jtagen(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_JTAGEN:
            ret = bsp_efuse_ioctl_get_jtagen();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_SECDBGRESET:
            ret = bsp_efuse_ioctl_set_secdbgreset(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_SECDBGRESET:
            ret = bsp_efuse_ioctl_get_secdbgreset();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_CSRESET:
            ret = bsp_efuse_ioctl_set_csreset(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_CSRESET:
            ret = bsp_efuse_ioctl_get_csreset();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_DFTSEL:
            ret = bsp_efuse_ioctl_set_dftsel(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_DFTSEL:
            ret = bsp_efuse_ioctl_get_dftsel();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_AUTHKEY:
            ret = bsp_efuse_ioctl_set_authkey(buf, len);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_AUTHKEY:
            ret = bsp_efuse_ioctl_get_authkey(buf, len);
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_AUTHKEYRD:
            ret = bsp_efuse_ioctl_set_authkeyrd(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_AUTHKEYRD:
            ret = bsp_efuse_ioctl_get_authkeyrd();
            break;
        case MDRV_EFUSE_IOCTL_CMD_SET_NSIVERIFY:
            ret = bsp_efuse_ioctl_set_nsiverify(arg);
            break;
        case MDRV_EFUSE_IOCTL_CMD_GET_NSIVERIFY:
            ret = bsp_efuse_ioctl_get_nsiverify();
            break;
        default:
            efuse_print_error("unsupported command");
            ret = EFUSE_ERROR_ARGS;
    }

    return ret;
}

#else

int mdrv_efuse_ioctl(int cmd, int arg, unsigned char* buf, int len)
{
    efuse_print_error("It's only a stub");

    return EFUSE_OK;
}

#endif


#endif

void DRV_DIEID_TEST(void)
{
    int i = 0;
    char *buf = kzalloc(EFUSE_DIEID_LEN, GFP_KERNEL);
    if(NULL == buf) {
        efuse_print_info("alloc mem is fail.\n");
        return;
    }

    if(EFUSE_OK == mdrv_efuse_get_dieid(buf, EFUSE_DIEID_LEN)) {
        for(i = 0;i < EFUSE_DIEID_LEN;i++) {
            efuse_print_info("0x%x \n", *(buf+i));
        }
    } else {
        efuse_print_error("DRV_GET_DIEID is fail.\n");
    }

    kfree(buf);
}

#else   /* CONFIG_EFUSE */

int mdrv_efuse_write_huk(char *pBuf,unsigned int len)
{
    return EFUSE_OK;
}

int mdrv_efuse_check_huk_valid(void)
{
    return EFUSE_OK;
}

int mdrv_efuse_get_dieid(unsigned char* buf,int length)
{
    return EFUSE_OK;
}

int mdrv_efuse_get_chipid(unsigned char* buf,int length)
{
    return EFUSE_OK;
}

int mdrv_efuse_ioctl(int cmd, int arg, unsigned char* buf, int len)
{
    return EFUSE_OK;
}

#endif

