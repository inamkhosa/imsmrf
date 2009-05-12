#!/bin/bash

echo "Enter the File uri please :"
read filename
echo "Enter the Repeat value :"
read repeat
echo "Enter the Delay value :"
read delay

duration=`sndfile-info $filename | grep Duration | awk '{print $3}'`
fileduration=`echo $duration | awk '{ split($1,a,":"); print (a[1]*60*60*1000)+(a[2]*60*1000)+(a[3]*1000) }'`
filerecorded=`expr $fileduration \* $repeat`
no_of_delays=`expr $repeat - 1`
playduration=`expr $fileduration \* $repeat + $delay \* $no_of_delays`


echo SEQUENTIAL >> play_info.csv
echo  "$filename;$filerecorded;$playduration;$repeat;$delay" >> play_info.csv
