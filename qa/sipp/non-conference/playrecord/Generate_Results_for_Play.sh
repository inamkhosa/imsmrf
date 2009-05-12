#!/bin/bash

lowerlimit=
upperlimit=
status="PASS"
duration_actual=`echo $1 | awk '{ split($1,b,"."); print (b[1])}'`
duration_expected=`echo $2 | awk '{ split($1,b,"."); print (b[1])}'`
playoffset_actual="$3"
fileduration="$4"
filename="$5"

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


Check_Range $duration_expected $duration_actual
duration_actual=`echo $duration_actual | bc -l`


../play/./Calc_and_Generate_Results_for_Duration.sh  $duration_actual $status $upperlimit $lowerlimit $fileduration $filename
sleep 5
../play/./Calc_and_Generate_Results_for_Playoffset.sh $duration_expected $playoffset_actual $fileduration
sleep 5
../.././resume_sipp.sh