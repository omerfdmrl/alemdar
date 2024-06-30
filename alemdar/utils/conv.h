#ifndef ALEMDAR_CONV_H

#define ALEMDAR_CONV_H

#ifdef ALEMDAR_CONV_IMPLEMENTATION

#ifndef CONV_PADDING
#define CONV_PADDING 0
#endif // !CONV_PADDING

#ifndef CONV_STRIDE
#define CONV_STRIDE 0
#endif // !CONV_STRIDE

#ifndef CONV_DILATION
#define CONV_DILATION 0
#endif // !CONV_DILATION

ALEMDAR_DEF void max_pooling_2d(Matrix *matrix, int pool_size);
ALEMDAR_DEF void min_pooling_2d(Matrix *matrix, int pool_size);
ALEMDAR_DEF void mean_pooling_2d(Matrix *matrix, size_t pool_size);
ALEMDAR_DEF void conv_2d(Matrix *matrix, size_t kernel_size);

void max_pooling_2d(Matrix *matrix, int pool_size) {
    int i, j, out_index = 0;
    int output_size = matrix->cols / pool_size;
    Matrix output = matrix_alloc(matrix->rows, output_size);

    for (size_t k = 0; k < matrix->rows; k++) {
        for (i = 0; i < matrix->cols; i += pool_size) {
            float max_val = matrix->data[k][i];
            for (j = 1; j < pool_size; j++) {
                if (matrix->data[k][i + j] > max_val) {
                    max_val = matrix->data[k][i + j];
                }
            }
            output.data[k][out_index++] = max_val;
        }
        out_index = 0;
    }
    matrix_free(*matrix);
    *matrix = output;
}
void min_pooling_2d(Matrix *matrix, int pool_size) {
    int i, j, out_index = 0;
    int output_size = matrix->cols / pool_size;
    Matrix output = matrix_alloc(matrix->rows, output_size);

    for (size_t k = 0; k < matrix->rows; k++) {
        for (i = 0; i < matrix->cols; i += pool_size) {
            float min_val = matrix->data[k][i];
            for (j = 1; j < pool_size; j++) {
                if (matrix->data[k][i + j] < min_val) {
                    min_val = matrix->data[k][i + j];
                }
            }
            output.data[k][out_index++] = min_val;
        }
        out_index = 0;
    }
    matrix_free(*matrix);
    *matrix = output;
}
void mean_pooling_2d(Matrix *matrix, size_t pool_size) {
    size_t out_index = 0;
    size_t output_size = matrix->cols / pool_size;
    Matrix output = matrix_alloc(matrix->rows, output_size);

    for (size_t k = 0; k < matrix->rows; k++) {
        for (size_t i = 0; i < matrix->cols; i += pool_size) {
            float mean_val = matrix->data[k][i];
            for (size_t j = 1; j < pool_size; j++) {
                mean_val += matrix->data[k][j];
            }
            output.data[k][out_index++] = mean_val / pool_size;
        }
        out_index = 0;
    }
    matrix_free(*matrix);
    *matrix = output;
}
void conv_2d(Matrix *matrix, size_t kernel_size) {
    size_t out_index = 0;
    size_t output_size = (matrix->cols + 2 * CONV_PADDING - CONV_DILATION * (kernel_size - 1) - 1) / (CONV_STRIDE == 0 ? 1 : CONV_STRIDE) + 1;
    Matrix output = matrix_alloc(matrix->rows, output_size);
    
    for (size_t k = 0; k < matrix->rows; k++)
    {
        for (size_t i = 0; i < output_size; i += kernel_size + CONV_STRIDE) {
            output.data[k][out_index] = 0;
            for (size_t j = 0; j < kernel_size; j += 1 + CONV_DILATION) {
                if((j == 0 && CONV_PADDING - j > 0) || (j + CONV_PADDING >= kernel_size)) {
                    j++;
                }
                output.data[k][out_index] += matrix->data[k][i + j] * random_uniform(0, 1);
            }
            out_index++;
        }
        out_index = 0;
    }
    matrix_free(*matrix);
    *matrix = output;
}

#endif // ALEMDAR_CONV_IMPLEMENTATION

#endif // !ALEMDAR_CONV_H