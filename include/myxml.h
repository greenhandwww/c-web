#include "myxmlall.h"

/*	程序介绍
 *		本应用库文件基于开源库libxml2，在原先基础进行封装
 *	
 *	使用说明
 *		需要包含用户lib库，即 $(HOME)/lib ，同时加参数 -lmyxml
 */





/*
 *	DESC:	查询etc目录下的Network，根据子节点查询 Desc，IP，Port 节点内容，
 *	IN:		char * node
 *	OUT: 	char * des
 *			char * ip
 *			char * port
 *
 *	Fail 	Return 	-1 
 *	Success Parse 	1
 */
int XmlGetNetwork( char * node, char * des, char * ip,char * port );
