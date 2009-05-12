#!/bin/bash

E_OPTIONERROR=65

if [ $# -lt 2 -o $# -gt 4 ]
then
	echo "Usage: `basename $0` version release [svn_tag] [configure_options]"
	echo "Example: ./make_distribution.sh 0.0.0 1 RELEASE_0_0_0 \"--enable-opt=8 --enable-ms-trace --enable-debug=no\""
	exit $E_OPTIONERROR
fi

if [ $USER != "root" ]
then
	echo "This script must be run with root privilages"
	exit -1
fi

if [ ! -f configure.ac ]
then
	echo "configure.ac not found"
	exit -1
fi

if [ ! -f rpmmacros ]
then
	echo "rpmmacros not found"
	exit -1
fi

#check if code is checked-in 
if [ "M" == `svnversion | sed 's/[0-9:]*\([M]\).*/\1/'` ]
then
	echo "Code is not checked in. Please checkin the code"
	exit -1
fi

./uncrustify.sh .

PACKAGE=`awk -F'=' '/PACKAGE=/{print $2}' configure.ac`
VERSION=$1
RELEASE=$2
SVN_TAG=$3
CONFIGURE_OPT="--datarootdir=/etc $4"

if [ ! -f $PACKAGE.spec.in ]
then
	echo "$PACKAGE.spec.in not found"
	exit -1
fi


PID=$!
RPM_DIR=`grep %_topdir rpmmacros | cut -d' ' -f2`

#get major, minor, micro version
MAJOR_VERSION=`echo $VERSION | awk -F'.' '{print $1}'`
MINOR_VERSION=`echo $VERSION | awk -F'.' '{print $2}'`
MICRO_VERSION=`echo $VERSION | awk -F'.' '{print $3}'`

echo "Modifying version in configure.ac"
sed -i 's/\(AC_INIT(\[ms\],\[\)\([0-9.]*\)\(\])\)/\1'$VERSION'\3/' configure.ac
sed -i 's/^MAJOR_VERSION=.*/MAJOR_VERSION='$MAJOR_VERSION'/' configure.ac
sed -i 's/^MINOR_VERSION=.*/MINOR_VERSION='$MINOR_VERSION'/' configure.ac
sed -i 's/^MICRO_VERSION=.*/MICRO_VERSION='$MICRO_VERSION'/' configure.ac

if [ "x$SVN_TAG" != "x" ]
then
	echo "Modifying vc_tag in rpmmacros: $SVN_TAG"
	sed -i 's/^%vc_tag.*/%vc_tag '"$SVN_TAG"'/' rpmmacros
fi

echo "Modifying configure_opt in rpmmacros: $CONFIGURE_OPT"
CONFIGURE_OPT_ESCAPED=`echo $CONFIGURE_OPT | sed -e 's/\//\\\\\//g'`
sed -i 's/^%configure_opt.*/%configure_opt '"$CONFIGURE_OPT_ESCAPED"'/' rpmmacros

echo "Modifying $PACKAGE.spec.in"
sed -i 's/%define release.*/%define release '$RELEASE'/' $PACKAGE.spec.in
sed -i 's/\(.*configure --prefix=%{_prefix}\).*/\1 '"$CONFIGURE_OPT_ESCAPED"'/' $PACKAGE.spec.in

if [ `which svn2cl` ]; then
	echo "Generating ChangeLog for svn logs"
	svn2cl -o ChangeLog
fi

./autogen.sh $CONFIGURE_OPT
if [ $? -ne "0"  ]
then
	echo "autogen.sh failed"
	exit -1
fi

make dist

if [ $? -ne "0"  ]
then
	echo "make dist failed"
	exit -1
fi


if [ ! -f $PACKAGE-$VERSION.tar.gz ]
then
	echo "$PACKAGE-$VERSION.tar.gz not found"
	exit -1
fi

if [ -f $HOME/.rpmmacros ]
then
	mv $HOME/.rpmmacros $HOME/.rpmmacros.$PID
fi

cp -f rpmmacros $HOME/.rpmmacros

mkdir -p $RPM_DIR/BUILD $RPM_DIR/RPMS $RPM_DIR/SOURCES $RPM_DIR/SRPMS $RPM_DIR/SPECS

rpmbuild --ta $PACKAGE-$VERSION.tar.gz

if [ $? -ne "0"  ]
then
	echo "Error in RPM build"
	exit -1
fi
echo "RPM Build complete"

if [ -f $HOME/.rpmmacros.$PID ]
then
	mv $HOME/.rpmmacros.$PID $HOME/.rpmmacros
fi


