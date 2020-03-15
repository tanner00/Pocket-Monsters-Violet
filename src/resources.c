#include "include/resources.h"
#include "include/tilemap.h"

#include <assert.h>
#include <stdlib.h>

Resources resources;

// A lot of the game uses an atlas of images for performance (Fewer texture
// swaps) and easier animation. This is a repetitive operation for the game, so
// I made a helper function.
sfSprite *make_cutout_sprite(const sfTexture *texture, sfIntRect cutout) {
	sfSprite *sprite = sfSprite_create();
	assert(sprite);
	sfSprite_setTexture(sprite, texture, true);
	sfSprite_setTextureRect(sprite, cutout);
	return sprite;
}

void resources_init(void) {
	sfTexture *player_atlas =
		sfTexture_createFromFile("res/player.png", NULL);
	assert(player_atlas);
	resources.player_atlas = player_atlas;

	sfSprite *player_sprite = make_cutout_sprite(
		player_atlas, (sfIntRect){0, 0, TILE_SIZE, TILE_SIZE});
	resources.player_sprite = player_sprite;

	// The tiles are stored in a big image which has dimensions
	// (16*n)x16 where n is the number of tiles. This layout makes it easy
	// to go from TileId to what sprite should be rendered on screen.
	sfTexture *const tile_atlas =
		sfTexture_createFromFile("res/tileset.png", NULL);
	assert(tile_atlas);
	resources.tile_atlas = tile_atlas;

	const size_t num_tile_sprites =
		sfTexture_getSize(tile_atlas).x / TILE_SIZE;
	sfSprite **const tile_sprites =
		malloc(num_tile_sprites * sizeof(*tile_sprites));
	assert(tile_sprites);
	resources.tile_sprites = tile_sprites;
	resources.num_tile_sprites = num_tile_sprites;

	// Fill out all the information for each sprite in the tile atlas.
	for (size_t i = 0; i < num_tile_sprites; ++i) {
		tile_sprites[i] = make_cutout_sprite(
			tile_atlas,
			(sfIntRect){i * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE});
	}
}

void resources_destroy(void) {
	free(resources.player_atlas);
	resources.player_atlas = NULL;
	free(resources.player_sprite);
	resources.player_sprite = NULL;
	for (size_t i = 0; i < resources.num_tile_sprites; ++i) {
		free(resources.tile_sprites[i]);
		resources.tile_sprites[i] = NULL;
	}
}
