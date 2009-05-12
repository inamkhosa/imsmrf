#!/bin/bash



gst_process_id=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`


kill -2 $gst_process_id > /dev/null 2>&1


../.././resume_sipp.sh









