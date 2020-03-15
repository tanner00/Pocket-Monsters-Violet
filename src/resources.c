#include "include/resources.h"
#include "include/tilemap.h"

#include <assert.h>
#include <stdlib.h>

Resources resources;

void resources_init(void) {
	sfTexture *player_atlas =
		sfTexture_createFromFile("res/player.png", NULL);
	assert(player_atlas);
	resources.player_atlas = player_atlas;

	sfSprite *player_sprite = sfSprite_create();
	assert(player_sprite);
	resources.player_sprite = player_sprite;

	sfSprite_setTexture(player_sprite, player_atlas, true);
	sfIntRect idle_animation = {0, 0, TILE_TEXTURE_SIZE, TILE_TEXTURE_SIZE};
	sfSprite_setTextureRect(player_sprite, idle_animation);
	sfSprite_scale(player_sprite, (sfVector2f){TILE_SCALE, TILE_SCALE});

	// The tiles are stored in a big image where the dimensions are
	// (16*n)x16 where n is the number of tiles. This layout makes it easy
	// to go from TileId to what sprite should be rendered on screen.
	sfTexture *const tile_atlas =
		sfTexture_createFromFile("res/tileset.png", NULL);
	assert(tile_atlas);
	resources.tile_atlas = tile_atlas;

	const size_t num_tile_sprites =
		sfTexture_getSize(tile_atlas).x / TILE_TEXTURE_SIZE;
	sfSprite **const tile_sprites =
		malloc(num_tile_sprites * sizeof(*tile_sprites));
	assert(tile_sprites);
	resources.tile_sprites = tile_sprites;
	resources.num_tile_sprites = num_tile_sprites;

	// Fill out all the information for each sprite in the tile atlas.
	for (size_t i = 0; i < num_tile_sprites; ++i) {
		// @TODO: We commonly make a sprite like this... Make this a
		// function?
		tile_sprites[i] = sfSprite_create();
		assert(tile_sprites[i]);

		sfSprite_setTexture(tile_sprites[i], tile_atlas, true);
		const sfIntRect tile_cutout = {i * TILE_TEXTURE_SIZE, 0,
					       TILE_TEXTURE_SIZE,
					       TILE_TEXTURE_SIZE};
		sfSprite_setTextureRect(tile_sprites[i], tile_cutout);

		sfSprite_scale(tile_sprites[i],
			       (sfVector2f){TILE_SCALE, TILE_SCALE});
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
