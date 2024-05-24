#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "./raylib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "./raylib/raygui.h"
#define STB_IMAGE_WRITE_IMPLEMENTAION
#include "./stb_image_write.h"

int screen_w = 1920;
int screen_h = 1080;

FilePathList dropped_files;
char image_path[256] = "\0";
char file_extension[5] = "\0";
char target_extension[5] = "none";
Image image = {0};
Texture2D texture = {0};
Vector2 texture_pos = {0};
bool image_loaded = false;
bool texture_loaded = false;
bool write_image = false;
float texture_scale = 1.f;

void load_image_file();
void load_texture();
void write_new_image();
void get_texture_pos();
void draw_gui();
float* convert_to_hdr();
int get_image_channels();

#endif