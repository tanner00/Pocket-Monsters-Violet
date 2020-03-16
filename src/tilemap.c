#include "include/tilemap.h"
#include "include/math.h"
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
	return (sfVector2u){map_coords.x / TILE_SIZE, map_coords.y / TILE_SIZE};
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

TileId tilemap_get_tile(const Tilemap *tilemap, sfVector2u pos) {
	assert(pos.x < tilemap->dimensions.x);
	assert(pos.y < tilemap->dimensions.y);

	return tilemap->tiles[pos.x + pos.y * tilemap->dimensions.x];
}

void tilemap_draw(const Tilemap *tilemap, sfRenderWindow *window,
		  const sfView *camera) {
	const sfVector2u dimensions = tilemap->dimensions;
	const TileId *tiles = tilemap->tiles;

	const sfVector2f camera_center = sfView_getCenter(camera);
	const sfVector2f camera_size = sfView_getSize(camera);

	// This prevents the game from drawing the entire map at once because
	// for even moderate sizes there's a huge drop in FPS.

	// The camera's center is used to see what should be drawn on the
	// screen.
	// If you have a center point, you can convert to a bounding box
	// if you know the size of the box.
	// For example: left_extent = x_center - x_size / 2

	// These world coordinates are then converted to tile coordinates to
	// index into the tiles array. The +-1's tell the game to render a
	// little over because half of a tile can be in the view of
	// the camera.

	// If a min or max exceeds the tile
	// boundaries (< 0 or >= a dimension), clamp it so it doesn't index out
	// of bounds.

	const size_t x_min =
		MAX((camera_center.x - camera_size.x / 2) / TILE_SIZE - 1, 0);
	const size_t x_max =
		MIN((camera_center.x + camera_size.x / 2) / TILE_SIZE + 1,
		    dimensions.x);
	const size_t y_min =
		MAX((camera_center.y - camera_size.y / 2) / TILE_SIZE - 1, 0);
	const size_t y_max =
		MIN((camera_center.y + camera_size.y / 2) / TILE_SIZE + 1,
		    dimensions.y);

	for (size_t i = x_min; i < x_max; ++i) {
		for (size_t j = y_min; j < y_max; ++j) {
			const TileId tile_id = tiles[i + j * dimensions.x];

			sfSprite *tile_sprite = resources.tiles[tile_id];
			sfSprite_setPosition(
				tile_sprite,
				(sfVector2f){i * TILE_SIZE, j * TILE_SIZE});
			sfRenderWindow_drawSprite(window, tile_sprite, NULL);
		}
	}
}
