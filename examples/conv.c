/**
 * make run SOURCE=./examples/conv.c
 */

#include "../src/ican.h"

int main() {
    /** Any image */
    Iray3D *img = img_read("./examples/cat.png", IMG_RGBA);
    Iray3D *img_edge_detected = img_edge_detect(img);

    ISDLContext *context = isdl_alloc(img_edge_detected->cols, img_edge_detected->rows);
    img_show(context, img_edge_detected);
    isdl_free(context);

    iray3d_free(img);
    iray3d_free(img_edge_detected);

    return 0;
}