// This is not a file to be included.
// Instead, it is a standalone program.
#include <SDL2/SDL.h>

#include "t_obj.h"
#include "t_math.h"

// copied in some boilerplate

int main(int argc, char *argv[]) {

    // Process input {
    if (strcmp(argv[1], "help") || strcmp(argv[1], "")) {
        printf("Start program like this:\n./render FILE.OBJ WIDTH HEIGHT");
        return 0;
    }
    char filename[256];
    strcpy(filename, argv[1]);
    int WIDTH = atoi(argv[2]);
    int HEIGHT = atoi(argv[3]);

    t_Face faces[MAX_FACES];
    int faces_count = t_decodeOBJ(filename, faces);
    // }

    // Event handling {
    int quit = 0;
    int leftMouseButtonDown = 0;
    SDL_Event event;
    // }

    // Create screen {
    SDL_Init(SDL_INIT_VIDEO);
 
    SDL_Window * window = SDL_CreateWindow(
        "SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        WIDTH, HEIGHT, 
        0);
    
    SDL_Renderer * renderer = SDL_CreateRenderer(
        window, 
        -1, 
        0);

    SDL_Texture * texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STATIC, 
        WIDTH, HEIGHT);
    
    Uint32 * pixels = malloc(WIDTH * HEIGHT * sizeof(Uint32));

    // memset sets an area of memory to one value
    memset(pixels, 255, WIDTH * HEIGHT * sizeof(Uint32));
    // }

    while (!quit)
    {
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

        SDL_PollEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = 1;
                break;
            
            // Put cases for input here:

            
        }

        // clear, fill, and publish screen.
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    // deallocate memory
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    free(pixels);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}