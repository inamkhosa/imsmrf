#!/bin/bash
# Script for running snmp client

NO_ARGS=4
E_OPTIONERROR=65

if [ $# -ne "$NO_ARGS" ]
then
	echo "Usage: `basename $0` Server_Ip Server_Port Oid Value"
	exit $E_OPTIONERROR
fi

# Get value of given oid
echo ./test_snmp_get1 $1 $2 $3
Result=`./test_snmp_get1 $1 $2 $3 | grep "<RESPONSE>"`
ErrorCode=`expr "$Result" : '.*\(<ERRORCODE>[0-9]*</ERRORCODE>\)' | cut -d'>' -f2 | cut -d'<' -f1`
if [ $ErrorCode -eq "200" ]
then
	echo Obtained Value: `expr "$Result" : '.*\(<VALUE>[0-9A-Za-z_]*</VALUE>\)' | cut -d'>' -f2 | cut -d'<' -f1`
else
	echo Error: `expr "$Result" : '.*\(<ERRORDESC>[0-9A-Za-z_ ]*</ERRORDESC>\)' | cut -d'>' -f2 | cut -d'<' -f1`
fi
echo

# Set value of given oid
echo ./test_snmp_set1 $1 $2 $3 $4
Result=`./test_snmp_set1 $1 $2 $3 $4 | grep "<RESPONSE>"`
ErrorCode=`expr "$Result" : '.*\(<ERRORCODE>[0-9]*</ERRORCODE>\)' | cut -d'>' -f2 | cut -d'<' -f1`
if [ $ErrorCode -eq 200 ]
then
	echo Set Value: `expr "$Result" : '.*\(<VALUE>[0-9A-Za-z_]*</VALUE>\)' | cut -d'>' -f2 | cut -d'<' -f1`
else
	echo Error: `expr "$Result" : '.*\(<ERRORDESC>[0-9A-Za-z_ ]*</ERRORDESC>\)' | cut -d'>' -f2 | cut -d'<' -f1`
fi
echo

# Get value of given oid
echo ./test_snmp_get1 $1 $2 $3
Result=`./test_snmp_get1 $1 $2 $3 | grep "<RESPONSE>"`
ErrorCode=`expr "$Result" : '.*\(<ERRORCODE>[0-9]*</ERRORCODE>\)' | cut -d'>' -f2 | cut -d'<' -f1`
if [ $ErrorCode -eq 200 ]
then
	echo Obtained Value: `expr "$Result" : '.*\(<VALUE>[0-9A-Za-z_]*</VALUE>\)' | cut -d'>' -f2 | cut -d'<' -f1`
else
	echo Error: `expr "$Result" : '.*\(<ERRORDESC>[0-9A-Za-z_ ]*</ERRORDESC>\)' | cut -d'>' -f2 | cut -d'<' -f1`
fi
echo

exit 0
