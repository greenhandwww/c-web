#include "headall.h"

extern char pHtmlHome[20];	// ���ñ���

void GetType(char *filename, char *filetype)
{
	if (strstr(filename, ".html"))
		strcpy(filetype, "text/html");
	else if (strstr(filename, ".gif"))
		strcpy(filetype, "image/gif");
	else if (strstr(filename, ".png"))
		strcpy(filetype, "image/png");
	else if (strstr(filename, ".jpg"))
		strcpy(filetype, "image/jpeg");
	else if (strstr(filename, ".css"))
		strcpy(filetype, "text/css");
	else if (strstr(filename, ".ico"))
		strcpy(filetype, "image/ico");
	else
		strcpy(filetype, "text/plain");
}


int WriteHead( int fd, char * route, int filesize )
{
	char type[20];
	char buf[512];

	memset( buf, 0, sizeof( buf ));
	memset( type, 0, sizeof( type ));
	GetType( route, type ); 
	sprintf(buf, "HTTP/1.0 200 OK\r\n"); 
	sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
	sprintf(buf, "%sConnection: close\r\n", buf);
	sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
	sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, type);
	
	return Write(fd, buf, strlen(buf)); 
}

// ���·���Ϸ���
int JudgeRoute( char * route )
{
	int i;
	int ret = 0;
	char sTmp[200];
	char sFileType[ 4 ][6] = { "jpg","html","css"};
	
	memset( sTmp, 0, sizeof( sTmp ));

	if( strstr( route, "..") != NULL )
	{
		ERRLOG(ERR,"�����·���Ƿ�!");
		return -1;
	}
	for(i = 0;i < 10;i++ )
	{
		if( strstr( route, sFileType[i] ) != NULL )
		{
			ret = 1;
			break;
		}
	}

	if( ret == 0 )
		return -1;
	if( strcmp( route, "/") == 0 )
		strcpy( sTmp, "/index.html");
	else
		strcpy( sTmp,  route );
	sprintf( route,"%s%s",pHtmlHome, sTmp );
	return 1;
}

// ����html�ļ�
int SendHtmlFile( int fd, char * route )
{
	int ret;
	int ifd;
	char *pHtmlFile;
	struct stat Fileinf;
	
	stat( route, &Fileinf);		//��ȡ�ļ�����
	ERRLOG( WAR,"%s size[%d]",route, Fileinf.st_size );

	ifd = open( route, O_RDONLY, 0 );
	if( ifd < 0 )
	{
		ERRLOG( ERR, "open [%s] err ",route );
		return -1;
	}
	// ӳ���ļ����ڴ�
	pHtmlFile = (char *)mmap( NULL, Fileinf.st_size, PROT_READ, MAP_PRIVATE,  
					ifd, 0);
	if( pHtmlFile == NULL )
	{
		ERRLOG( ERR, "mmap err [%s] can load",route);
		close( ifd );
		return -1;
	}
	close( ifd );
	// д�뱨��ͷ�ͱ�����
	WriteHead( fd, route, Fileinf.st_size );
	Write( fd, pHtmlFile, Fileinf.st_size );
	ret = munmap((void *)pHtmlFile, Fileinf.st_size );
	if( ret < 0 )
	{
		ERRLOG( ERR,"[%s] munmap err �ͷ��ļ��ڴ����!", route );
		return -1;
	}
	return 1;
}

// ����
int Operate(int fd)
{
	int ret;
	char sGetbuf[1024];		// �������ȡ����
	char sMeth[10]; 		// ��������󷽷�
	char sRoute[200];		// �����������Դ
	int iFileSize;			// �����ļ���С
	int n;
	char stype[20];
	
	memset( stype , 0, sizeof( stype ));
	memset( sRoute, 0, sizeof( sRoute ));
	memset( sMeth, 0, sizeof( sMeth ));
	memset( sGetbuf, 0, sizeof( sGetbuf ));
	
	//��ȡ��ҳ��������
	n = read( fd, sGetbuf, sizeof( sGetbuf ));
	ERRLOG( WAR, "��ȡ[%d]��ҳ����[%s]", n, sGetbuf);
	//�����ҳ����ķ�����������Դ
	GetResq( sGetbuf, sMeth, sRoute, sizeof( sRoute ) );
	if( strlen( sMeth ) == 0 || strlen( sRoute ) == 0 )
	{
		ERRLOG( ERR,"Method[%s] or Route[%s] is NULL",sMeth,sRoute);
		return -1;
	}
	ERRLOG( WAR, "sMeth[%s]sRoute[%s]",sMeth,sRoute);
	// �������󷽷�Ϊ post ��ʼ��������
	if( strcasecmp( "POST", sMeth) == 0 )
	{
		ParseData( sGetbuf );	
	}
	/*
	if( strcasecmp( "GET", sMeth ) != 0 )
	{
		ERRLOG( ERR,"����У�鲻��ȷ!");
		clienterror(fd, sMeth, "501", "Not Implemented", "Tiny does not implement this method");
		return -1;
	}*/
	// ��ȡ��ҳ����
	GetType( sRoute, stype );
	// У��������Դ�Ϸ��Լ���Դ�ض���
	ret = JudgeRoute( sRoute );
	if( ret < 0 )
	{
		ERRLOG( ERR, "JudgeRoute err!");
		return -1;
	}

	//����ҳ����Դ
	if( 0 > SendHtmlFile( fd, sRoute ))
		return -1;	
	
	return 1;

}

// ��ȡ��������󷽷�������ҳ��
int GetResq(char *buf, char * method, char * route, int maxsize )
{
	int i;
	int count = 0;
	int pos = 0;
	for(i = 0;i < maxsize;++i )
	{
		if( buf[i] == ' ' && count == 0)
		{
			memcpy( method, buf, i );
			pos = i + 1;
			count++;
		}
		else if( buf[i] == ' ' && count == 1 )
		{
			memcpy( route, buf + pos, i - pos);
			pos = i + 1;
			count++;
		}
		if( count == 2 )
			break;
	}
	if( count < 2)
		return -1;
	
	return 1;
}


int Read(int fd, void *usrbuf, int n)
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;

	while (nleft > 0) 
	{
		if ( ( nread = read( fd, bufp, nleft) ) < 0) 
		{
			if (errno == EINTR) /* Interrupted by sig handler return */
				nread = 0;      /* and call read() again */
			else
				return -1;      /* errno set by read() */
		}
		else if (nread == 0)
			break;              /* EOF */
		nleft -= nread;
		bufp += nread;
	}
	return (n - (int)nleft);         /* return >= 0 */
}

int Write(int fd, void *usrbuf, int n)
{
	int nleft = n;
	int nwritten;
	char *bufp = usrbuf;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, bufp, nleft)) <= 0)
		{
			if (errno == EINTR)  /* Interrupted by sig handler return */
				nwritten = 0;    /* and call write() again */
			else
				return -1;       /* errno set by write() */
		}
		nleft -= nwritten;
		bufp += nwritten;
	}
	return n;
}
