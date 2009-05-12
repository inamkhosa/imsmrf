#!/usr/bin/awk -f
{
	if( match($0, "lib(.*).so" ) )
	{
		split($0,name,".");
		printf("-l%s ", substr(name[1],4));
	}
}

