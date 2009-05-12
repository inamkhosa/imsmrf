 
#!/bin/sh

path=`cat path`


for i in $*
do
  
  $path/sipp -trace_err -trace_msg -trace_logs -inf play_info.csv -sf $i -m 1 127.0.0.1
  
sleep 10

if test $? -ne 0
    then
        echo Code $? returned by sipp client.Test $i has failed >> sipp-returncode.txt
    else
        echo Code 0 returned by sipp client.Test $i has passed >> sipp-returncode.txt
    fi
done
exit 0

