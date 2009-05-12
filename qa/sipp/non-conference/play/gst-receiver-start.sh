#!/bin/bash


./resume_sipp.sh


gst-launch -v udpsrc port=$1 ! rtppcmudepay ! mulawdec ! wavenc ! filesink location=$2 &











