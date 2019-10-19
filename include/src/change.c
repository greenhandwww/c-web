
/*	功能描述：装换类函数封装
 *	作者：wbl
 *	创建：20190430
 *	修改：20190503
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*	功能：分段截取子字符串
 *	输入：（int number, int len ）
 *	输出：字符串 (char * out ）
 *	返回值：-1		0
 *			失败	成功
 *	创建时间：20190806
 */ 
int strcut( const char * in, char * out, char c, int pos )
{
	int i = 0;
	int end = 0;
	int count = 0;
	char tmp[512];

	memset( tmp , 0, sizeof( tmp ) );
	while( 1 )
	{
		if( in[i] == c && count == pos )
		{
			if( out != NULL )
				memcpy( out , in + end , i - end);
			else
			{
				memcpy( tmp , in + end , i - end);
				printf( "%s",tmp);
			}
			break;
		}
		else if( in[i] == c )
		{
			count++;
			end = i + 1;
		}
		else if( in[i] == '\0' )
		{
			if( count == pos && out != NULL )
				strcpy( out , in + end );
			else if( count == pos )
			{
				strcpy( tmp , in + end );
				printf( "%s",tmp);
			}
			break;
		}

		i++;
	}
}


/*	功能：int转char *
 *	输入：数字和数字长度（int number, int len ）
 *	输出：字符串 (char * out ）
 *	返回值：-1		0
 *			失败	成功
 *	创建时间：20190430
 */
int InttoString( int number, char * out ,int len)
{
	int tmp = number;
	int ys;
	int i = 0;
	char StrNumber[ 10 ];

	memset( StrNumber, 0, sizeof( StrNumber ) );

	while( tmp )
	{
		ys = tmp % 10;
		tmp = tmp / 10;
		StrNumber[i] = ys + '0' ;
		i++;
	}

	if( len != i + 1)
	{
		printf(" err !\n");
		return -1;
	}

	for(;i > 0;i--)
	{
		out[0] = StrNumber[i];
	}

	return 0;
}


/*	功能：字符串去除头尾空格
 *	输入：字符串和输出字符串长度( char * input )
 *	输出：字符串 (char * out ）
 *	返回值：-1		0
 *			失败	成功
 *	创建时间：20190503
 */
int trimblank( char * input, int len )
{
	int i;
	int start;
	int end;
	char temp[ len + 1];

	memset( temp, 0,sizeof( temp ));
	for(i = 0;;i++)
	{
		if( input[i] != ' ' )
		{
			start = i;
			break;
		}
	}

	for(i = len - 1;;i--)
	{
		if( input[i] != ' ' )
		{
			end = i;
			break;
		}
	}
	if( end - start >= len )
		return -1;
	memcpy( temp, input + start, end - start + 1);
	memset( input, 0, len );
	memcpy( input, temp, strlen( temp ) );
	
	return 0;
}

