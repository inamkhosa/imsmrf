#!/bin/bash



playdur=`echo $1 | awk '{ split($1,b,"."); print (b[1])}'`
upperbound=`echo $3 | awk '{ split($1,b,"."); print (b[1])}'`
lowerbound=`echo $4 | awk '{ split($1,b,"."); print (b[1])}'`
filedur=`echo $5 | awk '{ split($1,b,"."); print (b[1])}'`
duration=`sndfile-info testing.wav | grep Duration | awk '{print $3}'`
time=`echo $duration | awk '{ split($1,a,":"); print (a[1]*60*60*1000)+(a[2]*60*1000)+(a[3]*1000) }'`


if [ $2 = "PASS" ] 
then
echo The Play Duration \in MSCML response $playdur ms is in the expected range \( $lowerbound to $upperbound \)ms >> results.txt



if [ $time -eq $filedur ] ; then
        
echo The Duration of the recorded \file matches the Expected File Duration $filedur ms >> results.txt
echo Hence the Test has passed >> results.txt

       else
            echo The Duration of the recorded \file does not match the Expected File Duration >> results.txt
		echo The Duration of the recorded \file is $time ms where as the Expected File Duration is $filedur ms >> results.txt
		echo Hence the Test has Failed >> results.txt
       fi


else
echo The Test has failed as the Play Duration \in MSCML response is outside the expected range  >> results.txt
fi


