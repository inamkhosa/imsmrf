#!/bin/bash


gst_process_id1=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`

 while [ `ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'` > 0 ]
do
  gst_process_id1=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`
done


playdur=`echo $1 | awk '{ split($1,b,"."); print (b[1])}'`
upperbound=`echo $3 | awk '{ split($1,b,"."); print (b[1])}'`
lowerbound=`echo $4 | awk '{ split($1,b,"."); print (b[1])}'`
filedur=`echo $5 | awk '{ split($1,b,"."); print (b[1])}'`
duration=`sndfile-info $6 | grep Duration | awk '{print $3}'`
time=`echo $duration | awk '{ split($1,a,":"); print (a[1]*60*60*1000)+(a[2]*60*1000)+(a[3]*1000) }'`



if [ "$lowerbound" -lt "0" ] 
then
lowerbound="0"
fi


 

echo -e "Attribute           :Duration from Response" >> results.txt
echo -e "Actual Value        :$playdur ms" >> results.txt
echo -e "Expected Value      :$lowerbound to $upperbound ms" >> results.txt



if [ $2 = "PASS" ] 
then


echo -e "Status              :PASS" >> results.txt
echo -e "Reason              :Duration is in the expected range" >> results.txt 


echo -e "---------------------------------------------------------------------------------------------------------------------------------" >> results.txt

if [ $time -eq $filedur ] ; then
        


 
echo -e "Attribute           :Duration of file recorded" >> results.txt
echo -e "Actual Value        :$time ms" >> results.txt
echo -e "Expected Value      :$filedur ms" >> results.txt
echo -e "Status              :PASS" >> results.txt
echo -e "Reason              :Duration matches the expected value" >> results.txt 

echo -e "---------------------------------------------------------------------------------------------------------------------------------" >> results.txt


       else

             #echo -e "Testname            :$8" >> results.txt 
             #echo -e "Call_ID             :$7" >> results.txt
	     echo -e "Attribute           :Duration of file recorded" >> results.txt
	     echo -e "Actual Value        :$time ms" >> results.txt
	     echo -e "Expected Value      :$filedur ms" >> results.txt
	     echo -e "Status              :FAIL" >> results.txt
	     echo -e "Reason              :Duration does not match the expected value" >> results.txt 
echo -e "---------------------------------------------------------------------------------------------------------------------------------" >> results.txt
       fi




else


echo -e "Status              :FAIL" >> results.txt
echo -e "Reason              :Duration is not in the expected range" >> results.txt 
echo -e "---------------------------------------------------------------------------------------------------------------------------------" >> results.txt


fi

../.././resume_sipp.sh


