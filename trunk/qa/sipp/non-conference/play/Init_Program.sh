#!/bin/bash


source ../../Common_Functions.sh
#Global Variables
mediaserver="ms_main"
sound_dir=
baseuri=
baseuristring=
filename=
convertedvalue=
durationrequest=
fileduration="0"
offset="0"
repeat="1"
delay="0"
filecounter="0"
absolutepath=
playfile="yes"
filename[100]=
allabsolutefilenames[100]=
allabsolutefilecounter="0"
validabsolutefilenames[100]=
validabsolutefilecounter="0"
fileexists="true"
directoryexists="true"
checktype=
durationcheck="false"
runtestcase="yes"
attributelist="filename"
attribute=
csvstring=
audiostring=
mscmlstring=
testrun="0"
Len=
fileexistcounter="0"
medias=
mediaspath=
countercsv="0"
csvfiles=
csvarray[100]=
csvchoice=
numlines=
counterxml="0"
xmlfiles=
xmlarray[100]=
xmlchoice=
menuchoice=




#========================================================================================================================
function Initialize_Globals() # Initialize global variables
#========================================================================================================================
#No Args
#All global variables get initialized
{
baseuri=""
filename=" "
absolutepath=" "
fileduration="0"
durationrequest="0"
offset="0"
repeat="1"
delay="0"
filecounter="0"
playfile="yes"
filename[100]=" "
allabsolutefilenames[100]=" "
allabsolutefilecounter="0"
validabsolutefilenames[100]=" "
validabsolutefilecounter="0"
fileexists="true"
checktype="0"
durationcheck="false"
runtestcase="yes"
attributelist="filename"
attribute=" "
audiostring=" "
mscmlstring=" "
Len="0"
fileexistcounter="0"
medias=" "
mediaspath=" "
countercsv="0"
csvfiles=" "
csvarray[100]=" "
csvchoice="0"
numlines="0"
counterxml="0"
xmlfiles=" "
xmlarray[100]=" "
xmlchoice="0"
menuchoice="0"
baseuristring=" "

for (( i = 0; i <= 100; i++ ))
do
   validabsolutefilenames[$i]="0"
   
done

} 

#========================================================================================================================
function Write_To_Csv() # writes the values to csv file
#========================================================================================================================
# Arg_1 = csv filename
#Output is cvsstring written to csvfile
{
csvstring="$baseuristring"



for (( k = 0; k < filecounter; k++ ))
do
   
   audiostring="$audiostring <audio url=\"${filename[$k]}\"/>"
done

csvstring="$csvstring;$audiostring;$filerecorded;$playduration;$playoffset;$mscmlstring;$fileduration"

echo $csvstring >> $1
} 



#========================================================================================================================
function Test_Main() #Main function that lets the user to select attributes to be tested and provide input
#========================================================================================================================
{
Remove_Unncessary_Files testing log results .sh~ .csv~ .xml~ 0
runtestcase="yes"
testrun="0"


while [ $runtestcase = "yes" ]
do
clear

echo "****************************** NEW TESTCASE *************************"
echo -e "\n"
echo "---------------------------------------------------------------------"
echo "			    Attribute Selection Section 		   "
echo "---------------------------------------------------------------------"
Initialize_Globals
echo -e "\n"

Select_Attribute_Prompt

echo "---------------------------------------------------------------------"
echo "			        Client Input Section 		           "
echo "---------------------------------------------------------------------"
Get_Client_Input_Prompt $attributelist
 
Write_to_CSV_SEQUENTIAL $csvfilename
 
if [ $validabsolutefilecounter -gt "0" ]
then
echo Calculating Expected Result.Please Wait...
Calculate_Expected_Result
Write_To_Csv $csvfilename
echo The Expected Values have been written to $csvfilename
fi

testrun=`expr $testrun + 1`
echo -e "Do you want to execute more test runs [yes/no] ?"
read runtestcase

done
} 


#========================================================================================================================
function Main() # lets user select attributes to be tested
#========================================================================================================================
{

while [ "$menuchoice" != 3 ]
do
Remove_Unncessary_Files testing log results .sh~ .csv~ .xml~ 0
clear
echo "****************************** MAIN MENU *****************************"
echo -e "[1] Provide Input for New Testcase  \n[2] Execute Testcase \n[3] Exit"
echo "====================================================================="
echo -e "Enter your choice [1-3]: \n "

read menuchoice

case "$menuchoice" in
1)
Test_Main;  
;;

2)

Execute_Options;

 ;;

3)
echo bbyes;  
;;

*)
echo "Opps!!! Please select choice 1,2 or 3";

esac
done
} 


Main


