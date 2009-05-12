#!/bin/bash

NO_ARGS=3
E_OPTIONERROR=65

if [ $# -ne "$NO_ARGS" ]
then
	echo "Usage: `basename $0` in_file out_file sampling_rate"
	exit $E_OPTIONERROR
fi

in_file=$1
out_file=$2
sampling_rate=$3

gst-launch -v filesrc location=$in_file ! wavparse ! audioresample ! audioconvert ! audio/x-raw-int, rate=$sampling_rate,  width=\(int\)16, depth=\(int\)16 ,channels=\(int\)1 ! wavenc ! filesink location=$out_file
