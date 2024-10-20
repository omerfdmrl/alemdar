/**
 * make run SOURCE=./examples/isdl.c
 */

#include "../src/ican.h"

int main() {
    /** Any image */
    Iray3D *img = img_read("./examples/cat.png", IMG_RGBA);

    ISDLContext *context = isdl_alloc(img->cols, img->rows);
    img_show(context, img);
    
    isdl_free(context);
    iray3d_free(img);

    return 0;
}