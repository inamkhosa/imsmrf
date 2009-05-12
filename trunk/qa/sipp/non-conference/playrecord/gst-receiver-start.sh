#!/bin/bash


../.././resume_sipp.sh

gst-launch -v udpsrc port=$1 ! rtppcmudepay ! mulawdec ! wavenc ! filesink location=$2 &

gst_process_id=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`

#kill -2 $gst_process_id

echo start receiving $gst_process_id >> pid.txt










