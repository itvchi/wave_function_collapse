#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include "wfclib_api.h"

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

    SDL_FRect       texture_rect[TILE_COUNT];
    SDL_FRect       window_rect[TILE_Y][TILE_X];
    int             x, y;  
    int             index;

    tilemap_t *map = wfclib_init(TILE_Y, TILE_X, TILE_COUNT);

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
    wfclib_print_tilemap(map);
    wfclib_random(map);
    wfclib_print_tilemap(map);

    wfclib_print_tile(&map->array[0][0]);
    wfclib_print_tile(&map->array[3][3]);
    wfclib_print_tile(&map->array[TILE_Y - 1][TILE_X - 1]);

    /* Create tile rectangles */
    for (index = 0; index < TILE_COUNT; index++) {
        texture_rect[index].x = 0;
        texture_rect[index].y = index * TILE_SIZE;
        texture_rect[index].w = TILE_SIZE;
        texture_rect[index].h = TILE_SIZE;
    }

    /* Create window rectangles */
    for (y = 0; y < TILE_Y; y++) {
        for (x = 0; x < TILE_X; x++) {
            window_rect[y][x].x = x * TILE_SIZE;
            window_rect[y][x].y = y * TILE_SIZE;
            window_rect[y][x].w = TILE_SIZE;
            window_rect[y][x].h = TILE_SIZE;
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

        /* Copy texture_rect into window_rect based on tile selected by tilemap */
        int tile_index;
        for (y = 0; y < TILE_Y; y++) {
            for (x = 0; x < TILE_X; x++) {
                tile_index = map->array[y][x].tile_no;
                    SDL_RenderTexture(renderer, texture, &texture_rect[tile_index], &window_rect[y][x]);
            }
        }

        /* Display the renderer */
        SDL_RenderPresent(renderer);

        /* Generate new tiles to display */
        wfclib_random(map);

        /* Delay before new run */
        SDL_Delay(1000);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    wfclib_destroy(map);

    return 0;
}
