#include "../src/ican.h"

int main() {
    Iray3D *img = iray3d_alloc(400, 400, 3);

    for (size_t i = 0; i < img->rows; i++) {
        for (size_t j = 0; j < img->cols; j++) {
            for (size_t k = 0; k < img->depth; k++) {
                img->data[i][j][k] = rand() % 256;
            }
        }
    }

    Iray3D *img_resized = img_resize(img, 224, 224);
    Iray3D *img_rotated = img_rotate(img, 90);
    Iray3D *img_conved_blur = img_conv(img,ICONV_BLUR, 2);
    Iray3D *img_conved_emboss = img_conv(img, ICONV_EMBOSS, 2);
    Iray3D *img_conved_emboss2 = img_conv(img, ICONV_EMBOSS2, 2);
    Iray3D *img_conved_laplacian = img_conv(img, ICONV_LAPLACIAN, 2);
    Iray3D *img_conved_sobelx = img_conv(img, ICONV_SOBELX, 2);
    Iray3D *img_conved_sobely = img_conv(img, ICONV_SOBELY, 2);
    Iray3D *img_edge_detected = img_edge_detect(img);
    Iray3D *img_max_pooled = img_max_pool(img, 2);
    Iray3D *img_min_pooled = img_min_pool(img, 2);
    Iray3D *img_mean_pooled = img_mean_pool(img, 2);

    img_write("./test/data/img.png", img);
    img_write("./test/data/img_resized.png", img_resized);
    img_write("./test/data/img_rotated.png", img_rotated);
    img_write("./test/data/img_conved_blur.png", img_conved_blur);
    img_write("./test/data/img_conved_emboss.png", img_conved_emboss);
    img_write("./test/data/img_conved_emboss2.png", img_conved_emboss2);
    img_write("./test/data/img_conved_laplacian.png", img_conved_laplacian);
    img_write("./test/data/img_conved_sobelx.png", img_conved_sobelx);
    img_write("./test/data/img_conved_sobely.png", img_conved_sobely);
    img_write("./test/data/img_edge_detect.png", img_edge_detected);
    img_write("./test/data/img_max_pool.png", img_max_pooled);
    img_write("./test/data/img_min_pool.png", img_min_pooled);
    img_write("./test/data/img_mean_pool.png", img_mean_pooled);

    iray3d_free(img);
    iray3d_free(img_resized);
    iray3d_free(img_rotated);
    iray3d_free(img_conved_blur);
    iray3d_free(img_conved_emboss);
    iray3d_free(img_conved_emboss2);
    iray3d_free(img_conved_laplacian);
    iray3d_free(img_conved_sobelx);
    iray3d_free(img_conved_sobely);
    iray3d_free(img_edge_detected);
    iray3d_free(img_max_pooled);
    iray3d_free(img_min_pooled);
    iray3d_free(img_mean_pooled);

    Iray3D *img_small = iray3d_alloc(2, 2, 3);
    img_small->data[0][0][0] = 237.0;
    img_small->data[0][0][1] = 28.0;
    img_small->data[0][0][2] = 36.0;

    img_small->data[0][1][0] = 168.0;
    img_small->data[0][1][1] = 230.0;
    img_small->data[0][1][2] = 29.0;

    img_small->data[1][0][0] = 111.0;
    img_small->data[1][0][1] = 49.0;
    img_small->data[1][0][2] = 152.0;

    img_small->data[1][1][0] = 156.0;
    img_small->data[1][1][1] = 90.0;
    img_small->data[1][1][2] = 60.0;

    img_write("./test/data/small/test.png", img_small);
    img_write("./test/data/small/test.jpg", img_small);

    iray3d_free(img_small);

    FILE *file = fopen("./test/data/test.csv", "w");

    fprintf(file, "-1.0, -0.8, -0.6, -0.4\n");
    fprintf(file, "-0.2, 0.0, 0.2, 0.4\n");
    fprintf(file, "0.6, 0.8, 1.0, 0.0\n");

    fclose(file);
}