#!/bin/bash

lowerlimit=
upperlimit=
status="PASS"
playoffset_actual=
playoffset_expected=
fileduration=


#===============================================================================================================================================
function Wait_Process_Ends() # Waits for gst-launch process to end
#===============================================================================================================================================
{
gst_process_id1=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`

 while [ `ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'` > 0 ]
do
  gst_process_id1=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`
done
}


#===============================================================================================================================================
function Calculate_Lower_Limit() # Calculates lower limit 
#===============================================================================================================================================
# Arg_1= Expected playoffset
{
lowerlimit=`expr $1 - 100`

if [ "$lowerlimit" -lt "0" ] 
then
lowerlimit="0"
fi
}

#===============================================================================================================================================
function Calculate_Upper_Limit() # Calculates upper limit 
#===============================================================================================================================================
# Arg_1= Expected playoffset
{
upperlimit=`expr $1 + 100`
}

#===============================================================================================================================================
function Check_Range() # Calculates upper limit 
#===============================================================================================================================================
# Arg_1= Expected playoffset
# Arg_1= Actual playoffset
{
Calculate_Lower_Limit $1
Calculate_Upper_Limit $1


if [ "$2" -le "$upperlimit" ] && [ "$2" -ge "$lowerlimit" ]
then
status="PASS"
else
status="FAIL"
fi

}

#===============================================================================================================================================
function Calculate_Result() # Calculates upper limit 
#===============================================================================================================================================
# Arg_1= Expected playoffset
# Arg_2= Fileduration
# Arg_3= Actual playoffset
{
if [ $1 == 0 ] || [ $1 == $2 ]
then

Check_Range 0 $3
firstcheck="$status"
Check_Range $2 $3
secondcheck="$status"

#echo l $lowerlimit >> results.txt
#echo u $upperlimit >> results.txt

if [ $firstcheck == PASS ]
then
Check_Range 0 $3
fi

if [ $secondcheck == PASS ]
then
Check_Range $2 $3
fi

if [ $firstcheck == PASS ] || [ $secondcheck == PASS ]
then
status="PASS"


else

status="FAIL"

fi

else #[ $1 == 0 ] || [ $1 == $2 ]

Check_Range $1 $3

fi #[ $1 == 0 ] || [ $1 == $2 ]

}


#===============================================================================================================================================
function Write_Result() # Writes result to file 
#===============================================================================================================================================

{
echo -e "Attribute           :Playoffset from Response" >> results.txt
echo -e "Actual Value        :$playoffset_actual ms" >> results.txt
echo -e "Expected Value      :$lowerlimit to $upperlimit  ms" >> results.txt


if [ $status = "PASS" ] 
then        

echo -e "Status              :PASS" >> results.txt
echo -e "Reason              :Playoffset is in the expected range" >> results.txt 

       else
            echo -e "Status              :FAIL" >> results.txt
            echo -e "Reason              :Playoffset is not in the expected range" >> results.txt 
  
fi
echo -e "---------------------------------------------------------------------------------------------------------------------------------" >> results.txt 

}


playoffset_actual="$2"
playoffset_expected="$1"
fileduration="$3"



Calculate_Result $playoffset_expected $fileduration $playoffset_actual
Write_Result
../.././resume_sipp.sh