#!/bin/sh
ISCSI_CONF_FILE=/etc/event_iscsi.conf

# the first session path or the current device
G_SESSION_PATH1=

# values of the configuration file
G_CFG_NAME=
G_CFG_IP=
G_CFG_TARGET=
G_CFG_USER=
G_CFG_PASSWD=
G_CFG_BMOUNT=

# parsing values in sysfs
G_SYS_PTNUM=
G_SYS_IP=
G_SYS_USER=
G_SYS_PASSWD=
G_SYS_TARGET=

myecho()
{
    # normal echo
    # echo "$*"
    # output to file
    # echo "$*" >> /autoscsi.log
    :
}

reset_cfg_value()
{
    G_CFG_NAME=
    G_CFG_IP=
    G_CFG_TARGET=
    G_CFG_USER=
    G_CFG_PASSWD=
    G_CFG_BMOUNT=
}

get_cfg_setting()
{
    #File content example:
    #iscsi_name=iscsi_name
    #iscsi_ip=192.168.0.6
    #iscsi_target_name=iqn.2006-03.com.kernsafe:jtv208-PC.RamDisk0
    #iscsi_user=
    #iscsi_password=
    #iscsi_bmount=0

    #assume TARGET_TAG=my_iscsi_name
    local FIND_IP=$1
    local FIND_TARGET=$2
    
    myecho FIND_IP=$FIND_IP
    myecho FIND_TARGET=$FIND_TARGET

    reset_cfg_value
    
    if [ ! -e $ISCSI_CONF_FILE ]; then
        myecho $ISCSI_CONF_FILE not found
        return 1
    fi
    
    exec < $ISCSI_CONF_FILE
    while read line; do
        #myecho $line
        TAG_NAME=${line%%=*}
        TAG_VAL=${line##*=}

        case "${TAG_NAME}" in
        "iscsi_name")
            #the following belongs to other groups, check the previous group
            if [ ! -z $G_CFG_NAME ] && [ $FIND_IP == $G_CFG_IP ] && [ $FIND_TARGET == $G_CFG_TARGET ]; then
                break #break while read line
            fi
            reset_cfg_value
            G_CFG_NAME=$TAG_VAL
            ;;
        "iscsi_ip")
            G_CFG_IP=$TAG_VAL
            ;;
        "iscsi_target_name")
            G_CFG_TARGET=$TAG_VAL
            ;;
        "iscsi_user")
            G_CFG_USER=$TAG_VAL
            ;;
        "iscsi_password")
            G_CFG_PASSWD=$TAG_VAL
            ;;
        "iscsi_bmount")
            G_CFG_BMOUNT=$TAG_VAL
            ;;
        *)
            ;;
        esac
    done
   
    # check the last group, it may double check here when we found it before the file end
    if [ ! -z $G_CFG_NAME ] && [ $FIND_IP == $G_CFG_IP ] && [ $FIND_TARGET == $G_CFG_TARGET ]; then
        myecho G_CFG_NAME=$G_CFG_NAME
        myecho G_CFG_IP=$G_CFG_IP
        myecho G_CFG_TARGET=$G_CFG_TARGET
        myecho G_CFG_USER=$G_CFG_USER
        myecho G_CFG_PASSWD=$G_CFG_PASSWD
        myecho G_CFG_BMOUNT=$G_CFG_BMOUNT
        return 0
    fi

    return 1
}

get_session_path1()
{
    #assume $1 is the iscsi device name, such as "sda"
    local TARGET_ISCSI_DEV=$1
    local HOST_DIR
    local SESSION_PATH1
    local BLOCK_FILE
    local BLOCK_DEV
    
    myecho TARGET_ISCSI_DEV=$TARGET_ISCSI_DEV
    
    for HOST_DIR in /sys/devices/platform/host*; do
    #e.g. HOST_DIR=/sys/devices/platform/host0
        if ! [ -d $HOST_DIR/iscsi_host* ]; then
            continue
        fi
            for SESSION_PATH1 in $HOST_DIR/session*; do
        #e.g. SESSION_PATH1=/sys/devices/platform/host0/session1
            if ! [ -d $SESSION_PATH1/target* ]; then
                continue
            fi
            for BLOCK_FILE in $SESSION_PATH1/target*/*\:*/block/*; do
            #e.g. BLOCK_FILE=/sys/devices/platform/host0/session1/target0:0:0/0:0:0:0/block/sda
                BLOCK_DEV=`echo "$BLOCK_FILE" | sed 's/.*block\///'`
                #e.g. BLOCK_DEV=sda
                if [ $BLOCK_DEV == $TARGET_ISCSI_DEV ]; then
                    myecho BLOCK_DEV=$BLOCK_DEV
                    G_SESSION_PATH1=$SESSION_PATH1
                    return 0
                fi
            done
        done
    done
    
    G_SESSION_PATH1=
    return 1
}

get_sys_devinfo()
{
    local SESSION_PATH1=$1
    local SESSION_PATH2
    local BLOCK_DEVPATH
    local DEV_PARTIPATH
    local CONNECTION_PATH
    local BLOCK_DEV
    local DEV_PARTINAME
   
    for BLOCK_DEVPATH in $SESSION_PATH1/target*/*\:*/block/*; do
    #e.g. BLOCK_DEVPATH=/sys/devices/platform/host2/session3/target2:0:0/2:0:0:0/block/sda
    myecho BLOCK_DEVPATH=$BLOCK_DEVPATH
        BLOCK_DEV=`echo "$BLOCK_DEVPATH" | sed 's/.*block\///'`
        #e.g. BLOCK_DEV=sda
        myecho BLOCK_DEV=$BLOCK_DEV

        G_SYS_PTNUM=0
        for DEV_PARTIPATH in $BLOCK_DEVPATH/$BLOCK_DEV*; do
        #e.g. DEV_PARTIPATH=/sys/devices/platform/host2/session3/target2:0:0/2:0:0:0/block/sda/sda1
        myecho DEV_PARTIPATH=$DEV_PARTIPATH

            DEV_PARTINAME=`echo "$DEV_PARTIPATH" | sed "s/.*$BLOCK_DEV\///"`
            #e.g. DEV_PARTINAME=sda1
            myecho DEV_PARTINAME=$DEV_PARTINAME
            let "G_SYS_PTNUM += 1"
        done
    done
    
    for SESSION_PATH2 in $SESSION_PATH1/iscsi_session/session*; do    
    #e.g. SESSION_PATH2=/sys/devices/platform/host1/session2/iscsi_session/session2
    #myecho SESSION_PATH2=$SESSION_PATH2
        G_SYS_USER=`cat $SESSION_PATH2/username`
        G_SYS_PASSWD=`cat $SESSION_PATH2/password`
        G_SYS_TARGET=`cat $SESSION_PATH2/targetname`
    done

    for CONNECTION_PATH in $SESSION_PATH1/connection*/iscsi_connection/connection*\:*; do
    #e.g. CONNECTION_PATH=/sys/devices/platform/host1/session2/connection2:0/iscsi_connection/connection2:0
    myecho CONNECTION_PATH=$CONNECTION_PATH
        G_SYS_IP=`cat $CONNECTION_PATH/address`
    done
    
    myecho G_SYS_PTNUM=$G_SYS_PTNUM
    myecho G_SYS_IP=$G_SYS_IP
    myecho G_SYS_USER=$G_SYS_USER
    myecho G_SYS_PASSWD=$G_SYS_PASSWD
    myecho G_SYS_TARGET=$G_SYS_TARGET
    
    return 0
}

my_umount()
{
    FOLDER=`grep "/dev/$1" /proc/mounts | cut -d ' ' -f 2`
    if [ ! -z "$FOLDER" ]; then
        umount -l "$FOLDER";
    fi
}
 
my_mount()
{
    #assume $1=sda, sdb, ...
    local DEV_NAME=$1
    
    #set default values
    local MOUNT_DEV=/dev/$DEV_NAME
    local MOUNT_FOLDER=/mnt/$DEV_NAME
    
    #if it is iscsi, update the values
    get_session_path1 $DEV_NAME
    if [ $? = 0 ]; then
    #is iscsi device
        get_sys_devinfo $G_SESSION_PATH1
        if [ $? = 0 ]; then
            get_cfg_setting $G_SYS_IP $G_SYS_TARGET
            if [ $? = 0 ]; then
                myecho G_SYS_PTNUM=$G_SYS_PTNUM
                if [ $G_SYS_PTNUM = 0 ]; then
                    MOUNT_DEV=/dev/$DEV_NAME
                    MOUNT_FOLDER=/mnt/iscsi/$G_CFG_NAME
                elif [ $G_SYS_PTNUM = 1 ]; then
                    MOUNT_DEV=/dev/$DEV_NAME$G_SYS_PTNUM
                    MOUNT_FOLDER=/mnt/iscsi/$G_CFG_NAME
                else
                    myecho Partition Num $G_SYS_PTNUM not supported
                    return 1
                fi
            fi
        fi
    fi

    myecho MOUNT_DEV=$MOUNT_DEV
    myecho MOUNT_FOLDER=$MOUNT_FOLDER
    
    mkdir -p "${MOUNT_FOLDER}" || return 1
    mount "${MOUNT_DEV}" "${MOUNT_FOLDER}" || return 1
}

case "${ACTION}" in
add|"")
   my_mount ${MDEV}
   ;;
remove)
   my_umount ${MDEV}
   ;;
esac
