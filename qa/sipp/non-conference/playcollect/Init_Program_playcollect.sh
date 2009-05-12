#!/bin/sh

#source Collect_Init_Program.sh
source ../../Common_Functions.sh
menuchoice_playcollect="0"
xmlfiles=

xmlarray[100]=

xmlchoice=

counterxml="0"

#========================================================================================================================
function Choose_XML_Playcollect() # Lets u choose xml file from the current directory
#========================================================================================================================
#No Args
#Output gets stored in global array xmlarray[$counterxml]
{
#csvfiles=`ls | grep csv`
xmlfiles=`grep -l 'uac' *.xml`
echo -e "XML files in Current Directory: \n "
echo "====================================================================="
for j in $xmlfiles
do
counterxml=`expr $counterxml + 1`

echo -e "[$counterxml] $j"
xmlarray[$counterxml]="$j"
done
echo "====================================================================="
echo -e "Enter the XML file of your choice [1-$counterxml]: \n "
read xmlchoice
} 



#========================================================================================================================
function Run_Test_Case_Playcollect() # Run the test case via sipP client 
#========================================================================================================================
#No Args
{
Mark_Asteriks
Mark_Time 'STARTED'
Mark_Dashes
Mark_File XML ${xmlarray[$xmlchoice]}
Mark_Nextline
../../run_sipp_client.sh -sf ${xmlarray[$xmlchoice]} -m 1 -trace_err -trace_logs
Mark_Time 'ENDED'
Mark_Asteriks

} 

#========================================================================================================================
function Run_Batch_playcollect() # Run the test case via sipP client 
#========================================================================================================================
#No Args
{
xmlfiles=`grep -l 'uac' *.xml`
Mark_Asteriks
Mark_Time 'STARTED'
for j in $xmlfiles
do
counterxml=`expr $counterxml + 1`
xmlarray[$counterxml]="$j"

Mark_Dashes
Mark_File XML ${xmlarray[$counterxml]}
Mark_Nextline

../../run_sipp_client.sh -sf ${xmlarray[$counterxml]} -m 1 -l 1 -trace_err -trace_logs

Wait_Process_Ends
done
Mark_Time 'ENDED'
Mark_Asteriks


} 

#========================================================================================================================
function Menu_playcollect() # lets user select attributes to be tested
#========================================================================================================================
{

while [ "$menuchoice_playcollect" != 3 ]
do
Remove_Unncessary_Files testing log .sh~ .csv~ .xml~ 0
clear
echo "****************************** MAIN MENU *****************************"
echo -e "[1] Execute Selected Testcase  \n[2] Execute all Testcases in a batch \n[3] Exit"
echo "====================================================================="
echo -e "Enter your choice [1-3]: \n "

read menuchoice_playcollect

case "$menuchoice_playcollect" in
1)
Choose_XML_Playcollect; 
Run_Test_Case_Playcollect; 
break; 
;;

2)
echo "****************************** EXECUTE TESTCASE *************************";

Run_Batch_playcollect; 
break; 
 ;;

3)
echo bbyes;  
;;

*)
echo "Opps!!! Please select choice 1,2 or 3";

esac
done
} 

Menu_playcollect