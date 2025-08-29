TARGET = dma_from_device_dsc_bypass
CC = gcc
CFLAGS = -Wall -O2
ifeq ($(DEBUG),1)
    CFLAGS += -DDEBUG
endif

SRCS = dma_from_device_dsc_bypass.c reg_rw.c user_reg.c control_reg.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c reg_rw.h user_reg.h control_reg.h
	$(CC) $(CFLAGS) -c $< -o $@

debug:
	$(MAKE) DEBUG=1

clean:
	rm -f $(TARGET) $(OBJS)
