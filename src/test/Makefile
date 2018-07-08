CC = gcc
CFLAGS = -Wall -std=c99 -DTESTING -D_XOPEN_SOURCE
LDFLAGS = -pthread -lm -lpthread -lcheck
EXEC = ./test

C_SOURCE_FILES += \
$(abspath ./test_fixedpoint.c) \
$(abspath ./test_main.c) \
$(abspath ./test_pc_terminal.c) \
$(abspath ./test_flightmodes.c) \
$(abspath ./test_state_transitions.c) \

C_SOURCE_FILES += \
$(abspath ../fixedpoint/fixedpoint.c) \
$(abspath ../pc_terminal/pc_terminal.c) \
$(abspath ../pc_terminal/serial.c) \
$(abspath ../pc_terminal/terminal.c) \
$(abspath ../pc_terminal/util.c) \
$(abspath ../pc_terminal/gui_server.c) \
$(abspath ../pc_terminal/ping_timer.c) \
$(abspath ../shared/frame.c) \
$(abspath ../shared/types.c) \
$(abspath ../shared/byte_queue.c) \
$(abspath ../shared/comm_protocol.c) \
$(abspath ../shared/values.c) \
$(abspath ../pc_terminal/comm_terminal.c) \
$(abspath ../pc_terminal/js.c) \
$(abspath ../states.c) \
$(abspath ../control.c) \


# Includes common to all targets
INC_PATHS += \
-I$(abspath ../) \
-I$(abspath ../shared/) \
-I$(abspath ../drivers/config/) \
-I$(abspath ../fixedpoint/) \
-I$(abspath ../pc_terminal/) \

all : $(OBJS)
	$(CC) $(CFLAGS) $(C_SOURCE_FILES) $(INC_PATHS) $(LDFLAGS) -o $(EXEC)

run: all
	$(EXEC)
