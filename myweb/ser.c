#include "headall.h"

char pHtmlHome[20] ;

void CLOSEFD( int num );

int main()
{	
	int iSock;
	int iCon ;
	struct sockaddr_in clie;
	int clielen ;
	int ret;
	
	InitDemon();
	signal( SIGCHLD, CLOSEFD);  /* 信号驱动解决僵尸进程 */
	Initlog("Myweb");

	memset( pHtmlHome, 0, sizeof( pHtmlHome ));

	iSock = InitTCP();
	if( iSock < 0 )
	{
		ERRLOG( ERR, "InitTCP err!");
		return -1;
	}

	ret = InitHOME( pHtmlHome );
	if( ret < 0 )
	{
		ERRLOG( ERR, "InitHOME err");
		return -1;
	}
	while( 1 )
	{
		iCon = accept( iSock, (struct sockaddr *)&clie, &clielen );
		if( iCon < 0 )
			continue;
		else
		{
			ret = fork();
			if( ret < 0 )
				return ;
			else if( ret == 0 )
			{
				close( iSock );
				ERRLOG(WAR,"new client !");
				ERRLOG(WAR,"连接客户端[%s]",inet_ntoa( clie.sin_addr ));
				ERRLOG(WAR,"连接端口[%d]",ntohs( clie.sin_port ));
				ret = Operate( iCon );
				if( ret < 0 )
					clienterror( iCon, "Try again", "501", "页面访问失败", "Service does not get resource");
				return 0;
			}
		}
		close( iCon );
	}
}
