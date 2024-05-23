#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../include/stb_image_write.h"
#include "../include/image_converter.h"

int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: supply full file path and file type.\n");
        return 1;
    }

    IMAGE image = {0};
    image.path = argv[1];
    image.target_extension = argv[2];
    image.output_path = argv[3];

    if (!strcmp(image.path, "help")) {
        printf("use: convert_image [path to image] [target file extension] [output path]\n");
        printf("\tInclude file name with desired extension in output path.\n");
        return 0;
    }

    convert_image(&image);
    free(image.data);
    
    return 0;
}

char* get_image_extension(char* path)
{
    int l = strlen(path);
    int offset = l - 4;
    char* p = path + offset;
    char* e = calloc(4, sizeof(char));
    e[0] = '\0';
    strcat(e, p);
    return e;
}

void convert_image(IMAGE* image)
{
    char* extension = get_image_extension(image->path);
    if (load_image(image)) {
        write_image(image);
    }
    free(extension);
}

int load_image(IMAGE* image)
{
    unsigned char* data = stbi_load(image->path, &image->w, &image->h, &image->n_channels, 0);
    if (!data) {
        fprintf(stderr, "Failed to load file: %s", image->path);
        stbi_image_free(data);
        return 0;
    }
    printf("Image: %s: loaded\n", image->path);
    image->data = data;
    return 1;
}

void convert_path_extension(char* path, char* extension)
{
    int offset = strlen(path) - 4;
    char* p = path + offset;
    char* e = extension;
    while (*p && *e) {
        *p = *e;
        p++;
        e++;
    }
}

float* convert_to_hdr(IMAGE* image)
{
    // need to implement this
    // convert unsigned int values to appropriate float values
    // Vout = (Vin / 255)^γ; Where γ = 2.2
    // R: 0.22 = (127 / 255)^2.2
    // G: 1.00 = (255 / 255)^2.2 
    // B: 0.01 = ( 32 / 255)^2.2
    
    // this is stbi__ldr_to_hdr function
    int comp = image->n_channels;
    int l = image->w * image->h;
    float* f = malloc(l * comp * sizeof(float));
    int i;
    int n;
    if (comp & 1) n = comp; else n = comp - 1;
    for (i = 0; i < l; i++) {
        for (int k = 0; k < n; k++) {
            f[i * comp + k] = (float) (pow(image->data[i*comp+k]/255.0f, 2.2f) * 1.0f);
        }
    }
    if (n < comp) {
        for (i = 0; i < l; ++i) {
            f[i*comp+n] = image->data[i*comp+n]/255.0f;
        }
    }
    return f;
}

void write_image(IMAGE* image)
{
    int success = 0;  
    if (!strcmp(image->target_extension, ".jpg")) {
        //convert_path_extension(image->path, image->target_extension);
        success = stbi_write_jpg(image->output_path, image->w, image->h, image->n_channels, image->data, JPG_QUALITY);
    }
    else if (!strcmp(image->target_extension, ".png")) {
        //convert_path_extension(image->path, image->target_extension);
        success = stbi_write_png(image->output_path, image->w, image->h, image->n_channels,
                                image->data, (image->w * image->n_channels));
    }
    else if (!strcmp(image->target_extension, ".bmp")) {
        //convert_path_extension(image->path, image->target_extension);
        success = stbi_write_bmp(image->output_path, image->w, image->h, image->n_channels, image->data);
    }
    else if (!strcmp(image->target_extension, ".tga")) {
        //convert_path_extension(image->path, image->target_extension);
        success = stbi_write_tga(image->output_path, image->w, image->h, image->n_channels, image->data);
    }
    else if (!strcmp(image->target_extension, ".hdr")) {
        //convert_path_extension(image->path, image->target_extension);
        // not sure if this is gonna work, prob not
        float* f = convert_to_hdr(image);
        //float* new = stbi_loadf_from_memory(image->data, (int)strlen(image->data), &image->w, &image->h, &image->n_channels, 0);
        //float* new = stbi_loadf(image->path, &image->w, &image->h, &image->n_channels, 0);
        success = stbi_write_hdr(image->output_path, image->w,image->h, image->n_channels, f);
        stbi_image_free(f);
    }

    if (success) {
        printf("Successfully wrote to: %s", image->output_path);
    } else {
        fprintf(stderr, "Failed to write to: %s", image->output_path);
    }
}