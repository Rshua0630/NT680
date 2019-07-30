# pylint: disable=locally-disabled, line-too-long, unused-argument, unused-variable, pointless-string-statement, too-many-locals, missing-docstring, too-many-statements, invalid-name, wrong-import-position, too-many-arguments, too-many-branches
# Copyright Novatek Microelectronics Corp. 2016.  All rights reserved.

import os
import sys
import time
import modelext as mymodelext
from ctypes import Structure, POINTER, pointer, cast, sizeof, addressof, byref, c_uint, c_ubyte, c_ushort, c_char

partition_number = 0
last_partition = 0
max_capacity = 0

def write_command(file, emb):

    global partition_number, last_partition, max_capacity
    if emb.EmbType != mymodelext.EMBTYPE_PSTORE and emb.EmbType != mymodelext.EMBTYPE_ROOTFS:
        print("write_command() can't support partition type {:d}".format(emb.EmbType))
        return -1

    if last_partition == 1:
        print("only last partition can have 0 partition size\n")
        return -1

    if partition_number == 3:
        file.write("n\n")
        file.write("e\n")
        file.write("{:d}\n".format(max_capacity))
        file.write("\n")

    if partition_number >= 3:
        file.write("n\n")
        #file.write("l\n")
        file.write("{:d}\n".format(emb.PartitionOffset))
    else:
        file.write("n\n")
        file.write("p\n")
        file.write("{:d}\n".format(partition_number+1))
        file.write("{:d}\n".format(emb.PartitionOffset))

    if emb.PartitionSize == 0:
        last_partition = 1
        file.write("+1\n")
        max_capacity = (emb.PartitionOffset + 2)
    else:
        file.write("+{:d}\n".format(emb.PartitionSize - 1))
        max_capacity = (emb.PartitionOffset + emb.PartitionSize)

    partition_number += 1

    return 0

def create_command_file(emb_addr):
    file = open("command.txt", "w")
    if file is None:
        print("failed to create command.txt")
        return -1

    file.write("o\n")

    ret = 0
    if emb_addr is not None:
        emb = cast(emb_addr, POINTER(mymodelext.EMB_PARTITION))
        for i in range(mymodelext.EMB_PARTITION_INFO_COUNT):
            if emb[i].EmbType == mymodelext.EMBTYPE_LOADER:
                print("skiped loader 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_MODELEXT:
                print("skiped modelext 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_UITRON:
                print("skiped uitron 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_ECOS:
                print("skiped ecos 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_UBOOT:
                print("skiped uboot 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_LINUX:
                print("skiped linux 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_DSP:
                print("skiped dsp 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_PSTORE:
                print("pstore 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
                ret = write_command(file, emb[i]);
                if ret != 0:
                    break
            elif emb[i].EmbType == mymodelext.EMBTYPE_FAT:
                print("skiped fat 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_EXFAT:
                print("skiped exfat 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_ROOTFS:
                print("rootfs 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
                ret = write_command(file, emb[i]);
                if ret != 0:
                    break
            elif emb[i].EmbType == mymodelext.EMBTYPE_RAMFS:
                print("skiped ramfs 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_UENV:
                print("skiped uenv 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            elif emb[i].EmbType == mymodelext.EMBTYPE_MBR:
                print("skiped mbr 0x{:08X} 0x{:08X} 0x{:08X}".format(emb[i].PartitionOffset, emb[i].PartitionSize, emb[i].ReversedSize))
            else:
                print("no more partition entry");
                break

    if ret == 0:
        file.write("p\n");
        file.write("w\n");
        file.write("q\n");

    file.close()

    return ret

def create_capacity_file():

    global max_capacity

    file = open("max_capacity.txt", "w")
    if file is None:
        print("failed to create max_capacity.txt")
        return -1

    file.write("{:d}".format(max_capacity))
    file.close()

    return 0

def main(argv):
    file = open(argv[1], "rb")
    if file is None:
        print("failed to open file")
        return -1
    ext = file.read()
    file.close()

    emb_addr = mymodelext.modelext_get_cfg(ext, mymodelext.MODELEXT_TYPE_EMB_PARTITION)
    print("start parsing {:s}".format(argv[1]));
    ret = create_command_file(emb_addr)
    print("end parsing {:s}".format(argv[1]));
    if ret != 0:
        print("fail to create command file\n");
        return ret

    ret = create_capacity_file()

    return ret

# ============================================================================================
# DO NOT EDIT FOLLOWING CODES
# ============================================================================================
if __name__ == "__main__":
    sys.exit(main(sys.argv))
# ============================================================================================
