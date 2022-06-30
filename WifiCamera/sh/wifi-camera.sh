#!/bin/bash

# set android device
adb wait-for-usb-device && adb root && adb remount && adb shell mount -o remount,rw /

PROJECT_DIR=`pwd`

echo ${PROJECT_DIR}

adb push ${PROJECT_DIR}/../env/hostapd.conf /etc/wlan/
adb push ${PROJECT_DIR}/../env/udhcpd.conf /etc/

adb push ${PROJECT_DIR}/../bin/gst_test /data
adb push ${PROJECT_DIR}/../bin/gst_test_server /data

adb shell ifconfig wlan0 192.168.0.1 netmask 255.255.255.0
adb shell hostapd -B /etc/wlan/hostapd.conf

adb shell mkdir -p /var/lib/misc
adb shell touch /var/lib/misc/udhcpd.leases
adb shell udhcpd -f /etc/udhcpd.conf  &

adb shell .//data/gst_test_server &
adb shell .//data/gst_test