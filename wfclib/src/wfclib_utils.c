#include "wfclib_api.h"
#include <stdio.h>

char *neighbor_string[4] = {"TOP", "RIGHT", "BOTTOM", "LEFT"};

void wfclib_print_tilemap(tilemap_t *map) {

    int x, y;

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

void wfclib_print_rules(tilemap_t *map) {

    int tile_no, neighbor, tile_count, tile_idx;

    printf("%5s %10s   %s\n", "TILE", "NEIGHBOR", "TILE_LIST");

    for (tile_no = 0; tile_no < map->tile_count; tile_no++) {
        for (neighbor = NEIGHBOR_TOP; neighbor < NEIGHBOR_NUM; neighbor++) {
            tile_count = map->rules[tile_no].possible_neighbor[neighbor].count;
            if (tile_count > 0) {
                printf("%5d %10s   ", tile_no, neighbor_string[neighbor]);
                for (tile_idx = 0; tile_idx < tile_count; tile_idx++) {
                    printf("%d, ", map->rules[tile_no].possible_neighbor[neighbor].tile_no_list[tile_idx]);
                }
                printf("\n");
            }
        }
    }
}
