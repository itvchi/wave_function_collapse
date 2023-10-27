#include "wfclib_api.h"
#include "wfclib.h"
#include <stdio.h>
#include <stdlib.h>

tilemap_t* wfclib_init(const int height, const int width, const unsigned int tile_count) {

    int         index_y, index_x;
    tilemap_t   *map = malloc(sizeof(tilemap_t));

    if (map) {
        map->width = width;
        map->height = height;
        map->tile_count = tile_count;

        /* Allocate memory for pointers to horizontal tile_t arrays */
        map->array = malloc(height * sizeof(tile_t *));

        if (map->array) {
            /* Allocate horizontal tile_t arrays */
            for (index_y = 0; index_y < height; index_y++) {
                map->array[index_y] = malloc(width * sizeof(tile_t));

                /* Init tile_t structure members */
                if (map->array[index_y]) {
                    for (index_x = 0; index_x < width; index_x++) {
                        map->array[index_y][index_x].state = ENTROPY;
                        map->array[index_y][index_x].entropy = tile_count;
                    }
                } else {
                    for (index_y = index_y - 1; index_y >= 0; index_y--) {
                        free(map->array[index_y]);
                    }

                    free(map->array);
                    free(map);
                    map = NULL;
                    break;
                }
            }

            /* Assign neighbors for each tile */
            for (index_y = 0; index_y < height; index_y++) {
                for (index_x = 0; index_x < width; index_x++) {
                    map->array[index_y][index_x].neighbor[NEIGHBOR_TOP] = ((index_y > 0) ? &map->array[index_y - 1][index_x] : NULL);
                    map->array[index_y][index_x].neighbor[NEIGHBOR_RIGHT] = ((index_x + 1 < width) ? &map->array[index_y][index_x + 1] : NULL);
                    map->array[index_y][index_x].neighbor[NEIGHBOR_BOTTOM] = ((index_y + 1 < height) ? &map->array[index_y + 1][index_x] : NULL);
                    map->array[index_y][index_x].neighbor[NEIGHBOR_LEFT] = ((index_x > 0) ? &map->array[index_y][index_x - 1] : NULL);
                }
            }
        } else {
            free(map);
            map = NULL;
        }
    }

    /* Return pointer to tilemap_t or NULL if malloc fails */
    return map;
}

void wfclib_destroy(tilemap_t *map) {

    int     index;

    for (index = 0; index < map->height; index++) {
        free(map->array[index]);
    }

    free(map->array);
    free(map);
}

void wfclib_random(tilemap_t *map) {

    int     x, y;

    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            map->array[y][x].state = TILE;
            map->array[y][x].tile_no = rand() % map->tile_count;
        }
    }
}

void wfclib_print_tilemap(tilemap_t *map) {

    int     x, y;

    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            if (map->array[y][x].state == ENTROPY) {
                printf("[%*d]  ", 2, map->array[y][x].entropy);
            } else if (map->array[y][x].state == TILE) {
                printf("%*d  ", 4, map->array[y][x].tile_no);
            }
        }
        printf("\n");
    }
}

void wfclib_print_tile(tile_t *tile) {

    /* Top neighbor */
    if (tile->neighbor[NEIGHBOR_TOP]) {
        printf("[    ]  [%*d]  [    ]\n", 4, tile->neighbor[NEIGHBOR_TOP]->tile_no);
    } else {
        printf("[    ]  [%s]  [    ]\n", "NULL");
    }
    /* Left neighbor */
    if (tile->neighbor[NEIGHBOR_LEFT]) {
        printf("[%*d]  ", 4, tile->neighbor[NEIGHBOR_LEFT]->tile_no);
    } else {
        printf("[%s]  ", "NULL");
    }
    /* This tile */
    printf("[%*d]  ", 4, tile->tile_no);
    /* Right neighbor */
    if (tile->neighbor[NEIGHBOR_RIGHT]) {
        printf("[%*d]\n", 4, tile->neighbor[NEIGHBOR_RIGHT]->tile_no);
    } else {
        printf("[%s]\n", "NULL");
    }
    /* Bottom neighbor */
    if (tile->neighbor[NEIGHBOR_BOTTOM]) {
        printf("[    ]  [%*d]  [    ]\n", 4, tile->neighbor[NEIGHBOR_BOTTOM]->tile_no);
    } else {
        printf("[    ]  [%s]  [    ]\n", "NULL");
    }
    printf("\n");
}