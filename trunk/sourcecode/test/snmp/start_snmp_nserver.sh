#!/bin/bash
# Script for running snmp server

echo "Which Server do you want to start?"
echo "1) test_snmp_nserver1"
echo "2) test_snmp_nserver2"
read Server_Select

if [ $Server_Select -eq 1 ]
then
	./test_snmp_nserver1 $1 $2 $3 $4
elif [ $Server_Select -eq 2 ]
then
	./test_snmp_nserver2 $1 $2 $3 $4
else
	echo "Invalid Server"
fi
