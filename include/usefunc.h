#include "printlog.h"
#include <unistd.h>

#define once
#define SUCC 0
#define FAIL -1

int InitFork()
{
	char * home = getenv( "HOME" );
	pid_t pid;

	if((pid = fork()) < 0)
	{
		errLOG( ERR, "fork err !");
		return (-1);
	}
	else if(pid > 0){
		return (-1);//kill parent process
	}

	if( 0 > setsid())
	{
		errLOG( ERR, " setsid err!");
		return (-1);
	}

	if((pid = fork()) > 0){
		return (0);
	}
	else if(pid < 0){
		return (-1);
	}

	chdir( home );

	umask(0);

	return 0;
}
