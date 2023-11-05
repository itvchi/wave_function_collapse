#include "wfclib_api.h"
#include "wfclib.h"
#include "wfclib_stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int step = 0;

static unsigned int value_to_bit(unsigned int value) {

    return (1 << value);
}

static unsigned int bits_to_value(unsigned int bits) {

    int index, value = 0;

    for (index = 0; index < (8 * sizeof(int)); index++) {
        if (bits & (1 << index)) {
            value++;
        }
    }

    return value;
}

tilemap_t* wfclib_init(const int height, const int width, const unsigned int tile_count) {

    int         index_y, index_x;
    tilemap_t   *map = malloc(sizeof(tilemap_t));

    if (map) {
        map->width = width;
        map->height = height;
        map->tile_count = tile_count;
        map->rules = NULL;

        /* Allocate memory for pointers to horizontal tile_t arrays */
        map->array = malloc(height * sizeof(tile_t *));

        if (map->array) {
            /* Allocate horizontal tile_t arrays */
            for (index_y = 0; index_y < height; index_y++) {
                map->array[index_y] = malloc(width * sizeof(tile_t));

                /* Init tile_t structure members */
                if (map->array[index_y]) {
                    for (index_x = 0; index_x < width; index_x++) {
                        map->array[index_y][index_x].y = index_y;
                        map->array[index_y][index_x].x = index_x;
                        map->array[index_y][index_x].state = ENTROPY;
                        /* Decrement to set all bits below tile_count bit */
                        map->array[index_y][index_x].entropy_bitfields = value_to_bit(tile_count) - 1;
                        map->array[index_y][index_x].entropy_value = tile_count;
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
                (map->array[y][x].entropy_value <= min_entropy)) {
                    min_entropy = map->array[y][x].entropy_value;
            }
        }
    }

    return min_entropy;
}

stack_t *wfclib_lowest_entropy(tilemap_t *map) {

    int x, y;
    int min_entropy_value = map->tile_count + 1; /* Init with non existing value */
    int min_entropy_count = 0;

    /* Traverse all tiles and find lowest entropy - it's better approach than create and destroy
        the stack every time lower entropy is found */
    for (y = 0; y < map->height; y++) {
        for (x = 0; x < map->width; x++) {
            if (map->array[y][x].state == ENTROPY) {
                /* Update entropy value and reset counter (set 1 because first value was found in this iteration) */
                if (map->array[y][x].entropy_value < min_entropy_value) {
                    min_entropy_value = map->array[y][x].entropy_value;
                    min_entropy_count = 1;
                /* Increment tiles count with lowest entropy */
                } else if (map->array[y][x].entropy_value == min_entropy_value) {
                    min_entropy_count++;
                } 
            }
        }
    }

    printf("Lowest entropy is %d (%d)\n", min_entropy_value, min_entropy_count);

    stack_t *result = wfclib_stack_init(min_entropy_count);

    if (result) {
        for (y = 0; y < map->height; y++) {
            for (x = 0; x < map->width; x++) {
                if ((map->array[y][x].state == ENTROPY) && (map->array[y][x].entropy_value == min_entropy_value)) {
                    wfclib_stack_push(result, &map->array[y][x]);
                    printf("Added tile y= %d, x= %d\n", y, x);
                }
            }
        }
    }

    return result;
}

tile_t* wfclib_find_with_entropy(tilemap_t *map, int entropy) {

    int x, y;
    tile_t *tile_ptr = NULL;
    int next = rand() % 4 + 1;

    while (next > 0) {
        for (y = 0; y < map->height; y++) {
            for (x = 0; x < map->width; x++) {
                if ((map->array[y][x].state == ENTROPY) &&
                    (map->array[y][x].entropy_value == entropy)) {
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

    for (index = NEIGHBOR_TOP; index < NEIGHBOR_NUM; index++)
    if (tile->neighbor[index]) {
        if ((tile->neighbor[index]->state == ENTROPY) &&
            (tile->neighbor[index]->entropy_value > 0)) {
                random = rand();
                if (random % 3) {
                    tile->neighbor[index]->entropy_value -= 2;
                } else if(random % 7) {
                    tile->neighbor[index]->entropy_value -= 3;
                } else {
                    tile->neighbor[index]->entropy_value--;
                }
        }
    }
}

void wfclib_generate_step(tilemap_t *map) {

    //int x, y;
    tile_t *tile_ptr;
    unsigned int random;
    unsigned int tiles = map->height * map->width;
    //int min_entropy;

    random = (tiles / 4) + (rand() % (tiles / 2));
    
    stack_t *result = wfclib_lowest_entropy(map);

    if (result) {

        random = random % result->capacity;

        printf("Got stack: size %d\n", result->capacity);
        printf("Take element no %d\n", random);
        tile_ptr = wfclib_stack_element(result, random);
        printf("Taken tile y= %d, x= %d\n", tile_ptr->y, tile_ptr->x);

        tile_ptr->state = TILE;
        tile_ptr->tile_no = rand() % map->tile_count;
        wfclib_update_neighbors(tile_ptr);

        wfclib_stack_free(result);
    }

    // if (step == 0) {
    //     x = rand() % map->width;
    //     y = rand() % map->height;

    //     tile_ptr = &map->array[y][x];

    //     tile_ptr->state = TILE;
    //     tile_ptr->tile_no = rand() % map->tile_count;
    //     wfclib_update_neighbors(tile_ptr);
    // } else {
    //     min_entropy = wfclib_find_min_entropy(map);

    //     if (min_entropy <= map->tile_count) {
    //         printf("Found min entropy: %d\n", min_entropy);
    //         tile_ptr = wfclib_find_with_entropy(map, min_entropy);

    //         tile_ptr->state = TILE;
    //         tile_ptr->tile_no = rand() % map->tile_count;
    //         wfclib_update_neighbors(tile_ptr);
    //     } else {
    //         printf("Nothing to do\n");
    //     }
    // }

    step++;
}

void wfclib_tilemap_add_rule(tilemap_t *map, int tile_no, neighbor_t side, int *tile_no_list, int count) {

    unsigned int index;

    /* Init map->rules array if not initialized yet */
    if (map->rules == NULL) {
        map->rules = calloc(map->tile_count, sizeof(tile_rules_t));
    }   

    /* Add rule */
    if (map->rules[tile_no].possible_neighbor[side].possible_tile_no_list == NULL) {
        map->rules[tile_no].possible_neighbor[side].count = count;
        map->rules[tile_no].possible_neighbor[side].possible_tile_no_list = malloc(count * sizeof(int));
        if (map->rules[tile_no].possible_neighbor[side].possible_tile_no_list) {
            memcpy(map->rules[tile_no].possible_neighbor[side].possible_tile_no_list, tile_no_list, count * sizeof(int));

            /* Create possible_tiles_bitfield from possible_tile_no_list */
            map->rules[tile_no].possible_neighbor[side].possible_tiles_bitfield = 0;
            for (index = 0; index < count; index++) {
                map->rules[tile_no].possible_neighbor[side].possible_tiles_bitfield |= value_to_bit(map->rules[tile_no].possible_neighbor[side].possible_tile_no_list[index]);
            }
        }
    }
}