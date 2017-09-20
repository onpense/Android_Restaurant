
vpath %.c = ./src
vpath %.h = ./inc
CC = gcc
CFLAGS = -g -I./inc  -L./lib
OPTION = -lsqlite3 -lpthread -ldl -ljson

kitchen:main.c send.c db_encap.c func_recv.c req_dispose.c json_handle.c
	$(CC) $(CFLAGS) -o $@ $^ $(OPTION)

.PHONY: clean
clean:
	$(RM) -f *.o *.sw? *.bak kitchen ksystem.db
