#ifndef ALEMDAR_CONV_H

#define ALEMDAR_CONV_H

#ifdef ALEMDAR_CONV_IMPLEMENTATION
#ifdef ALEMDAR_IMG_IMPLEMENTATION

float CONV_EMBOSS[3][3] = {{-1, -1, 0}, {-1, 0, 1}, {0, -1, -1}};
float CONV_EMBOSS2[3][3] = {{-2, -1, 0}, {-1, 1, 1}, {0, 1, 2}};
float CONV_LAPLACIAN[3][3] = {{1, 1, 1}, {1, -8, 1}, {1, 1, 1}};
float CONV_BLUR[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
float CONV_SOBELX[3][3] = {{1, 0, -2}, {2, 0, -2}, {1, 0, 1}};
float CONV_SOBELY[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};



ALEMDAR_DEF Img img_conv(Img img, float kernel[3][3], size_t stride);
ALEMDAR_DEF Img img_max_pool(Img img, size_t pool_size);
ALEMDAR_DEF Img img_min_pool(Img img, size_t pool_size);
ALEMDAR_DEF Img img_mean_pool(Img img, size_t pool_size);
ALEMDAR_DEF Img img_edge_detect(Img img);

ALEMDAR_DEF Img img_conv(Img img, float kernel[3][3], size_t stride) {
    size_t new_width = (img.width - 3) / stride + 1;
    size_t new_height = (img.height - 3) / stride + 1;

    Img output = img_alloc(new_width, new_height);

    for (size_t i = 0; i < new_height; i++) {
        for (size_t j = 0; j < new_width; j++) {
            float sum[4] = {0, 0, 0, 0};

            for (size_t ki = 0; ki < 3; ki++) {
                for (size_t kj = 0; kj < 3; kj++) {
                    size_t img_x = j * stride + kj;
                    size_t img_y = i * stride + ki;
                    size_t index = img_y * img.width + img_x;

                    for (size_t channel = 0; channel < 4; channel++) {
                        sum[channel] += img.data.data[index][channel] * kernel[ki][kj];
                    }
                }
            }

            size_t output_index = i * new_width + j;
            for (size_t channel = 0; channel < 4; channel++) {
                output.data.data[output_index][channel] = sum[channel];
            }
        }
    }

    return output;
}

Img img_max_pool(Img img, size_t pool_size) {
    size_t new_width = img.width / pool_size;
    size_t new_height = img.height / pool_size;
    
    Img output = img_alloc(new_width, new_height);
    
    for (size_t i = 0; i < new_height; i++) {
        for (size_t j = 0; j < new_width; j++) {
            float max_val[4] = {0, 0, 0, 0};
            for (size_t ki = 0; ki < pool_size; ki++) {
                for (size_t kj = 0; kj < pool_size; kj++) {
                    size_t img_x = j * pool_size + kj;
                    size_t img_y = i * pool_size + ki;
                    size_t index = img_y * img.width + img_x;
                    
                    for (size_t channel = 0; channel < 4; channel++) {
                        if (img.data.data[index][channel] > max_val[channel]) {
                            max_val[channel] = img.data.data[index][channel];
                        }
                    }
                }
            }
            
            size_t output_index = i * new_width + j;
            for (size_t channel = 0; channel < 4; channel++) {
                output.data.data[output_index][channel] = max_val[channel];
            }
        }
    }
    
    return output;
}

Img img_min_pool(Img img, size_t pool_size) {
    size_t new_width = img.width / pool_size;
    size_t new_height = img.height / pool_size;
    
    Img output = img_alloc(new_width, new_height);
    
    for (size_t i = 0; i < new_height; i++) {
        for (size_t j = 0; j < new_width; j++) {
            float min_val[4] = {FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX};
            
            for (size_t ki = 0; ki < pool_size; ki++) {
                for (size_t kj = 0; kj < pool_size; kj++) {
                    size_t img_x = j * pool_size + kj;
                    size_t img_y = i * pool_size + ki;
                    size_t index = img_y * img.width + img_x;
                    
                    for (size_t channel = 0; channel < 4; channel++) {
                        if (img.data.data[index][channel] > min_val[channel]) {
                            min_val[channel] = img.data.data[index][channel];
                        }
                    }
                }
            }
            
            size_t output_index = i * new_width + j;
            for (size_t channel = 0; channel < 4; channel++) {
                output.data.data[output_index][channel] = min_val[channel];
            }
        }
    }
    
    return output;
}

Img img_mean_pool(Img img, size_t pool_size) {
    size_t new_width = img.width / pool_size;
    size_t new_height = img.height / pool_size;

    Img output = img_alloc(new_width, new_height);

    for (size_t i = 0; i < new_height; i++) {
        for (size_t j = 0; j < new_width; j++) {
            float sum_val[4] = {0, 0, 0, 0};

            for (size_t ki = 0; ki < pool_size; ki++) {
                for (size_t kj = 0; kj < pool_size; kj++) {
                    size_t img_x = j * pool_size + kj;
                    size_t img_y = i * pool_size + ki;
                    size_t index = img_y * img.width + img_x;

                    // Her kanalı toplayın
                    for (size_t channel = 0; channel < 4; channel++) {
                        sum_val[channel] += img.data.data[index][channel];
                    }
                }
            }

            size_t output_index = i * new_width + j;
            for (size_t channel = 0; channel < 4; channel++) {
                output.data.data[output_index][channel] = sum_val[channel] / (pool_size * pool_size);  // Ortalama
            }
        }
    }

    return output;
}


Img img_edge_detect(Img img) {
    Img grad_x = img_conv(img, CONV_SOBELX, 1);

    Img grad_y = img_conv(img, CONV_SOBELY, 1);

    size_t new_width = grad_x.width;
    size_t new_height = grad_x.height;
    Img output = img_alloc(new_width, new_height);

    for (size_t i = 0; i < new_height; i++) {
        for (size_t j = 0; j < new_width; j++) {
            size_t index = i * new_width + j;

            float gx = grad_x.data.data[index][0];
            float gy = grad_y.data.data[index][0];
            float magnitude = sqrtf(gx * gx + gy * gy);

            output.data.data[index][0] = fminf(fmaxf(magnitude, 0), 255);
            output.data.data[index][1] = output.data.data[index][0];
            output.data.data[index][2] = output.data.data[index][0];
            output.data.data[index][3] = 255;
        }
    }

    img_free(grad_x);
    img_free(grad_y);

    return output;
}

Img* imgs_edge_detect(Img* imgs, size_t count) {
    Img *output = ALEMDAR_MALLOC(sizeof(Img) * count);
    for (size_t i = 0; i < count; i++)
    {
        output[i] = img_edge_detect(imgs[i]);
    }
    return output;
}
Img* imgs_max_pool(Img* imgs, size_t count, size_t pool_size) {
    Img *output = ALEMDAR_MALLOC(sizeof(Img) * count);
    for (size_t i = 0; i < count; i++)
    {
        output[i] = img_max_pool(imgs[i], pool_size);
    }
    return output;
}

#endif // !ALEMDAR_IMG_IMPLEMENTATION
#endif // !ALEMDAR_CONV_IMPLEMENTATION

#endif // !ALEMDAR_CONV_H