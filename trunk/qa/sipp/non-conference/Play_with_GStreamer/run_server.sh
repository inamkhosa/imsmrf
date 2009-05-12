 
#!/bin/sh


path1=`cat path`

$path1/sipp -sf $1 -trace_err -trace_logs
  

exit 0
