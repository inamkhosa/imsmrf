#!/bin/bash

toupper()
{
	local char="$*"

	out=$(echo $char | tr [:lower:] [:upper:])
	local retval=$?
	echo $out
	unset out char
	return $retval
}

tolower()
{
	local char="$*"

	out=$(echo $char | tr [:upper:] [:lower:])
	local retval=$?
	echo $out
	unset out
	unset char
	return $retval
}

NO_ARGS=2
E_OPTIONERROR=65

if [ $# -ne "$NO_ARGS" ]
then
	echo "Usage: `basename $0` source_component destination_component"
	echo "../../tmp/copy_media_comp.sh G729Enc G729Dec"
	exit $E_OPTIONERROR
fi

SRC_NAME=$1
DEST_NAME=$2

SRC_NAME_L=$(tolower $SRC_NAME)
DEST_NAME_L=$(tolower $DEST_NAME)

SRC_NAME_U=$(toupper $SRC_NAME)
DEST_NAME_U=$(toupper $DEST_NAME)


if [ ! -d $SRC_NAME_L ]
then
	echo "No directory by name $SRC_NAME_L"
	exit -1
fi

if [ -d $DEST_NAME_L ]
then
	echo "Directory by name $DEST_NAME_L already exists"
	exit -1
fi

cp -r $SRC_NAME_L $DEST_NAME_L
cp -f ../../include/media/$SRC_NAME_L.h ../../include/media/$DEST_NAME_L.h

cd $DEST_NAME_L
rm *~ Makefile.in Makefile *.lo *.o *.la


#rename files
for filename in `ls *.[ch]`
do
	new_name=`echo $filename | sed 's/\('$SRC_NAME_L'\)\(_*\)/'$DEST_NAME_L'\2/'`
	mv $filename $new_name
done

ls *.[ch] ../../../include/media/$DEST_NAME_L.h Makefile.am | xargs sed -i 's/'$SRC_NAME'/'$DEST_NAME'/g'
ls *.[ch] ../../../include/media/$DEST_NAME_L.h Makefile.am | xargs sed -i 's/'$SRC_NAME_L'/'$DEST_NAME_L'/g'
ls *.[ch] ../../../include/media/$DEST_NAME_L.h Makefile.am | xargs sed -i 's/'$SRC_NAME_U'/'$DEST_NAME_U'/g'

echo "Please update configure.ac and Makefile.am and MCT_$DEST_NAME_U in include/media/media.h and src/media/media_cb.c and MCT_${DEST_NAME_U}_MAGIC_NUM"

