#!/bin/bash
# Script for calling dtmfsend utility and then resuming sipp

# Get directory path of current script
current_path=`dirname $0`

# Call Dtmfsend Utility
dtmfsend $1 $2 "$3" $4 $5

# Resume Sipp client
$current_path/resume_sipp.sh
