# pylint: disable=locally-disabled, line-too-long, unused-argument, unused-variable, pointless-string-statement, too-many-locals, missing-docstring, too-many-statements, invalid-name, wrong-import-position, too-many-arguments, too-many-branches
# Copyright Novatek Microelectronics Corp. 2016.  All rights reserved.

import os
import sys
import time
from ctypes import Structure, POINTER, pointer, cast, sizeof, addressof, byref, c_uint, c_ubyte, c_ushort, c_char

MODELEXT_INFO_VER = 0x16072219
MODELEXT_TYPE_DUMMY = 0
MODELEXT_TYPE_INFO = 1
MODELEXT_TYPE_BIN_INFO = 2
MODELEXT_TYPE_PINMUX_CFG = 3
MODELEXT_TYPE_INTDIR_CFG = 4
MODELEXT_TYPE_EMB_PARTITION = 5
MODELEXT_TYPE_GPIO_INFO = 6
MODELEXT_TYPE_DRAM_PARTITION = 7
MODELEXT_TYPE_MODEL_CFG = 8
MODELEXT_TYPE_MAX = 9

class MODELEXT_HEADER(Structure):
    _fields_ = [
        ("size", c_uint),
        ("type", c_uint),
        ("number", c_uint),
        ("version", c_uint),
    ]

class MODELEXT_INFO(Structure):
    _fields_ = [
        ("name", c_char*8),
        ("chip_name", c_char*8),
        ("version", c_char*8),
        ("date", c_char*8),
        ("ext_bin_length", c_uint),
        ("check_sum", c_uint),
    ]

EMB_PARTITION_INFO_VER = 0x16072117
EMB_PARTITION_INFO_COUNT = 16
EMBTYPE_UNKNOWN = 0x00
EMBTYPE_LOADER  = 0x01
EMBTYPE_MODELEXT = 0x02
EMBTYPE_UITRON = 0x03
EMBTYPE_ECOS = 0x04
EMBTYPE_UBOOT = 0x05
EMBTYPE_LINUX = 0x06
EMBTYPE_DSP = 0x07
EMBTYPE_PSTORE = 0x08
EMBTYPE_FAT = 0x09
EMBTYPE_EXFAT = 0x0A
EMBTYPE_ROOTFS = 0x0B
EMBTYPE_RAMFS = 0x0C
EMBTYPE_UENV = 0x0D
EMBTYPE_MBR = 0x0E

class EMB_PARTITION(Structure):
    _fields_ = [
        ("EmbType", c_ushort),
        ("OrderIdx", c_ushort),
        ("PartitionOffset", c_uint),
        ("PartitionSize", c_uint),
        ("ReversedSize", c_uint),
    ]

class BYTE(Structure):
    _fields_ = [
        ("byte", c_ubyte)
    ]

def modelext_get_cfg(buf, type):
    MAGIC = b"MODELEXT"
    p = cast(buf, POINTER(BYTE))
    h = cast(pointer(p[0]), POINTER(MODELEXT_HEADER))[0]
    d = cast(pointer(p[sizeof(MODELEXT_HEADER)]), POINTER(MODELEXT_INFO))[0]
    if type >= MODELEXT_TYPE_MAX:
        return None
    if d.name != MAGIC:
        return None

    h_tail = addressof(p[d.ext_bin_length])
    while h.size:
        if h.type == type:
            break
        h = cast(byref(h, h.size), POINTER(MODELEXT_HEADER))[0]
        if addressof(h)+1 >= h_tail:
            return None

    if h.size == 0:
        return None

    return addressof(h)+sizeof(MODELEXT_HEADER)
    

def main(argv):
    file = open(r"FW96687A.ext.bin", "rb")
    if file is None:
        print("failed to open file")
        return -1
    ext = file.read()
    file.close()

    emb_addr = modelext_get_cfg(ext, MODELEXT_TYPE_EMB_PARTITION)
    if emb_addr is not None:
        emb = cast(emb_addr, POINTER(EMB_PARTITION))   
        for i in range(EMB_PARTITION_INFO_COUNT):
            print("0x{:X} {} 0x{:08X} 0x{:08X} 0x{:08X}".format(
                emb[i].EmbType,
                emb[i].OrderIdx,
                emb[i].PartitionOffset,
                emb[i].PartitionSize,
                emb[i].ReversedSize))
    return 0

# ============================================================================================
# DO NOT EDIT FOLLOWING CODES
# ============================================================================================
if __name__ == "__main__":
    sys.exit(main(sys.argv))
# ============================================================================================
