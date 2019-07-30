#!/bin/sh
# Usage:
#	./awk_get_target.sh openssl: Makefile
#	Get target paragraph content.
#
#
awk -v pattern=$1 \
'$1 == pattern \
{
	while (getline line_str > 0)
	{
		val=match(line_str,":");
		if (val == 0)
			print line_str;
		else
			break;
	}
}' $2
