#!/bin/bash

lowerlimit=
upperlimit=
status="PASS"
recurl="$1"
#reclength_expected=`echo $2 | awk '{ split($1,b,"."); print (b[1])}'`
reclength_actual=
reclength_response="$2"
recduration_actual="$4"
recduration_expected=`echo $3 | awk '{ split($1,b,"."); print (b[1])}'`
recduration_filerecorded=
recduration_expected_response=`echo $5 | awk '{ split($1,b,"."); print (b[1])}'`
#========================================================================================================================
function Calculate_Reclength() # Gets input from Client 
#===============================================================================================================================================
# Any number of Args as name of parameters to be input by the user 
#Output gets stored in the variable input by the user
{
reclength_actual=`sndfile-info $recurl | grep Length | awk '{print $3}'`
} 

#===============================================================================================================================================
function Calculate_Recduration() # Gets input from Client 
#===============================================================================================================================================
# Any number of Args as name of parameters to be input by the user 
#Output gets stored in the variable input by the user
{
recduration_filerecorded=`sndfile-info $recurl | grep Duration | awk '{print $3}'`
recduration_filerecorded=`echo $recduration_filerecorded | awk '{ split($1,a,":"); print (a[1]*60*60*1000)+(a[2]*60*1000)+(a[3]*1000) }'`
} 

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
function Write_Result() # Writes result to file 
#===============================================================================================================================================

{
echo -e "Attribute           :$1" >> results.txt
echo -e "Actual Value        :$2 $3" >> results.txt
echo -e "Expected Value      :$lowerlimit to $upperlimit  $3" >> results.txt


if [ $status = "PASS" ] 
then        

echo -e "Status              :PASS" >> results.txt
echo -e "Reason              :$1 is in the expected range" >> results.txt 

       else
            echo -e "Status              :FAIL" >> results.txt
            echo -e "Reason              :$1 is not in the expected range" >> results.txt 
  
fi
echo -e "---------------------------------------------------------------------------------------------------------------------------------" >> results.txt 

}
sleep 10

Calculate_Reclength

Check_Range $reclength_response $reclength_actual 

Write_Result 'reclength' $reclength_actual 'bytes'

#Check_Range $reclength_expected $reclength_actual

#Write_Result 'reclength of recorded file' $reclength_actual 'bytes'

Check_Range $recduration_expected_response $recduration_actual

Write_Result 'recduration from response' $recduration_actual 'ms'

Calculate_Recduration
Check_Range $recduration_expected $recduration_filerecorded
Write_Result 'recduration of recorded file' $recduration_filerecorded 'ms'

../.././resume_sipp.sh