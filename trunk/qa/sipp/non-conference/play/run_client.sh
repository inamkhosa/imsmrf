 
#!/bin/sh

path=`cat path`


for i in $*
do
  
  $path/sipp -trace_err -trace_msg -trace_logs -inf play_info.csv -sf $i -l 1 -m 1 127.0.0.1 -i 127.0.0.1 
   
  
sleep 10

if test $? -ne 0
    then
        echo Code $? returned by sipp client.Test $i has failed
    else
        echo Code 0 returned by sipp client.Test $i has passed
    fi
done
exit 0

