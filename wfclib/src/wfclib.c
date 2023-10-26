#include "wfclib_api.h"
#include "wfclib.h"
#include <stdio.h>
#include <stdlib.h>

tilemap_t* wfclib_init(const int height, const int width) {

    int         index;
    tilemap_t   *map = malloc(sizeof(tilemap_t));

    if (map) {
        map->width = width;
        map->height = height;

        /* Allocate memory for pointers to horizontal tile_t arrays */
        map->array = malloc(height * sizeof(tile_t *));

        if (map->array) {
            /* Allocate horizontal tile_t arrays */
            for (index = 0; index < height; index++) {
                map->array[index] = malloc(width * sizeof(tile_t));

                if (map->array[index] == NULL) {
                    for (index = index - 1; index >= 0; index--) {
                        free(map->array[index]);
                    }

                    free(map->array);
                    free(map);
                    map = NULL;
                    break;
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