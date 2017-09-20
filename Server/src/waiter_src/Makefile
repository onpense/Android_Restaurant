
vpath %.c = ./src
vpath %.h = ./inc
CC = gcc
CFLAGS = -g -I./inc  -L./lib
OPTION = -lsqlite3 -lpthread -ldl

waiter:waiter_main.c encapsulation_sock.c  send.c  send_request.c rcv_feedback.c
	$(CC) $(CFLAGS) -o $@ $^ $(OPTION)

.PHONY: clean
clean:
	$(RM) -f *.o waiter *.sw? *.bak
