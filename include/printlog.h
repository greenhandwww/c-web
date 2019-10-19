#ifndef LOGFILE_H
#define LOGFILE_H

#define ERR __FILE__,__LINE__,0
#define WAR __FILE__,__LINE__,1

#define _MAX_N_ 100
#endif

// 引用变量，不建议使用、改变
extern int _MAX_F_NAME_ ;				//最大文件名长度
extern char _ERR_NAME_[ _MAX_N_ + 1 ];	
extern char _FILE_LOG_NAME_[ _MAX_N_ ];	//InitLOG初始化名备份
extern int _DAYS_ ;						//时间节点，判断log日志时间
extern char _LOG_DIR_[ _MAX_N_ ];		//日志目录

// 错误日志，配合 宏定义（ERR，WAR）使用，代替 errLOG 函数
void ERRLOG( char * filenname, int line, int flag, char * format, ... );

// 错误日志，配合 宏定义（ERR，WAR）使用
void errLOG( char * filenname, int line, int flag, char * format, ... );

// 初始化 log 文件名，必须先使用
void Initlog( char * filename );

//获取当前时间 年月日
int nowtime();

// 获取时分秒时间 供errLOG 函数调用
void SFMtime( char * out);
