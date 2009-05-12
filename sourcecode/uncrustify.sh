#!/bin/bash

#Checking if the library is installed
if [ ! `which uncrustify`  ]; then
  echo "uncrustify not found"
  exit -1  
fi

TOP=$1
TMP=./tmp

#Checking if 'tmp' directory exists
if [ ! -d ${TMP}  ]; then
  echo "'tmp' directory not found"
  echo "Please create a 'tmp' directory in main directory"
  exit -1  
fi

UNCRUSTIFY_VER=`uncrustify -v | awk '{print $2}'`

#Checking the version of uncrustify
if [ ${UNCRUSTIFY_VER} != "0.47" ] ; then
	echo "The configuration file is for version 0.47, current version is ${UNCRUSTIFY_VER}"
	exit -1
fi


echo "Beautifying Code in .c and .h files"
find ${TOP} -name "*.[ch]" > ${TMP}/files.txt

uncrustify -c ./conf/uncrustify.cfg -F ${TMP}/files.txt --no-backup

echo "Beautifying Code in .cpp files"

find ${TOP} -name "*.cpp" > ${TMP}/files.txt
uncrustify -l CPP -c ./conf/uncrustify.cfg -F ${TMP}/files.txt --no-backup

rm -f ${TMP}/files.txt

exit 0
