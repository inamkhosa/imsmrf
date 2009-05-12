killall -2 gst-launch-0.10
sleep 2
killall gst-launch-0.10
rm -f *.wav
gst-launch -v udpsrc port=6002 ! rtppcmudepay ! mulawdec ! wavenc ! filesink location=6002.wav&
gst-launch -v udpsrc port=7002 ! rtppcmudepay ! mulawdec ! wavenc ! filesink location=7002.wav&
gst-launch -v udpsrc port=8002 ! rtppcmudepay ! mulawdec ! wavenc ! filesink location=8002.wav&

#gst-launch -v udpsrc port=6002 ! 'audio/x-mulaw, rate=(int)8000, channels=(int)1'  ! mulawdec ! wavenc ! filesink location=6002.wav&
#gst-launch -v udpsrc port=7002 ! 'audio/x-mulaw, rate=(int)8000, channels=(int)1'  ! mulawdec ! wavenc ! filesink location=7002.wav&
#gst-launch -v udpsrc port=8002 ! 'audio/x-mulaw, rate=(int)8000, channels=(int)1'  ! mulawdec ! wavenc ! filesink location=8002.wav&

