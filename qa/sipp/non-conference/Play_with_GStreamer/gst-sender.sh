#!/bin/bash

echo start sending
delaytime=`echo $4 / 1000.0`

delaytime1=`echo "scale=10; ${delaytime}" | bc`



for((i=1 ; i<= $3 ; i++))
do

gst-launch -v filesrc location=$2 ! wavparse ! audioconvert ! audioresample ! mulawenc ! rtppcmupay ! udpsink port=$1 host=192.168.0.159 

if [ $i -eq $3 ] ; then

break

fi

sleep $delaytime1

done

sleep 10

./Resume_SIP_Server.sh








