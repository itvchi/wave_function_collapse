#ifndef _WFCLIB_H_
#define _WFCLIB_H_

int wfclib_find_min_entropy(tilemap_t *map);
tile_t* wfclib_find_with_entropy(tilemap_t *map, int entropy);
void wfclib_update_neighbors(tile_t *tile);

#endif /* _WFCLIB_H_ */