CC = g++
CFLAGS = -c -Wall -O3
TARGET = libpugixml.a
SRCS = pugixml.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(SRCS) $(TARGET)
	
$(TARGET): $(OBJS) 
	$(AR) $(ARFLAGS) $@ $?

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) $(TARGET)
