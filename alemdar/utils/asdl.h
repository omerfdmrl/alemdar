#ifndef ALEMDAR_ASDL_H

#define ALEMDAR_ASDL_H

#ifdef ALEMDAR_ASDL_IMPLEMENTATION

#include <SDL2/SDL.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} ASDLContext;

ALEMDAR_DEF ASDLContext asdl_alloc(int width, int height);
ALEMDAR_DEF void asdl_free(ASDLContext context);
ALEMDAR_DEF void img_show(ASDLContext context, Img img);

ASDLContext asdl_alloc(int width, int height) {
    ASDLContext context;

    ALEMDAR_ASSERT(SDL_Init(SDL_INIT_VIDEO) == 0);

    context.window = SDL_CreateWindow("ALEMDAR", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    ALEMDAR_ASSERT(context.window != NULL);

    context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_ACCELERATED);
    ALEMDAR_ASSERT(context.renderer != NULL);

    return context;
}

void asdl_free(ASDLContext context) {
    SDL_DestroyRenderer(context.renderer);
    SDL_DestroyWindow(context.window);
    SDL_Quit();
}

void img_show(ASDLContext context, Img img) {
    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_Texture *texture = NULL;

        SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, img.width, img.height, 32, SDL_PIXELFORMAT_RGBA32);
        if (!surface) {
            printf("SDL Surface couldn't be created! SDL_Error: %s\n", SDL_GetError());
            return;
        }

        for (size_t i = 0; i < img.height; i++) {
            for (size_t j = 0; j < img.width; j++) {
                size_t index = i * img.width + j;
                Uint32 pixel = SDL_MapRGBA(surface->format, 
                                        (Uint8)img.data.data[index][0],  // Red
                                        (Uint8)img.data.data[index][1],  // Green
                                        (Uint8)img.data.data[index][2],  // Blue
                                        (Uint8)img.data.data[index][3]); // Alpha
                ((Uint32 *)surface->pixels)[i * img.width + j] = pixel;
            }
        }

        texture = SDL_CreateTextureFromSurface(context.renderer, surface);
        if (!texture) {
            printf("Texture could not be created from surface! SDL_Error: %s\n", SDL_GetError());
        }

        SDL_RenderClear(context.renderer);
        SDL_RenderCopy(context.renderer, texture, NULL, NULL);
        SDL_RenderPresent(context.renderer);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        SDL_Delay(16);
    }
}

void imgs_show(ASDLContext context, Img *imgs, size_t count) {
    SDL_Event e;
    int quit = 0;
    int window_width, window_height;
    SDL_GetWindowSize(context.window, &window_width, &window_height);

    SDL_SetWindowFullscreen(context.window, SDL_WINDOW_FULLSCREEN);

    int offset_x = 0;
    int offset_y = 0;
    int row_height = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    window_width = e.window.data1;
                    window_height = e.window.data2;
                }
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym >= 33 && e.key.keysym.sym <= 126) {
                    SDL_SetWindowFullscreen(context.window, 0);
            }
        }

        for (size_t i = 0; i < count; i++) {
            Img img = imgs[i];

            SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, img.width, img.height, 32, SDL_PIXELFORMAT_RGBA32);
            if (!surface) {
                printf("SDL Surface couldn't be created! SDL_Error: %s\n", SDL_GetError());
                continue;
            }

            for (size_t y = 0; y < img.height; y++) {
                for (size_t x = 0; x < img.width; x++) {
                    size_t index = y * img.width + x;
                    Uint32 pixel = SDL_MapRGBA(surface->format, 
                                              (Uint8)img.data.data[index][0],  // Red
                                              (Uint8)img.data.data[index][1],  // Green
                                              (Uint8)img.data.data[index][2],  // Blue
                                              (Uint8)img.data.data[index][3]); // Alpha
                    ((Uint32 *)surface->pixels)[y * img.width + x] = pixel;
                }
            }

            SDL_Texture *texture = SDL_CreateTextureFromSurface(context.renderer, surface);
            if (!texture) {
                printf("Texture could not be created from surface! SDL_Error: %s\n", SDL_GetError());
                SDL_FreeSurface(surface);
                continue;
            }

            SDL_Rect dst_rect = {offset_x, offset_y, img.width, img.height};
            SDL_RenderCopy(context.renderer, texture, NULL, &dst_rect);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);

            offset_x += img.width + 5;

            if (offset_x + img.width > window_width) {
                offset_x = 0;
                offset_y += row_height + 5;
                row_height = 0;
            }

            if (img.height > row_height) {
                row_height = img.height;
            }
        }

        SDL_RenderPresent(context.renderer);

        SDL_Delay(16);
    }
}

#endif //ALEMDAR_ASDL_IMPLEMENTATION

#endif // ALEMDAR_ASDL_H