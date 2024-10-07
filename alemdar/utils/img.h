#ifndef ALEMDAR_IMG_H

#define ALEMDAR_IMG_H

#ifdef ALEMDAR_IMG_IMPLEMENTATION

#include <png.h>
#include <jpeglib.h>
#include <sys/types.h>
#include <dirent.h>

typedef enum {
    RGBA,
    RGB,
    GRAY
} ImgTypes;
static const char* ImgTypesChar[] = {
	[RGBA] = "RGBA",
	[RGB] = "RGB",
	[GRAY] = "GRAY",
};
static const int ImgTypesSize[] = {
    [RGBA] = 4,
    [RGB] = 3,
    [GRAY] = 1,
};

typedef struct {
    size_t width;
    size_t height;
    ImgTypes type;
    Matrix data;
} Img;
typedef struct Imgs Imgs;
struct Imgs {
    char **paths;
    size_t limit;
    size_t currentImg;
    size_t size;
};

ALEMDAR_DEF Img img_alloc(size_t width, size_t height, ImgTypes type);
ALEMDAR_DEF void img_free(Img img);
ALEMDAR_DEF Img img_read_png(FILE *fp, ImgTypes type);
ALEMDAR_DEF Img img_read_jpg(FILE *fp, ImgTypes type);
ALEMDAR_DEF Img img_read(const char *imageName, ImgTypes type);
ALEMDAR_DEF Img *imgs_read(const char *folderPath, size_t count, ImgTypes type);
ALEMDAR_DEF Img img_resize(Img img, size_t width, size_t height);
ALEMDAR_DEF Img *imgs_resize(Img *imgs, size_t count, size_t width, size_t height);

Imgs imgs_alloc(const char *folderPath) {
    Imgs imgs;
    imgs.currentImg = 0;
    imgs.limit = 0;

    DIR *dir = opendir(folderPath);
    if (dir == NULL) {
        perror("Klasör açılamadı");
        imgs.paths = NULL;
        return imgs;
    }

    struct dirent *entry;
    size_t pathCount = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            pathCount++;
        }
    }

    imgs.paths = malloc(pathCount * sizeof(char *));
    if (!imgs.paths) {
        perror("Dosya yolları için bellek ayrılamadı");
        closedir(dir);
        return imgs;
    }

    imgs.size = pathCount;

    rewinddir(dir);
    size_t index = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            imgs.paths[index] = malloc(strlen(folderPath) + strlen(entry->d_name) + 2);
            if (imgs.paths[index]) {
                snprintf(imgs.paths[index], strlen(folderPath) + strlen(entry->d_name) + 2, "%s/%s", folderPath, entry->d_name);
                index++;
            }
        }
    }

    imgs.limit = pathCount;
    closedir(dir);
    return imgs;
}

void imgs_free(Imgs *imgs) {
    if (imgs->paths) {
        for (size_t i = 0; i < imgs->limit; i++) {
            free(imgs->paths[i]);
        }
        free(imgs->paths);
    }
}

Img imgs_get(Imgs *imgs) {
    if (imgs->currentImg >= imgs->limit) {
        imgs->currentImg = 0;
    }
    const char *currentPath = imgs->paths[imgs->currentImg];
    imgs->currentImg++;
    return img_read(currentPath, RGBA);
}


Img img_alloc(size_t width, size_t height, ImgTypes type) {
    Img img;
    img.width = width;
    img.height = height;
    img.type = type;
    img.data = matrix_alloc(height * width, ImgTypesSize[type]);
    return img;
}

void img_free(Img img) {
    matrix_free(img.data);
}

void img_print(Img img) {
    printf("type: %s, width: %zu, height: %zu\n", ImgTypesChar[img.type], img.width, img.height);
    for (size_t i = 0; i < img.height; i++) {
        for (size_t j = 0; j < img.width; j++) {
            size_t index = i * img.width + j;
            if(img.type == RGBA) {
                printf("(%3.0f, %3.0f, %3.0f, %3.0f) ", 
                   img.data.data[index][0], 
                   img.data.data[index][1], 
                   img.data.data[index][2], 
                   img.data.data[index][3]);
            }else if(img.type == RGB) {
                printf("(%3.0f, %3.0f, %3.0f) ", 
                   img.data.data[index][0], 
                   img.data.data[index][1], 
                   img.data.data[index][2]);
            }else if(img.type == GRAY) {
                printf("(%3.0f) ", 
                   img.data.data[index][0]);
            }
        }
        printf("\n");
    }
}

Img img_read_png(FILE *fp, ImgTypes type) {
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

    Img output = img_alloc(width, height, type);
    for (int i = 0; i < height; i++)
    {
        png_bytep row = row_pointers[i];
        for (int j = 0; j < width; j++)
        {
            png_bytep px = &(row[j * 4]);
            size_t index = i * width + j;
            if(type == RGBA) {
                output.data.data[index][0] = px[0];
                output.data.data[index][1] = px[1];
                output.data.data[index][2] = px[2];
                output.data.data[index][3] = px[3];
            }else if(type == RGB) {
                output.data.data[index][0] = px[0];
                output.data.data[index][1] = px[1];
                output.data.data[index][2] = px[2];
            }else if(type == GRAY) {
                output.data.data[index][0] = px[0] * 0.229 + px[1] * 0.587 + px[2] * 0.114;
            }
        }
    }
    for(int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    return output;
}
Img img_read_jpg(FILE *fp, ImgTypes type) {
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

    Img output = img_alloc(cinfo.output_width, cinfo.output_height, type);
    int index = 0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (int i = 0; i < cinfo.output_width; i++) {
            if(type == RGBA) {
                output.data.data[index][0] = buffer[0][i * cinfo.output_components];
                output.data.data[index][1] = buffer[0][i * cinfo.output_components + 1];
                output.data.data[index][2] = buffer[0][i * cinfo.output_components + 2];
                output.data.data[index][3] = 255;
            }else if(type == RGB) {
                output.data.data[index][0] = buffer[0][i * cinfo.output_components];
                output.data.data[index][1] = buffer[0][i * cinfo.output_components + 1];
                output.data.data[index][2] = buffer[0][i * cinfo.output_components + 2];
            }else if(type == GRAY) {
                output.data.data[index][0] = buffer[0][i * cinfo.output_components] * 0.229 + buffer[0][i * cinfo.output_components + 1] * 0.587 + buffer[0][i * cinfo.output_components + 2] * 0.114;
            }
            index++;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(fp);

    return output;
}
Img img_read(const char *imageName, ImgTypes type) {
    const char *extension = strrchr(imageName, '.');
    ALEMDAR_ASSERT(extension != NULL);
    ALEMDAR_ASSERT(extension != imageName);

    FILE *fp = fopen(imageName, "rb");
    ALEMDAR_ASSERT(fp != NULL);
    
    Img output;

    if(strcmp(extension, ".png") == 0) {
        output = img_read_png(fp, type);
    }else if(strcmp(extension, ".jpg") == 0 || strcmp(extension, ".jpeg") == 0) {
        output = img_read_jpg(fp, type);
    }
    
    return output;
}
Img *imgs_read(const char *folderPath, size_t count, ImgTypes type) {
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
    return output;
}
Img img_resize(Img img, size_t width, size_t height) {
    Img output = img_alloc(height, width, img.type);
    float scale_x = (float)img.width / width;
    float scale_y = (float)img.height / height;

    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            size_t orig_x = (size_t)(j * scale_x);
            size_t orig_y = (size_t)(i * scale_y);
            output.data.data[i * width + j] = img.data.data[orig_y * img.width + orig_x];
        }
    }
    return output;
}
Img *imgs_resize(Img *imgs, size_t count, size_t width, size_t height) {
    Img *output = ALEMDAR_MALLOC(sizeof(Img) * count);
    for (size_t i = 0; i < count; i++)
    {
        output[i] = img_resize(imgs[i], width, height);
    }
    return output;
}

Vec img_to_vec(Img img) {
    Vec output = vec_alloc(img.width * img.height * img.data.cols);
    size_t index = 0;
    for (size_t i = 0; i < img.height; i++) {
        for (size_t j = 0; j < img.width; j++) {
            for (size_t k = 0; k < img.data.cols; k++) {
                output.data[index++] = img.data.data[i][j * img.data.cols + k];
            }
        }
    }
    return output;
}

Matrix imgs_to_matrix(Img *imgs, size_t count) {
    Matrix output = matrix_alloc(count, imgs[0].width * imgs[0].height * imgs[0].data.cols);
    for (size_t c = 0; c < count; c++)
    {
        size_t index = 0;
        Img img = imgs[c];
        for (size_t i = 0; i < img.height; i++) {
            for (size_t j = 0; j < img.width; j++) {
                for (size_t k = 0; k < img.data.cols; k++) {
                    output.data[c][index++] = img.data.data[i][j * img.data.cols + k];
                }
            }
        }
    }
    return output;
}
#endif // ALEMDAR_IMG_IMPLEMENTATION

#endif // !ALEMDAR_IMG_H