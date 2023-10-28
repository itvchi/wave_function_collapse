#include "wfclib_api.h"
#include "wfclib.h"
#include <stdio.h>
#include <stdlib.h>

static int step = 0;

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

int wfclib_find_min_entropy(tilemap_t *map) {

    int x, y;
    int min_entropy = map->tile_count + 1; /* Init with non existing value */

    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            if ((map->array[y][x].state == ENTROPY) &&
                (map->array[y][x].entropy <= min_entropy)) {
                    min_entropy = map->array[y][x].entropy;
            }
        }
    }

    return min_entropy;
}

tile_t* wfclib_find_with_entropy(tilemap_t *map, int entropy) {

    int x, y;
    tile_t *tile_ptr = NULL;
    int next = rand() % 4 + 1;

    while (next > 0) {
        for (y = 0; y < map->height; y++) {
            for (x = 0; x < map->width; x++) {
                if ((map->array[y][x].state == ENTROPY) &&
                    (map->array[y][x].entropy == entropy)) {
                        tile_ptr = &map->array[y][x];
                        next--;
                        if (next == 0) {
                            break;
                        }
                }
            }
        }
    }

    return tile_ptr;
}

void wfclib_update_neighbors(tile_t *tile) {

    int index, random;

    for (index = 0; index < NEIGHBOR_NUM; index++)
    if (tile->neighbor[index]) {
        if ((tile->neighbor[index]->state == ENTROPY) &&
            (tile->neighbor[index]->entropy > 0)) {
                random = rand();
                if (random % 3) {
                    tile->neighbor[index]->entropy -= 2;
                } else if(random % 7) {
                    tile->neighbor[index]->entropy -= 3;
                } else {
                    tile->neighbor[index]->entropy--;
                }
        }
    }
}

void wfclib_generate_step(tilemap_t *map) {

    int x, y;
    tile_t *tile_ptr;
    int min_entropy;

    if (step == 0) {
        x = rand() % map->width;
        y = rand() % map->height;

        tile_ptr = &map->array[y][x];
        printf("Step 1 done\n");

        tile_ptr->state = TILE;
        tile_ptr->tile_no = rand() % map->tile_count;
        wfclib_update_neighbors(tile_ptr);
    } else {
        min_entropy = wfclib_find_min_entropy(map);

        if (min_entropy <= map->tile_count) {
            printf("Found min entropy: %d\n", min_entropy);
            tile_ptr = wfclib_find_with_entropy(map, min_entropy);

            tile_ptr->state = TILE;
            tile_ptr->tile_no = rand() % map->tile_count;
            wfclib_update_neighbors(tile_ptr);
        } else {
            printf("Nothing to do\n");
        }
    }

    step++;
}