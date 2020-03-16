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

// @TODO: Separate this into different components when there are more resources
void resources_init(void) {
	// Load textures
	sfTexture *const player_atlas_texture =
		sfTexture_createFromFile("res/player.png", NULL);
	assert(player_atlas_texture);
	resources.player_atlas_texture = player_atlas_texture;

	sfTexture *battle_ui_atlas_texture =
		sfTexture_createFromFile("res/battle_ui.png", NULL);
	assert(battle_ui_atlas_texture);
	resources.battle_ui_atlas_texture = battle_ui_atlas_texture;

	sfTexture *const battle_bg_texture =
		sfTexture_createFromFile("res/battle_bg.png", NULL);
	assert(battle_bg_texture);
	resources.battle_bg_texture = battle_bg_texture;

	// The monster sprites are stored in a big image which has dimensions
	// (64*n)x64 where n is the number of monsters.
	sfTexture *const monsters_front_texture =
		sfTexture_createFromFile("res/monsters_front.png", NULL);
	assert(monsters_front_texture);
	resources.monsters_front_texture = monsters_front_texture;

	// The tiles are stored in a big image which has dimensions
	// (16*n)x16 where n is the number of tiles. This layout makes it easy
	// to go from TileId to what sprite should be rendered on screen.
	sfTexture *const tile_atlas_texture =
		sfTexture_createFromFile("res/tileset.png", NULL);
	assert(tile_atlas_texture);
	resources.tile_atlas_texture = tile_atlas_texture;

	// Use loaded textures to make sprites
	sfSprite *player_idle = make_cutout_sprite(
		player_atlas_texture, (sfIntRect){0, 0, TILE_SIZE, TILE_SIZE});
	resources.player_idle = player_idle;

	sfSprite *battle_button_bg = make_cutout_sprite(
		battle_ui_atlas_texture, (sfIntRect){0, 0, 240, 45});
	resources.battle_button_bg = battle_button_bg;

	sfSprite *battle_bg = sfSprite_create();
	assert(battle_bg);
	sfSprite_setTexture(battle_bg, battle_bg_texture, true);
	resources.battle_bg = battle_bg;

	const size_t num_monsters =
		sfTexture_getSize(monsters_front_texture).x /
		MONSTER_SPRITE_SIZE;
	resources.num_monsters = num_monsters;
	sfSprite **const monsters_front =
		malloc(num_monsters * sizeof(*monsters_front));
	assert(monsters_front);
	resources.monsters_front = monsters_front;
	// Fill out all the information for each monster sprite
	for (size_t i = 0; i < num_monsters; ++i) {
		monsters_front[i] = make_cutout_sprite(
			monsters_front_texture,
			(sfIntRect){i * MONSTER_SPRITE_SIZE, 0,
				    MONSTER_SPRITE_SIZE, MONSTER_SPRITE_SIZE});
	}

	const size_t num_tiles =
		sfTexture_getSize(tile_atlas_texture).x / TILE_SIZE;
	sfSprite **const tiles = malloc(num_tiles * sizeof(*tiles));
	assert(tiles);
	resources.tiles = tiles;
	resources.num_tiles = num_tiles;
	// Fill out all the information for each sprite in the tile atlas.
	for (size_t i = 0; i < num_tiles; ++i) {
		tiles[i] = make_cutout_sprite(
			tile_atlas_texture,
			(sfIntRect){i * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE});
	}

	sfFont *font = sfFont_createFromFile("res/font.ttf");
	assert(font);
	resources.font = font;
}

void resources_destroy(void) {
	sfSprite_destroy(resources.player_idle);
	sfTexture_destroy(resources.player_atlas_texture);

	sfSprite_destroy(resources.battle_bg);
	sfTexture_destroy(resources.battle_bg_texture);

	for (size_t i = 0; i < resources.num_tiles; ++i) {
		sfSprite_destroy(resources.tiles[i]);
	}
	sfTexture_destroy(resources.player_atlas_texture);

	sfFont_destroy(resources.font);
}
