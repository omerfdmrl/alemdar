#ifndef IMG_H

#define IMG_H

#include "ican.h"

Iray3D *img_read_png(FILE *fp, ImgTypes type) {
    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = NULL;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    ISERT_MSG(png != NULL, "Png struct could not created");

    png_infop info = png_create_info_struct(png);
    ISERT_MSG(info != NULL, "Png struct informations could not created");

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

    png_destroy_read_struct(&png, &info, NULL);

    Iray3D *output = iray3d_alloc(height, width, ImgTypesSize[type]);

    for (int i = 0; i < height; i++)
    {
        png_bytep row = row_pointers[i];
        for (int j = 0; j < width; j++)
        {
            png_bytep px = &(row[j * 4]);
            if(type == IMG_RGBA) {
                output->data[i][j][0] = px[0];
                output->data[i][j][1] = px[1];
                output->data[i][j][2] = px[2];
                output->data[i][j][3] = px[3];
            }else if(type == IMG_RGB) {
                output->data[i][j][0] = px[0];
                output->data[i][j][1] = px[1];
                output->data[i][j][2] = px[2];
            }else if(type == IMG_GRAY) {
                output->data[i][j][0] = px[0] * 0.229 + px[1] * 0.587 + px[2] * 0.114;
            }
        }
    }
    for(int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    return output;
}

Iray3D *img_read_jpg(FILE *fp, ImgTypes type) {
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

    Iray3D *output = iray3d_alloc(cinfo.output_height, cinfo.output_width, ImgTypesSize[type]);

    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    for (unsigned int y = 0; y < cinfo.output_height; y++) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        
        for (unsigned int x = 0; x < cinfo.output_width; x++) {
            if (type == IMG_RGBA) {
                output->data[y][x][0] = buffer[0][x * cinfo.output_components];
                output->data[y][x][1] = buffer[0][x * cinfo.output_components + 1];
                output->data[y][x][2] = buffer[0][x * cinfo.output_components + 2];
                output->data[y][x][3] = 255;
            } else if (type == IMG_RGB) {
                output->data[y][x][0] = buffer[0][x * cinfo.output_components];
                output->data[y][x][1] = buffer[0][x * cinfo.output_components + 1];
                output->data[y][x][2] = buffer[0][x * cinfo.output_components + 2];
            } else if (type == IMG_GRAY) {
                output->data[y][x][0] = 
                    buffer[0][x * cinfo.output_components] * 0.229 + 
                    buffer[0][x * cinfo.output_components + 1] * 0.587 + 
                    buffer[0][x * cinfo.output_components + 2] * 0.114;
            }
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return output;
}

Iray3D *img_read(const char *imageName, ImgTypes type) {
    const char *extension = strrchr(imageName, '.');
    ISERT_MSG(extension != NULL, "Image extension could not detected");

    FILE *fp = fopen(imageName, "rb");
    ISERT_MSG(fp != NULL, "Image could not loaded");
    
    Iray3D *output;

    if(strcmp(extension, ".png") == 0) {
        output = img_read_png(fp, type);
    }else if(strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
        output = img_read_jpg(fp, type);
    }

    fclose(fp);

    return output;
}

Iray3D *img_resize(Iray3D *img, size_t width, size_t height) {
    float x_scale_factor = (float)img->cols / width;
    float y_scale_factor = (float)img->rows / height;

    Iray3D *output = iray3d_alloc(height, width, img->depth);

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            size_t src_x = (size_t)j * x_scale_factor;
            size_t src_y = (size_t)i * y_scale_factor;
            for (size_t k = 0; k < img->depth; k++) {
                output->data[i][j][k] = img->data[src_y][src_x][k];
            }
        }
    }
    return output;
}

Iray3D *img_rotate(Iray3D *img, float angle) {
    float radians = angle * (PI / 180.0);

    int new_width = (int)(fabs(img->cols * cos(radians)) + fabs(img->rows * sin(radians)));
    int new_height = (int)(fabs(img->rows * cos(radians)) + fabs(img->cols * sin(radians)));

    Iray3D *output = iray3d_alloc(new_height, new_width, img->depth);
    ISERT_MSG(output != NULL, "Output image allocation failed");

    float center_x = img->cols / 2.0;
    float center_y = img->rows / 2.0;
    float new_center_x = new_width / 2.0;
    float new_center_y = new_height / 2.0;

    for (int y = 0; y < new_height; y++) {
        for (int x = 0; x < new_width; x++) {
            float relative_x = x - new_center_x;
            float relative_y = y - new_center_y;

            float original_x = relative_x * cos(-radians) - relative_y * sin(-radians) + center_x;
            float original_y = relative_x * sin(-radians) + relative_y * cos(-radians) + center_y;

            if (original_x >= 0 && original_x < img->cols && original_y >= 0 && original_y < img->rows) {
                int src_x = (int)original_x;
                int src_y = (int)original_y;

                for (size_t k = 0; k < img->depth; k++) {
                    output->data[y][x][k] = img->data[src_y][src_x][k];
                }
            } else {
                for (size_t k = 0; k < img->depth; k++) {
                    output->data[y][x][k] = 0;
                }
            }
        }
    }

    return output;
}

Iray3D **imgs_read(const char *folderPath, size_t count, ImgTypes type) {
    DIR *dir;
    struct dirent *entry;
    size_t file_count = 0;

    dir = opendir(folderPath);
    ISERT_MSG(dir != NULL, "Dir could not read");

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            file_count++;
        }
    }
    ISERT_MSG(file_count >= count, "Count is bigger than file count");
    Iray3D **output = malloc(sizeof(Iray3D *) * count);
    size_t i = 0;
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            const char *extension = strrchr(entry->d_name, '.');
            if(strcmp(extension, ".png") == 0 || strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
                char fullPath[256];
                snprintf(fullPath,sizeof(fullPath), "%s%s", folderPath, entry->d_name);
                output[i] = img_read(fullPath, type);
                i++;
            }
        }
        if(count == i) break;
    }
    closedir(dir);
    return output;
}

void imgs_free(Iray3D **imgs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        iray3d_free(imgs[i]);
    }
    free(imgs);
}

#endif // !IMG_H