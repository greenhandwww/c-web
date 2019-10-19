
all:begin 

WBL=/home/web

OUTNAME=WebService

begin:
	gcc -g -o $(OUTNAME) *.c \
	-I$(HOME)/myweb/include \
	-I$(WBL)/include \
	-I/usr/local/include/libxml2 \
	-L$(WBL)/lib \
	-L/usr/local/lib -lxml2 -llog -lmyxml -lmyerr -lm -ldl
	mv $(OUTNAME) $(HOME)/bin
