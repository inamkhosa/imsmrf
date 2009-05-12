#!/bin/bash


pid2=`ps -ef | grep sipp | grep duration_f1_uas.xml | grep -v grep | awk '{print $2}'`
for k in $pid2; do
kill -SIGCONT $k
break 
done


exit 0
