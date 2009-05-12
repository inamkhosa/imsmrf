#!/bin/bash

NO_ARGS=0
E_OPTIONERROR=65

if [ $# -eq "$NO_ARGS" ]
then
	echo "Usage: `basename $0` [options] [sipp_options]"
	echo
	echo "Available options:"
	echo
	echo "  -B base_uri :"
	echo "     Default value is the absolute path of sound folder in the folder containing this script."
	echo "     Base URI to be used in to get path of audio files"
	echo
	echo "  -D destination_ip :"
	echo "     Default value is the IP obtained through selected local interface."
	echo "     IP of the remote system."
	echo
	echo "  -I interface :"
	echo "     Default value = eth0."
	echo "     Interface to be used for getting the IP of the local system being used."
	echo
	exit $E_OPTIONERROR
fi

#MY_INTERFACE="wlan0"

MY_INTERFACE="eth0"
MEDIA_PORT=8000
NUM_ITERATION=1
NUM_CALL=0

while getopts ":B:D:I:" Option
do
	case $Option in
		B) arg=`expr substr $OPTARG 1 1`;
		if [ $arg == "-" ]
		then
			echo "Option -$Option requires an argument";
			exit $E_OPTIONERROR;
		else
			BASE_URI=$OPTARG;
		fi;;
		D ) arg=`expr substr $OPTARG 1 1`;
		if [ $arg == "-" ]
		then
			echo "Option -$Option requires an argument";
			exit $E_OPTIONERROR;
		else
			DEST_IP=$OPTARG;
		fi;;
		I ) arg=`expr substr $OPTARG 1 1`;
		if [ $arg == "-" ]
		then
			echo "Option -$Option requires an argument";
			exit $E_OPTIONERROR;
		else 
			MY_INTERFACE=$OPTARG;
		fi;;
		: ) echo "Option -$OPTARG requires an argument";
		exit $E_OPTIONERROR;;
	esac
done

shift $(($OPTIND - 2))

CMD1+="$@";

MY_IP=`/sbin/ifconfig  $MY_INTERFACE | grep 'inet addr:'| grep -v '127.0.0.1' | cut -d: -f2 | awk '{ print $1}'`

if [ -z $DEST_IP ]
then
	DEST_IP=$MY_IP
fi

CRNT_PATH=`dirname $0`
ABS_PATH="`cd \"$CRNT_PATH\" 2>/dev/null && pwd || echo \"$CRNT_PATH\"`"
if [ -z $BASE_URI ]
then
	BASE_URI="`echo $ABS_PATH`/sound"
fi

SIPP_CMD="sipp $DEST_IP -i $MY_IP $CMD1 -key script_path $ABS_PATH/ -key base_uri $BASE_URI/"

echo $SIPP_CMD

$SIPP_CMD
