#!/bin/bash

testarray[100]=
testcounter="1"
testchoice=
csvfiles=
xmlfiles=
xmlarray[100]=
xmlchoice=
counterxml="0"
csvarray[100]=
csvcounter="1"
#========================================================================================================================
function Remove_Unncessary_Files() # Removes unnecessary files 
#========================================================================================================================
# Any number of Args as keywords of filenames to be deleted
{
for j in $@
do
rm -rf `ls * | grep "$j"` > /dev/null 2>&1
done
} 


#========================================================================================================================
function Mark_Time() # Marks the end of the Automated testing in the result output file 
#========================================================================================================================
#No Args
#Output gets written to the result file with csvarray[$csvchoice],xmlarray[$xmlchoice] and date
{
echo -e "					AUTOMATED TEST $1 AT $(date)" >> results.txt
}

#========================================================================================================================
function Mark_File() # Marks the end of the Automated testing in the result output file 
#========================================================================================================================
#Arg_1 filetype
#Arg_2 filename
#Output gets written to the result file
{
echo -e "$1 FILE: $2 \t \c" >> results.txt

}

#========================================================================================================================
function Mark_Nextline() # Marks the end of the Automated testing in the result output file 
#========================================================================================================================
#Arg_1 filetype
#Arg_2 filename
#Output gets written to the result file
{
echo -e "\n" >> results.txt

}

#========================================================================================================================
function Mark_Dashes() # Marks the end of the Automated testing in the result output file 
#========================================================================================================================
#No Args
#Output gets written to the result file with csvarray[$csvchoice],xmlarray[$xmlchoice] and date
{


echo -e "=================================================================================================================================" >> results.txt

}


#========================================================================================================================
function Mark_Asteriks() # Marks the end of the Automated testing in the result output file 
#========================================================================================================================
#No Args
#Output gets written to the result file with csvarray[$csvchoice],xmlarray[$xmlchoice] and date
{

echo -e "*********************************************************************************************************************************" >> results.txt

}


#========================================================================================================================
function Calculate_Absolute_FilePath() # Calculates the absolute path of file
#========================================================================================================================
# Arg_1 = filename
# Output gets stored in global array allabsolutefilenames
{
currentpath=`pwd`
absolutepath="$sound_dir/$baseuri"
allabsolutefilenames[$allabsolutefilecounter]="$absolutepath/$1"

cd $currentpath
allabsolutefilecounter=`expr $allabsolutefilecounter + 1`
} 




#========================================================================================================================
function Check_File_Exists() # Checks if the file exists 
#========================================================================================================================
# Arg_1 = absolute filename
#Output gets stored in a global boolean variable fileexists
{
if [ $# -ne 1 ]
then
    echo "Usage - $0  file-name"
    exit 1
fi

if [ -f $1 ]
then
    fileexists="true"
    

else

    fileexists="false"
fi
} 

#========================================================================================================================
function Convert() # Converts the input values to ms
#========================================================================================================================
# Arg_1 = input value
#Output gets stored in a global variable converted value
{


convertedvalue="$1"
ms=`echo $1 | awk '{ print index( $1 , "ms" ) }'`



if [ "$ms" -gt "0" ]
then


convertedvalue=`echo $1 | awk '{ split($1,a,"ms"); print a[1] }'`

else
s=`echo $1 | awk '{ print index( $1 , "s" ) }'`

if [ "$s" -gt "0" ]
then
convertedvalue=`echo $1 | awk '{ split($1,a,"s"); print (a[1]*1000) }'`
fi


fi


} 




#========================================================================================================================
function Get_Client_Input_Prompt() # Gets input from Client 
#========================================================================================================================
# Any number of Args as name of parameters to be input by the user 
#Output gets stored in the variable input by the user
{
durationcheck="false"


if [ $testrun == 0 ]
then
echo "* Enter the name of the csv-file you want to use :"
read csvfilename

echo "* Enter the sound_dir value in config.xml file of the Media Server currently running:"
read sound_dir
fi

for j in $@
do

if [ $j == filename ]
then
Get_Input_Audio

if [ $validabsolutefilecounter == 0 ]
then
echo " Error: No File to Play!"
break
fi



elif [ $j == duration ]
           then		 
		echo "* Enter the $j value please :"
                read durationrequest
		durationcheck="true"
		mscmlstring="$mscmlstring $j=\"$durationrequest\"" 
		
		Convert $durationrequest
		durationrequest="$convertedvalue"
		echo $durationrequest
		
		

else
		echo "* Enter the $j value please :"
		read $j

		
		
if [ $j != baseuri ]
then
		myval='$'
		myval="$myval$j"
		myval1=`eval echo $myval`
		mscmlstring="$mscmlstring $j=\"$myval1\"" 
		
		if [ $j == delay ]
		then
		Convert $myval1
		delay="$convertedvalue"
		#echo $delay
		fi

		if [ $j == offset ]
		then
		Convert $myval1
		offset="$convertedvalue"
		#echo $offset
		fi
		

else
		myval='$'
		myval="$myval$j"
		myval1=`eval echo $myval`
		baseuristring=" baseurl=\"$myval1/\""
fi
		
fi

done
} 

#========================================================================================================================
function Check_Type() # Checks if type is numeric/string 
#========================================================================================================================
# Arg_1 = String or number whose type needs to be checked
#Output gets stored in a global variable checktype
{
case $1 in *[!0-9]*) 

checktype="string"
	;;

esac

case $1 in *[0-9]*) 

checktype="numeric"
	;;

esac

} 

#========================================================================================================================
function Get_Input_Audio() # Gets input for audio files from Client 
#========================================================================================================================
# No Args
#Output get stored in a global array validabsolutefilenames
{

while [ $playfile = "yes" ]
do

echo "* Enter the audio url please :"
read filename1

Calculate_Absolute_FilePath $filename1

Check_File_Exists ${allabsolutefilenames[$allabsolutefilecounter-1]}



if [ $fileexists == "true" ]
then
#echo myfile exists
filename[$filecounter]="$filename1"
filecounter=`expr $filecounter + 1`
validabsolutefilenames[$validabsolutefilecounter]="${allabsolutefilenames[$allabsolutefilecounter-1]}"
validabsolutefilecounter=`expr $validabsolutefilecounter + 1`
else

echo " Error!!! The file does not exist!"

fi

echo "-> Do you want to play more files [yes/no] :"
read playfile

done
} 

#========================================================================================================================
function Write_to_CSV_SEQUENTIAL() # Lets u choose csv file from the current directory 
#========================================================================================================================
#Arg_1=csvfilename
#Output is a global boolean seq_check.If true SEQUENTIAL gets written to the csvfile
{

seq_check="false"
#csvfiles=`ls | grep csv`
csvfile=`ls | grep $1`


if [ $testrun == 0 ] 
then
seq_check="true"
fi
if [ -z $csvfile ]
then
seq_check="true"
else
seq_check="false"
fi

if [ $seq_check == "true" ] 
then
echo SEQUENTIAL >> $1
fi

} 


#========================================================================================================================
function Choose_XML() # Lets u choose xml file from the current directory
#========================================================================================================================
#No Args
#Output gets stored in global array xmlarray[$counterxml]
{
#csvfiles=`ls | grep csv`
xmlfiles=`ls | grep xml`
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
function Choose_CSV() # Lets u choose csv file from the current directory 
#========================================================================================================================
#No Args
#Output gets stored in global array csvarray[$csvchoice];numlines(No of lines in csvfile)
{
#csvfiles=`ls | grep csv`
csvfiles=`ls | grep .csv`
echo -e "CSV files in Current Directory: \n "
echo "====================================================================="
for j in $csvfiles
do
countercsv=`expr $countercsv + 1`

echo -e "[$countercsv] $j"
csvarray[$countercsv]="$j"
done
echo "====================================================================="
echo -e "Enter the CSV file of your choice [1-$countercsv]: \n "
read csvchoice
numlines=`wc -l ${csvarray[$csvchoice]} | awk '{print $1}'`
numlines=`expr $numlines - 1`

}  



#========================================================================================================================
function Choose_XML_from_file() # Lets u choose xml file from the current directory
#========================================================================================================================
#No Args
#Output gets stored in global array xmlarray[$counterxml]
{
relationfile="$1"

while read line
do


mycsv=`echo $line | grep ${csvarray[$csvchoice]} | grep -v grep | awk '{print $0}'`
mylen1=`echo $mycsv | wc -c`
if [ "$mylen1" -gt "1" ]
then
myxml=`echo $mycsv | sed -n -e "s/${csvarray[$csvchoice]}//" -e "p"`
break
fi
done < $relationfile 

xmlfiles="$myxml"
echo -e "XML files for this CSV: \n "
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
function Run_Test_Case() # Run the test case via sipP client 
#========================================================================================================================
#No Args
{



#Mark_Start_Result
Mark_Time 'STARTED'
Mark_Dashes
Mark_File XML ${xmlarray[$xmlchoice]}
Mark_File CSV ${csvarray[$csvchoice]}
Mark_Nextline



../../run_sipp_client.sh -inf ${csvarray[$csvchoice]} -m $numlines -l 1 -sf ${xmlarray[$xmlchoice]} -trace_logs

sleep 10
Mark_Time 'ENDED'
Mark_Asteriks
#Mark_End_Result
} 

#===============================================================================================================================================
function Select_Attribute_Prompt() # lets user select attributes to be tested
#===============================================================================================================================================
#No Args
#Output is stored in a global variable attributelist
{
echo -e " \n"
echo -e " PROMPT PHASE \n" 
echo "====================================================================="
echo -e "[1] Baseuri \n[2] Delay  \n[3] Duration \n[4] Repeat \n[5] Offset \n[6] Default/Continue \n "
echo "====================================================================="
echo -e "Enter the variable(s) of your choice for a single test run [1-6]: \n "

while [ "$attribute" != 6 ]
do

read attribute

case "$attribute" in
2)
attributelist="$attributelist delay";  
;;

3)
attributelist="$attributelist duration"; 
 ;;

4)
attributelist="$attributelist repeat";  
;;

5)
attributelist="$attributelist offset"; 
 ;;

6)

break ;;

1)
attributelist=`echo $attributelist | sed 's/filename//g'`;
attributelist="$attributelist baseuri filename"; 
 ;;

*)
echo "Opps!!! Please select choice 1,2,3,4,5 or 6";

esac
echo -e "Enter your next choice >>"
done

} 

#========================================================================================================================
function Compute_File_Duration() # Adds up fileduration of all files
#========================================================================================================================
#No Arg
#Output gets stored in a global variable fileduration
{
for (( i = 0; i < validabsolutefilecounter; i++ ))
do
   
   duration1=`sndfile-info ${validabsolutefilenames[$i]} | grep Duration | awk '{print $3}'`
   fileduration1=`echo $duration1 | awk '{ split($1,a,":"); print (a[1]*60*60*1000)+(a[2]*60*1000)+(a[3]*1000) }'`
   fileduration=`expr $fileduration + $fileduration1`   
	
done

if [ "$offset" -gt "$fileduration" ]
then
offset="0"
fi
} 

#===============================================================================================================================================
function Duration_Not_Entered() # Computes expected result assuming duration is not input by the user
#===============================================================================================================================================
#No Args
#Output gets stored in global variables playduration,filerecorded,playoffset
{
Check_Type $repeat

if [ $checktype == "numeric" ]
then


filerecorded=`expr $fileduration \* $repeat`
no_of_delays=`expr $repeat - 1`



if [ "$repeat" -gt "0" ]
then
playduration=`expr $fileduration \* $repeat + $delay \* $no_of_delays`
playduration=`expr $playduration - $offset`
filerecorded=`expr $filerecorded - $offset`
playoffset="$fileduration"

else

filerecorded="0"
playduration="$filerecorded"
playoffset="$playduration"

fi

fi # $checktype == "numeric"




if [ $repeat == "infinite" ] || [ $repeat == "INFINITE" ]
then

playduration="max"
filerecorded="max"
playoffset="max"

fi # [ $repeat == "infinite" ] || [ $repeat == "INFINITE" ]


} 



#===============================================================================================================================================
function Duration_Entered_String() # Computes expected result assuming duration entered is a string value
#===============================================================================================================================================
#No Args
#Output gets stored in global variables playduration,filerecorded,playoffset
{
if [ $durationrequest == "infinite" ] || [ $durationrequest == "INFINITE" ]
then
Duration_Not_Entered
fi

if [ $durationrequest == "immediate" ] || [ $durationrequest == "IMMEDIATE" ]
then
fileduration="0"
filerecorded="0"
playduration="0"
playoffset="0"
fi

}

#===============================================================================================================================================
function Duration_Entered_Numeric() # Computes expected result assuming duration is input by the user and the value is numeric
#===============================================================================================================================================
#No Args
#Output gets stored in global variables playduration,filerecorded,playoffset
{

Check_Type $repeat

if [ $checktype == "numeric" ]
then



if [ "$repeat" -gt "0" ]
then
durationrequest=`expr $durationrequest + $offset`
filerecorded=`expr $fileduration \* $repeat`
no_of_delays=`expr $repeat - 1`
playduration=`expr $fileduration \* $repeat + $delay \* $no_of_delays`
totalplayduration=`expr $fileduration \* $repeat + $delay \* $no_of_delays`
#durationwithdelay=`expr $fileduration + $delay`
durationwithdelay=`expr $fileduration + $delay`
remainder=`expr $durationrequest % $durationwithdelay`
repeatsinplay=`expr $durationrequest / $fileduration`


if test $totalplayduration -ge $durationrequest
then



if test $remainder -le $fileduration
then

playoffset="$remainder"
fi


if test $remainder -gt $fileduration
then

playoffset="0"
fi

filerecorded1=`expr $fileduration \* $repeatsinplay`
filerecorded2=`expr $filerecorded1 + $playoffset`
filerecorded="$filerecorded2"
playduration="$durationrequest"

fi #test $remainder -le $fileduration

if test $totalplayduration -lt $durationrequest
then


playduration="$totalplayduration"

if test $repeat -eq 1
then

playoffset="$fileduration"

else

playoffset="0"
fi

filerecorded=`expr $fileduration \* $repeat`

fi #$totalplayduration -lt $durationrequest

playduration=`expr $playduration - $offset`
filerecorded=`expr $filerecorded - $offset`

else

playduration="0"
filerecorded="0"
playoffset="0"

fi #$repeat" -gt "0"
fi # repeat == "numeric"


if [ $repeat == "infinite" ] || [ $repeat == "INFINITE" ]
then
durationrequest=`expr $durationrequest + $offset`

if test $durationrequest -gt $fileduration
then

playduration="$durationrequest"
durationwithdelay=`expr $fileduration + $delay`
repeatsinplay=`expr $durationrequest / $fileduration`

no_of_delays=`expr $repeatsinplay - 1`

#fileduration=`expr $durationrequest - $delay \* $no_of_delays`
remainder=`expr $durationrequest % $durationwithdelay`


if test $remainder -le $fileduration
then

playoffset="$remainder"
fi


if test $remainder -gt $fileduration
then

playoffset="0"
fi

filerecorded1=`expr $fileduration \* $repeatsinplay`
filerecorded2=`expr $filerecorded1 + $playoffset`
filerecorded="$filerecorded2"
playduration="$durationrequest"

fi # $durationrequest -gt $fileduration


if test $durationrequest -le $fileduration
then

playduration="$durationrequest"
filerecorded="$durationrequest"

fi #$durationrequest -le $fileduration

playduration=`expr $playduration - $offset`
filerecorded=`expr $filerecorded - $offset`

fi # [ $repeat == "infinite" ] || [ $repeat == "INFINITE" ]

} 


#===============================================================================================================================================
function Calculate_Expected_Result() # Calculates expected result 
#===============================================================================================================================================
#No Args
#Output gets stored in global variables playduration,filerecorded,playoffset 
{

Compute_File_Duration

if [ $durationcheck == "false" ]
then

Duration_Not_Entered

fi # $durationcheck == "false" 


if [ $durationcheck == "true" ]
then

Check_Type $durationrequest



if [ $checktype == "numeric" ]
then


Duration_Entered_Numeric
fi #duration is numeric



if [ $checktype == "string" ]
then

Duration_Entered_String
fi #duration is numeric

fi #$durationcheck == "true"



} #end of function 

#========================================================================================================================
function Execute_Options() # lets choose the mode to execute the test
#========================================================================================================================
{


clear
echo "****************************** EXECUTE TESTCASE *************************"
echo -e "[1] Individual Test  \n[2] Batch Test \n[3] Exit"
echo "====================================================================="
echo -e "Enter your choice [1-3]: \n "

read exemenuchoice

case "$exemenuchoice" in
1)
Choose_CSV;
Choose_XML_from_file csv_xml_relation;
Run_Test_Case;   
break;
;;

2)
GroupTest 'Test_Groups';
Run_Batch 'csv_xml_relation';

break; 
 ;;

3)
echo bbyes;  
;;

*)
echo "Opps!!! Please select choice 1,2 or 3";

esac

} 


#========================================================================================================================
function GroupTest() # Reads test group values from file and Lets u choose 
#========================================================================================================================
#Arg_1 Filename of Groups
{
relationfile1="$1"

echo -e "Test Groups: \n "
echo "====================================================================="

while read line1
do

test1=`echo $line1 | awk '{ split($1,b," "); print (b[1])}'`
testarray[$testcounter]="$test1"
echo -e "[$testcounter] $test1"
testcounter=`expr $testcounter + 1`
done < $relationfile1 
echo "====================================================================="
echo -e "Enter the Test Group of your choice [1-$testcounter]: \n "
read testchoice

while read line
do
mycsv=`echo $line | grep ${testarray[$testchoice]} | grep -v grep | awk '{print $0}'`
mylen1=`echo $mycsv | wc -c`
if [ "$mylen1" -gt "1" ]
then
csvfiles=`echo $mycsv | sed -n -e "s/${testarray[$testchoice]}//" -e "p"`
break
fi
done < $relationfile1

}

#========================================================================================================================
function Wait_Process_Ends() # Waits until sipp and gst-launch processes end 
#========================================================================================================================
#No Args

{
gst_process_id=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`
gstlen=`echo $gst_process_id | wc -c`


while [ "$gstlen" -gt "1" ] > /dev/null 2>&1; 
do 

sleep 1; 
gst_process_id=`ps -ef | grep gst-launch | grep -v grep | awk '{print $2}'`
gstlen=`echo $gst_process_id | wc -c`

done


sipp_process_id1=`ps -ef | grep "sipp " | grep -v grep | awk '{print $2}'`
sipp_process=`ps -ef | grep "sipp " | grep -v grep | awk '{print $0}'`
sipplen=`echo $sipp_process_id1 | wc -c`



if [ "$sipplen" -gt "1" ]
then

for j in $sipp_process_id1
do
kill -2 $j > /dev/null 2>&1;

break
done
fi





while [ "$sipplen" -gt "1" ] > /dev/null 2>&1; 
do 

sleep 1; 

sipp_process_id1=`ps -ef | grep "sipp " | grep -v grep | awk '{print $2}'`
sipp_process=`ps -ef | grep "sipp " | grep -v grep | awk '{print $0}'`
sipplen=`echo $sipp_process_id1 | wc -c`
done
  
}

#========================================================================================================================
function Run_Batch() # Run the test case in group 
#========================================================================================================================
#Arg_1 The relation file name
{
Mark_Asteriks
Mark_Time 'STARTED'
relationfile="$1"
for l in $csvfiles
do
csvarray[$csvcounter]="$l"
echo -e "[$testcounter] $test1"

numlines=`wc -l $l | awk '{print $1}'`
numlines=`expr $numlines - 1`
Mark_Dashes
Mark_File CSV $l
while read line
do
#echo -e "$line \n" 
mycsv=`echo $line | grep $l | grep -v grep | awk '{print $0}'`
mylen1=`echo $mycsv | wc -c`
if [ "$mylen1" -gt "1" ]
then
myxml=`echo $mycsv | sed -n -e "s/$l//" -e "p"`
break
fi
done < $relationfile
xmlfiles="$myxml"


for j in $xmlfiles
do
counterxml=`expr $counterxml + 1`
xmlarray[$counterxml]="$j"
Mark_Nextline
Mark_Dashes
Mark_File XML ${xmlarray[$counterxml]}
Mark_Nextline


../../run_sipp_client.sh -sf $j -inf ${csvarray[$csvcounter]} -m $numlines -l 1 

Wait_Process_Ends

done



csvcounter=`expr $csvcounter + 1`

done

sleep 10
Mark_Time 'ENDED'
Mark_Asteriks

} 

