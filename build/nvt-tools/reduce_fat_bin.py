#!/usr/bin/python2.7

import sys, os
import math
import base64
from array import array
SEGMENT=64

'''
	This tool can be used to get reduced fat image
'''


def usage():
	print "\033[01;33mUsage: %s fat_input_image fat_output_image\033[0m" % sys.argv[0]
	print "e.g."
	print "\t %s fat_image.bin fat_reduce_image.bin" % sys.argv[0]

def data_array_is_zero(data_list):
	val=base64.b16encode(data_list)
	if (int(val,16) == 0):
		return 1
	else:
		return 0

def process_image(input_name, output_name):
	print "\033[01;32mInput image name is %s" % input_name
	print "Output image name is %s\033[0m" % output_name

	''' Generate reduced fat image '''
	file_line_num=0
	with open(input_name, "rb") as file_in, open(output_name, "wb") as file_out:
		file_in.seek(0,2)
		size=file_in.tell()
		total_lines=size/SEGMENT
		file_line_num=total_lines-1

		while file_line_num >= 0:
			file_in.seek(SEGMENT*file_line_num,0)
			input_data = file_in.read(SEGMENT)
			ret=data_array_is_zero(input_data)
			if (ret == 0):
				break;
			else:
				file_line_num-=1

		file_in.seek(0,0)
		input_data = file_in.read((file_line_num+2)*SEGMENT + 64*1024)
		for data in input_data:
			file_out.write(data)
	file_in.close()
	file_out.close()

	print "\033[01;32mGenerate reduced fat image finished.\033[0m"

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
