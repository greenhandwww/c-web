#include "headall.h"

// 获取html资源路径
int InitHOME(char * HtmlHome)
{
	char *pTmp = getenv("HTMLHOME");
	if( pTmp == NULL )
	{
		ERRLOG( ERR,"HTMLHOME is NULL");
		return -1;
	}

	strcpy( HtmlHome, pTmp );

	return 1;
}


// 返回 sock 套接字
int InitTCP()
{
	struct sockaddr_in serv;
	int sock;
	int ret;
	int iPort;
	int on = 1;
	char port[6];

	memset( port, 	0, sizeof( port ));
	memset( &serv,	0, sizeof( serv ));

	sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( sock < 0 )
	{
		ERRLOG( ERR, "socket err!" );
		return -1;
	}

	ret = XmlGetNetwork( "WebService", NULL, NULL, port );
	if( ret < 0 )
	{
		ERRLOG( ERR,"XmlGetNetwork err! ");
		return -1;
	}

	iPort = atoi( port );

	serv.sin_family = AF_INET;
	serv.sin_port = htons( iPort );
	serv.sin_addr.s_addr = htonl( INADDR_ANY );

	if( setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof( on ) ) < 0)
	{
		ERRLOG( ERR,"setsockopt err !");
		return -1;
	}

	if( 0 > bind( sock, (struct sockaddr *)&serv, sizeof(serv) ))
	{
		ERRLOG( ERR,"bind err!" );
		return -1;
	}

	if( 0 > listen( sock, SOMAXCONN ) )
	{
		ERRLOG( ERR, "listen err!" );
		return -1;
	}

	return sock;

}

int InitDemon()
{
	pid_t pid;
	char tmp[40];
	if((pid = fork()) < 0)
	{
		ERRLOG( ERR, "fork err !");
		exit (-1);
	}
	else if(pid > 0)
		exit (-1);//kill parent process

	if( 0 > setsid())
	{
		ERRLOG( ERR, " setsid err!");
		exit (-1);
	}
	if((pid = fork()) > 0){
		exit (0);
	}
	else if(pid < 0){
		exit (-1);
	}

	chdir( getenv("HOME") );
	umask(0);

	return 0;
}

void KillSig()
{
	int i;
	for( i = 0;i < 32;i++)
	{
		if(i == SIGKILL || i == SIGCHLD || i == SIGHUP)
			continue;
		signal( i ,SIG_IGN); 
	}
}

void CLOSEFD( int num )
{
	int status;
	ERRLOG( WAR, "接收到退出指令，客户端程序终止" );
	while( waitpid(-1, &status, WNOHANG) > 0);
}
