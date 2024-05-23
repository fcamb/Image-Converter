CC = gcc
INCLUDES = -I../include/
CFLAGS = -Wall -O3 $(INCLUDES)
TARGET = convert_image.exe
CFILES = image_converter.c

.PHONY: all
all: $(TARGET)

$(TARGET):
	$(CC) $(CFILES) -o $@ $(CFLAGS)

.PHONY: clean
clean: 
	rm -f $(TARGET)
