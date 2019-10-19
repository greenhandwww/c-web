#include "headall.h"

int ParseData(char * buf )
{
	char sTmpbuf[ BUFSIZE ];
	char *p;
	int size = 0;
	int i;
	FILE * fd;
	int n;
	
	memset( sTmpbuf, 0, sizeof( sTmpbuf ));
	memcpy( sTmpbuf, buf, BUFSIZE );
	p = sTmpbuf;

	fd = fopen("/home/wbl/tmp.rec", "w");
	for(i = 0;i < strlen( sTmpbuf );i++)
	{
		
	}
	
}
