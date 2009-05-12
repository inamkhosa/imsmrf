#!/usr/bin/awk
{
	if( $1 == "AMPS_API" )
	{
		func_idx = 0
		split($0,line)
		for( fs = 2; fs < NF; fs++)
		{

			if( substr(line[fs],1,5) == "AMPS_" )
			{
				#print line[fs]
				func_idx = fs
				break
			}	
		}
		if( func_idx > 0 )
		{
			split(line[fs],name,"(")
			#print name[1]
			func_name = name[1]
			sub("AMPS_","FW_",func_name)
			#print name[1]

		}
		#print "function index = ", func_idx
		print "#define",func_name,name[1]
	}
	else
	{
		print
	}
}
