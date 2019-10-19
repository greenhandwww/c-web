#include "printlog.h"

#define _MY_ERR_SIZE 1004

struct _MY_ERR_S
{
	char ERR_STR[ _MY_ERR_SIZE ];
	int ERR_CODE;
	int (* SetErr)(char * buf);
	int (* SetErrCode)(int code);
	int (* GetErrCode)();
	char * (* GetErr)();
	void (* DesErr)();
};

#ifndef _MYERR_H
#define _MYERR_H

struct _MY_ERR_S * ERR_PTR ;

#endif


/*
 * 	初始化内存
 */
int Initmyerr();

/*
 * 设置错误信息
 */
int Seterr( char * buf );

/*
 * 设置错误代码
 */
int Seterrcode( int code );

/*
 * 获取错误代码
 */
int Geterrcode();

char * Geterr();

void Deserr();
