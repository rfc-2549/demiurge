# demiurge makefile

TARGET = demiurge
OBJS   = login.o util.o post.o main.o upload_file.o asprintf.o follow.o
CC     = cc
CFLAGS += -O2 -MD -std=c11 -Wall -Wextra -Werror -Wpedantic -lcurl -g -lreadline -ljson-c


$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CFLAGSDEF) $< -o $@

-include *.d

.PHONY: clean
clean:
	rm -f *.d *.o demiurge
