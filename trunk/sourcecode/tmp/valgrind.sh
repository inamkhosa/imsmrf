#!/bin/bash

valgrind --leak-check=full --leak-resolution=high --log-file-exactly=log.txt $1 $2 $3 $4 $5 $6 $7 $8 $9

#--show-reachable=yes 