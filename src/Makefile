# demiurge makefile

PROG     = demiurge
SRCS    += asprintf.c follow.c login.c main.c post.c upload_file.c util.c 
CC       = cc
CPPFLAGS = -I/usr/local/include
MAN	 =
LDADD    != pkg-config --libs libcurl readline json-c
BINDIR=/usr/local/bin
.include <bsd.prog.mk>
