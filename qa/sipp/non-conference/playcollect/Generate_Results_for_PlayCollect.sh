#!/bin/bash


Attribute="$1"
ExpectedValue="$2"
ActualValue="$3"
Status="$4"
Reason="$5"


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
function Write_Result() # Writes result to file 
#===============================================================================================================================================

{
echo -e "Attribute           :$Attribute" >> results.txt
echo -e "Actual Value        :$ActualValue" >> results.txt
echo -e "Expected Value      :$ExpectedValue" >> results.txt
echo -e "Status              :$Status" >> results.txt
echo -e "Reason              :$Reason" >> results.txt        
echo -e "---------------------------------------------------------------------------------------------------------------------------------" >> results.txt 

}


Write_Result
../.././resume_sipp.sh