#ifndef ALEMDAR_IMG_H

#define ALEMDAR_IMG_H

#ifdef ALEMDAR_IMG_IMPLEMENTATION

#include <png.h>
#include <jpeglib.h>
#include <sys/types.h>
#include <dirent.h>

typedef struct {
    size_t width;
    size_t height;
    Matrix data;
} Img;

ALEMDAR_DEF Img img_alloc(size_t width, size_t height);
ALEMDAR_DEF void img_free(Img img);
ALEMDAR_DEF Img img_read_png(FILE *fp);
ALEMDAR_DEF Img img_read_jpg(FILE *fp);
ALEMDAR_DEF Img img_read(const char *imageName);
ALEMDAR_DEF Img *imgs_read(const char *folderPath, size_t count);
ALEMDAR_DEF Img img_resize(Img img, size_t width, size_t height);
ALEMDAR_DEF Img *imgs_resize(Img *imgs, size_t count, size_t width, size_t height);

Img img_alloc(size_t width, size_t height) {
    Img img;
    img.width = width;
    img.height = height;
    img.data = matrix_alloc(height * width, 4);
    return img;
}

void img_free(Img img) {
    matrix_free(img.data);
}

void img_print(Img img) {
    printf("width: %zu, height: %zu\n", img.width, img.height);
    for (size_t i = 0; i < img.height; i++) {
        for (size_t j = 0; j < img.width; j++) {
            size_t index = i * img.width + j;
            printf("(%3.0f, %3.0f, %3.0f, %3.0f) ", 
                   img.data.data[index][0], 
                   img.data.data[index][1], 
                   img.data.data[index][2], 
                   img.data.data[index][3]);
        }
        printf("\n");
    }
}

Img img_read_png(FILE *fp) {
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = NULL;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if(!png) abort();

    png_infop info = png_create_info_struct(png);
    if(!info) abort();

    if(setjmp(png_jmpbuf(png))) abort();

    png_init_io(png, fp);

    png_read_info(png, info);

    width      = png_get_image_width(png, info);
    height     = png_get_image_height(png, info);
    color_type = png_get_color_type(png, info);
    bit_depth  = png_get_bit_depth(png, info);

    if(bit_depth == 16)
        png_set_strip_16(png);

    if(color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);

    if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);

    if(png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    if(color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

    if(color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for(int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
    }

    png_read_image(png, row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);

    Img output = img_alloc(width, height);
    for (int i = 0; i < height; i++)
    {
        png_bytep row = row_pointers[i];
        for (int j = 0; j < width; j++)
        {
            png_bytep px = &(row[j * 4]);
            size_t index = i * width + j;
            output.data.data[index][0] = px[0];
            output.data.data[index][1] = px[1];
            output.data.data[index][2] = px[2];
            output.data.data[index][3] = px[3];
        }
    }
    return output;
}
Img img_read_jpg(FILE *fp) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPARRAY buffer;
    int row_stride;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, fp);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    Img output = img_alloc(cinfo.output_width, cinfo.output_height);
    int index = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (int i = 0; i < cinfo.output_width; i++) {
            output.data.data[index][0] = buffer[0][i * cinfo.output_components];
            output.data.data[index][1] = buffer[0][i * cinfo.output_components + 1];
            output.data.data[index][2] = buffer[0][i * cinfo.output_components + 2];
            output.data.data[index][3] = 255;
            index++;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return output;
}

Img img_read(const char *imageName) {
    const char *extension = strrchr(imageName, '.');
    ALEMDAR_ASSERT(extension != NULL);
    ALEMDAR_ASSERT(extension != imageName);

    FILE *fp = fopen(imageName, "rb");
    ALEMDAR_ASSERT(fp != NULL);
    
    Img output;

    if(strcmp(extension, ".png") == 0) {
        output = img_read_png(fp);
    }else if(strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
        output = img_read_jpg(fp);
    }
    
    return output;
}
Img *imgs_read(const char *folderPath, size_t count) {
    DIR *dir;
    struct dirent *entry;
    size_t file_count = 0;

    dir = opendir(folderPath);
    ALEMDAR_ASSERT(dir != NULL);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            file_count++;
        }
    }
    ALEMDAR_ASSERT(file_count > count);
    Img *output = ALEMDAR_MALLOC(sizeof(Img) * count);
    size_t i = 0;
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char fullPath[256];
            snprintf(fullPath,sizeof(fullPath), "%s%s", folderPath, entry->d_name);
            output[i] = img_read(fullPath);
            i++;
        }
        if(count == i) break;
    }
    return output;
}
Img img_resize(Img img, size_t width, size_t height) {
    Img output = img_alloc(height, width);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            output.data.data[i * width + j] = img.data.data[(i * img.width / width) * img.width + (j * img.height / height)];
        }
    }
    return output;
}
Img *imgs_resize(Img *imgs, size_t count, size_t width, size_t height) {
    Img *output = ALEMDAR_MALLOC(sizeof(Img) * count);
    for (size_t i = 0; i < count; i++)
    {
        output[i] = img_resize(imgs[i], 10, 10);
    }
    return output;
}
#endif // ALEMDAR_IMG_IMPLEMENTATION

#endif // !ALEMDAR_IMG_H