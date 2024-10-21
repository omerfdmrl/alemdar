#include "../src/ican.h"
#include "src/unity.h"

float square(float x) {
    return x * x;
}

void setUp()
{

}

void tearDown()
{

}

void test_iray1d_alloc(void) {
    size_t size = 5;
    Iray1D *A = iray1d_alloc(size);
    TEST_ASSERT_NOT_NULL(A);
    TEST_ASSERT_EQUAL_size_t(size, A->rows);

    for (size_t i = 0; i < size; i++)
    {
        A->data[i] = i * 2.0f;
        TEST_ASSERT_EQUAL_FLOAT(i * 2.0f, A->data[i]);
    }
    iray1d_free(A);
}

void test_iray1d_free(void) {
    size_t size = 5;
    Iray1D *A = iray1d_alloc(size);
    iray1d_free(A);
    TEST_ASSERT_NOT_EQUAL_size_t(size, A->rows);
    A = NULL;
    TEST_ASSERT_NULL(A);
}

void test_iray1d_fill(void) {
    size_t size = 5;
    float value = 3.0f;
    Iray1D *A = iray1d_alloc(size);
    Iray1D *B = iray1d_fill(A, value);

    for (size_t i = 0; i < size; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(value, B->data[i]);
    }
    iray1d_free(A);
    iray1d_free(B);
}

void test_iray1d_add(void) {
    size_t size = 5;
    Iray1D *A = iray1d_alloc(size);
    Iray1D *B = iray1d_alloc(size);

    Iray1D *C = iray1d_fill(A, 2.0f);
    Iray1D *D = iray1d_fill(B, 3.0f);

    Iray1D *E = iray1d_add(C, D);

    for (size_t i = 0; i < size; i++)
    {
        TEST_ASSERT_EQUAL_FLOAT(5.0f, E->data[i]);
    }

    iray1d_free(A);
    iray1d_free(B);
    iray1d_free(C);
    iray1d_free(D);
    iray1d_free(E);
}

void test_iray1d_dot(void) {
    size_t size = 5;
    Iray1D *A = iray1d_alloc(size);
    Iray1D *B = iray1d_alloc(size);
    Iray1D *C = iray1d_fill(A, 2.0f);
    Iray1D *D = iray1d_fill(B, 3.0f);

    Iray1D *E = iray1d_dot(C, D);
    for (size_t i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL_FLOAT(6.0f, E->data[i]);
    }

    iray1d_free(A);
    iray1d_free(B);
    iray1d_free(C);
    iray1d_free(D);
    iray1d_free(E);
}

void test_iray1d_clone(void) {
    size_t size = 5;
    Iray1D *A = iray1d_alloc(size);
    Iray1D *B = iray1d_fill(A, 2.0f);

    Iray1D *C = iray1d_clone(B);
    for (size_t i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL_FLOAT(B->data[i], C->data[i]);
    }

    iray1d_free(A);
    iray1d_free(B);
    iray1d_free(C);
}

void test_iray1d_apply(void) {
    size_t size = 5;
    Iray1D *A = iray1d_alloc(size);
    Iray1D *B = iray1d_fill(A, 2.0f);

    Iray1D *C = iray1d_apply(B, square);
    for (size_t i = 0; i < size; i++) {
        TEST_ASSERT_EQUAL_FLOAT(4.0f, C->data[i]);
    }

    iray1d_free(A);
    iray1d_free(B);
    iray1d_free(C);
}

void test_iray1d_slice(void) {
    size_t size = 5;
    Iray1D *A = iray1d_alloc(size);
    Iray1D *B = iray1d_fill(A, 3.0f);
    Iray1D *C = iray1d_slice(B, 1, 4);

    for (size_t i = 1, j = 0; i < 4; i++, j++) {
        TEST_ASSERT_EQUAL_FLOAT(B->data[i], C->data[j]);
    }

    iray1d_free(A);
    iray1d_free(B);
    iray1d_free(C);
}

void test_iray2d_alloc(void) {
    size_t rows = 3, cols = 3;
    Iray2D *A = iray2d_alloc(rows, cols);
    TEST_ASSERT_NOT_NULL(A);
    TEST_ASSERT_EQUAL_size_t(rows, A->rows);
    TEST_ASSERT_EQUAL_size_t(cols, A->cols);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++)
        {
            A->data[i][j] = i * j * 2.0f;
            TEST_ASSERT_EQUAL_FLOAT(i * j * 2.0f, A->data[i][j]);
        }
        
    }
    
    iray2d_free(A);
}

void test_iray2d_free(void) {
    size_t rows = 3, cols = 3;
    Iray2D *A = iray2d_alloc(rows, cols);
    iray2d_free(A);
    TEST_ASSERT_NOT_EQUAL_size_t(rows, A->rows);
    TEST_ASSERT_NOT_EQUAL_size_t(cols, A->cols);
    A = NULL;
    TEST_ASSERT_NULL(A);
}

void test_iray2d_fill(void) {
    size_t rows = 3, cols = 3;
    float value = 2.0f;
    Iray2D *A = iray2d_alloc(rows, cols);
    Iray2D *B = iray2d_fill(A, value);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            TEST_ASSERT_EQUAL_FLOAT(value, B->data[i][j]);
        }
    }

    iray2d_free(A);
    iray2d_free(B);
}

void test_iray2d_add(void) {
    size_t rows = 3, cols = 3;
    Iray2D *A = iray2d_alloc(rows, cols);
    Iray2D *B = iray2d_alloc(rows, cols);
    Iray2D *C = iray2d_fill(A, 2.0f);
    Iray2D *D = iray2d_fill(B, 3.0f);

    Iray2D *E = iray2d_add(C, D);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            TEST_ASSERT_EQUAL_FLOAT(5.0f, E->data[i][j]);
        }
    }

    iray2d_free(A);
    iray2d_free(B);
    iray2d_free(C);
    iray2d_free(D);
    iray2d_free(E);
}

void test_iray2d_dot(void) {
    Iray2D *A = iray2d_alloc(2, 3);
    Iray2D *B = iray2d_alloc(3, 4);
    Iray2D *C = iray2d_fill(A, 2.0f);
    Iray2D *D = iray2d_fill(B, 3.0f);

    Iray2D *E = iray2d_dot(C, D);
    for (size_t i = 0; i < E->rows; i++) {
        for (size_t j = 0; j < E->cols; j++) {
            TEST_ASSERT_EQUAL_FLOAT(18.0f, E->data[i][j]);
        }
    }

    iray2d_free(A);
    iray2d_free(B);
    iray2d_free(C);
    iray2d_free(D);
    iray2d_free(E);
}

void test_iray2d_clone(void) {
    size_t rows = 3, cols = 3;
    Iray2D *A = iray2d_alloc(rows, cols);
    Iray2D *B = iray2d_fill(A, 2.0f);

    Iray2D *C = iray2d_clone(B);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            TEST_ASSERT_EQUAL_FLOAT(B->data[i][j], C->data[i][j]);
        }
    }

    iray2d_free(A);
    iray2d_free(B);
    iray2d_free(C);
}

void test_iray2d_transpose(void) {
    size_t rows = 2, cols = 3;
    Iray2D *A = iray2d_alloc(rows, cols);
    Iray2D *B = iray2d_fill(A, 1.0f);

    Iray2D *C = iray2d_transpose(B);
    TEST_ASSERT_EQUAL_INT(cols, C->rows);
    TEST_ASSERT_EQUAL_INT(rows, C->cols);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            TEST_ASSERT_EQUAL_FLOAT(B->data[i][j], C->data[j][i]);
        }
        
    }

    iray2d_free(A);
    iray2d_free(B);
    iray2d_free(C);
}

void test_iray2d_apply(void) {
    size_t rows = 3, cols = 3;
    float value = 2.0f;
    Iray2D *A = iray2d_alloc(rows, cols);
    Iray2D *B = iray2d_fill(A, value);
    Iray2D *C = iray2d_apply(B, square);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            TEST_ASSERT_EQUAL_FLOAT(4.0f, C->data[i][j]);
        }
    }

    iray2d_free(A);
    iray2d_free(B);
    iray2d_free(C);
}

void test_iray3d_alloc(void) {
    size_t rows = 3, cols = 3, depth = 3;
    Iray3D *A = iray3d_alloc(rows, cols, depth);
    TEST_ASSERT_NOT_NULL(A);
    TEST_ASSERT_EQUAL_size_t(rows, A->rows);
    TEST_ASSERT_EQUAL_size_t(cols, A->cols);
    TEST_ASSERT_EQUAL_size_t(depth, A->depth);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            for (size_t k = 0; k < depth; k++) {
                A->data[i][j][k] = i * j * k * 2.0f;
                TEST_ASSERT_EQUAL_FLOAT(i * j * k * 2.0f, A->data[i][j][k]);
            }
        }
    }
    
    iray3d_free(A);
}

void test_iray3d_free(void) {
    size_t rows = 3, cols = 3, depth = 3;
    Iray3D *A = iray3d_alloc(rows, cols, depth);
    iray3d_free(A);
    TEST_ASSERT_NOT_EQUAL_size_t(rows, A->rows);
    TEST_ASSERT_NOT_EQUAL_size_t(cols, A->cols);
    TEST_ASSERT_NOT_EQUAL_size_t(depth, A->depth);
    A = NULL;
    TEST_ASSERT_NULL(A);
}

void test_iray3d_fill(void) {
    size_t rows = 3, cols = 3, depth = 3;
    float value = 2.0f;
    Iray3D *A = iray3d_alloc(rows, cols, depth);
    Iray3D *B = iray3d_fill(A, value);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            for (size_t k = 0; k < depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(value, B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
}

void test_iray3d_add(void) {
    size_t rows = 3, cols = 3, depth = 3;
    Iray3D *A = iray3d_alloc(rows, cols, depth);
    Iray3D *B = iray3d_alloc(rows, cols, depth);
    Iray3D *C = iray3d_fill(A, 2.0f);
    Iray3D *D = iray3d_fill(B, 3.0f);

    Iray3D *E = iray3d_add(C, D);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            for (size_t k = 0; k < depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(5.0f, E->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
    iray3d_free(D);
    iray3d_free(E);
}

void test_iray3d_clone(void) {
    size_t rows = 3, cols = 3, depth = 3;
    Iray3D *A = iray3d_alloc(rows, cols, depth);
    Iray3D *B = iray3d_fill(A, 2.0f);

    Iray3D *C = iray3d_clone(B);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            for (size_t k = 0; k < depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(B->data[i][j][k], C->data[i][j][k]);
            }
            
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_iray3d_apply(void) {
    size_t rows = 3, cols = 3, depth = 3;
    Iray3D *A = iray3d_alloc(rows, cols, depth);
    Iray3D *B = iray3d_fill(A, 2.0f);

    Iray3D *C = iray3d_apply(B, square);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            for (size_t k = 0; k < depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(4.0f, C->data[i][j][k]);
            }
            
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_read_png(void) {
    Iray3D *A = img_read("./test/data/small/test.png", IMG_RGBA);

    float *l0 = (float *)malloc(4 * sizeof(float));
    float *l1 = (float *)malloc(4 * sizeof(float));
    float *l2 = (float *)malloc(4 * sizeof(float));
    float *l3 = (float *)malloc(4 * sizeof(float));

    l0[0] = 237.000; l0[1] = 28.000; l0[2] = 36.000; l0[3] = 255.000;
    l1[0] = 168.000; l1[1] = 230.000; l1[2] = 29.000; l1[3] = 255.000;
    l2[0] = 111.000; l2[1] = 49.000; l2[2] = 152.000; l2[3] = 255.000;
    l3[0] = 156.000; l3[1] = 90.000; l3[2] = 60.000; l3[3] = 255.000;

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l0, A->data[0][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l1, A->data[0][1], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l2, A->data[1][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l3, A->data[1][1], 4);

    iray3d_free(A);
    free(l0);
    free(l1);
    free(l2);
    free(l3);
}

void test_img_read_jpg(void) {
    Iray3D *A = img_read("./test/data/small/test.jpg", IMG_RGBA);

    float *l0 = (float *)malloc(4 * sizeof(float));
    float *l1 = (float *)malloc(4 * sizeof(float));
    float *l2 = (float *)malloc(4 * sizeof(float));
    float *l3 = (float *)malloc(4 * sizeof(float));

    l0[0] = 143.000; l0[1] = 74.000; l0[2] = 43.000; l0[3] = 255.000;
    l1[0] = 241.000; l1[1] = 172.000; l1[2] = 141.000; l1[3] = 255.000;
    l2[0] = 131.000; l2[1] = 62.000; l2[2] = 31.000; l2[3] = 255.000;
    l3[0] = 158.000; l3[1] = 89.000; l3[2] = 58.000; l3[3] = 255.000;

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l0, A->data[0][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l1, A->data[0][1], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l2, A->data[1][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l3, A->data[1][1], 4);

    iray3d_free(A);
    free(l0);
    free(l1);
    free(l2);
    free(l3);
}

void test_img_rgba(void) {
    Iray3D *A = img_read("./test/data/small/test.png", IMG_RGBA);

    TEST_ASSERT_EQUAL_size_t(4, A->depth);

    float *l0 = (float *)malloc(4 * sizeof(float));
    float *l1 = (float *)malloc(4 * sizeof(float));
    float *l2 = (float *)malloc(4 * sizeof(float));
    float *l3 = (float *)malloc(4 * sizeof(float));

    l0[0] = 237.000; l0[1] = 28.000; l0[2] = 36.000; l0[3] = 255.000;
    l1[0] = 168.000; l1[1] = 230.000; l1[2] = 29.000; l1[3] = 255.000;
    l2[0] = 111.000; l2[1] = 49.000; l2[2] = 152.000; l2[3] = 255.000;
    l3[0] = 156.000; l3[1] = 90.000; l3[2] = 60.000; l3[3] = 255.000;

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l0, A->data[0][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l1, A->data[0][1], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l2, A->data[1][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l3, A->data[1][1], 4);

    iray3d_free(A);
    free(l0);
    free(l1);
    free(l2);
    free(l3);
}

void test_img_rgb(void) {
    Iray3D *A = img_read("./test/data/small/test.png", IMG_RGB);

    TEST_ASSERT_EQUAL_size_t(3, A->depth);

    float *l0 = (float *)malloc(3 * sizeof(float));
    float *l1 = (float *)malloc(3 * sizeof(float));
    float *l2 = (float *)malloc(3 * sizeof(float));
    float *l3 = (float *)malloc(3 * sizeof(float));

    l0[0] = 237.000; l0[1] = 28.000; l0[2] = 36.000;
    l1[0] = 168.000; l1[1] = 230.000; l1[2] = 29.000;
    l2[0] = 111.000; l2[1] = 49.000; l2[2] = 152.000;
    l3[0] = 156.000; l3[1] = 90.000; l3[2] = 60.000;

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l0, A->data[0][0], 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l1, A->data[0][1], 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l2, A->data[1][0], 3);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l3, A->data[1][1], 3);

    iray3d_free(A);
    free(l0);
    free(l1);
    free(l2);
    free(l3);
}

void test_img_gray(void) {
    Iray3D *A = img_read("./test/data/small/test.png", IMG_GRAY);
    TEST_ASSERT_EQUAL_size_t(1, A->depth);

    float *l0 = (float *)malloc(1 * sizeof(float));
    float *l1 = (float *)malloc(1 * sizeof(float));
    float *l2 = (float *)malloc(1 * sizeof(float));
    float *l3 = (float *)malloc(1 * sizeof(float));

    l0[0] = 74.813;
    l1[0] = 176.788;
    l2[0] = 71.510;
    l3[0] = 95.394;

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l0, A->data[0][0], 1);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l1, A->data[0][1], 1);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l2, A->data[1][0], 1);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l3, A->data[1][1], 1);

    iray3d_free(A);
    free(l0);
    free(l1);
    free(l2);
    free(l3);
}

void test_img_resize(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_resize(A, 224, 224);
    Iray3D *C = img_read("./test/data/img_resized.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_rotate(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_rotate(A, 90);
    Iray3D *C = img_read("./test/data/img_rotated.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_imgs_read(void) {
    Iray3D **A = imgs_read("./test/data/small/", 2, IMG_RGBA);

    float *l0 = (float *)malloc(4 * sizeof(float));
    float *l1 = (float *)malloc(4 * sizeof(float));
    float *l2 = (float *)malloc(4 * sizeof(float));
    float *l3 = (float *)malloc(4 * sizeof(float));

    float *l4 = (float *)malloc(4 * sizeof(float));
    float *l5 = (float *)malloc(4 * sizeof(float));
    float *l6 = (float *)malloc(4 * sizeof(float));
    float *l7 = (float *)malloc(4 * sizeof(float));

    l0[0] = 229.000; l0[1] = 32.000; l0[2] = 49.000; l0[3] = 255.000;
    l1[0] = 164.000; l1[1] = 225.000; l1[2] = 26.000; l1[3] = 255.000;
    l2[0] = 107.000; l2[1] = 45.000; l2[2] = 142.000; l2[3] = 255.000;
    l3[0] = 168.000; l3[1] = 94.000; l3[2] = 67.000; l3[3] = 255.000;


    l4[0] = 237.000; l4[1] = 28.000; l4[2] = 36.000; l4[3] = 255.000;
    l5[0] = 168.000; l5[1] = 230.000; l5[2] = 29.000; l5[3] = 255.000;
    l6[0] = 111.000; l6[1] = 49.000; l6[2] = 152.000; l6[3] = 255.000;
    l7[0] = 156.000; l7[1] = 90.000; l7[2] = 60.000; l7[3] = 255.000;

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l0, A[0]->data[0][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l1, A[0]->data[0][1], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l2, A[0]->data[1][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l3, A[0]->data[1][1], 4);

    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l4, A[1]->data[0][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l5, A[1]->data[0][1], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l6, A[1]->data[1][0], 4);
    TEST_ASSERT_EQUAL_FLOAT_ARRAY(l7, A[1]->data[1][1], 4);

    imgs_free(A, 2);
    free(l0);
    free(l1);
    free(l2);
    free(l3);
    free(l4);
    free(l5);
    free(l6);
    free(l7);
}

void test_img_write_png(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_rotate(A, 90);
    img_write("./test/data/img_write.png", B);
    Iray3D *C = img_read("./test/data/img_rotated.png", IMG_RGB);
    Iray3D *D = img_read("./test/data/img_write.png", IMG_RGB);

    for (size_t i = 0; i < C->rows; i++) {
        for (size_t j = 0; j < C->cols; j++) {
            for (size_t k = 0; k < C->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], D->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
    iray3d_free(D);
}

void test_img_write_jpg(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_rotate(A, 90);
    img_write("./test/data/img_write.jpg", B);
    Iray3D *C = img_read("./test/data/img_rotated.png", IMG_RGB);
    Iray3D *D = img_read("./test/data/img_write.jpg", IMG_RGB);
    
    for (size_t i = 0; i < C->rows; i++) {
        for (size_t j = 0; j < C->cols; j++) {
            for (size_t k = 0; k < C->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], D->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
    iray3d_free(D);
}

void test_img_conv_blur(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_conv(A, ICONV_BLUR, 2);
    Iray3D *C = img_read("./test/data/img_conved_blur.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_conv_emboss(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_conv(A, ICONV_EMBOSS, 2);
    Iray3D *C = img_read("./test/data/img_conved_emboss.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_conv_emboss2(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_conv(A, ICONV_EMBOSS2, 2);
    Iray3D *C = img_read("./test/data/img_conved_emboss2.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_conv_laplacian(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_conv(A, ICONV_LAPLACIAN, 2);
    Iray3D *C = img_read("./test/data/img_conved_laplacian.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_conv_sobelx(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_conv(A, ICONV_SOBELX, 2);
    Iray3D *C = img_read("./test/data/img_conved_sobelx.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_conv_sobely(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_conv(A, ICONV_SOBELY, 2);
    Iray3D *C = img_read("./test/data/img_conved_sobely.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_edge_detect(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_edge_detect(A);
    Iray3D *C = img_read("./test/data/img_edge_detect.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_max_pool(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_max_pool(A, 2);
    Iray3D *C = img_read("./test/data/img_max_pool.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_min_pool(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_min_pool(A, 2);
    Iray3D *C = img_read("./test/data/img_min_pool.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_img_mean_pool(void) {
    Iray3D *A = img_read("./test/data/img.png", IMG_RGB);
    Iray3D *B = img_mean_pool(A, 2);
    Iray3D *C = img_read("./test/data/img_mean_pool.png", IMG_RGB);

    for (size_t i = 0; i < B->rows; i++) {
        for (size_t j = 0; j < B->cols; j++) {
            for (size_t k = 0; k < B->depth; k++) {
                TEST_ASSERT_EQUAL_FLOAT(C->data[i][j][k], B->data[i][j][k]);
            }
        }
    }

    iray3d_free(A);
    iray3d_free(B);
    iray3d_free(C);
}

void test_standart_scaler(void) {
    double xorData[4][2] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1}
    };
    Iray2D *data = iray2d_alloc(4, 2);
    for (size_t i = 0; i < data->rows; i++) {
        for (size_t j = 0; j < data->cols; j++) {
            data->data[i][j] = xorData[i][j];
        }
    }
    Iray2D *standardScled = standard_scaler(data);

    TEST_ASSERT_EQUAL_FLOAT(-1.0, standardScled->data[0][0]);
    TEST_ASSERT_EQUAL_FLOAT(-1.0, standardScled->data[0][1]);
    TEST_ASSERT_EQUAL_FLOAT(-1.0, standardScled->data[1][0]);
    TEST_ASSERT_EQUAL_FLOAT(1.0, standardScled->data[1][1]);
    TEST_ASSERT_EQUAL_FLOAT(1.0, standardScled->data[2][0]);
    TEST_ASSERT_EQUAL_FLOAT(-1.0, standardScled->data[2][1]);
    TEST_ASSERT_EQUAL_FLOAT(1.0, standardScled->data[3][0]);
    TEST_ASSERT_EQUAL_FLOAT(1.0, standardScled->data[3][1]);

    iray2d_free(data);
    iray2d_free(standardScled);
}

void test_one_hot_encoding(void) {
    Iray2D *data = iray2d_alloc(3, 1);
    data->data[0][0] = 1.0;
    data->data[1][0] = 2.0;
    data->data[2][0] = 1.0;

    Iray2D *result = one_hot_encoding(data);

    TEST_ASSERT_EQUAL_FLOAT(1.0, result->data[0][0]);
    TEST_ASSERT_EQUAL_FLOAT(0.0, result->data[1][0]);
    TEST_ASSERT_EQUAL_FLOAT(1.0, result->data[2][0]);
    TEST_ASSERT_EQUAL_FLOAT(0.0, result->data[0][1]);
    TEST_ASSERT_EQUAL_FLOAT(1.0, result->data[1][1]);
    TEST_ASSERT_EQUAL_FLOAT(0.0, result->data[2][1]);

    iray2d_free(data);
    iray2d_free(result);
}

void test_csv_read(void) {
    Iray2D *data = csv_read("./test/data/test.csv", 3, 4);
    
    TEST_ASSERT_EQUAL_FLOAT(-1.0,data->data[0][0]);
    TEST_ASSERT_EQUAL_FLOAT(-0.8,data->data[0][1]);
    TEST_ASSERT_EQUAL_FLOAT(-0.6,data->data[0][2]);
    TEST_ASSERT_EQUAL_FLOAT(-0.4,data->data[0][3]);
    TEST_ASSERT_EQUAL_FLOAT(-0.2,data->data[1][0]);
    TEST_ASSERT_EQUAL_FLOAT(0.0,data->data[1][1]);
    TEST_ASSERT_EQUAL_FLOAT(0.2,data->data[1][2]);
    TEST_ASSERT_EQUAL_FLOAT(0.4,data->data[1][3]);
    TEST_ASSERT_EQUAL_FLOAT(0.6,data->data[2][0]);
    TEST_ASSERT_EQUAL_FLOAT(0.8,data->data[2][1]);
    TEST_ASSERT_EQUAL_FLOAT(1.0,data->data[2][2]);
    TEST_ASSERT_EQUAL_FLOAT(0.0,data->data[2][3]);

    iray2d_free(data);
}

void test_layer_dense(void) {
    Layer *layer = layer_dense(3, 5);
    TEST_ASSERT_EQUAL_CHAR(Dense, layer->name);
    TEST_ASSERT_EQUAL_size_t(3, layer->inputSize);
    TEST_ASSERT_EQUAL_size_t(5, layer->outputSize);
    TEST_ASSERT_EQUAL_size_t(3, layer->input->rows);
    TEST_ASSERT_EQUAL_size_t(5, layer->output->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->params->rows);
    TEST_ASSERT_EQUAL_size_t(5, layer->bias->rows);
    TEST_ASSERT_EQUAL_size_t(3, layer->weight->rows);
    TEST_ASSERT_EQUAL_size_t(5, layer->weight->cols);
    layer_free(layer);
}

void test_layer_activation(void) {
    Layer *layer = layer_activation(Sigmoid);
    TEST_ASSERT_EQUAL_CHAR(Activation, layer->name);
    TEST_ASSERT_EQUAL_size_t(0, layer->inputSize);
    TEST_ASSERT_EQUAL_size_t(0, layer->outputSize);
    TEST_ASSERT_EQUAL_size_t(0, layer->input->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->output->rows);
    TEST_ASSERT_EQUAL_size_t(1, layer->params->rows);
    TEST_ASSERT_EQUAL_CHAR(Sigmoid, layer->params->data[0]);
    TEST_ASSERT_EQUAL_size_t(0, layer->bias->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->weight->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->weight->cols);
    layer_free(layer);
}

void test_layer_shuffle(void) {
    Layer *layer = layer_shuffle(0.3);
    TEST_ASSERT_EQUAL_CHAR(Shuffle, layer->name);
    TEST_ASSERT_EQUAL_size_t(0, layer->inputSize);
    TEST_ASSERT_EQUAL_size_t(0, layer->outputSize);
    TEST_ASSERT_EQUAL_size_t(0, layer->input->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->output->rows);
    TEST_ASSERT_EQUAL_size_t(1, layer->params->rows);
    TEST_ASSERT_EQUAL_CHAR(0.3, layer->params->data[0]);
    TEST_ASSERT_EQUAL_size_t(0, layer->bias->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->weight->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->weight->cols);
    layer_free(layer);
}

void test_layer_dropout(void) {
    Layer *layer = layer_dropout(0.3);
    TEST_ASSERT_EQUAL_CHAR(Dropout, layer->name);
    TEST_ASSERT_EQUAL_size_t(0, layer->inputSize);
    TEST_ASSERT_EQUAL_size_t(0, layer->outputSize);
    TEST_ASSERT_EQUAL_size_t(0, layer->input->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->output->rows);
    TEST_ASSERT_EQUAL_size_t(1, layer->params->rows);
    TEST_ASSERT_EQUAL_CHAR(0.3, layer->params->data[0]);
    TEST_ASSERT_EQUAL_size_t(0, layer->bias->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->weight->rows);
    TEST_ASSERT_EQUAL_size_t(0, layer->weight->cols);
    layer_free(layer);
}

void test_model_alloc(void) {
    Model *model = model_alloc(5);
    TEST_ASSERT_EQUAL(sizeof(Layer *), sizeof(model->layers));
    model_free(model);
}

void test_model_add(void) {
    Model *model = model_alloc(2);
    Layer *dense = layer_dense(2, 1);
    Layer *activation = layer_activation(Sigmoid);
    model_add(model, dense);
    model_add(model, activation);

    TEST_ASSERT_EQUAL_CHAR(Dense, model->layers[0]->name);
    TEST_ASSERT_EQUAL_size_t(2, model->layers[0]->inputSize);
    TEST_ASSERT_EQUAL_size_t(1, model->layers[0]->outputSize);
    TEST_ASSERT_EQUAL_size_t(2, model->layers[0]->input->rows);
    TEST_ASSERT_EQUAL_size_t(1, model->layers[0]->output->rows);
    TEST_ASSERT_EQUAL_size_t(0, model->layers[0]->params->rows);
    TEST_ASSERT_EQUAL_size_t(1, model->layers[0]->bias->rows);
    TEST_ASSERT_EQUAL_size_t(2, model->layers[0]->weight->rows);
    TEST_ASSERT_EQUAL_size_t(1, model->layers[0]->weight->cols);

    TEST_ASSERT_EQUAL_CHAR(Activation, model->layers[1]->name);
    TEST_ASSERT_EQUAL_size_t(1, model->layers[1]->inputSize);
    TEST_ASSERT_EQUAL_size_t(1, model->layers[1]->outputSize);
    TEST_ASSERT_EQUAL_size_t(0, model->layers[1]->input->rows);
    TEST_ASSERT_EQUAL_size_t(0, model->layers[1]->output->rows);
    TEST_ASSERT_EQUAL_size_t(1, model->layers[1]->params->rows);
    TEST_ASSERT_EQUAL_CHAR(Sigmoid, model->layers[1]->params->data[0]);
    TEST_ASSERT_EQUAL_size_t(0, model->layers[1]->bias->rows);
    TEST_ASSERT_EQUAL_size_t(0, model->layers[1]->weight->rows);
    TEST_ASSERT_EQUAL_size_t(0, model->layers[1]->weight->cols);

    model_free(model);
    layer_free(dense);
    layer_free(activation);
}

void test_initializer_ones(void) {
    Model *m = model_alloc(1);
    Layer *l = layer_dense(3, 2);
    model_add(m, l);
    model_randomize(Ones, m);
    for (size_t i = 0; i < l->weight->rows; i++) {
        for (size_t j = 0; j < l->weight->cols; j++) {
            TEST_ASSERT_EQUAL_FLOAT(1, l->weight->data[i][j]);
        }
    }
    for (size_t i = 0; i < l->bias->rows; i++) {
        TEST_ASSERT_EQUAL_FLOAT(1, l->bias->data[i]);
    }
    
    model_free(m);
}

void test_initializer_zeros(void) {
    Model *m = model_alloc(1);
    Layer *l = layer_dense(3, 2);
    model_add(m, l);
    model_randomize(Zeros, m);
    for (size_t i = 0; i < l->weight->rows; i++) {
        for (size_t j = 0; j < l->weight->cols; j++) {
            TEST_ASSERT_EQUAL_FLOAT(0, l->weight->data[i][j]);
        }
    }
    for (size_t i = 0; i < l->bias->rows; i++) {
        TEST_ASSERT_EQUAL_FLOAT(0, l->bias->data[i]);
    }
    
    model_free(m);
}

void test_initializer_random_heuniform(void) {
    Model *m = model_alloc(1);
    Layer *l = layer_dense(3, 2);
    model_add(m, l);
    model_randomize(RandomHeUniform, m);
    for (size_t i = 0; i < l->weight->rows; i++) {
        for (size_t j = 0; j < l->weight->cols; j++) {
            TEST_ASSERT_TRUE(l->weight->data[i][j] > -1);
            TEST_ASSERT_TRUE(l->weight->data[i][j] < 1);
        }
    }
    for (size_t i = 0; i < l->bias->rows; i++) {
            TEST_ASSERT_TRUE(l->bias->data[i] > -1);
            TEST_ASSERT_TRUE(l->bias->data[i] < 1);
    }
    
    model_free(m);
}

void test_initializer_random_normal(void) {
    Model *m = model_alloc(1);
    Layer *l = layer_dense(3, 2);
    model_add(m, l);
    model_randomize(RandomNormal, m);
    for (size_t i = 0; i < l->weight->rows; i++) {
        for (size_t j = 0; j < l->weight->cols; j++) {
            TEST_ASSERT_TRUE(l->weight->data[i][j] > -1);
            TEST_ASSERT_TRUE(l->weight->data[i][j] < 1);
        }
    }
    for (size_t i = 0; i < l->bias->rows; i++) {
            TEST_ASSERT_TRUE(l->bias->data[i] > -1);
            TEST_ASSERT_TRUE(l->bias->data[i] < 1);
    }
    
    model_free(m);
}

void test_initializer_random_uniform(void) {
    Model *m = model_alloc(1);
    Layer *l = layer_dense(3, 2);
    model_add(m, l);
    model_randomize(RandomUniform, m);
    for (size_t i = 0; i < l->weight->rows; i++) {
        for (size_t j = 0; j < l->weight->cols; j++) {
            TEST_ASSERT_TRUE(l->weight->data[i][j] > -0.05);
            TEST_ASSERT_TRUE(l->weight->data[i][j] < 0.05);
        }
    }
    for (size_t i = 0; i < l->bias->rows; i++) {
            TEST_ASSERT_TRUE(l->bias->data[i] > -0.05);
            TEST_ASSERT_TRUE(l->bias->data[i] < 0.05);
    }
    
    model_free(m);
}

void test_initializer_random_xavier(void) {
    Model *m = model_alloc(1);
    Layer *l = layer_dense(3, 2);
    model_add(m, l);
    model_randomize(RandomXavier, m);
    for (size_t i = 0; i < l->weight->rows; i++) {
        for (size_t j = 0; j < l->weight->cols; j++) {
            TEST_ASSERT_TRUE(l->weight->data[i][j] > -1);
            TEST_ASSERT_TRUE(l->weight->data[i][j] < 1);
        }
    }
    for (size_t i = 0; i < l->bias->rows; i++) {
            TEST_ASSERT_TRUE(l->bias->data[i] > -1);
            TEST_ASSERT_TRUE(l->bias->data[i] < 1);
    }
    
    model_free(m);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_iray1d_alloc);
    RUN_TEST(test_iray1d_free);
    RUN_TEST(test_iray1d_fill);
    RUN_TEST(test_iray1d_add);
    RUN_TEST(test_iray1d_dot);
    RUN_TEST(test_iray1d_clone);
    RUN_TEST(test_iray1d_apply);
    RUN_TEST(test_iray1d_slice);

    RUN_TEST(test_iray2d_alloc);
    RUN_TEST(test_iray2d_free);
    RUN_TEST(test_iray2d_fill);
    RUN_TEST(test_iray2d_add);
    RUN_TEST(test_iray2d_dot);
    RUN_TEST(test_iray2d_clone);
    RUN_TEST(test_iray2d_transpose);
    RUN_TEST(test_iray2d_apply);

    RUN_TEST(test_iray3d_alloc);
    RUN_TEST(test_iray3d_free);
    RUN_TEST(test_iray3d_fill);
    RUN_TEST(test_iray3d_add);
    RUN_TEST(test_iray3d_clone);
    RUN_TEST(test_iray3d_apply);

    RUN_TEST(test_img_read_png);
    RUN_TEST(test_img_read_jpg);
    RUN_TEST(test_img_rgba);
    RUN_TEST(test_img_rgb);
    RUN_TEST(test_img_gray);
    RUN_TEST(test_img_resize);
    RUN_TEST(test_img_rotate);
    RUN_TEST(test_imgs_read);
    RUN_TEST(test_img_write_png);
    RUN_TEST(test_img_write_jpg);

    RUN_TEST(test_img_conv_blur);
    RUN_TEST(test_img_conv_emboss);
    RUN_TEST(test_img_conv_emboss2);
    RUN_TEST(test_img_conv_laplacian);
    RUN_TEST(test_img_conv_sobelx);
    RUN_TEST(test_img_conv_sobely);
    RUN_TEST(test_img_edge_detect);
    RUN_TEST(test_img_max_pool);
    RUN_TEST(test_img_min_pool);
    RUN_TEST(test_img_mean_pool);

    RUN_TEST(test_standart_scaler);
    RUN_TEST(test_one_hot_encoding);

    RUN_TEST(test_csv_read);

    RUN_TEST(test_initializer_ones);
    RUN_TEST(test_initializer_zeros);
    RUN_TEST(test_initializer_random_heuniform);
    RUN_TEST(test_initializer_random_normal);
    RUN_TEST(test_initializer_random_uniform);
    RUN_TEST(test_initializer_random_xavier);

    RUN_TEST(test_layer_dense);
    RUN_TEST(test_layer_activation);
    RUN_TEST(test_layer_shuffle);
    RUN_TEST(test_layer_dropout);

    RUN_TEST(test_model_alloc);

    UNITY_END();

    return 0;
}