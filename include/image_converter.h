#ifndef IMAGE_CONVERTER_H
#define IMAGE_CONVERTER_H
#define JPG_QUALITY 90

// PNG/BMP/TGA/JPEG/HDR
// png works
// jpg works
// bmp works
// tga works
// hdr works

typedef struct 
{
    char* path;
    char* output_path;
    char* target_extension;
    int w; 
    int h;
    int n_channels;
    unsigned char* data;
} IMAGE;

void convert_image(IMAGE* image);
int load_image(IMAGE* image);
void write_image(IMAGE* image);
char* get_image_extension(char* path);
void convert_path_extenstion(char* path, char* extension);
float* convert_to_hdr(IMAGE* image);

#endif