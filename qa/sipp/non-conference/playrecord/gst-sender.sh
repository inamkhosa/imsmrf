#!/bin/bash






../.././resume_sipp.sh

gst-launch -v filesrc location=$2 ! wavparse ! audioconvert ! audioresample ! mulawenc ! rtppcmupay ! udpsink port=$1 host=$3



gst_process_id=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`











