# demiurge makefile

PROG     = demiurge
SRCS    += asprintf.c follow.c login.c main.c post.c upload_file.c util.c 
CC       = cc
CPPFLAGS = -I/usr/local/include
LDADD    = -lz -lssl -lcrypto -lnghttp2 -lcurses -lcurl -ljson-c\
	   -lpthread -lereadline -L/usr/local/lib
BINDIR=/usr/local/bin
.include <bsd.prog.mk>
