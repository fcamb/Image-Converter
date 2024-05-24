#include "../include/main.h"

int
main(void)
{
    InitWindow(screen_w, screen_h, "Image Converter");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        if (IsFileDropped()) {
            load_image_file();
        }
        if (image_loaded) {
            load_texture();
        }
        if (texture_loaded) {
            get_texture_pos();
        }
        if (write_image) {
            write_new_image();
        }
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            draw_gui();
            if (texture_loaded) {
                DrawTextureEx(texture, texture_pos, 0, texture_scale, RAYWHITE);
            }
        }
        EndDrawing();
    }

    UnloadImage(image);
    UnloadTexture(texture);

    CloseWindow();
    return 0;
}

void
load_image_file()
{
    dropped_files = LoadDroppedFiles();  
    if (dropped_files.count == 1) {
        image = LoadImage(dropped_files.paths[0]);   
        strcpy(image_path, dropped_files.paths[0]);     
        strcpy(file_extension, GetFileExtension(GetFileName(dropped_files.paths[0])));
        image_loaded = true;
    } 
    UnloadDroppedFiles(dropped_files);
}

void
load_texture()
{
    texture = LoadTextureFromImage(image);
    if (texture.id > 0) {
        texture_loaded = true;
        image_loaded = false;
        get_texture_pos();
        texture_scale = (float) (screen_h - 200) / texture.height;
    }
}

int
get_image_channels()
{
    switch(image.format) {
        case PIXELFORMAT_UNCOMPRESSED_GRAYSCALE:
            return 1;
        case PIXELFORMAT_UNCOMPRESSED_GRAY_ALPHA:
            return 2;
        case PIXELFORMAT_UNCOMPRESSED_R8G8B8:
            return 3;
        case PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
            return 4;
        case PIXELFORMAT_UNCOMPRESSED_R32:
            return 1;
        case PIXELFORMAT_UNCOMPRESSED_R32G32B32:
            return 3;
        case PIXELFORMAT_UNCOMPRESSED_R32G32B32A32:
            return 4;
    }
    
    return 4;
}

float*
convert_to_hdr()
{
    // stbi__ldr_to_hdr
    int l = image.width * image.height;
    int comp = get_image_channels();
    unsigned char*c = (unsigned char*)image.data;
    float* f = malloc(sizeof(float) * l * image.format);
    int i, n;
    if (comp & 1) n = comp; else n = comp - 1;
    for (i = 0; i < l; i++) {
        for (int k = 0; k < n; k++) {
            f[i * comp + k] = (float)(pow(c[i*comp+k]/255.f, 2.2f) * 1.f);
        }
    }
    if (n < comp) {
        for (i = 0; i < 1; ++i) {
            f[i * comp + n] = c[i * comp + n] / 255.f;
        }
    }
    return f;
}

void
write_new_image()
{
    int i, j, l;
    l = strlen(image_path);
    char new_path[256];
    strcpy(new_path, image_path);
    j = 1;
    for (i = l - 3; i < l; i++) {
        new_path[i] = target_extension[j++];
    }
    write_image = false;

    int channels = get_image_channels();
    int success = 0;  
    if (!strcmp(target_extension, ".jpg")) {
        //convert_path_extension(image.path, image.target_extension);
        success = stbi_write_jpg(new_path, image.width, image.height, channels, image.data, 90);
    }
    else if (!strcmp(target_extension, ".png")) {
        //convert_path_extension(image.path, image.target_extension);
        success = stbi_write_png(new_path, image.width, image.height, channels,
                                image.data, (image.width * image.format));
    }
    else if (!strcmp(target_extension, ".bmp")) {
        //convert_path_extension(image.path, image.target_extension);
        success = stbi_write_bmp(new_path, image.width, image.height, channels, image.data);
    }
    else if (!strcmp(target_extension, ".tga")) {
        //convert_path_extension(image.path, image.target_extension);
        success = stbi_write_tga(new_path, image.width, image.height, channels, image.data);
    }
    else if (!strcmp(target_extension, ".hdr")) {
        float* f = convert_to_hdr();
        success = stbi_write_hdr(new_path, image.width, image.height, channels, f);
        free(f);
    }

    if (success) {
        printf("Successfully wrote to: %s", new_path);
    } else {
        fprintf(stderr, "Failed to write to: %s", new_path);
    }
}

void
get_texture_pos()
{
    float t = texture_scale + (float)GetMouseWheelMove();
    if (t < 0) {
        return;
    }
    float x = (screen_w / 2.f) - ((texture.width * t) / 2.f);
    float y = (screen_h / 2.f) - ((texture.height * t) / 2.f);
    if (x < 0 || y < 0 || x + (texture.width*t) > screen_w || y + (texture.height*t) > screen_h) {
        return;
    }
    texture_scale = t;
    texture_pos = (Vector2){x, y};    
}

void 
draw_gui()
{
    GuiLabel((Rectangle){200, 0, 200, 60}, TextFormat("Target Extension: %s", target_extension));
    if (GuiButton((Rectangle){400, 20, 40, 20}, "PNG")) strcpy(target_extension, ".png");
    if (GuiButton((Rectangle){450, 20, 40, 20}, "JPG")) strcpy(target_extension, ".jpg");
    if (GuiButton((Rectangle){500, 20, 40, 20}, "BMP")) strcpy(target_extension, ".bmp");
    if (GuiButton((Rectangle){550, 20, 40, 20}, "TGA")) strcpy(target_extension, ".tga");
    if (GuiButton((Rectangle){600, 20, 40, 20}, "HDR")) strcpy(target_extension, ".hdr");

    if (GuiButton((Rectangle){800, 20, 200, 20}, "#02#Save New image")) {
        if (strcmp(target_extension, "none")) {
            write_image = true;
        }
    }
}