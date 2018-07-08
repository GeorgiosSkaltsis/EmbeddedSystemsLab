
CC=gcc
override CFLAGS += -g -Wall -lm -std=c99 -D_XOPEN_SOURCE
EXEC = ./pc-terminal

C_SOURCE_FILES += \
$(abspath ./main.c) \
$(abspath ./pc_terminal.c) \
$(abspath ./serial.c) \
$(abspath ./terminal.c) \
$(abspath ./util.c) \
$(abspath ./ping_timer.c) \
$(abspath ../shared/frame.c) \
$(abspath ../shared/byte_queue.c) \
$(abspath ../shared/comm_protocol.c) \
$(abspath ../shared/types.c) \
$(abspath ../shared/values.c) \
$(abspath ./comm_terminal.c) \
$(abspath ./gui_server.c) \
$(abspath ./js.c) \

all:
	$(CC) $(CFLAGS) $(C_SOURCE_FILES) -o $(EXEC)

run: all
	$(EXEC)
