#!/bin/bash

NO_ARGS=2
E_OPTIONERROR=65

if [ $# -ne "$NO_ARGS" ]
then
	echo "Usage: `basename $0` sound_in_dir sound_out_dir"
	exit $E_OPTIONERROR
fi

sound_in_dir=$1
sound_out_dir=$2

for file in `ls $sound_in_dir/*.wav`
do
	filename=`basename $file`
	echo Converting $file to $sound_out_dir/$filename
	./convert_rate.sh $file $sound_out_dir/$filename 8000
	if [ $? -ne 0 ]
	then
		echo "Error in converting $file to $sound_out_dir/$filename"
		break
	fi
done
