#ifndef TILEMAP_H
#define TILEMAP_H

#include <SFML/Graphics.h>

#include <stdbool.h>
#include <stddef.h>

// Tile ID numbers which index into the tile_sprites array
typedef enum {
	GRASS = 0,
	TALL_GRASS = 1,
	TREE = 2,
	WATER = 3,
} TileId;

// @NOTE: Third generation Pokemon games used 16x16 tile images and the screen
// showed 15x10 tiles at a time
#define TILE_SIZE 16
#define SCREEN_WIDTH (4 * (TILE_SIZE) * (15))
#define SCREEN_HEIGHT (4 * (TILE_SIZE) * (10))

typedef struct {
	// The map is made up of equally sized sprites arranged in a 2D grid.
	// This is a linear array that is indexed using the formula x + y *
	// map_width.
	TileId *tiles;

	sfVector2u dimensions;
} Tilemap;

bool is_tile_solid(TileId tile);
sfVector2u to_tile_coords(sfVector2u map_coords);

void tilemap_init(Tilemap *tilemap, const char *map_path);
void tilemap_destroy(Tilemap *tilemap);

TileId tilemap_get_tile(const Tilemap *tilemap, sfVector2u pos);
void tilemap_draw(const Tilemap *tilemap, sfRenderWindow *window,
		  const sfView *camera);

#endif
