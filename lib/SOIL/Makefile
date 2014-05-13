CC = gcc
CFLAGS = -c -Wall -O3
TARGET = libSOIL.a
SRCS = \
image_helper.c \
stb_image_aug.c \
image_DXT.c \
SOIL.c
OBJS = $(SRCS:.c=.o)

all: $(SRCS) $(TARGET)
	
$(TARGET): $(OBJS) 
	$(AR) $(ARFLAGS) $@ $?

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) $(TARGET)
