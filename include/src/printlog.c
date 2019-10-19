/*  功能：对数据进行监控
 *  输入：
 *  输出：
 *  返回值：错误终止程序
 *          
 *  创建时间：20190504
 *  修改时间：20190701 修改次数：2
 *	修改内容：
 *			  为 消息 系统服务端进行兼容修改
 */

#pragma ince
#include <stdio.h>
#include <stdarg.h>
#include "printlog.h"
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define _MAX_N_ 100
int _MAX_F_NAME_ = _MAX_N_ ;
char _ERR_NAME_[ _MAX_N_ + 1 ];
char _FILE_LOG_NAME_[ _MAX_N_ ];
int _DAYS_ ;
char _LOG_DIR_[ _MAX_N_ ];
static unsigned short _Initlog_FLAG = 0;


/*  功能：控制LOG路径和名字
 *  输入：绝对路径名（char * filename）
 *  输出：
 *  返回值：错误终止程序
 *          
 *  创建时间：20190504
 *  修改时间：20190908
 *	修改内容：
 *			修改函数名字，修改函数兼容性
 *			扩展 warm 警告内容
 */
void Initlog( char * filename )
{
	FILE * puterr;
	char nr[100];
	int iret;
	char cmd[100];
	char tmp[100];
	char * envlog;

	if( _Initlog_FLAG == 1 )
	{
		ERRLOG( ERR, "Initlog 错误,请勿初始化两次!");
		exit (-1);
	}

	memset( _LOG_DIR_, 0, sizeof( _LOG_DIR_ ));
	envlog = getenv( "LOGDIR" );
	if( envlog == NULL )
	{
		strcpy( tmp, getenv( "HOME") );
		strcat( tmp, "/log/funcLOG" );
		puterr = fopen( tmp, "a+");
		fprintf( puterr,"cant find _LOG_DIR_ in the env !\n");
		exit (-1);	
	}
	strcpy( _LOG_DIR_, envlog );

	if( strlen( filename ) > _MAX_F_NAME_ || strstr( filename,"/" ) || \
			strstr( filename, " " ) )
	{
		memset( nr, 0, sizeof( nr ) );
		strcpy( tmp, envlog );
		strcat( tmp, "/log/funcLOG" );
		puterr = fopen( tmp, "a+" );
		strcpy( nr, "请输入文件名或者文件名过长");
		fputs( nr, puterr );
		fprintf( puterr,"\n--------_FILE_LOG_NAME_[%s]",filename);
		fprintf( puterr,"\n--------TIME[%d]",nowtime());
		exit (-1);
	}
	_ERR_NAME_[_MAX_F_NAME_];
	memset( _FILE_LOG_NAME_, 0, sizeof( _FILE_LOG_NAME_ ));
	memset( _ERR_NAME_, 0, sizeof( _ERR_NAME_ ) );
	memset( tmp, 0, sizeof( tmp ));
	strcpy( _ERR_NAME_, envlog );
	strcat( _ERR_NAME_,"/");
	strcat( _ERR_NAME_, filename );
	strcpy( _FILE_LOG_NAME_, filename );
	
	sprintf( cmd, "mkdir -p %s/%d", _ERR_NAME_, ( _DAYS_ = nowtime() ) );
	iret = system( cmd );
	if( iret < 0 )
	{
		strcpy( tmp, envlog );
		strcat( tmp, "/log/specialERR");
		puterr = fopen( tmp, "a+" );
		strcpy( tmp, "系统指令出错，系统强行结束！\n");
		fputs( tmp, puterr );
		fprintf( puterr,"\n--------_FILE_LOG_NAME_[%s]",filename);
		exit(-1);
	}
	
	_Initlog_FLAG = 1;
	return ;
}

/*  功能：打印数据信息 封装 fprintf 函数
 *  输入：封装前三个参数，代替为 ERRLOG，WARLOG 风别为警告，和错误日志
 *		  后接格式化字符串，控制输出内容，形如 fprintf
 *  输出：
 *  返回值：错误终止程
 *          
 *  创建时间：20190504
 *  修改时间：20191010
 *	修改内容：
 *				淘汰 errLOG 函数
 */

//char ERRLOGFILECONTENT[1024];

void ERRLOG( char * filename,int line, int flag, char * format, ... )
{
	FILE * errlog;
	FILE * puterr;
	char wtlj[150];
	char ftime[100];
	char tmp[100];
	va_list ap;
	va_start( ap,format);
	int cs = 0;
	int iret;
	char cmd[100];
	
	memset( cmd, 0, sizeof( cmd ));
	memset( tmp, 0, sizeof( tmp ));
	memset( ftime, 0, sizeof( ftime ));
	memset( wtlj, 0, sizeof( wtlj ));

	if( strlen(_FILE_LOG_NAME_) == 0 )
	{
		strcpy( wtlj, _LOG_DIR_ );
		strcat( wtlj, "/specialERR");
		puterr = fopen( wtlj, "a+" );
		strcpy( tmp, "未执行Initlog函数！\n");
		fputs( tmp, puterr );
		fprintf( puterr,"\n--------_FILE_LOG_NAME_[%s]",filename);
		exit (-1);	
	}

	if( _DAYS_ != nowtime() )
	{
		memset( cmd, 0, sizeof( cmd ));
		sprintf( cmd, "mkdir -p %s/%d", _ERR_NAME_, ( _DAYS_ = nowtime() ) );
		iret = system( cmd );
		if( iret < 0 )
		{
			strcpy( wtlj, _LOG_DIR_ );
			strcat( wtlj, "/specialERR");
			puterr = fopen( wtlj, "a+" );
			strcpy( tmp, "系统指令出错，系统强行结束！\n");
			fputs( tmp, puterr );
			fprintf( puterr,"\n--------_FILE_LOG_NAME_[%s]",filename);
			exit(-1);
		}
	}

	if( flag == 0 )
	{
		// 错误日志
		sprintf( wtlj, "%s/%d/%s_ERR", _ERR_NAME_, _DAYS_, _FILE_LOG_NAME_ );
		SFMtime(ftime);
		errlog = fopen( wtlj,"a+");
	}
	else if( flag == 1 )
	{
		// 警告日志
		sprintf( wtlj, "%s/%d/%s_WARN", _ERR_NAME_, _DAYS_, _FILE_LOG_NAME_ );
		SFMtime(ftime);
		errlog = fopen( wtlj,"a+");
	}
	
	fprintf( errlog, "%s %s [%d]: ", ftime, filename, line);
	vfprintf ( errlog, format, ap );

	fprintf( errlog,"\n");	// 数据从函数缓冲区输出
	va_end(ap);
	fclose( errlog );

	return ;
}

/*  功能：打印数据信息（代替 fprintf ）
 *  输入：封装前三个参数，代替为 ERRLOG，WARLOG 风别为警告，和错误日志
 *		  后接格式化字符串，控制输出内容，形如 fprintf
 *  输出：
 *  返回值：错误终止程
 *          
 *  创建时间：20190504
 *  修改时间：20190630 修改次数：2
 *	修改内容：
 *			  增加 flag 判断标记，控制 log 输出
 *			  增加 日志文件夹 创建
 */

void errLOG( char * filename,int line, int flag, char * format, ... )
{	
	FILE * ERRLOG;
	FILE * puterr;
	char wtlj[150];
	char ftime[100];
	char tmp[100];
	int i;
	va_list ap;
	va_start( ap,format);
	int cs = 0;
	int len = strlen(format);
	int valin;
	int valch;
	double valfl;
	int iret;
	char * valst;
	char cmd[100];

	memset( tmp, 0, sizeof( tmp ));
	memset( ftime, 0, sizeof( ftime ) );
	memset( wtlj, 0, sizeof( wtlj ) );

	if( strlen(_FILE_LOG_NAME_) == 0 )
	{
		strcpy( wtlj, _LOG_DIR_ );
		strcat( wtlj, "/specialERR");
		puterr = fopen( wtlj, "a+" );
		strcpy( tmp, "未执行Initlog函数！\n");
		fputs( tmp, puterr );
		fprintf( puterr,"\n--------_FILE_LOG_NAME_[%s]",filename);
		exit (-1);	
	}

	if( _DAYS_ != nowtime() )
	{
		memset( cmd, 0, sizeof( cmd ));
		sprintf( cmd, "mkdir -p %s/%d", _ERR_NAME_, ( _DAYS_ = nowtime() ) );
		iret = system( cmd );
		if( iret < 0 )
		{
			strcpy( wtlj, _LOG_DIR_ );
			strcat( wtlj, "/specialERR");
			puterr = fopen( wtlj, "a+" );
			strcpy( tmp, "系统指令出错，系统强行结束！\n");
			fputs( tmp, puterr );
			fprintf( puterr,"\n--------_FILE_LOG_NAME_[%s]",filename);
			exit(-1);
		}
	}

	if( flag == 0 )
	{
		// 错误日志
		sprintf( wtlj, "%s/%d/%s_ERR", _ERR_NAME_, _DAYS_, _FILE_LOG_NAME_ );
		SFMtime(ftime);
		ERRLOG = fopen( wtlj,"a+");
	}
	else
	{
		// 警告日志
		sprintf( wtlj, "%s/%d/%s_WARN", _ERR_NAME_, _DAYS_, _FILE_LOG_NAME_ );
		SFMtime(ftime);
		ERRLOG = fopen( wtlj,"a+");
	}
	
	fprintf( ERRLOG, "%s %s [%d]: ", ftime, filename, line);
	for(i = 0;i < len;i++ )
	{
		if( format[len - 1] == '%')
		{
			fputc( format[len - 1], ERRLOG);
			break;
		}
		else if( format[i] == '%' && ( format[i+1] == 'c' || format[i+1] == 's' || format[i+1] == 'f' || format[i+1] == 'd'))
		{
			switch( format[i+1] )
			{
				case 'c' :
				{
					valch = va_arg( ap, int);
					fputc( valch, ERRLOG );
					break;
				}
				case '%' :
				{
					fputc( '%', ERRLOG );
					break;
				}
				case 'd' :
				{
					valin = va_arg( ap, int );
					fprintf( ERRLOG, "%d",valin);
					break;
				}
				case 'f' :
				{
					valfl = va_arg( ap, double );
					fprintf( ERRLOG, "%f",valfl);
					break;
				}
				case 's' :
				{	
					valst = va_arg( ap, char * );
					fprintf( ERRLOG, "%s",valst);
					break;
				}
				default :
				{
					fputc( '%', ERRLOG );
					break;
				}
			}
			i = i + 1;
		}
		else
		{
			fputc( format[i], ERRLOG );
		}
	}
	fprintf( ERRLOG,"\n");	// 数据从函数缓冲区输出
	va_end(ap);
	fclose( ERRLOG );
	
}

/*  功能：获取当前系统时间
 *  输入：
 *  输出：
 *  返回值：当前系统时间
 *          例如：[20190504]
 *  创建时间：20190504
 */
int nowtime()
{
	int it;
	time_t t;
	struct tm * nowt;
	time(&t);
	nowt = localtime( &t );
	it = (nowt->tm_year+1900) * 10000 + (nowt->tm_mon+1) * 100 + (nowt->tm_mday);
	return it;
}

/*  功能：获取当前系统时间
 *  输入：
 *  输出：
 *  返回值：当前系统时间时分秒
 *          例如：[5/4 12:12:12]
 *  创建时间：20190504
 */
void SFMtime( char * out)
{
	char sfm[100];
	time_t t;
	struct tm * nowt;
	time(&t);
	nowt = localtime( &t );

	memset( sfm, 0, sizeof( sfm ) );

	sprintf( sfm, "%0.2d/%0.2d %0.2d:%0.2d:%0.2d",nowt->tm_mon+1,nowt->tm_mday,nowt->tm_hour,nowt->tm_min,nowt->tm_sec);
	
	strcpy( out, sfm );

}


void choiceout( char word )
{
}


