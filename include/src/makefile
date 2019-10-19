.PHONY: clean

all:ToChange ToLog Tomyxml Tomyerr clean

CC=gcc
LOGH=/home/web/log/makefileLOG

Tomyerr:
	@echo "myerr动态库加载中"
	@$(CC) -fPIC -shared -o libmyerr.so myerr.c \
		-I$(HOME)/include \
		-L$(HOME)/lib -llog

ToChange:
	@echo "change动态库加载中" 
	@$(CC) -g -fPIC -o libchange.o -c change.c 
	@$(CC) libchange.o -fPIC -shared -o libchange.so

Tomyxml:
	@echo "libmyxml动态库加载中" 
	@$(CC) myxml.c -fPIC -shared -o libmyxml.so \
		-I/usr/local/include/libxml2 \
		-I$(HOME)/include \
	 	-L/usr/local/lib -lxml2 -lm -ldl 

ToLog:
	@echo "liblog动态库加载中" 
	@$(CC) -g -fPIC -o liblog.o -c printlog.c -w \
		-I$(HOME)/include	
	@$(CC) liblog.o -fPIC -shared -o liblog.so

clean:
	rm *.o
	mv *.so $(HOME)/lib
	@echo "-----------------------------------------------  makefile 执行成功 ------------------------------------------------"
	@echo "			lib库编译完成" 
	
	

	
