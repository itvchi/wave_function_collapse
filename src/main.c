#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>

#define TILE_SIZE       64
#define TILE_COUNT      4
#define TILE_X          28
#define TILE_Y          14
#define WINDOW_WIDTH    TILE_SIZE * TILE_X
#define WINDOW_HEIGHT   TILE_SIZE * TILE_Y

int main(int argc, char const *argv[])
{
    SDL_Window      *window;  
    SDL_Renderer    *renderer;  
    SDL_Surface     *surface;    
    SDL_Texture     *texture;
    bool            is_running;
    SDL_Event       event;  

    SDL_FRect       texture_tile[TILE_COUNT];
    SDL_FRect       window_tile[TILE_Y][TILE_X];
    int             tilemap[TILE_Y][TILE_X];
    int             x, y;  
    int             index;

    /* Init SDL library */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not be initialized: %s\n", SDL_GetError());
        return 1;
    }

    /* Create an application window */
    window = SDL_CreateWindow("window", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    /* Create an SDL Renderer */
    renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1;
    }

    /* Load tile bitmat into surface */
    surface = SDL_LoadBMP("../resources/tiles.bmp");

    /* Create textures from surface and free surface */
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    /* Prepare array of tile indices to display in window */
    /* Tilemap will be generated by wave function collapse algorithm */
    srand(time(NULL));
    for (y = 0; y < TILE_Y; y++) {
        for (x = 0; x < TILE_X; x++) {
            tilemap[y][x] = rand() % TILE_COUNT;
        }
    }

    /* Create tile rectangles */
    for (index = 0; index < TILE_COUNT; index++) {
        texture_tile[index].x = 0;
        texture_tile[index].y = index * TILE_SIZE;
        texture_tile[index].w = TILE_SIZE;
        texture_tile[index].h = TILE_SIZE;
    }

    /* Create window rectangles */
    for (y = 0; y < TILE_Y; y++) {
        for (x = 0; x < TILE_X; x++) {
            window_tile[y][x].x = x * TILE_SIZE;
            window_tile[y][x].y = y * TILE_SIZE;
            window_tile[y][x].w = TILE_SIZE;
            window_tile[y][x].h = TILE_SIZE;
        }
    }

    /* Event loop */
    is_running = true;
    while (is_running) {
        /* Handle input */
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
        }

        /* Clear and draw the screen */
        SDL_SetRenderDrawColor(renderer, 0x66, 0x66, 0xBB, 0xFF);
        SDL_RenderClear(renderer);

        /* Copy texture_tile into window_tile based on tile selected by tilemap */
        int tile_index;
        for (y = 0; y < TILE_Y; y++) {
            for (x = 0; x < TILE_X; x++) {
                tile_index = tilemap[y][x];
                SDL_RenderTexture(renderer, texture, &texture_tile[tile_index], &window_tile[y][x]);
            }
        }

        /* Display the renderer */
        SDL_RenderPresent(renderer);

        /* Generate new tiles to display */
        for (y = 0; y < TILE_Y; y++) {
            for (x = 0; x < TILE_X; x++) {
                tilemap[y][x] = rand() % TILE_COUNT;
            }
        }

        /* Delay before new run */
        SDL_Delay(1000);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();

    return 0;
}
