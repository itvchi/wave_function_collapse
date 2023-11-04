#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include "wfclib_api.h"

#define TILE_SIZE       64
#define TILE_COUNT      10
#define ENTROPY_COUNT   (TILE_COUNT + 1) /* From 0 to TILE_COUNT */
#define TILE_X          28
#define TILE_Y          14
#define WINDOW_WIDTH    TILE_SIZE * TILE_X
#define WINDOW_HEIGHT   TILE_SIZE * TILE_Y

#define TILE_FILE "../resources/map.bmp"
#define ENTROPY_FILE "../resources/entropy.bmp"

void set_rules(tilemap_t *map);

int main(int argc, char const *argv[])
{
    SDL_Window      *window;  
    SDL_Renderer    *renderer;  
    SDL_Surface     *tile_surface, *entropy_surface;    
    SDL_Texture     *tile_texture, *entropy_texture;
    bool            is_running;
    SDL_Event       event;  

    SDL_FRect       entropy_rect[ENTROPY_COUNT];
    SDL_FRect       texture_rect[TILE_COUNT];
    SDL_FRect       window_rect[TILE_Y][TILE_X];
    int             x, y;  
    int             index;

    srand(time(NULL));

    tilemap_t *map = wfclib_init(TILE_Y, TILE_X, TILE_COUNT);
    set_rules(map);

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
    tile_surface = SDL_LoadBMP(TILE_FILE);
    entropy_surface = SDL_LoadBMP(ENTROPY_FILE);

    /* Create textures from surface and free surface */
    tile_texture = SDL_CreateTextureFromSurface(renderer, tile_surface);
    entropy_texture = SDL_CreateTextureFromSurface(renderer, entropy_surface);
    SDL_DestroySurface(tile_surface);
    SDL_DestroySurface(entropy_surface);

    /* Create entropy rectangles */
    for (index = 0; index < ENTROPY_COUNT; index++) {
        entropy_rect[index].x = 0;
        entropy_rect[index].y = index * TILE_SIZE;
        entropy_rect[index].w = TILE_SIZE;
        entropy_rect[index].h = TILE_SIZE;
    }

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
                if (map->array[y][x].state == ENTROPY) {
                    tile_index = map->array[y][x].entropy - 1;
                    SDL_RenderTexture(renderer, entropy_texture, &entropy_rect[tile_index], &window_rect[y][x]);
                } else {
                    tile_index = map->array[y][x].tile_no;
                    SDL_RenderTexture(renderer, tile_texture, &texture_rect[tile_index], &window_rect[y][x]);
                }
            }
        }

        /* Display the renderer */
        SDL_RenderPresent(renderer);

        /* Generate new tiles to display */
        //wfclib_random(map);
        wfclib_generate_step(map);

        /* Delay before new run */
        SDL_Delay(40);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    wfclib_destroy(map);

    return 0;
}

void set_rules(tilemap_t *map) {
    /* Tile 0 rules */
    wfclib_tilemap_add_rule(map, 0, NEIGHBOR_TOP,       (int[]){2, 5, 8, 9}, 4);
    wfclib_tilemap_add_rule(map, 0, NEIGHBOR_RIGHT,     (int[]){3, 6}, 2);
    wfclib_tilemap_add_rule(map, 0, NEIGHBOR_BOTTOM,    (int[]){1, 2}, 2);
    wfclib_tilemap_add_rule(map, 0, NEIGHBOR_LEFT,      (int[]){6, 7, 8, 9}, 4);
    /* Tile 1 rules */
    wfclib_tilemap_add_rule(map, 1, NEIGHBOR_TOP,       (int[]){0, 1}, 2);
    wfclib_tilemap_add_rule(map, 1, NEIGHBOR_RIGHT,     (int[]){4, 7}, 2);
    wfclib_tilemap_add_rule(map, 1, NEIGHBOR_BOTTOM,    (int[]){1, 2}, 2);
    wfclib_tilemap_add_rule(map, 1, NEIGHBOR_LEFT,      (int[]){6, 7, 8, 9}, 4);
    /* Tile 2 rules */
    wfclib_tilemap_add_rule(map, 2, NEIGHBOR_TOP,       (int[]){0, 1}, 2);
    wfclib_tilemap_add_rule(map, 2, NEIGHBOR_RIGHT,     (int[]){5, 8}, 2);
    wfclib_tilemap_add_rule(map, 2, NEIGHBOR_BOTTOM,    (int[]){0, 3, 6, 9}, 4);
    wfclib_tilemap_add_rule(map, 2, NEIGHBOR_LEFT,      (int[]){6, 7, 8, 9}, 4);
    /* Tile 3 rules */
    wfclib_tilemap_add_rule(map, 3, NEIGHBOR_TOP,       (int[]){2, 5, 8, 9}, 4);
    wfclib_tilemap_add_rule(map, 3, NEIGHBOR_RIGHT,     (int[]){3, 6}, 2);
    wfclib_tilemap_add_rule(map, 3, NEIGHBOR_BOTTOM,    (int[]){4, 5}, 2);
    wfclib_tilemap_add_rule(map, 3, NEIGHBOR_LEFT,      (int[]){0, 3}, 2);
    /* Tile 4 rules */
    wfclib_tilemap_add_rule(map, 4, NEIGHBOR_TOP,       (int[]){3, 4}, 2);
    wfclib_tilemap_add_rule(map, 4, NEIGHBOR_RIGHT,     (int[]){4, 7}, 2);
    wfclib_tilemap_add_rule(map, 4, NEIGHBOR_BOTTOM,    (int[]){4, 5}, 2);
    wfclib_tilemap_add_rule(map, 4, NEIGHBOR_LEFT,      (int[]){1, 4}, 2);
    /* Tile 5 rules */
    wfclib_tilemap_add_rule(map, 5, NEIGHBOR_TOP,       (int[]){3, 4}, 2);
    wfclib_tilemap_add_rule(map, 5, NEIGHBOR_RIGHT,     (int[]){5, 8}, 2);
    wfclib_tilemap_add_rule(map, 5, NEIGHBOR_BOTTOM,    (int[]){2, 5, 8 ,9}, 4);
    wfclib_tilemap_add_rule(map, 5, NEIGHBOR_LEFT,      (int[]){2, 5}, 2);
    /* Tile 6 rules */
    wfclib_tilemap_add_rule(map, 6, NEIGHBOR_TOP,       (int[]){2, 5, 8, 9}, 4);
    wfclib_tilemap_add_rule(map, 6, NEIGHBOR_RIGHT,     (int[]){0, 1, 2, 9}, 4);
    wfclib_tilemap_add_rule(map, 6, NEIGHBOR_BOTTOM,    (int[]){7, 8}, 2);
    wfclib_tilemap_add_rule(map, 6, NEIGHBOR_LEFT,      (int[]){0, 3}, 2);
    /* Tile 7 rules */
    wfclib_tilemap_add_rule(map, 7, NEIGHBOR_TOP,       (int[]){6, 7}, 2);
    wfclib_tilemap_add_rule(map, 7, NEIGHBOR_RIGHT,     (int[]){0, 1, 2, 9}, 4);
    wfclib_tilemap_add_rule(map, 7, NEIGHBOR_BOTTOM,    (int[]){7, 8}, 2);
    wfclib_tilemap_add_rule(map, 7, NEIGHBOR_LEFT,      (int[]){1, 4}, 2);
    /* Tile 8 rules */
    wfclib_tilemap_add_rule(map, 8, NEIGHBOR_TOP,       (int[]){6, 7}, 2);
    wfclib_tilemap_add_rule(map, 8, NEIGHBOR_RIGHT,     (int[]){0, 1, 2, 9}, 4);
    wfclib_tilemap_add_rule(map, 8, NEIGHBOR_BOTTOM,    (int[]){0, 3, 6, 9}, 4);
    wfclib_tilemap_add_rule(map, 8, NEIGHBOR_LEFT,      (int[]){2, 5}, 2);
    /* Tile 9 rules */
    wfclib_tilemap_add_rule(map, 9, NEIGHBOR_TOP,       (int[]){2, 5, 8, 9}, 4);
    wfclib_tilemap_add_rule(map, 9, NEIGHBOR_RIGHT,     (int[]){0, 1, 2, 9}, 4);
    wfclib_tilemap_add_rule(map, 9, NEIGHBOR_BOTTOM,    (int[]){0, 3, 6, 9}, 4);
    wfclib_tilemap_add_rule(map, 9, NEIGHBOR_LEFT,      (int[]){6, 7, 8, 9}, 4);

    //wfclib_print_rules(map);
}