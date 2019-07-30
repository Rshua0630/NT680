#!/usr/bin/python2.7

import sys, os
import math
from array import array
from ctypes import Structure, POINTER, pointer, cast, sizeof, addressof, byref, c_uint, c_ubyte, c_ushort, c_char
path=os.getenv("BUILD_DIR")+"/nvt-tools/source"
sys.path.insert(0,path)
import modelext as mymodelext
MAGIC = array("I", [0x55aa55aa,0xaa55aa55,0x11223344,0x55667788])

'''
	This tool can be used to create factory preburn image
'''


def usage():
	print "\033[01;33mUsage: %s input_image_list output_image\033[0m" % sys.argv[0]
	print "e.g."
	print "\t %s \"LD{SOC}.bin FW{SOC}.ext.bin FW{SOC}.bin u-boot.lz.bin uImage.bin rootfs.ubifs.bin.raw\" output.bin" % sys.argv[0]
	print "\n\033[01;31mPlease follow partition table sequence to add your images \033[0m"

def get_part_offset(emb, image_name):
	if ("FW" in image_name and "ext.bin" in image_name):
		type=mymodelext.EMBTYPE_MODELEXT
	elif ("FW" in image_name and ".bin" in image_name):
		type=mymodelext.EMBTYPE_UITRON
	elif ("u-boot" in image_name):
		type=mymodelext.EMBTYPE_UBOOT
	elif ("uImage" in image_name):
		type=mymodelext.EMBTYPE_LINUX
	elif ("rootfs" in image_name and ".bin" in image_name):
		type=mymodelext.EMBTYPE_ROOTFS
	elif ("MBR" in image_name):
		type=mymodelext.EMBTYPE_MBR
	elif ("LD" in image_name and ".bin" in image_name):
		type=mymodelext.EMBTYPE_LOADER
	elif ("DSP" in image_name and ".bin" in image_name):
		type=mymodelext.EMBTYPE_DSP
	else:
		type=mymodelext.EMBTYPE_LOADER

	if ("1.bin" in image_name):
		index=0
	elif ("2.bin" in image_name):
		index=1
	elif ("3.bin" in image_name):
		index=2
	else:
		index=0

	for i in range(mymodelext.EMB_PARTITION_INFO_COUNT):
		if type == emb[i].EmbType:
			if index == emb[i].OrderIdx:
				return emb[i].PartitionOffset
	return -1

def process_image(input_name, output_name):
	input_list = input_name.split (' ')
	print "\033[01;32mYour input list: %s" % input_list
	print "Output is %s\033[0m" % output_name

	''' Get modelext info '''
	print "\033[01;32mYour modelext info is(unit: block): \033[0m"
	for x in input_list:
		if ("ext.bin" in x):
			file = open(x, "rb")
			if file is None:
				print("failed to open file")
				return -1
			mymodelext_obj = file.read()
			file.close()
			emb_addr = mymodelext.modelext_get_cfg(mymodelext_obj, mymodelext.MODELEXT_TYPE_EMB_PARTITION)
			if emb_addr is not None:
				emb = cast(emb_addr, POINTER(mymodelext.EMB_PARTITION))
				for i in range(mymodelext.EMB_PARTITION_INFO_COUNT):
					print("0x{:X} {} 0x{:08X} 0x{:08X} 0x{:08X}".format(
						emb[i].EmbType,
						emb[i].OrderIdx,
						emb[i].PartitionOffset,
						emb[i].PartitionSize,
						emb[i].ReversedSize))
		break

	''' Generate preburn image '''
	input_num=1
	for x in input_list:
		print "Processing %s " % x
		with open(x, "rb") as file:
			input_data = file.read()
		file.close()

		with open(output_name, "ab") as file:
			for data in input_data:
				file.write(data)
			''' Padding 0 if not 4 align '''
			while file.tell()%4 != 0:
				file.write(c_ubyte(0))
			file.write(MAGIC)
			if (input_num < len(input_list)):
				off=get_part_offset(emb,input_list[input_num])
				file.write(c_uint(off))
		file.close()
		input_num+=1
	print "\033[01;32mGenerate preburn image finished.\033[0m"

def main():
	input_name = sys.argv[1]
	output_name = sys.argv[len(sys.argv)-1]
	if os.path.isfile(output_name):
		os.remove(output_name)

	process_image(input_name, output_name)


if __name__ == "__main__":
	if (len(sys.argv) != 3) :
		usage()
		sys.exit(0)

	main()
