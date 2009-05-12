#!/bin/bash
#Script for receiving an audio file with gstreamer

#Example command to start gst_receiver in Sipp
#	<exec command="[script_path]gst_receiver.sh [$src_port] \"[base_uri]test.wav\" 0"/>
#	<exec int_cmd="pause_call"/>
#Example command to Stop gst_receiver in Sipp
#	<exec command="[script_path]gst_stop.sh \"[base_uri]test.wav\""/>
#	<exec int_cmd="pause_call"/>

NO_ARGS=3
E_OPTIONERROR=65

if [ $# -ne "$NO_ARGS" ]
then
	echo "Usage: `basename $0` local_port audio_file payload_number"
	exit $E_OPTIONERROR
fi

# Get directory path of current script
current_path=`dirname $0`

# Resume Sipp client
$current_path/resume_sipp.sh

#Start receiving audio file
if [ $3 == 0 ]
then
	gst-launch -v udpsrc port=$1 ! rtppcmudepay ! mulawdec ! wavenc ! filesink location="$2"
elif [ $3 == 3 ]
then
	gst-launch udpsrc port=$1 ! msgsmdepayloader ! msgsmdec ! wavenc ! filesink location="$2"
else
	echo "Unsupported Payload Number"
	exit $E_OPTIONERROR
fi

# Resume Sipp client
$current_path/resume_sipp.sh
