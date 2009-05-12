#!/usr/bin/awk
BEGIN{
	ver = "VERSION";
	if(ARGC == 3)
	{
		ver = ARGV[2];
		ARGC = 2;
	}	
}
{
	if( 0 == match($0, "^$") )
	{
		split($0,line)
		split(line[2],name,"(")
		printf( "#define %s DEFINE(%s, %s)\n",name[1],name[1], ver);
	}
}
