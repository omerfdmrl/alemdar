#ifndef ALEMDAR_MATRIX_H

#define ALEMDAR_MATRIX_H

typedef struct {
    size_t rows;
    size_t cols;
    float **data;
} Matrix;

ALEMDAR_DEF Matrix matrix_alloc(size_t rows, size_t cols);
ALEMDAR_DEF Matrix matrix_transpose(Matrix matrix);
ALEMDAR_DEF Matrix matrix_dot(Matrix A, Matrix B);
ALEMDAR_DEF Matrix matrix_add(Matrix A, Matrix B);
ALEMDAR_DEF void matrix_apply(Matrix matrix, float(*fn)(float value));
ALEMDAR_DEF void matrix_add_vec(Matrix matrix, Vec vec);
ALEMDAR_DEF Vec matrix_dot_vec(Matrix matrix, Vec vec);
ALEMDAR_DEF void matrix_print(Matrix matrix);
ALEMDAR_DEF void matrix_free(Matrix matrix);

Matrix matrix_alloc(size_t rows, size_t cols) {
    Matrix matrix;
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.data = (float **)ALEMDAR_MALLOC(matrix.rows * sizeof(*matrix.data));
    for (size_t i = 0; i < matrix.rows; i++) {
        matrix.data[i] = (float *)ALEMDAR_MALLOC(matrix.cols * sizeof(*matrix.data));
        ALEMDAR_ASSERT(matrix.data[i] != NULL);
    }
    ALEMDAR_ASSERT(matrix.data != NULL);
    return matrix;
}
Matrix matrix_transpose(Matrix matrix) {
    Matrix transposed = matrix_alloc(matrix.cols, matrix.rows);
    for (size_t i = 0; i < matrix.rows; i++)
    {
        for (size_t j = 0; j < matrix.cols; j++)
        {
            transposed.data[j][i] = matrix.data[i][j];
        }
    }
    return transposed;
}
Matrix matrix_dot(Matrix A, Matrix B) {
    ALEMDAR_ASSERT(A.cols != B.rows);
    Matrix dotProduct = matrix_alloc(A.cols, B.rows);
    for (size_t i = 0; i < A.cols; i++)
    {
        for (size_t j = 0; j < A.rows; j++)
        {
            dotProduct.data[i][j] = 0;
            for (size_t k = 0; k < B.rows; k++)
            {
                dotProduct.data[i][j] += A.data[i][k] * B.data[k][j];
            }
        }
    }
    return dotProduct;
}
Matrix matrix_add(Matrix A, Matrix B) {
    ALEMDAR_ASSERT(A.rows == B.rows);
    ALEMDAR_ASSERT(A.cols == B.cols);
    Matrix sum = matrix_alloc(A.rows, B.cols);
    for (size_t i = 0; i < A.rows; i++)
    {
        for (size_t j = 0; j < A.cols; j++)
        {
            sum.data[i][j] += A.data[i][j] + B.data[i][j];
        }
    }
    return sum;
}
void matrix_apply(Matrix matrix, float(*fn)(float value)) {
    for (size_t i = 0; i < matrix.rows; i++)
    {
        for (size_t j = 0; j < matrix.cols; j++)
        {
            matrix.data[i][j] = fn(matrix.data[i][j]);
        }
    }
}
void matrix_add_vec(Matrix matrix, Vec vec) {
    ALEMDAR_ASSERT(matrix.cols == vec.size);
    for (size_t i = 0; i < matrix.rows; i++)
    {
        for (size_t j = 0; j < matrix.cols; j++)
        {
            for (size_t k = 0; k < vec.size; k++)
            {
                matrix.data[i][j] += vec.data[k];
            }
        }
    }
}
Vec matrix_dot_vec(Matrix matrix, Vec vec) {
    ALEMDAR_ASSERT(matrix.rows == vec.size);
    Vec output = vec_alloc(matrix.cols);
    for (size_t i = 0; i < matrix.cols; i++)
    {
        output.data[i] = 0;
        for (size_t j = 0; j < matrix.rows; j++) {
            output.data[i] += matrix.data[j][i] * vec.data[j];
        }
    }
    return output;
}
void matrix_print(Matrix matrix) {
    printf("rows = %zu, cols = %zu\n", matrix.rows, matrix.cols);
    for (size_t i = 0; i < matrix.rows; i++) {
        printf("[");
        for (size_t j = 0; j < matrix.cols; j++) {
            printf(" %.3f", matrix.data[i][j]);
        }
        printf(" ]\n");
    }
    printf("\n");
}
void matrix_free(Matrix matrix) {
    for (size_t i = 0; i < matrix.rows; i++)
    {
        ALEMDAR_FREE(matrix.data[i]);
    }
    ALEMDAR_FREE(matrix.data);
}

#endif // ALEMDAR_MATRIX_H