#include "myerr.h"
#include <string.h>
#include "printlog.h"
#include <malloc.h>

char * _NULL_ERR_RETURN_ = "程序未初始化";

/*  功能：初始化错误信息
 *  输入：无
 *  输出：无
 *  返回值：
 *  		-1 程序初始化失败
 *  		0  程序已经初始化过 
 *          1  程序初始化成功
 *  创建时间：20190926
 *  修改内容：
 */
int Initmyerr()
{
	if( ERR_PTR == NULL )
	{
		ERR_PTR = (struct _MY_ERR_S *)malloc( _MY_ERR_SIZE );
		if( ERR_PTR == NULL )
			return -1;
		ERR_PTR->SetErr = Seterr;
		ERR_PTR->GetErrCode = Geterrcode;
		ERR_PTR->SetErrCode = Seterrcode;
		ERR_PTR->GetErr = Geterr;
		ERR_PTR->DesErr = Deserr;
	}
	else
		return 0;
	
	memset( ERR_PTR , 0, _MY_ERR_SIZE );
	return 1;
}

/*  功能：设置错误信息
 *  输入：char * buf
 *  输出：无
 *  返回值：
 *  		-1 程序初始化失败
 *  		0  程序已经初始化过 
 *          1  程序初始化成功
 *  创建时间：20190926
 *  修改内容：
 */
int Seterr( char * buf )
{
	int iret ;
	if( ERR_PTR == NULL )
	{
		iret = Initmyerr();
		if( iret < 0 )
		{
			if( strlen(_FILE_LOG_NAME_) != 0 )
				errLOG( ERR, "内存分配失败");
			return -1;
		}
	}
	strcpy( ERR_PTR->ERR_STR, buf );
	return 1;
}

int Seterrcode( int code )
{
	int iret;
	if( ERR_PTR == NULL )
	{
		iret = Initmyerr();
		if( iret < 0 )
		{
			if( strlen(_FILE_LOG_NAME_) != 0 )
				errLOG( ERR, "内存分配失败");
			return -1;
		}
	}
	ERR_PTR->ERR_CODE = code ;	
	return 1;
}

int Geterrcode()
{
	return ERR_PTR->ERR_CODE ;	
}

char * Geterr()
{
	if( ERR_PTR != NULL )
		return ERR_PTR->ERR_STR ;	
	else
		return _NULL_ERR_RETURN_ ;
}

void Deserr()
{
	ERR_PTR->SetErr = NULL;
	ERR_PTR->GetErrCode = NULL;
	ERR_PTR->SetErrCode = NULL;
	ERR_PTR->GetErr = NULL;
	ERR_PTR->DesErr = NULL;
	free( ERR_PTR );
}
