#!/bin/bash

if [ ! `which doxygen`  ]; then
  echo "doxygen not found"
  exit -1  
fi

if [ ! `which dot`  ]; then
  echo "dot utility not found"
  exit -1  
fi

DOXYGEN_VER=`doxygen --version`

DOT_VER=`dot -V 2>&1 | awk '{print $5}'`

#Checking the version of doxygen
if [ ${DOXYGEN_VER} != "1.5.6" ] ; then
	echo "Doxygen configuration file is for version 1.5.6, current version is ${DOXYGEN_VER}"
	exit -1
fi

#Recommended version 2.14.1
REC_VER="2.14.1"

#Checking the version of dot utililty
if [ "${DOT_VER}" \< "${REC_VER}" ] ; then
	echo "update your dot utility, current version is ${DOT_VER}"
	exit -1
fi

CONFIG_FILE="./conf/Doxyfile"

old_path=`pwd`

script_path=`dirname $0`
cd $script_path

cd images
jpg_files=`ls | grep jpg`
for nIndx in $jpg_files
do
	pdf_file=`ls $nIndx | cut -d. -f1`.pdf
	convert -page Letter $nIndx $pdf_file
done

cd ..

rm -rf doc

# Added starting and ending comments so that it can be processed by doxygen
if [ -f ChangeLog ]
then
	sed -i '1i\/*! @page ChangeLog Change Log' ChangeLog
	sed -i '$a\*/' ChangeLog
fi

echo "Generating HTML and Latex"

doxygen $CONFIG_FILE >& doxygen_log.txt

if [ $? -ne "0"  ]
then
	echo "Error generating documenation"
	exit
fi


if [ ! -d ./doc ]
then
	echo "Documentation dirctory does not exist"
	exit
fi

cd doc/latex

echo "Generating PDF"

#Doxygen does not wrap up the function lines in a pdf file when its generated
#we have written this script to proply align and format those cut line

#This sed command inserts new line in refman.tex and save
#save the results in the existing file

sed -i '2{p;s/.*/\\setlength{\\textheight}{6in}        \
\\setlength{\\topmargin}{-0.15in}    \
\\setlength{\\oddsidemargin}{-0.7in} \
\\setlength{\\evensidemargin}{-0.7in} \
\\\setlength{\\headsep}{0.25in} /;}' refman.tex


#sed script to replace 'a4paper' type format to 'landscape in 
#input file refman.tex

sed -e 's/a4paper/landscape/' -i refman.tex

make >& make_log.txt

cp -f refman.pdf ../../ms-refman.pdf

cd $old_path




