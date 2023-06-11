// This is not a file to be included.
// Instead, it is a standalone program.
#include <SDL2/SDL.h>

#include "t_obj.h"
#include "t_math.h"

int main(int argc, char *argv[]) {

    t_Model camera = (t_Model) {
        {0, 0, 1}, // pos
        {0, 0, 0}  // rot
    };

    t_Model object = (t_Model) {
        {0, 0, 0}, 
        {0, 0, 0}
    };
    
    t_Matrix proj;
    t_Matrix view;
    t_Matrix model;

    // Process input {

        char filename[256];
        strcpy(filename, argv[1]);
        int WIDTH = atoi(argv[2]);
        int HEIGHT = atoi(argv[3]);

        printf("Got input: resolution: %dx%d, file: %s\n", WIDTH, HEIGHT, filename);

        t_Face faces[MAX_FACES];
        t_Face tr_faces[MAX_FACES];
        int faces_count = t_decodeOBJ(filename, faces);

        printf("Rendering %d triangles.\n", faces_count);
    // }

    // Event handling {
        int quit = 0;
        SDL_Event event;
        int mouseDown = 0;
    // }

    // Create screen {
        SDL_Init(SDL_INIT_VIDEO);
    
        SDL_Window * window = SDL_CreateWindow(
            "rendering window",
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

    // Call library functions {
        float FOV = 90; //default

        // call when FOV changes.
        t_genProj(&proj, &WIDTH, &HEIGHT, &FOV);

        // call when object model changes.
        t_genModel(&model, &object);

        // call when camera model changes.
        t_genView(&view, &camera);

        // Put faces in correct order
        int i;
        for (i = 0; i < faces_count; i++)
            t_reorderFace(&(faces[i]));
        
    // }

    // Main loop
    while (!quit)
    {
        SDL_UpdateTexture(texture, NULL, pixels, WIDTH * sizeof(Uint32));

        SDL_Delay(10);
        SDL_PollEvent(&event);
 
        switch (event.type)
        {
            case SDL_QUIT:
                quit = 1;
                break;
            
            // Put cases for input here:
            case SDL_MOUSEWHEEL:
                if (event.wheel.y > 0) FOV++;
                if (event.wheel.y < 0) FOV--;

                t_genProj(&proj, &WIDTH, &HEIGHT, &FOV);
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                mouseDown = 1;
                break;
            
            case SDL_MOUSEBUTTONUP:
                mouseDown = 0;
                break;
            
            case SDL_MOUSEMOTION:
                if (mouseDown) {
                    if(event.motion.xrel > 0) camera.rot[_Y]++;
                    if(event.motion.xrel < 0) camera.rot[_Y]--;
                    if(event.motion.yrel > 0) camera.rot[_X]++;
                    if(event.motion.yrel < 0) camera.rot[_X]--;
                    t_genView(&view, &camera);
                }
                break;
            
        }
        // Rendering loop {
            t_project(&proj, &view, &model, faces, tr_faces, faces_count, WIDTH, HEIGHT);
            for (i = 0; i < faces_count; i++) {
                t_drawLine(
                    tr_faces[i].p1.m[_X], tr_faces[i].p1.m[_Y],
                    tr_faces[i].p2.m[_X], tr_faces[i].p2.m[_Y],
                    pixels, WIDTH, HEIGHT);
                t_drawLine(
                    tr_faces[i].p2.m[_X], tr_faces[i].p2.m[_Y],
                    tr_faces[i].p3.m[_X], tr_faces[i].p3.m[_Y],
                    pixels, WIDTH, HEIGHT);
                t_drawLine(
                    tr_faces[i].p1.m[_X], tr_faces[i].p1.m[_Y],
                    tr_faces[i].p3.m[_X], tr_faces[i].p3.m[_Y],
                    pixels, WIDTH, HEIGHT);

                printf("Drew face ((%f, %f), (%f, %f), (%f, %f))\n", 
                tr_faces[i].p1.m[_X], tr_faces[i].p1.m[_Y],
                tr_faces[i].p2.m[_X], tr_faces[i].p2.m[_Y],
                tr_faces[i].p3.m[_X], tr_faces[i].p3.m[_Y]);
            }
        // }

        // clear, fill, and publish screen.
        // SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    // After main loop terminates.
    // free memory
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    free(pixels);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}