#!/bin/bash

gst-launch filesrc location=~/sounds/10.gsm blocksize=33 ! 'audio/x-msgsm, rate=(int)8000, channels=(int)1' ! msgsmpayloader pt=3  ! udpsink port=9000 host=127.0.0.1
