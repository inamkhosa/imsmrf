#!/bin/bash
#Script for stopping gst-launch commmand

#Example command to Stop gst_receiver or gst_sender in Sipp
#	<exec command="[script_path]gst_stop.sh \"[base_uri]test.wav\""/>
#	<exec int_cmd="pause_call"/>

NO_ARGS=0
E_OPTIONERROR=65

if [ $# -eq "$NO_ARGS" ]
then
	echo "Usage: `basename $0` audio_file"
	exit $E_OPTIONERROR
fi

#Get process id
gst_process_id=`ps -ef | grep gst-launch | grep -v grep | grep location="$1" | awk '{print $2}'`

#stop gst-launch
if [ ! -z $gst_process_id ]
then
	kill -2 $gst_process_id
fi
