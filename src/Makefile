# demiurge makefile

TARGET = demiurge
OBJS   = login.o util.o post.o main.o upload_file.o
CC     = cc
CFLAGS = -O2 -march=native -MD -std=c11 -Wall -Wextra -lcurl -lreadline -ljson-c


$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(CFLAGSDEF) -o $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $(CFLAGSDEF) $< -o $@

-include *.d

.PHONY: clean
clean:
	rm -f *.d *.o demiurge
