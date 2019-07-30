#!/bin/sh

TRY=0
HOSTAPD=`ps`
HOSTAPD=`echo $HOSTAPD | grep hostapd`
if [ -n "$HOSTAPD" ]; then
        echo "hostapd is running. Try to stop it"
	killall -9 hostapd
	while [ 1 ]
	do
		HOSTAPD=`ps`
		HOSTAPD=`echo $HOSTAPD | grep hostapd`
		if [ -n "$HOSTAPD" ]; then
			if [ $TRY -gt 5 ]; then
				echo "can't stop hostapd"
				exit 255
			else
				echo "TRY=$TRY"
				TRY=$((${TRY}+1))
	                        sleep 1
			fi
		else
			echo "hostapd is stopped"
			break
		fi
	done
fi

TRY=0
WPA=`ps`
WPA=`echo $WPA | grep wpa_supplicant`
if [ -n "$WPA" ]; then
        echo "wpa_supplicant is running. Try to stop it"
        killall -9 wpa_supplicant
        while [ 1 ]
        do
		WPA=`ps`
		WPA=`echo $WPA | grep wpa_supplicant`
                if [ -n "$WPA" ]; then
                        if [ $TRY -gt 5 ]; then
                                echo "can't stop wpa_supplicant"
                                exit 255
                        else
                                echo "TRY=$TRY"
                                TRY=$((${TRY}+1))
                                sleep 1
                        fi
                else
                        echo "wpa_supplicant is stopped"
                        break
                fi
        done
fi

if [ "$1" == "ap" ]; then
	echo driver=nl80211 >> /var/run/hostapd.conf
	echo wmm_enabled=1 >> /var/run/hostapd.conf
	if [ "$2" == "0" ]; then
		ifconfig wlan0 up
		iwpriv wlan0 setmib channel=0
		iwpriv wlan0 setmib autoch_maskmap_2g=28672
		echo channel=7 >> /var/run/hostapd.conf
	else
		echo channel=$2 >> /var/run/hostapd.conf
	fi
	/usr/bin/hostapd /var/run/hostapd.conf &
	TRY=0
	while [ 1 ]
	do
	        UP=`ifconfig wlan0 | grep UP`
	        echo "UP=$UP"
	
	        if [ -z "$UP" ]; then
	                echo "TRY=$TRY"
	                if [ $TRY -gt 5 ]; then
	                        hostapd stop
	                        exit 255
	                else
	                        TRY=$((${TRY}+1))
	                        sleep 1
	                fi
	        else
	                echo "UP ok"
	                ifconfig wlan0 192.168.1.254
			break
	        fi
	done

	udhcpd /etc/udhcpdw.conf
	TRY=0
	while [ 1 ]
	do
	        DHCP=`ps`
	        DHCP=`echo $DHCP | grep udhcpd`
	        echo "DHCP=$DHCP"
	
	        if [ -z "$DHCP" ]; then
	                echo "TRY=$TRY"
	                if [ $TRY -gt 5 ]; then
	                        echo "Can't run udhcpd"
	                        exit 255
	                else
	                        TRY=$((${TRY}+1))
	                        sleep 1
	                fi
	        else
	                echo "udhcpd ok"
	                exit 0
	        fi
	done
elif [ "$1" == "sta" ]; then
        wpa_supplicant -Dnl80211 -i wlan0 -c /var/run/wpa_supplicant.conf &
        TRY=0
        while [ 1 ]
        do
                UP=`ifconfig wlan0 | grep UP`
                echo "UP=$UP"

                if [ -z "$UP" ]; then
                        echo "TRY=$TRY"
                        if [ $TRY -gt 5 ]; then
                                hostapd stop
                                exit 255
                        else
                                TRY=$((${TRY}+1))
                                sleep 1
                        fi
                else
                        echo "UP ok"
                        ifconfig wlan0 192.168.1.1
                        break
                fi
        done

        udhcpc -i wlan0
        TRY=0
        while [ 1 ]
        do
                DHCP=`ps`
                DHCP=`echo $DHCP | grep udhcpc`
                echo "DHCP=$DHCP"

                if [ -z "$DHCP" ]; then
                        echo "TRY=$TRY"
                        if [ $TRY -gt 5 ]; then
                                echo "Can't run udhcpc"
                                exit 255
                        else
                                TRY=$((${TRY}+1))
                                sleep 1
                        fi
                else
                        echo "udhcpd ok"
                        exit 0
                fi
        done
fi

exit 255
