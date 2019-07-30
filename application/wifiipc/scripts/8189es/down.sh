#!/bin/sh

TRY=0
DHCPD=`ps`
DHCPD=`echo $DHCPD | grep udhcpd`
if [ -n "$DHCPD" ]; then
        echo "udhcpd is running. Try to stop it"
        killall -9 udhcpd
        while [ 1 ]
        do
                DHCPD=`ps`
                DHCPD=`echo $DHCPD | grep udhcpd`
                if [ -n "$DHCPD" ]; then
                        if [ $TRY -gt 5 ]; then
                                echo "can't stop udhcpd"
                                exit 255
                        else
                                echo "TRY=$TRY"
                                TRY=$((${TRY}+1))
                                sleep 1
                        fi
                else
                        echo "udhcpd is stopped"
                        break
                fi
        done
fi

TRY=0
DHCPC=`ps`
DHCPC=`echo $DHCPC | grep udhcpc`
if [ -n "$DHCPC" ]; then
        echo "udhcpc is running. Try to stop it"
        killall -9 udhcpc
        while [ 1 ]
        do
                DHCPC=`ps`
                DHCPC=`echo $DHCPC | grep udhcpc`
                if [ -n "$DHCPC" ]; then
                        if [ $TRY -gt 5 ]; then
                                echo "can't stop udhcpc"
                                exit 255
                        else
                                echo "TRY=$TRY"
                                TRY=$((${TRY}+1))
                                sleep 1
                        fi
                else
                        echo "udhcpc is stopped"
                        break
                fi
        done
fi

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

ulink /var/run/hostapd.conf
ulink /var/run/wpa_supplicant.conf
rmmod 8189es

exit 0
