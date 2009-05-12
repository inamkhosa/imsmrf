#!/usr/bin/awk
{
	gsub("AMPS","MS", $0)
	print $0
}
