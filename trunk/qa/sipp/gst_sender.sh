#!/bin/bash
#Script for playing an audio file through gstreamer

#Example command to start gst_sender in Sipp
#	<exec command="[script_path]gst_sender.sh [$dest_ip] [$dest_port] \"[base_uri]test.wav\" 0"/>
#	<exec int_cmd="pause_call"/>
#Example command to Stop gst_sender in Sipp
#	<exec command="[script_path]gst_stop.sh \"[base_uri]test.wav\""/>
#	<exec int_cmd="pause_call"/>

NO_ARGS=4
E_OPTIONERROR=65

if [ $# -ne "$NO_ARGS" ]
then
	echo "Usage: `basename $0` dest_ip dest_port audio_file payload_number"
	exit $E_OPTIONERROR
fi

# Get directory path of current script
current_path=`dirname $0`

#Start Playing an audio file through gstreamer
if [ $4 == 0 ]
then
	gst-launch -v filesrc location="$3" ! wavparse ! audioconvert ! audioresample ! mulawenc ! rtppcmupay ! udpsink port=$2 host=$1
elif [ $4 == 3 ]
then
	gst-launch -v filesrc location="$3" ! wavparse ! audioconvert ! audioresample ! msgsmenc ! msgsmpayloader pt=3 ! udpsink port=$2 host=$1
else
	echo "Unsupported Payload Number"
	exit $E_OPTIONERROR
fi

# Resume Sipp client
$current_path/resume_sipp.sh
