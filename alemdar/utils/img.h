#ifndef ALEMDAR_IMG_H

#define ALEMDAR_IMG_H

#ifdef ALEMDAR_IMG_IMPLEMENTATION

#include <png.h>
#include <jpeglib.h>
#include <sys/types.h>
#include <dirent.h>

ALEMDAR_DEF Matrix img_read_png(FILE *fp);
ALEMDAR_DEF Matrix img_read_jpg(FILE *fp);
ALEMDAR_DEF Matrix img_read(const char *imageName);
ALEMDAR_DEF Matrix *imgs_read(const char *folderPath, size_t count);
ALEMDAR_DEF Matrix img_resize(Matrix img, size_t width, size_t height);
ALEMDAR_DEF Matrix *imgs_resize(Matrix *imgs, size_t count, size_t width, size_t height);

Matrix img_read_png(FILE *fp) {
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

    Matrix output = matrix_alloc(height * width, 4);
    for (int i = 0; i < height; i++)
    {
        png_bytep row = row_pointers[i];
        for (int j = 0; j < width; j++)
        {
            png_bytep px = &(row[j * 4]);
            size_t index = i * width + j;
            output.data[index][0] = px[0];
            output.data[index][1] = px[1];
            output.data[index][2] = px[2];
            output.data[index][3] = px[3];
        }
    }
    return output;
}
Matrix img_read_jpg(FILE *fp) {
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

    Matrix output = matrix_alloc(cinfo.output_width * cinfo.output_height, 4);
    int index = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (int i = 0; i < cinfo.output_width; i++) {
            output.data[index][0] = buffer[0][i * cinfo.output_components];
            output.data[index][1] = buffer[0][i * cinfo.output_components + 1];
            output.data[index][2] = buffer[0][i * cinfo.output_components + 2];
            output.data[index][3] = 255;
            index++;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return output;
}

Matrix img_read(const char *imageName) {
    const char *extension = strrchr(imageName, '.');
    ALEMDAR_ASSERT(extension != NULL);
    ALEMDAR_ASSERT(extension != imageName);

    FILE *fp = fopen(imageName, "rb");
    ALEMDAR_ASSERT(fp != NULL);
    
    Matrix output;

    if(strcmp(extension, ".png") == 0) {
        output = img_read_png(fp);
    }else if(strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
        output = img_read_jpg(fp);
    }
    
    return output;
}
Matrix *imgs_read(const char *folderPath, size_t count) {
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
    Matrix *output = ALEMDAR_MALLOC(sizeof(Matrix) * count);
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
Matrix img_resize(Matrix img, size_t width, size_t height) {
    Matrix output = matrix_alloc(height * width, 4);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            output.data[i * width + j] = img.data[(i * img.cols / width) * img.cols + (j * img.rows / height)];
        }
    }
    return output;
}
Matrix *imgs_resize(Matrix *imgs, size_t count, size_t width, size_t height) {
    Matrix *output = ALEMDAR_MALLOC(sizeof(Matrix) * count);
    for (size_t i = 0; i < count; i++)
    {
        output[i] = img_resize(imgs[i], 10, 10);
    }
    return output;
}
#endif // ALEMDAR_IMG_IMPLEMENTATION

#endif // !ALEMDAR_IMG_H