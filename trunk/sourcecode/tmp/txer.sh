#gst-launch filesrc blocksize=160 location=~/sounds/1.ulaw  ! 'audio/x-mulaw, rate=8000' ! rtppcmupay ! udpsink port=6000
#gst-launch filesrc blocksize=160 location=~/sounds/1.wav !  mulawenc ! rtppcmupay ! udpsink port=7000

#gst-launch audiotestsrc num-buffers=10 samplesperbuffer=80 ! audioresample ! 'audio/x-raw-int, rate=8000' ! mulawenc ! rtppcmupay ! udpsink port=6000 &
#gst-launch audiotestsrc num-buffers=10 samplesperbuffer=80 ! audioresample ! 'audio/x-raw-int, rate=8000' ! mulawenc ! rtppcmupay ! udpsink port=7000 

#gst-launch audiotestsrc num-buffers=10 samplesperbuffer=80 ! mulawenc ! rtppcmupay ! udpsink port=6000 &
#gst-launch audiotestsrc num-buffers=10 samplesperbuffer=80 ! mulawenc ! rtppcmupay ! udpsink port=7000 

#gst-launch -v filesrc location=$1 ! wavparse ! filesink location=temp.raw
#gst-launch -v filesrc location=$1 ! wavparse ! audioconvert ! 'audio/x-raw-int, depth=(int)8, width=(int)8' ! filesink location=temp.raw
#gst-launch -v filesrc blocksize=160 location=temp.raw ! 'audio/x-raw-int, endianness=(int)1234, channels=(int)1, width=(int)16, depth=(int)16, signed=(boolean)true, rate=(int)8000' ! mulawenc ! rtppcmupay ! udpsink port=6002

gst-launch -v filesrc location=$1 ! wavparse ! mulawenc !  filesink location=temp.raw
gst-launch -v filesrc blocksize=80 location=temp.raw ! 'audio/x-mulaw, rate=(int)8000, channels=(int)1' ! rtppcmupay ! udpsink port=6000&
