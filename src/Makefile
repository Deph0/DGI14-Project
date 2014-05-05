CC = g++
CFLAGS = -c -Wall -ggdb
#CFLAGS = -c -Wall -O3
#LDFLAGS =
LIBS = -lGL -lglut -lGLU -lSOIL
LDFLAGS= -framework GLUT -framework OpenGL -framework Cocoa
#LIBS = -l3ds -lGL -lglut -lGLU -lGLEW
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
