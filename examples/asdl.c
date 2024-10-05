#define ALEMDAR_IMPLEMENTATION
#define ALEMDAR_CSV_IMPLEMENTATION
#define ALEMDAR_CONV_IMPLEMENTATION
#define ALEMDAR_IO_IMPLEMENTATION
#define ALEMDAR_IMG_IMPLEMENTATION
#define ALEMDAR_ASDL_IMPLEMENTATION

#include "alemdar/alemdar.h"
#include <time.h>
#include <SDL2/SDL.h>

int main() {
    /** Any image */
    Img img = img_read("cat.jpg");
    Img img_edge_detected = img_edge_detect(img);

    ASDLContext context = asdl_alloc(img.width, img.height);
    img_show(context, img);
    
    asdl_free(context);
    img_free(img);

    context = asdl_alloc(img_edge_detected.width, img_edge_detected.height);
    img_show(context, img_edge_detected);

    asdl_free(context);
    img_free(img_edge_detected);

    return 0;
}
