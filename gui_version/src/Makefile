CC = gcc
TARGET = image_converter_gui.exe
C_FILES = main.c

INCLUDE = -I../include/raylib -I../include
LIB = -L../lib/

FLAGS = -Wall -O3
LIB_FLAGS = $(LIB) -lraylib -lgdi32 -lwinmm

all: $(TARGET)
#raylib.lib
#winmm.lib
#kernel32.lib
#opengl32.lib
#gdi32.lib
$(TARGET):
	$(CC) $(C_FILES) -o $@ $(FLAGS) $(INCLUDE) $(LIB_FLAGS)

.PHONEY: clean
clean:
	rm $(TARGET)