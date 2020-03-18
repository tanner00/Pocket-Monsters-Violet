#include "include/resources.h"
#include "include/tilemap.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

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

// @TODO: Separate this into different components when there are more resources.
// Split into related parts (battle, overworld, etc...)
void resources_init(void) {

	sfImage *font_8x8 = sfImage_createFromFile("res/font.png");
	assert(font_8x8);
	resources.font_8x8 = font_8x8;

	// Load textures
	sfTexture *const player_atlas_texture =
		sfTexture_createFromFile("res/player.png", NULL);
	assert(player_atlas_texture);
	resources.player_atlas_texture = player_atlas_texture;

	// The tiles are stored in a big image which has dimensions
	// (16*n)x16 where n is the number of tiles. This layout makes it easy
	// to go from TileId to what sprite should be rendered on screen.
	sfTexture *const tile_atlas_texture =
		sfTexture_createFromFile("res/tileset.png", NULL);
	assert(tile_atlas_texture);
	resources.tile_atlas_texture = tile_atlas_texture;

	sfTexture *battle_ui_texture =
		sfTexture_createFromFile("res/battle_ui.png", NULL);
	assert(battle_ui_texture);
	resources.battle_ui_texture = battle_ui_texture;

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

	// Use loaded textures to make sprites
	sfSprite *player_idle = make_cutout_sprite(
		player_atlas_texture, (sfIntRect){0, 0, TILE_SIZE, TILE_SIZE});
	resources.player_idle = player_idle;

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

	sfSprite *battle_button_bg = sfSprite_create();
	assert(battle_button_bg);
	resources.battle_button_bg = battle_button_bg;
	sfSprite_setTexture(battle_button_bg, battle_ui_texture, true);

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

	sfRectangleShape *const move_button = sfRectangleShape_create();
	assert(move_button);
	resources.move_button = move_button;
	// Dimensions of the transparent button areas in res/battle_ui.png
	sfRectangleShape_setSize(move_button, (sfVector2f){89, 18});

	resources.battle_action_text =
		malloc(3 * sizeof(*resources.battle_action_text));
	resources.battle_action_text[0] = draw_text("Items", sfWhite, 1.0);
	resources.battle_action_text[1] = draw_text("Switch", sfWhite, 1.0);
	resources.battle_action_text[2] = draw_text("Run", sfWhite, 1.0);
}

void resources_destroy(void) {
	sfImage_destroy(resources.font_8x8);

	sfTexture_destroy(resources.player_atlas_texture);
	sfTexture_destroy(resources.tile_atlas_texture);

	sfTexture_destroy(resources.battle_ui_texture);
	sfTexture_destroy(resources.battle_bg_texture);
	sfTexture_destroy(resources.monsters_front_texture);

	sfSprite_destroy(resources.player_idle);
	for (size_t i = 0; i < resources.num_tiles; ++i) {
		sfSprite_destroy(resources.tiles[i]);
	}

	sfSprite_destroy(resources.battle_button_bg);
	sfSprite_destroy(resources.battle_bg);
	for (size_t i = 0; i < resources.num_monsters; ++i) {
		sfSprite_destroy(resources.monsters_front[i]);
	}

	sfRectangleShape_destroy(resources.move_button);
	for (size_t i = 0; i < 3; ++i) {
		sfTexture_destroy(resources.battle_action_text[i].texture);
		sfSprite_destroy(resources.battle_action_text[i].sprite);
	}
}

// SFML's interface for drawing text forces fonts to be anti-aliased. In the
// general case, this is good. However, for text being rendered at such a small
// scale with a font that's supposed to be pixelated, this destroys the look of
// the text. I made the decision to make my own simple text renderer using a
// monospace bitmap font I found (on this page
// https://www.seasip.info/VintagePC/cga.html).
// Every character is 8x8 and the font uses the same order as ASCII, so
// it's very easy to take an ASCII character code and convert it into where the
// top left of a character will be in the image.
DrawableText draw_text(const char *text, sfColor color, double scale) {
	const size_t len = strlen(text);
	const size_t image_width = len * 8;
	const size_t image_height = 8;
	sfImage *const text_image = sfImage_create(image_width, image_height);
	assert(text_image);
	for (size_t i = 0; i < len; ++i) {
		sfIntRect src;
		// The font image has 32 characters per row
		src.left = (text[i] % 32) * 8;
		src.top = (text[i] / 32) * 8;
		src.width = 32;
		src.height = 32;

		sfImage_copyImage(text_image, resources.font_8x8, i * 8, 0, src,
				  false);
	}

	sfTexture *const text_texture =
		sfTexture_createFromImage(text_image, NULL);
	assert(text_texture);

	sfSprite *const rendered_text = sfSprite_create();
	assert(rendered_text);
	sfSprite_setTexture(rendered_text, text_texture, false);
	sfSprite_setScale(rendered_text, (sfVector2f){scale, scale});
	sfSprite_setColor(rendered_text, color);

	sfImage_destroy(text_image);

	DrawableText finished_text;
	finished_text.texture = text_texture;
	finished_text.sprite = rendered_text;
	finished_text.len = len;
	return finished_text;
}
