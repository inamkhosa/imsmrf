#!/bin/bash

sleeptime=`echo $1 / 1000.0 + 0.5`

sleep2=`echo "scale=10; ${sleeptime}" | bc`

gst-launch -v udpsrc port=$2 ! rtppcmudepay ! mulawdec ! wavenc ! filesink location=testing.wav &

echo "sleeping"



sleep $sleep2

gst_process_id=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`


kill -2 $gst_process_id
