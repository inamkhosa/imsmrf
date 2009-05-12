#!/bin/sh


source ../../Common_Functions.sh

duration_record="false"; 
sound_dir=
baseuri=
baseuristring=
filename=
convertedvalue=
durationrequest=
recordfile=
reclength=
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
attributelist_record=
attribute=
attributerecord=
csvstring=
audiostring=
mscmlstring=
recordstring=
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
function Write_To_Csv_Playrecord() # writes the values to csv file
#========================================================================================================================
# Arg_1 = csv filename
#Output is cvsstring written to csvfile
{
csvstring="$baseuristring"



for (( k = 0; k < filecounter; k++ ))
do
   
   audiostring="$audiostring <audio url=\"${filename[$k]}\"/>"
done

csvstring="$csvstring;$audiostring;$mscmlstring;$recordstring;$recordfile;$filerecorded;$playduration;$playoffset;$fileduration;$reclength;$recurl;$recduration"

echo $csvstring >> $1
} 



#========================================================================================================================
function Select_Attribute_Record() # lets user select attributes to be tested
#========================================================================================================================
#No Args
#Output is stored in a global variable attributelist_record
{
echo -e " \n"
echo -e " RECORD PHASE \n" 
echo "====================================================================="
echo -e " [1] Recurl \n [2] Recencoding \n [3] Initsilence \n [4] Endsilence \n [5] Barge \n [6] Cleardigits \n [7] Escapekey \n [8] Beep \n [9] Mode \n[10] Recstopmask \n[11] Duration \n[12] Continue \n "
echo "====================================================================="
echo -e "Enter the variable(s) of your choice for a single test run [1-17]: \n "

while [ "$attribute_record" != 12 ]
do

read attribute_record

case "$attribute_record" in
1)
attributelist_record="$attributelist_record recurl"; 
;;

2)
attributelist_record="$attributelist_record recencoding"; 
;;

3)
attributelist_record="$attributelist_record initsilence"; 
;;

4)
attributelist_record="$attributelist_record endsilence"; 
;;

5)
attributelist_record="$attributelist_record barge"; 
;;

6)
attributelist_record="$attributelist_record cleardigits"; 
;;

7)
attributelist_record="$attributelist_record escapekey"; 
;;

8)
attributelist_record="$attributelist_record beep"; 
;;

11)
attributelist_record="$attributelist_record duration";
;;

9)
attributelist_record="$attributelist_record mode"; 
;;

12)
break;
;;

10)
attributelist_record="$attributelist_record recstopmask"; 
;;

*)
echo "Opps!!! Please select the right choice";
;;
esac
echo -e "Enter your next choice >>"
done
} 


#========================================================================================================================
function Get_RecordFile() # Gets input for audio files from Client 
#========================================================================================================================
# No Args
#Output get stored in a global variable recordfile
{
rfile="yes"
while [ $rfile = "yes" ]
do

echo "* Enter the file to be recorded please :"
read recordfile

Check_File_Exists $recordfile


if [ $fileexists == "true" ]
then

rfile="no"

else

echo " Error!!! The file does not exist!"
rfile="yes"

fi
done
} 

#========================================================================================================================
function Get_Client_Input_Record() # Gets input from Client 
#========================================================================================================================
# Any number of Args as name of parameters to be input by the user 
#Output gets stored in the variable input by the user
{
Get_RecordFile
for j in $@
do
		if [ $j == escapekey ]
           	then		 
		echo "* Enter the $j value please :"
                read escapekey
		recordstring="$recordstring $j=\"$escapekey\"" 

elif [ $j == duration ]
           then		 
		echo "* Enter the $j value please :"
                read recordduration
		
		recordstring="$recordstring $j=\"$recordduration\"" 
		duration_record="true"
		Convert $recordduration
		recordduration="$convertedvalue"		
		
else

		echo "* Enter the $j value please :"
		read $j
		myval='$'
		myval="$myval$j"
		myval1=`eval echo $myval`
		recordstring="$recordstring $j=\"$myval1\"" 	
fi	

done

} 

#========================================================================================================================
function Calculate_Expected_Reclength() # Gets input from Client 
#========================================================================================================================
# No Args
#Output gets stored in the variable reclength
{
reclength=`sndfile-info $recordfile | grep Length | awk '{print $3}'`
} 


#========================================================================================================================
function Calculate_Expected_Recduration() # Gets input from Client 
#========================================================================================================================
# No Args
#Output gets stored in the variable recduration
{
recduration=`sndfile-info $recordfile | grep Duration | awk '{print $3}'`
recduration=`echo $recduration | awk '{ split($1,a,":"); print (a[1]*60*60*1000)+(a[2]*60*1000)+(a[3]*1000) }'`
} 



#========================================================================================================================
function RecordDuration_Not_Entered() # Computes expected result assuming duration is not input by the user
#========================================================================================================================
#No Args
#Output gets stored in global variables playduration,filerecorded,playoffset
{
Calculate_Expected_Recduration
Calculate_Expected_Reclength
} 

#========================================================================================================================
function RecordDuration_Entered_String() # Computes expected result assuming duration entered is a string value
#========================================================================================================================
#No Args
#Output gets stored in global variables playduration,filerecorded,playoffset
{
if [ $recordduration == "infinite" ] || [ $recordduration == "INFINITE" ]
then
RecordDuration_Not_Entered
fi

if [ $recordduration == "immediate" ] || [ $recordduration == "IMMEDIATE" ]
then
recduration="0"
reclength="0"
fi

}


#========================================================================================================================
function RecordDuration_Entered_Numeric() # Computes expected result assuming duration is input by the user and the value is numeric
#========================================================================================================================
#No Args
#Output gets stored in global variables playduration,filerecorded,playoffset
{
Calculate_Expected_Recduration



if test $recduration -le $recordduration
then

Calculate_Expected_Recduration
Calculate_Expected_Reclength


else

recduration="$recordduration"
reclength=`expr $recordduration \* 16`

fi #$durationrequest -le $fileduration

} 


#========================================================================================================================
function Calculate_Expected_Result_Record() # Calculates expected result 
#========================================================================================================================
#No Args
#Output gets stored in global variables  
{


if [ $duration_record == "false" ]
then

RecordDuration_Not_Entered

fi # $durationcheck == "false" 


if [ $duration_record == "true" ]
then

Check_Type $recordduration



if [ $checktype == "numeric" ]
then


RecordDuration_Entered_Numeric
fi #duration is numeric



if [ $checktype == "string" ]
then

RecordDuration_Entered_String
fi #duration is numeric

fi #$durationcheck == "true"


if [ $initsilence == "immediate" ] || [ $initsilence == "IMMEDIATE" ]
then
recduration="0"
reclength="0"
fi
} #end of function 



#========================================================================================================================
function Initialize_Globals_Playrecord() # Initialize global variables
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
attributelist_record=""
attribute_record=
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




duration_record="false"; 
sound_dir=" "
baseuri=" "
baseuristring=" "
filename=" "
convertedvalue=" "
durationrequest=" "
recordfile=" "
reclength=" "
fileduration="0"
offset="0"
repeat="1"
delay="0"
filecounter="0"
absolutepath=
playfile="yes"
filename[100]=" "
allabsolutefilenames[100]=" "
allabsolutefilecounter="0"
validabsolutefilenames[100]=" "
validabsolutefilecounter="0"
fileexists="true"
directoryexists="true"
checktype=" "
durationcheck="false"
runtestcase="yes"
attributelist="filename"
attributelist_record=" "
attribute=" "
attributerecord=" "
csvstring=" "
audiostring=" "
mscmlstring=" "
recordstring=" "
testrun="0"
Len=" "
fileexistcounter="0"
medias=" "
mediaspath=" "
countercsv="0"
csvfiles=" "
csvarray[100]=" "
csvchoice=" "
numlines=" "
counterxml="0"
xmlfiles=" "
xmlarray[100]=" "
xmlchoice=" "
menuchoice=" "

for (( i = 0; i <= 100; i++ ))
do
   validabsolutefilenames[$i]="0"
   
done

} 

 




#========================================================================================================================
function Playrecordtest() #Main function that lets the user to select attributes to be tested and provide input
#========================================================================================================================
{
Remove_Unncessary_Files testing log results .sh~ .csv~ .xml~
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
Initialize_Globals_Playrecord
echo -e "\n"

Select_Attribute_Prompt

Select_Attribute_Record

clear
echo "---------------------------------------------------------------------"
echo "			        Client Input Section 		           "
echo -e "---------------------------------------------------------------------\n"
echo -e " PROMPT PHASE"
echo -e "====================================================================="
Get_Client_Input_Prompt $attributelist
echo -e "\n====================================================================="
echo -e " RECORD PHASE"
echo -e "====================================================================="
Get_Client_Input_Record $attributelist_record
 
Write_to_CSV_SEQUENTIAL $csvfilename
 
if [ $validabsolutefilecounter -gt "0" ]
then
echo Calculating Expected Result.Please Wait...
Calculate_Expected_Result
Calculate_Expected_Reclength
Calculate_Expected_Recduration
Calculate_Expected_Result_Record
Write_To_Csv_Playrecord $csvfilename
echo The Expected Values have been written to $csvfilename
fi

testrun=`expr $testrun + 1`
echo -e "Do you want to execute more test runs [yes/no] ?"
read runtestcase

done
} 

#===============================================================================================================================================
function Main_Playrecord() # lets user select attributes to be tested
#===============================================================================================================================================
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
Playrecordtest;  
;;

2)
Execute_Options;
#echo "****************************** EXECUTE TESTCASE *************************";
#Choose_CSV;
#Choose_XML_from_file csv_xml_relation;

#Run_Test_Case; 
#break; 
 ;;

3)
echo bbyes;  
;;

*)
echo "Opps!!! Please select choice 1,2 or 3";

esac
done
} 

Main_Playrecord