#!/bin/sh
# Script reference: http://superuser.com/questions/332252/creating-and-formating-a-partition-using-a-bash-script

PARSER="$BUILD_DIR/nvt-tools/source/parser.py"
FDISK="$BUILD_DIR/nvt-tools/nvtprefdisk"

if [ -z $1 ]; then
	echo "Please specify the modelext bin."
	exit -1
fi

echo using $1 to create mbr

rm -f command.txt
rm -f max_capacity.txt

chmod 777 $PARSER
echo ==========start $PARSER==========
python $PARSER $1
RET=$?
if [[ !( "$RET" == 0 ) ]] ; then
    echo $PARSER returns error = $RET
    exit -1
fi
echo ==========end $PARSER==========

CMD=`cat command.txt`
echo ==========start dumping command ==========
echo "$CMD"
echo ==========end dumping command ==========

CAPACITY=`cat max_capacity.txt`
echo "max capacity = $CAPACITY"

chmod 777 $FDISK
echo ==========start running $FDISK to create mbr==========
cat << EOF | $FDISK /dev/zero $CAPACITY
$CMD
EOF
echo ==========end running $FDISK to create mbr==========

rm -f command.txt
rm -f max_capacity.txt

exit 0
