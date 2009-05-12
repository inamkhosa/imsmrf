#!/bin/bash

ls -l | sort -k 8 | awk '{if(match($0,"^d.*")){print $8;}}'| awk '{printf("-I$(TOP)/include/%s ",$0);}'
