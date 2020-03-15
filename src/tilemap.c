#include "include/tilemap.h"
#include "include/resources.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

// @NOTE: Tilemaps are created by sampling a PNG image. These values represent
// the values of that tile in the map PNG image.
// Tile ID to RGBA color
#define GRASS_COLOR 0x79d54ff
#define TALL_GRASS_COLOR 0x007039ff
#define TREE_COLOR 0x3f1900ff
#define WATER_COLOR 0x30a1f3ff

TileId color_to_tile_id(uint32_t color) {
	switch (color) {
	case GRASS_COLOR:
		return GRASS;
	case TALL_GRASS_COLOR:
		return TALL_GRASS;
	case TREE_COLOR:
		return TREE;
	case WATER_COLOR:
		return WATER;
	}
	return -1;
}

bool is_tile_solid(TileId tile) {
	switch (tile) {
	case GRASS:
		return false;
	case TALL_GRASS:
		return false;
	case TREE:
		return true;
	case WATER:
		return true;
	}
	return true;
}

sfVector2u to_tile_coords(sfVector2u map_coords) {
	return (sfVector2u){map_coords.x / TILE_SPRITE_SIZE,
			    map_coords.y / TILE_SPRITE_SIZE};
}

void tilemap_init(Tilemap *tilemap, const char *map_path) {
	// An image represents some of the information needed for a map.
	// The image stores pixels where each pixel represents what tile will go
	// there.

	// This stage loads the image that will define the map and converts the
	// pixel values into tile ids.
	sfImage *const map_image = sfImage_createFromFile(map_path);
	const sfVector2u map_size = sfImage_getSize(map_image);
	TileId *tiles =
		malloc(map_size.x * map_size.y * sizeof(*tilemap->tiles));
	assert(tiles);
	tilemap->tiles = tiles;
	for (size_t i = 0; i < map_size.x; ++i) {
		for (size_t j = 0; j < map_size.y; ++j) {
			uint32_t p = sfColor_toInteger(
				sfImage_getPixel(map_image, i, j));

			tilemap->tiles[i + j * map_size.x] =
				color_to_tile_id(p);
		}
	}
	tilemap->dimensions = map_size;
	sfImage_destroy(map_image);
}

void tilemap_destroy(Tilemap *tilemap) { free(tilemap->tiles); }

TileId tilemap_get_tile(Tilemap *tilemap, sfVector2u pos) {
	assert(pos.x < tilemap->dimensions.x);
	assert(pos.y < tilemap->dimensions.y);

	return tilemap->tiles[pos.x + pos.y * tilemap->dimensions.x];
}

// @TODO: only draw what is on screen
// @NOTE: this requires the camera view
void tilemap_draw(Tilemap *tilemap, sfRenderWindow *window) {
	const sfVector2u dimensions = tilemap->dimensions;
	const TileId *tiles = tilemap->tiles;
	sfSprite *const *const tile_sprites = resources.tile_sprites;

	for (size_t i = 0; i < dimensions.x; ++i) {
		for (size_t j = 0; j < dimensions.y; ++j) {
			const TileId tile_id = tiles[i + j * dimensions.x];

			sfSprite *tile_sprite = tile_sprites[tile_id];
			sfSprite_setPosition(
				tile_sprite,
				(sfVector2f){i * TILE_SPRITE_SIZE,
					     j * TILE_SPRITE_SIZE});
			sfRenderWindow_drawSprite(window, tile_sprites[tile_id],
						  NULL);
		}
	}
}
