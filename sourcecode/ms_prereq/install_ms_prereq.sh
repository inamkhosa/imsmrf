#! /bin/sh
# Script for Installing the Prerequisite Libraries Required for Media Server

CurrentDir=`pwd`

cd `dirname $0`

#Install AMPS Library
cp -r include/amps /usr/include
cp lib/libamps.so /usr/lib

#Install Sipstack Library
cp -r include/sipstack /usr/include
cp lib/libsipstack.so /usr/lib

#Install DNS Resolver Library
cp lib/libDNSResolver.so /usr/lib

#Install Util Library
cp pkgconfig/libutil-0.3.pc /usr/local/lib/pkgconfig
cp -r include/libutil-0.3 /usr/local/include
cp lib/libutil-0.3/libutil*.* /usr/local/lib
chmod 644 /usr/local/lib/libutil.a
ranlib /usr/local/lib/libutil.a

#Install SDP Parser Library
cp pkgconfig/libsdpparser-0.5.pc /usr/local/lib/pkgconfig
cp -r include/libsdpparser-0.5 /usr/local/include
cp lib/libsdpparser-0.5/libsdpparser*.* /usr/local/lib
chmod 644 /usr/local/lib/libsdpparser.a
ranlib /usr/local/lib/libsdpparser.a

#Install SDP Offer Answer Model Library
cp pkgconfig/libsdp_oam-0.0.pc /usr/local/lib/pkgconfig
cp -r include/libsdp_oam-0.0 /usr/local/include
cp lib/libsdp_oam-0.0/libsdp_oam*.* /usr/local/lib
chmod 644 /usr/local/lib/libsdp_oam.a
ranlib /usr/local/lib/libsdp_oam.a

PATH="$PATH:/sbin" ldconfig -n /usr/local/lib

cd $CurrentDir
