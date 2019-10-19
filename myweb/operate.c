#include "headall.h"

extern char pHtmlHome[20];	// 引用变量

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

// 检查路径合法性
int JudgeRoute( char * route )
{
	int i;
	int ret = 0;
	char sTmp[200];
	char sFileType[ 4 ][6] = { "jpg","html","css"};
	
	memset( sTmp, 0, sizeof( sTmp ));

	if( strstr( route, "..") != NULL )
	{
		ERRLOG(ERR,"请求的路径非法!");
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

// 发送html文件
int SendHtmlFile( int fd, char * route )
{
	int ret;
	int ifd;
	char *pHtmlFile;
	struct stat Fileinf;
	
	stat( route, &Fileinf);		//获取文件属性
	ERRLOG( WAR,"%s size[%d]",route, Fileinf.st_size );

	ifd = open( route, O_RDONLY, 0 );
	if( ifd < 0 )
	{
		ERRLOG( ERR, "open [%s] err ",route );
		return -1;
	}
	// 映射文件到内存
	pHtmlFile = (char *)mmap( NULL, Fileinf.st_size, PROT_READ, MAP_PRIVATE,  
					ifd, 0);
	if( pHtmlFile == NULL )
	{
		ERRLOG( ERR, "mmap err [%s] can load",route);
		close( ifd );
		return -1;
	}
	close( ifd );
	// 写入报文头和报文体
	WriteHead( fd, route, Fileinf.st_size );
	Write( fd, pHtmlFile, Fileinf.st_size );
	ret = munmap((void *)pHtmlFile, Fileinf.st_size );
	if( ret < 0 )
	{
		ERRLOG( ERR,"[%s] munmap err 释放文件内存错误!", route );
		return -1;
	}
	return 1;
}

// 操作
int Operate(int fd)
{
	int ret;
	char sGetbuf[1024];		// 浏览器读取数据
	char sMeth[10]; 		// 浏览器请求方法
	char sRoute[200];		// 浏览器请求资源
	int iFileSize;			// 请求文件大小
	int n;
	char stype[20];
	
	memset( stype , 0, sizeof( stype ));
	memset( sRoute, 0, sizeof( sRoute ));
	memset( sMeth, 0, sizeof( sMeth ));
	memset( sGetbuf, 0, sizeof( sGetbuf ));
	
	//读取网页请求数据
	n = read( fd, sGetbuf, sizeof( sGetbuf ));
	ERRLOG( WAR, "读取[%d]网页数据[%s]", n, sGetbuf);
	//获得网页请求的方法和请求资源
	GetResq( sGetbuf, sMeth, sRoute, sizeof( sRoute ) );
	if( strlen( sMeth ) == 0 || strlen( sRoute ) == 0 )
	{
		ERRLOG( ERR,"Method[%s] or Route[%s] is NULL",sMeth,sRoute);
		return -1;
	}
	ERRLOG( WAR, "sMeth[%s]sRoute[%s]",sMeth,sRoute);
	// 设置请求方法为 post 则开始解析数据
	if( strcasecmp( "POST", sMeth) == 0 )
	{
		ParseData( sGetbuf );	
	}
	/*
	if( strcasecmp( "GET", sMeth ) != 0 )
	{
		ERRLOG( ERR,"方法校验不正确!");
		clienterror(fd, sMeth, "501", "Not Implemented", "Tiny does not implement this method");
		return -1;
	}*/
	// 获取网页类型
	GetType( sRoute, stype );
	// 校验请求资源合法性及资源重定向
	ret = JudgeRoute( sRoute );
	if( ret < 0 )
	{
		ERRLOG( ERR, "JudgeRoute err!");
		return -1;
	}

	//发送页面资源
	if( 0 > SendHtmlFile( fd, sRoute ))
		return -1;	
	
	return 1;

}

// 获取浏览器请求方法和请求页面
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
