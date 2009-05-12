gst-launch filesrc blocksize=160 location=~/sounds/1.wav ! wavparse ! mulawenc ! rtppcmupay ! udpsink port=8000
