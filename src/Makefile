CC = g++
CFLAGS = -c -Wall -ggdb
INCS = -I../lib
DEFS =
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
LDFLAGS = -L../lib/pugixml -framework GLUT -framework OpenGL
LIBS = -lSOIL -lpugixml
else
LDFLAGS = -L../lib/pugixml -L../lib/SOIL
LIBS = -lGL -lglut -lGLU -lGLEW -lSOIL -lpugixml
endif
TARGET = rainyday
SRCS = \
object.cpp \
material.cpp \
texture.cpp \
camera.cpp \
light.cpp \
point_light.cpp \
directional_light.cpp \
geometry.cpp \
dae_model.cpp \
obj_model.cpp \
scene.cpp \
util.cpp \
shader.cpp \
shader_program.cpp \
rain_drops.cpp \
rain_scene.cpp \
rainy_day.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(SRCS) $(TARGET)
	
$(TARGET): $(OBJS) 
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(DEFS) $(INCS) $< -o $@

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) $(TARGET)
