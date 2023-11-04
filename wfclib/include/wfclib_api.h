#ifndef _WFCLIB_API_H_
#define _WFCLIB_API_H_

typedef enum {
    ENTROPY = 0,
    TILE
} tile_state_t;

typedef enum {
    NEIGHBOR_TOP = 0,
    NEIGHBOR_RIGHT,
    NEIGHBOR_BOTTOM,
    NEIGHBOR_LEFT,
    NEIGHBOR_NUM
} neighbor_t;

typedef struct {
    int count;
    int *tile_no_list;
} tile_rule_t;

typedef struct {
    tile_rule_t possible_neighbor[4];
} tile_rules_t;

typedef struct tile_s {
    int x, y;
    tile_state_t state; 
    /* Store entropy value or final tile number selected by algorithm */
    union {
        int entropy;
        int tile_no;
    };
    struct tile_s *neighbor[4];
} tile_t;

typedef struct {
    int width;
    int height;
    unsigned int tile_count;
    tile_t** array;
    tile_rules_t *rules;
} tilemap_t;

tilemap_t* wfclib_init(const int height, const int width, const unsigned int tile_count);
void wfclib_destroy(tilemap_t *map);
void wfclib_random(tilemap_t *map);
void wfclib_generate_step(tilemap_t *map);
void wfclib_tilemap_add_rule(tilemap_t *map, int tile_no, neighbor_t side, int *tile_no_list, int count);

/* Util functions */
void wfclib_print_tilemap(tilemap_t *map);
void wfclib_print_tile(tile_t *tile);
void wfclib_print_rules(tilemap_t *map);

#endif /* _WFCLIB_API_H_ */