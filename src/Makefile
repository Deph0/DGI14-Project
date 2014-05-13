CC = g++
CFLAGS = -c -Wall -ggdb
INCS = -I../lib
DEFS = -DRESOURCE_PATH="\"../res/\""
LDFLAGS = -L../lib/pugixml -L../lib/SOIL
LIBS = -lGL -lglut -lGLU -lSOIL -lpugixml
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
app.cpp
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
