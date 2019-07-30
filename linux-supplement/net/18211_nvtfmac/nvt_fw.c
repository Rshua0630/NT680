#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/firmware.h>
#include "nvt_fw.h"
#include "nvt_wlan_linux.h"
#include "nvt_diag.h"

//20151015 nash: workaround for calibration which take long time
//#define NVT_WAIT_FW_RDY_TIMEOUT 10000
#define NVT_WAIT_FW_RDY_TIMEOUT 20000
static u64 elf_offset;

struct _nvt_config_patch_file_header nvt_config_patch_file_header;
struct _nvt_config_img_sector_header nvt_config_img_sector_header;

#if (NVT_FW_CFG_DOWNLOAD_BY_CHIPID == 1)
static s8 *nvt_fw_get_name(struct _nvt_bus *, enum nvt_fw_mode);

static struct _nvt_fw_name fw_name_table[] = {
        {0x18202, NVT_FW_STA, "nvt/dsw_18202_sta.img"},
        {0x18202, NVT_FW_AP,  "nvt/dsw_18202_ap.img"},
        {0x18202, NVT_FW_HUT, "nvt/dsw_18202_hut.img"},
        {0x18211, NVT_FW_STA, "nvt/dsw_18211_sta.img"},
        {0x18211, NVT_FW_AP,  "nvt/dsw_18211_ap.img"},
        {0x18211, NVT_FW_HUT, "nvt/dsw_18211_hut.img"}
};
#endif
//X^8 + X^2 + X^1 + X^0
static const u8 Crc8Table[] = {
        0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
        0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
        0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
        0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
        0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
        0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
        0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
        0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
        0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
        0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
        0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
        0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
        0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
        0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
        0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
        0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
        0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
        0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
        0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
        0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
        0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
        0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
        0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C,
        0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
        0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B,
        0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
        0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B,
        0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
        0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB,
        0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
        0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB,
        0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};

//20160726 nash:
const struct firmware *resume_cfg_fw = NULL;

s32 nvt_fw_get_ver(struct _nvt_bus *nvt_bus, u8 *ver, u32 *ver_len)
{
        s32 ret;
        //20151218 nash: coverity#48988
        s32 pkt_len = 0;
        u32 fw_ver;
        struct _nvt_diag_req *diag_req = NULL;
        struct _nvt_diag_resp *diag_resp = NULL;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(FW, "%s: diag_req kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        diag_resp = kzalloc(sizeof(struct _nvt_diag_resp), GFP_KERNEL);
        if (diag_resp == NULL) {
                nvt_dbg(DIAG, "%s: diag_resp kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        /* Get firmware type first */
        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_GET_FW_VER_CMD, 0, 0,
                NULL, diag_req, &pkt_len);

        ret = nvt_bus->nvt_wdev_bus_ops.tx_ctrl(nvt_bus,
                (u8 *)diag_req, pkt_len);
        if (ret < 0) {
                nvt_dbg(FW, "%s: failed to send diag req!!\n", __func__);
                goto alloc_failed;
        }

        ret = nvt_bus->nvt_wdev_bus_ops.rx_ctrl(nvt_bus, (u8 *)diag_resp,
                sizeof(struct _nvt_diag_resp));
        if (ret < 0) {
                nvt_dbg(FW, "%s: failed to recv diag resp!!\n", __func__);
                goto alloc_failed;
        }

        fw_ver = *(u32 *)(diag_resp->sel.get_fw_ver.fw_ver);

        //20151218 nash: coverity#48968??
        //sprintf(ver, "%d", fw_ver);
        snprintf(ver, 10, "%d", fw_ver);
        *ver_len = strlen(ver);

        ret = 0;

alloc_failed:
        kfree(diag_req);
        kfree(diag_resp);

        return ret;

}

static s32 nvt_fw_get_type(struct _nvt_bus *nvt_bus)
{
        s32 ret;
        //20151218 nash: coverity#48987
        s32 pkt_len = 0;
        s32 *fw_type;
        struct _nvt_diag_req *diag_req = NULL;
        struct _nvt_diag_resp *diag_resp = NULL;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(FW, "%s: diag_req kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        diag_resp = kzalloc(sizeof(struct _nvt_diag_resp), GFP_KERNEL);
        if (diag_resp == NULL) {
                nvt_dbg(DIAG, "%s: diag_resp kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        /* Get firmware type first */
        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_GET_FW_VER_CMD, 0, 0,
                NULL, diag_req, &pkt_len);

        ret = nvt_bus->nvt_wdev_bus_ops.tx_ctrl(nvt_bus,
                (u8 *)diag_req, pkt_len);
        if (ret < 0) {
                nvt_dbg(FW, "%s: failed to send diag req!!\n", __func__);
                goto alloc_failed;
        }

        ret = nvt_bus->nvt_wdev_bus_ops.rx_ctrl(nvt_bus, (u8 *)diag_resp,
                sizeof(struct _nvt_diag_resp));
        if (ret < 0) {
                nvt_dbg(FW, "%s: failed to recv diag resp!!\n", __func__);
                goto alloc_failed;
        }

        fw_type = (u32 *)(diag_resp->sel.get_fw_ver.fw_type);
        ret = *fw_type;
        nvt_dbg(FW, "%s: FW type=%d\n", __func__, ret);

alloc_failed:
        kfree(diag_req);
        kfree(diag_resp);

        return ret;
}

static s32 nvt_fw_is_elf(Elf32_Ehdr eh)
{
        /* ELF magic bytes are 0x7f,'E','L','F'
             * Using  octal escape sequence to represent 0x7f
             */
        if ((eh.e_ident[EI_MAG0] != ELFMAG0) ||
             (eh.e_ident[EI_MAG1] != ELFMAG1) ||
             (eh.e_ident[EI_MAG2] != ELFMAG2) ||
             (eh.e_ident[EI_MAG3] != ELFMAG3) ||
             (eh.e_ehsize != sizeof(Elf32_Ehdr))) {
                nvt_dbg(FW, "%s: is a ELF\n", __func__);
                return 1;
        } else {
                nvt_dbg(FW, "%s: ELFMAGIC mismatch!\n", __func__);
                return 0;
        }
}

static s32 nvt_fw_fm_goto_address(struct _nvt_bus *bus, void *entry_addr)
{
        s32 ret = 0;
        s32 pkt_len;
        u32 addr = NVT_FW_FM_GO_ADDR;
        struct _nvt_diag_req *diag_req;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(DIAG, "%s: kzalloc is failed\n", __func__);
                goto alloc_failed;
        }

        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_WRITE_CMD, addr, 4,
                (u8 *)(entry_addr), diag_req, &pkt_len);

        ret = bus->nvt_wdev_bus_ops.tx_ctrl(bus, (u8 *)diag_req, pkt_len);
        if (ret < 0) {
                nvt_dbg(FW, "%s: send diag msg fail\n", __func__);
        }

        kfree(diag_req);
        return ret;
alloc_failed:
        return -1;
}

static s32 nvt_fw_crc_check(struct _nvt_bus *bus, u32 addr, s32 cnt, u8 crc,
        u32 retry_count)
{
        struct _nvt_diag_req  *diag_req = NULL;
        struct _nvt_diag_resp *diag_resp = NULL;
        ssize_t retval = 0;
        //20151218 nash: coverity#48985
        s32 pkt_len = 0;
        u32 crc_value_by_fw = 0;
        u32 retry_time = 0;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(FW, "%s: kzalloc is failed\n", __func__);
                retval = -1;
                goto alloc_failed;
        }

        diag_resp = kzalloc(sizeof(struct _nvt_diag_resp), GFP_KERNEL);
        if (diag_resp == NULL) {
                nvt_dbg(FW, "%s: kzalloc is failed\n", __func__);
                retval = -1;
                goto alloc_failed;
        }

        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_CHECK_SUM_CMD, addr,
                        cnt, NULL, diag_req, &pkt_len);

        do {
                bus->nvt_wdev_bus_ops.tx_ctrl(bus, (u8 *)diag_req, pkt_len);


                //4. get resp by diag_node
                retval = bus->nvt_wdev_bus_ops.rx_ctrl(bus, (u8 *)diag_resp,
                                 sizeof(struct _nvt_diag_resp));
                if (retval < 0) {
                        nvt_dbg(FW, "%s: failed to recv diag resp!!\n",
                        __func__);
                        goto alloc_failed;
                }

                //5. get crc check sum value from driver
                //crc_value_by_driver = CRC_CalcBlockDataCRC8(buf,cnt);

                //6. get crc check sum value from FW
                crc_value_by_fw = diag_resp->sel.cksum_veri.cksum_val;

                nvt_dbg(FW, "%s: crc_value_from_driver=%08x\n",
                        __func__, crc);
                nvt_dbg(FW, "%s: crc_value_from_fw=%08x\n",
                        __func__, crc_value_by_fw);

                //20151112 nash: code bug
                //if (crc_value_by_fw == crc_value_by_driver) {
                if (crc_value_by_fw == crc) {
                        goto get_correct_crc;
                }

        } while (retry_time++ < retry_count);

        /* time out */
        retval = -1;

get_correct_crc:
alloc_failed:


        kfree(diag_req);

        kfree(diag_resp);

        return retval;
}

static s32 nvt_fw_fm_write(struct _nvt_bus *bus,
        u32 addr, s32 cnt, s8 *buf)
{
        s32 pkt_len;
        s32 ret = 0;
        struct _nvt_diag_req *diag_req;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(DIAG, "%s[0]kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_WRITE_CMD, addr, cnt,
                buf, diag_req, &pkt_len);

        ret = bus->nvt_wdev_bus_ops.tx_ctrl(bus, (u8 *)diag_req, pkt_len);

alloc_failed:
        kfree(diag_req);

        return ret;

}


/**
 * nvt_fw_util_calc_crc8 - calculate the CRC value of block data
 *
 * @data_start: the pointer point to data block
 * @init_seed: CRC init value
 * @length: copy length *
 *
 * Return: the CRC value of the input data block
 */
static u8 nvt_fw_util_calc_crc8(u8 *data_start, u8 init_seed,
        u32 length)
{
        u32 i;
        u8 loc_crc = init_seed;

        for (i = 0; i < length; i++) {
                loc_crc ^= *(data_start + i);
                loc_crc = Crc8Table[loc_crc];
        }

        return loc_crc;
}

static s32 nvt_fw_util_endian_trans(u8 *buff, u32 transfer_len)
{

        u8 *buff_p;
        u32 count;

        buff_p = buff;
        count = transfer_len;

        while (count > 0) {
                if (count >= 4) {
                        //EndianCnv_32((u32 *)buff_p, 4);
                        be32_to_cpus((u32 *)buff_p);
                        count -= 4;
                        buff_p += 4;
                } else if (count >= 2) {
                        //EndianCnv_16((u16 *)buff_p, 2);
                        be16_to_cpus((u16 *)buff_p);
                        count -= 2;
                        buff_p += 2;
                } else {
                        count -= 1;
                        buff_p += 1;
                }
        }

        return 0;
}


static u32 nvt_fw_elf_offset_set(u64 offset)
{
        elf_offset = offset;
        return 0;
}

static u64 nvt_fw_elf_offset_get(void)
{
        return elf_offset;
}

static s32 nvt_fw_elf_seek(const u8 *store_buf, u64 offset, u8 cmd)
{
        switch (cmd) {
        case ELF_SEEK_SET:
                nvt_fw_elf_offset_set(offset);
                break;
        default:
                nvt_dbg(FW, "%s: incorrect command\n", __func__);
                return -1;
        }

        return 0;
}

static u16 nvt_fw_elf_read(u8 *read_buf, u8 size, u16 count,
        const u8 *store_buf)
{
        u16 len = size * count;
        u64 offset = nvt_fw_elf_offset_get();

        nvt_dbg(FW, "%s: now offset=%lld\n", __func__,
                nvt_fw_elf_offset_get());

        memcpy(read_buf, store_buf + offset, len);
        nvt_fw_elf_offset_set(offset + len);

        nvt_dbg(FW, "%s: modified offset=%lld\n", __func__,
                nvt_fw_elf_offset_get());
        return len;
}

static void nvt_fw_elf_read_header(const u8 *store_buf,
        Elf32_Ehdr *elf_header)
{
        if (nvt_fw_elf_seek(store_buf, 0, ELF_SEEK_SET) != 0) {
                memset((void *)elf_header, 0, sizeof(Elf32_Ehdr));
                return;
        } else {
                if (nvt_fw_elf_read((u8 *)elf_header,
                   sizeof(Elf32_Ehdr), 1, store_buf) == 0) {
                        memset((void *)elf_header, 0, sizeof(Elf32_Ehdr));
                        return;
                }

                nvt_dbg(FW, "%s: got elf header", __func__);

        }

        return;
}

static s32 nvt_fw_elf_data_program(struct _nvt_bus *bus,
        Elf32_Shdr sh, u8 *fw_buf)
{
        u8 *buff;
        u16 buff_size = 1024;
        u8 crc = 0;

        s32 len;
        u32 sh_addr = 0;
        u32 crc_sh_addr = 0;
        u32 crc_range_fw = 0;
        u32 transfer_len;

        //20161021 nash: retry for crc error
        s32 ret;
        s32 retry = 3;

        buff = kmalloc(buff_size, GFP_ATOMIC);
        //20151218 nash:  coverity#48956
        if (buff == NULL) {
                nvt_dbg(ERROR, "%s: kmalloc fail!!\n", __func__);
                return -1;
        }
        //20161021 nash: retry for crc error
        do {
                crc_sh_addr = (u32)sh.sh_addr;
                sh_addr = (u32)sh.sh_addr;
                crc_range_fw = sh.sh_size;
                len = sh.sh_size;
                crc = 0;

        nvt_fw_elf_seek(fw_buf, sh.sh_offset, ELF_SEEK_SET);
        while (len) {
                transfer_len = (len >= buff_size) ? buff_size : len;
                        transfer_len = nvt_fw_elf_read(buff, 1,
                                transfer_len, fw_buf);

                crc = nvt_fw_util_calc_crc8(buff, crc, transfer_len);

                nvt_dbg(FW, "%s: write section to memory\n", __func__);

                len -= transfer_len;

                        nvt_fw_util_endian_trans(buff, (transfer_len+3) &
                                0xFFFFFFFC);

                if (nvt_fw_fm_write(bus, sh_addr,
                    (transfer_len+3) & 0xFFFFFFFC, buff)) {
                                ret = -1;
                        goto failed;
                }
                sh_addr += transfer_len;

        }

        /* check if crc value is ok */
                ret = nvt_fw_crc_check(bus, crc_sh_addr, crc_range_fw, crc , 3);
                if (ret < 0) {
                        nvt_dbg(ERROR, "%s: crc check is incorrect\n",
                                __func__);
        }

        } while (ret != 0 && --retry > 0);

failed:
        kfree(buff);
        return ret;
}

static s32 nvt_fw_config_program_in_img(struct _nvt_bus *bus,
const u8 *img_buffer)
{
        s32 img_total_length = 0;
        s32 sector_total_length = 0;
        u32 write_addr = 0;
        u8 *read_buf = NULL;
        u32 actual_read_count = 0 , actual_write_count = 0;
        s32 retcode = 0;
        u8  crc = 0;
        u32 i = 0;

        nvt_dbg(FW, "%s: total length=%d\n", __func__,
        nvt_config_patch_file_header.total_length);
        img_total_length = nvt_config_patch_file_header.total_length -
        NVT_CONFIG_PATCH_FILE_HEADER_LEN;
        nvt_dbg(FW, "%s: total length=%d\n", __func__,
        nvt_config_patch_file_header.total_length);
        nvt_dbg(FW, "%s: img_total_length =%d\n", __func__, img_total_length);
        if (img_total_length <= 0) {
                nvt_dbg(ERROR, "%s: incorrect img_total_length!!\n", __func__);
                return -1;
        }

        nvt_fw_elf_offset_set(0);
        read_buf = kmalloc(NVT_CONFIG_IMG_SECTOR_READ_MAX_LEN, GFP_ATOMIC);
        if (read_buf == NULL) {
                //fix CID #64558 by jerry lin.
                nvt_dbg(ERROR, "%s: failed to allocate memory\n", __func__);
                return -1;
        }
        do {
                // processing of image sector

                // read the sector header of config img
                nvt_fw_elf_read((u8 *)(&nvt_config_img_sector_header),
                sizeof(struct _nvt_config_img_sector_header), 1, img_buffer);
                sector_total_length = nvt_config_img_sector_header.total_length
- sizeof(struct _nvt_config_img_sector_header);
                // get starting write address
                write_addr = nvt_config_img_sector_header.write_addr;
                nvt_dbg(FW, "%s: nvt_config_img_sector_header.total_length=%d\n"
, __func__, nvt_config_img_sector_header.total_length);
                nvt_dbg(FW, "%s: sector_total_length=%d\n", __func__,
                sector_total_length);
                nvt_dbg(FW, "%s: write_addr=%x\n", __func__, write_addr);

                if (sector_total_length <= 0) {
                        nvt_dbg(ERROR, "%s: incorrect sector_total_length!!\n",
                        __func__);
                        retcode = -1;
                        goto error_free;
                }

                crc = 0;
                do {
                        actual_read_count = actual_write_count = (
                        sector_total_length >=
                        NVT_CONFIG_IMG_SECTOR_READ_MAX_LEN)
                        ? NVT_CONFIG_IMG_SECTOR_READ_MAX_LEN
                        : sector_total_length;
                        nvt_fw_elf_read(read_buf, 1, actual_read_count,
                        img_buffer);
                        nvt_dbg(FW, "%s:write_addr=%x\n", __func__, write_addr);
                        nvt_dbg(FW, "%s: actual_write_count=%d\n", __func__,
                        actual_write_count);
                        nvt_dbg(FW, "%s: read_buf:", __func__);
                        for (i = 0; i < ((actual_write_count+3) & 0xFFFFFFFC);
                        i++)
                                nvt_dbg(FW, "%x ", *(read_buf+i));

                        crc = nvt_fw_util_calc_crc8(read_buf, crc,
                        (actual_write_count+3) & 0xFFFFFFFC);
                        nvt_fw_util_endian_trans(read_buf,
                        (actual_write_count+3) & 0xFFFFFFFC);
                        nvt_fw_fm_write(bus, write_addr,
                        (actual_write_count+3) & 0xFFFFFFFC, read_buf);
                        sector_total_length -= actual_read_count;
                        write_addr += actual_read_count;
                        nvt_dbg(FW, "%s: sector_total_length=%d\n", __func__,
                        sector_total_length);
                        nvt_dbg(FW, "%s: write addr=%x\n", __func__, write_addr)
;
                } while (sector_total_length > 0);

                /* check if crc value is ok */
                nvt_dbg(FW, "%s: nvt_config_img_sector_header.write_addr=%x\n",
__func__, nvt_config_img_sector_header.write_addr);
                nvt_dbg(FW, "%s: sector total length=%d\n",  __func__ ,
nvt_config_img_sector_header.total_length -
sizeof(struct _nvt_config_img_sector_header));
                nvt_dbg(FW, "%s: crc_by_driver=%x\n", __func__, crc);

                if (nvt_fw_crc_check(bus,
                nvt_config_img_sector_header.write_addr,
                nvt_config_img_sector_header.total_length -
                sizeof(struct _nvt_config_img_sector_header),
                crc , 3) == 0) {
                        nvt_dbg(FW, "%s[]crc check is correct", __func__);
                } else {
                        nvt_dbg(FW, "%s[]crc check is incorrect", __func__);
                }

                img_total_length -= nvt_config_img_sector_header.total_length;
                nvt_dbg(FW, "%s: img_total_length =%d\n", __func__,
                img_total_length);

        } while (img_total_length > sizeof(struct _nvt_config_img_sector_header)
);
        if (img_total_length != 1) {
                nvt_dbg(ERROR, "%s: incorrect img_total_length=%d\n", __func__,
                img_total_length);
                retcode = -1;
        }
error_free:
        kfree(read_buf);

        return retcode;

}



s32 nvt_config_patch_file_crc_check(const struct firmware *img,
struct _nvt_config_patch_file_header *header)
{
        u8 crc_seed_init = 0, crc_calc_value = 0, crc_value_by_img = 0;

        nvt_dbg(FW, "%s: 3 total length=%d\n", __func__,
        nvt_config_patch_file_header.total_length);

        crc_calc_value = nvt_fw_util_calc_crc8((u8 *)(img->data+
        NVT_CONFIG_PATCH_FILE_HEADER_LEN), crc_seed_init,
        img->size - NVT_CONFIG_PATCH_FILE_HEADER_LEN-1);
        crc_value_by_img = *(img->data + header->total_length - 1);
        nvt_dbg(FW, "%s crc_calc_value=%d, crc value of img=%d\n", __func__,
        crc_calc_value, crc_value_by_img);

        if (crc_value_by_img != crc_calc_value) {
                nvt_dbg(ERROR, "%s config patch file crc error!!\r\n",
                __func__);
                return -1;
        }
        return 0;
}


s32 nvt_config_patch_read_file_header(const struct firmware *img,
struct _nvt_config_patch_file_header *header)
{
        u8 crc_seed_init = 0, crc_calc_value = 0,
        crc_len = NVT_CONFIG_PATCH_FILE_HEADER_LEN-1;
        memcpy(header, img->data, sizeof
        (struct _nvt_config_patch_file_header));
        crc_calc_value =  nvt_fw_util_calc_crc8((u8 *)header, crc_seed_init,
        crc_len);
        nvt_dbg(FW, "%s prj_code=%d, total_length=%d,crc=%d\n", __func__,
        header->project_code, header->total_length, header->crc);
        nvt_dbg(FW, "%s crc_calc_value=%d, crc value of img header=%d\n",
        __func__, crc_calc_value, header->crc);
        if (crc_calc_value != header->crc) {
                nvt_dbg(ERROR, "%s header CRC error!!\r\n", __func__);
                return -1;
        }
        return 0;
}

static s32 nvt_config_patch_download(struct _nvt_bus *nvt_bus, bool is_resume)
{
        s8 *config_path_file_name = NVT_FW_CONFIG_PATCH_FILE_NAME;
        s32 error_code = 0;

        const struct firmware *config_patch_file = NULL;

        /*sanity check*/
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s nvt_bus is NULL\r\n", __func__);
                return -1;
        }

        if (is_resume) {
                if (resume_cfg_fw == NULL) {
                        nvt_dbg(ERROR, "%s no config patch file\r\n", __func__);
                        return -1;
                }
                config_patch_file = resume_cfg_fw;
        } else {
                error_code = request_firmware(&config_patch_file,
                                config_path_file_name, nvt_bus->dev);

        if (error_code < 0) {
                    nvt_dbg(ERROR, "%s failed to request_firmware", __func__);
                return -1;
        }
        }

        nvt_dbg(FW, "%s request config patch file succesfully!!\n", __func__);
        if (nvt_config_patch_read_file_header(config_patch_file,
            &nvt_config_patch_file_header) == -1) {
                nvt_dbg(ERROR, "%s failed on crc of img file header\n",
                       __func__);
                error_code = -1;
                goto error_handling;

        }
        nvt_dbg(FW, "%s config patch read file header succesfully!!\n",
                __func__);
        nvt_dbg(FW, "%s: 1 total length=%d\n", __func__,
nvt_config_patch_file_header.total_length);


        if (nvt_config_patch_file_crc_check(config_patch_file,
&nvt_config_patch_file_header) == -1) {
                nvt_dbg(ERROR, "%s failed on crc of img file\n", __func__);
                error_code = -1;
                goto error_handling;

        }

        nvt_dbg(FW, "%s config patch read file succesfully!!\n", __func__);
        nvt_dbg(FW, "%s:2 total length=%d\n", __func__,
        nvt_config_patch_file_header.total_length);
        if (nvt_fw_config_program_in_img(nvt_bus,
config_patch_file->data+NVT_CONFIG_PATCH_FILE_HEADER_LEN) == -1) {
                nvt_dbg(ERROR, "%s failed to config program in img\n",
                        __func__);
                error_code = -1;
                goto error_handling;
        }

error_handling:
        // fix CID #64459 by jerry lin.
        release_firmware(config_patch_file);
        return error_code;
}


static s32 nvt_fw_program_in_elf(struct _nvt_bus *bus,
        const u8 *fw_buffer)
{
        Elf32_Ehdr eh;          /* elf-header is fixed size */
        Elf32_Shdr sh_str;

        u8 *sh_name_tbl;
        u16 e_shnum;
        u16 e_shentsize;
        u16 e_shstrndx;
        u32 e_shoff;
        u32 entry_address;
        u32 total_size = 0;
        s32 ret = 0;

        Elf32_Shdr sh;

        nvt_fw_elf_read_header(fw_buffer, &eh);
        if (!nvt_fw_is_elf(eh)) {
                nvt_dbg(FW, "%s: not ELF file\n", __func__);
                ret = -1;
                goto invalid_format;
        }

        //entry address
        entry_address = eh.e_entry;
        if (eh.e_ident[EI_DATA] == ELFDATA2MSB) {
                entry_address = be32_to_cpu(entry_address);
        }

        nvt_dbg(FW, "%s: entry_point = 0x%08x\n", __func__, entry_address);
        nvt_fw_elf_seek(fw_buffer, 0, ELF_SEEK_SET);

        if (eh.e_ident[EI_DATA] == ELFDATA2MSB) {
                e_shnum = be16_to_cpu(eh.e_shnum);
                e_shentsize = be16_to_cpu(eh.e_shentsize);
                e_shstrndx = be16_to_cpu(eh.e_shstrndx);
                e_shoff = be32_to_cpu(eh.e_shoff);
        } else {
                e_shnum = eh.e_shnum;
                e_shentsize = eh.e_shentsize;
                e_shstrndx = eh.e_shstrndx;
                e_shoff = eh.e_shoff;
        }
        nvt_dbg(FW, "%s: e_shoff = %d, e_shentsize*e_shstrndx = %d\n", __func__,
                e_shoff, e_shentsize*e_shstrndx);

        /*get section name string*/
        nvt_fw_elf_seek(fw_buffer, e_shoff + e_shentsize*e_shstrndx,
                ELF_SEEK_SET);

        nvt_fw_elf_read(((u8 *)(&sh_str)), e_shentsize, 1, fw_buffer);

        if (eh.e_ident[EI_DATA] == ELFDATA2MSB) {
                be32_to_cpus(&sh_str.sh_name);
                be32_to_cpus(&sh_str.sh_type);
                be32_to_cpus(&sh_str.sh_flags);
                be32_to_cpus(&sh_str.sh_addr);
                be32_to_cpus(&sh_str.sh_offset);
                be32_to_cpus(&sh_str.sh_size);
                be32_to_cpus(&sh_str.sh_link);
                be32_to_cpus(&sh_str.sh_info);
                be32_to_cpus(&sh_str.sh_addralign);
                be32_to_cpus(&sh_str.sh_entsize);
        }

        nvt_dbg(FW, "%s: sh_str.sh_size=%d\n", __func__, sh_str.sh_size);

        if (sh_str.sh_size <= 0) {
                nvt_dbg(FW, "%s: sh_str.sh_size is incorrect!!\n", __func__);
                ret = -1;
                goto invalid_sh_size;
        }

        sh_name_tbl = kmalloc(sh_str.sh_size, GFP_KERNEL);
        if (sh_name_tbl == NULL) {
                nvt_dbg(FW, "%s: could not kamlloc sh_name_tbl\n", __func__);
                ret = -1;
                goto alloc_sh_name_tbl_failed;
        }

        nvt_fw_elf_seek(fw_buffer, sh_str.sh_offset, ELF_SEEK_SET);
        nvt_fw_elf_read(sh_name_tbl, sh_str.sh_size, 1, fw_buffer);

        total_size = 0;
        while (e_shnum) {
                nvt_dbg(FW, "%s: e_shnum=%x, e_shoff=%x\n", __func__, e_shnum,
                        e_shoff);

                nvt_fw_elf_seek(fw_buffer, e_shoff, ELF_SEEK_SET);
                if (nvt_fw_elf_read(((u8 *)(&sh)), e_shentsize, 1,
                        fw_buffer) == 0) {
                                break;
                }

                if (eh.e_ident[EI_DATA] == ELFDATA2MSB) {
                        be32_to_cpus(&sh.sh_name);
                        be32_to_cpus(&sh.sh_type);
                        be32_to_cpus(&sh.sh_flags);
                        be32_to_cpus(&sh.sh_addr);
                        be32_to_cpus(&sh.sh_offset);
                        be32_to_cpus(&sh.sh_size);
                        be32_to_cpus(&sh.sh_link);
                        be32_to_cpus(&sh.sh_info);
                        be32_to_cpus(&sh.sh_addralign);
                        be32_to_cpus(&sh.sh_entsize);
                }

                /* skip first section and string section */
                if ((sh.sh_type == SHT_NULL) || (sh.sh_type == SHT_STRTAB)) {
                        e_shoff += e_shentsize;
                        e_shnum--;
                        continue;
                }

                /* skip .bss */
                if ((sh.sh_flags & SHF_ALLOC) && (sh.sh_type == SHT_PROGBITS)) {
                        nvt_dbg(FW, "%s: load section: %s\n", __func__,
                                sh_name_tbl + sh.sh_name);
                        ret = nvt_fw_elf_data_program(bus, sh, (u8 *)fw_buffer);
                        if (ret < 0) {
                                goto fm_download_failed;
                        }
                        total_size += ret;

                }
                e_shnum--;
                e_shoff += sizeof(Elf32_Shdr);
        }
        nvt_dbg(FW, "%s: Total load size=%d\n", __func__, total_size);

        nvt_dbg(FW, "%s: starting config patch download...\n" , __func__);
        ret = nvt_config_patch_download(bus, false);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: config patch file download fail\n",
                        __func__);
                ret = 0;
                /* the driver will go on even if the config patch file
                download fail.*/
        }
        nvt_fw_fm_goto_address(bus, &entry_address);

fm_download_failed:
        kfree(sh_name_tbl);
alloc_sh_name_tbl_failed:
invalid_sh_size:
invalid_format:
        return ret;

}

/**
 * nvt_fw_download - Download the firmware when driver is inserted into kernel
 * @nvt_bus: nvt bus structure
 * @mode: STA, AP, or HUT mode
 *
 * Return: 0:success, a negative number:fail
 */
s32 nvt_fw_download(struct _nvt_bus *nvt_bus, enum nvt_fw_mode mode)
{
        s32 retval = 0;
        s32 err;
        s8 *fw_name;
        s32 fw_type;
        const struct firmware *fw = NULL;

        struct _nvt_diag_req  *diag_req = NULL;
        u32 data;
        //20151218 nash: coverity#48986
        s32 pkt_len = 0;

        nvt_dbg(FW, "%s: Start fw downloading\n", __func__);

        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s: nvt_bus is NULL\n", __func__);
                return -1;
        }

#if (NVT_FW_CFG_DOWNLOAD_BY_CHIPID == 1)
        fw_name = nvt_fw_get_name(nvt_bus, mode);
        nvt_dbg(INFO, "The filename of firmware: %s\n", fw_name);
#else
        switch (mode) {
        case NVT_FW_STA:
                fw_name = NVT_FW_NAME_STA;
                break;
        case NVT_FW_AP:
                fw_name = NVT_FW_NAME_AP;
                break;
        case NVT_FW_HUT:
                fw_name = NVT_FW_NAME_HUT;
                break;
        default:
                return -1;
        }
#endif

        err = request_firmware(&fw, fw_name, nvt_bus->dev);
        if (err < 0) {
                nvt_dbg(FW, "%s: request_firmware() fail\n", __func__);
                return -1;
        }

        fw_type = nvt_fw_get_type(nvt_bus);
        if (fw_type < 0) {
                nvt_dbg(ERROR, "%s: get fw type fail\n", __func__);
                retval = fw_type;
                goto error;
        }

        //20151127 nash: if fw is not in bootloader, force it to bootloader
        if (fw_type != NVT_FW_TYPE_BOOTLOADER) {
                diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
                if (diag_req == NULL) {
                        nvt_dbg(DIAG, "%s: kzalloc is for rep failed\n",
                                __func__);
                        retval = -1;
                        goto error;
                }

                /* trigger warm boot */
                data = cpu_to_le32(1);
                nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_REBOOT_CMD, 0,
                        4, (u8 *)&data, diag_req, &pkt_len);

                retval = nvt_bus->nvt_wdev_bus_ops.tx_ctrl(nvt_bus,
                        (u8 *)diag_req, pkt_len);
                if (retval < 0) {
                        nvt_dbg(ERROR, "%s: bus tx_ctrl error\n", __func__);
                        retval = -1;
                        goto error;
                }
        }

        //20161020 nash: delay time for FW to do warm boot
        msleep(10);

        err = nvt_fw_program_in_elf(nvt_bus,
                (fw->data + NVT_FW_IMAGE_HEADER_LEN));

        if (err < 0) {
                //nash:TODO
                nvt_dbg(FW, "%s: program fw fail\n", __func__);
                retval = -1;
                goto error;
        }


        /* Wait FW ready event */
        nvt_dbg(INFO, "%s: wait FW ready...\n", __func__);
        nvt_bus->fw_rdy_completed = 0;
        if (wait_event_timeout(nvt_bus->fw_rdy_wait,
                nvt_bus->fw_rdy_completed,
                msecs_to_jiffies(NVT_WAIT_FW_RDY_TIMEOUT))
                == 0) {

                nvt_dbg(ERROR, "%s: wait FW ready timeout\n",
                        __func__);
                //retval = -1;
                goto error;
        }
        nvt_dbg(INFO, "%s: DONE\n", __func__);


        nvt_dbg(FW, "%s: download to FW(%d) ok!\n", __func__, mode);
error:

        if (fw) {
                release_firmware(fw);
        }

        //20151218 nash: coverity#48966
        if (diag_req) {
                kfree(diag_req);
        }

        return retval;

}

/**
 * nvt_fw_change_opmode - download firmware to check operation mode
 * @nvt_if: nvt_if structure
 * @mode: sta, ap, or hut mode
 *
 * Return 0:success, a negative value:fail
 */
s32 nvt_fw_change_opmode(struct _nvt_if *nvt_if, enum nvt_fw_mode mode)
{
        s32 ret;
        struct _nvt_bus *nvt_bus;

        if (nvt_if == NULL) {
                nvt_dbg(ERROR, "%s: nvt_if is NULL\n", __func__);
                return -1;
        }

        //20150806 nash: get correct pointer of nvt_bus
        //nvt_bus = dev_get_drvdata(nvt_if->nvt_adapter->nvt_bus.dev);
        nvt_bus = nvt_if->nvt_adapter->nvt_bus;

        ret = nvt_fw_download(nvt_bus, mode);
        if (ret < 0) {
                nvt_dbg(ERROR, "%s: fw download fail\n", __func__);
                goto alloc_failed;
        }
        //20150709 nash: record mode
        nvt_if->mode = mode;

        //20150722 nash: set mac address
        nvt_set_macaddress_to_fw(nvt_bus->nvt_adapter);

        /* remove network interface when changing to HUT mode */
        if (NVT_FW_HUT == mode) {
                nvt_del_all_if(nvt_bus->nvt_adapter);
        }


alloc_failed:

        return ret;
}

/**
 * nvt_fw_cold_boot - do firmware cold boot
 * @nvt_if: _nvt_if structure
 *
 *
 * Return: 0:success, -1:fail
 */
s32 nvt_fw_cold_boot(struct _nvt_if *nvt_if)
{
        struct _nvt_diag_req  *diag_req;
        struct _nvt_bus *nvt_bus;
        s32 ret = 0;
        //20151218 nash: coverity#48984
        s32 pkt_len = 0;
        u32 data;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(DIAG, "%s: kzalloc is failed for req\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        nvt_bus = nvt_if->nvt_adapter->nvt_bus;
        data = 0;
        data = cpu_to_le32(data);

        ret = nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_REBOOT_CMD, 0x00,
                4, (u8 *)&data, diag_req, &pkt_len);
         //fix CID #61625 by jerry lin.
		if (ret < 0) {
                nvt_dbg(FW, "%s: failed to pack diag req!!\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        ret = nvt_bus->nvt_wdev_bus_ops.tx_ctrl(nvt_bus, (u8 *)diag_req,
                pkt_len);
        if (ret < 0) {
                nvt_dbg(FW, "%s: failed to send diag req!!\n", __func__);
                ret = -1;
                goto alloc_failed;
        }


alloc_failed:

        kfree(diag_req);
        return ret;

}


/**
 * nvt_fw_deep_sleep_resume - resume follow for deep sleep
 * @nvt_bus: _nvt_bus struct
 *
 * Return: 0: success, others: fail
 */
s32 nvt_fw_deep_sleep_resume(struct _nvt_bus *nvt_bus)
{
        s32 ret;
        s32 entry_addr;

        if (nvt_bus == NULL) {
                return -EINVAL;
        }

        ret = nvt_config_patch_download(nvt_bus, true);
        if (ret) {
                nvt_dbg(ERROR, "%s: config patch download fail!!\n",
                        __func__);
                return ret;
        }

        entry_addr = 4;
        ret = nvt_fw_fm_goto_address(nvt_bus, &entry_addr);
        if (ret) {
                nvt_dbg(ERROR, "%s: goto addr fail!!\n", __func__);
                return ret;
        }

        //waiting FW ready event
        nvt_bus->fw_rdy_completed = 0;
        if (wait_event_timeout(nvt_bus->fw_rdy_wait,
                nvt_bus->fw_rdy_completed,
                msecs_to_jiffies(NVT_WAIT_FW_RDY_TIMEOUT)) == 0) {
                        nvt_dbg(ERROR, "%s: wait FW ready timeout\n", __func__);
                return -1;
        }

        return 0;
}

/**
 * nvt_fw_request_cpf - request config patch file before suspend for deep sleep
 * @nvt_bus: _nvt_bus struct
 *
 * Return: 0: success, others: fail
 */
s32 nvt_fw_request_cpf(struct _nvt_bus *nvt_bus)
{
        s32 ret;
        s8 *config_path_file_name = NVT_FW_CONFIG_PATCH_FILE_NAME;

        nvt_dbg(FW, "%s: enter\r\n", __func__);

        /*sanity check*/
        if (nvt_bus == NULL) {
                nvt_dbg(ERROR, "%s nvt_bus is NULL\r\n", __func__);
                return -1;
        }

        ret = request_firmware(&resume_cfg_fw,
                        config_path_file_name,
                        nvt_bus->dev);

        if (ret < 0) {
                nvt_dbg(ERROR, "%s failed to config patch file fail!!",
                        __func__);
                return -1;
        }

        return 0;
}

/* nvt_set_firmware_capability
 * ..note: get f/w capability and update the nvt_fw_capability define
 * in nvt_adapter */
void nvt_set_firmware_capability(struct _nvt_adapter *nvt_adapter)
{
        s32 val_len = 0;
        u8 resp[512] = {0};
        u32 buff_size = 512;
        u8 wid_val_pos = 0;
        struct _nvt_bus *nvt_bus = nvt_adapter->nvt_bus;
        struct _nvt_fw_capability *fw_cap;
        val_len = nvt_get_wid_value(nvt_bus, WID_FIRMWARE_INFO,
                         resp, buff_size, &wid_val_pos);
        fw_cap = (struct _nvt_fw_capability *)(resp + wid_val_pos);
        memcpy(&nvt_adapter->nvt_fw_cap, (u8 *)fw_cap,
               sizeof(struct _nvt_fw_capability));
}

/**
 * nvt_fw_get_name - get corresponding firmware filename
 * @nvt_bus: _nvt_bus struct
 * @mode: nvt_fw_mode
 *
 * Return: the firmware filename
 */
#if (NVT_FW_CFG_DOWNLOAD_BY_CHIPID == 1)
static s8 *nvt_fw_get_name(struct _nvt_bus *nvt_bus, enum nvt_fw_mode mode)
{
        int i;

        s32 ret;
        s32 pkt_len = 0;
        s32 *chip_id;
        s8 *filename = NULL;
        struct _nvt_diag_req *diag_req = NULL;
        struct _nvt_diag_resp *diag_resp = NULL;
        struct _nvt_fw_name *fw_name_entry = NULL;

        diag_req = kzalloc(sizeof(struct _nvt_diag_req), GFP_KERNEL);
        if (diag_req == NULL) {
                nvt_dbg(FW, "%s: diag_req kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        diag_resp = kzalloc(sizeof(struct _nvt_diag_resp), GFP_KERNEL);
        if (diag_resp == NULL) {
                nvt_dbg(DIAG, "%s: diag_resp kzalloc is failed\n", __func__);
                ret = -1;
                goto alloc_failed;
        }

        /* Get chip id first */
        nvt_diag_pack_req(NVT_DIAG_CMD_CLASS_0, NVT_DIAG_GET_CHIP_ID_CMD, 0, 0,
                NULL, diag_req, &pkt_len);

        ret = nvt_bus->nvt_wdev_bus_ops.tx_ctrl(nvt_bus,
                (u8 *)diag_req, pkt_len);
        if (ret < 0) {
                nvt_dbg(FW, "%s: failed to send diag req!!\n", __func__);
                goto alloc_failed;
        }

        ret = nvt_bus->nvt_wdev_bus_ops.rx_ctrl(nvt_bus, (u8 *)diag_resp,
                sizeof(struct _nvt_diag_resp));
        if (ret < 0) {
                nvt_dbg(FW, "%s: failed to recv diag resp!!\n", __func__);
                goto alloc_failed;
        }

        chip_id = (s32 *)(diag_resp->sel.get_chip_id.chip_id);
        if (diag_resp->err_code != 0) {
                *chip_id = 0x18202;
        }

        ret = *chip_id;
        nvt_dbg(FW, "%s: Chip ID=0x%x\n", __func__, ret);

        for (i = 0; i < sizeof(fw_name_table)/sizeof(struct _nvt_fw_name);
                i++) {
                if (fw_name_table[i].chip_id == *chip_id &&
                        fw_name_table[i].mode == mode) {
                        fw_name_entry = &fw_name_table[i];
                        filename = fw_name_entry->filename;
                        break;
                }
        }

alloc_failed:
        kfree(diag_req);
        kfree(diag_resp);

        return filename;
}
#endif
