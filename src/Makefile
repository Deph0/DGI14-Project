CC = g++
CFLAGS = -c -Wall -ggdb
#CFLAGS = -c -Wall -O3
LDFLAGS =
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
LDFLAGS = -framework GLUT -framework OpenGL
LIBS = -lSOIL
else
LIBS = -lGL -lglut -lGLU -lSOIL
#LIBS = -l3ds -lGL -lglut -lGLU -lGLEW
endif
TARGET = model
SRCS = \
model.cpp \
texture.cpp \
scene.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(SRCS) $(TARGET)
	
$(TARGET): $(OBJS) 
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) $(TARGET)
