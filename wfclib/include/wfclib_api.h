#ifndef _WFCLIB_API_H_
#define _WFCLIB_API_H_

typedef enum {
    ENTROPY = 0,
    TILE
} tile_state_t;

typedef struct {
    tile_state_t state; 
    /* Store entropy value or final tile number selected by algorithm */
    union {
        int entropy;
        int tile_no;
    };
} tile_t;

typedef struct {
    int width;
    int height;
    unsigned int tile_count;
    tile_t** array;
} tilemap_t;

tilemap_t* wfclib_init(const int height, const int width, const unsigned int tile_count);
void wfclib_destroy(tilemap_t *map);
void wfclib_random(tilemap_t *map);

#endif /* _WFCLIB_API_H_ */