#ifndef RESOURCES_H
#define RESOURCES_H

#include <SFML/Graphics.h>

// All resources live for the entirety of the game which isn't a big deal
// because the game is so small.

#define MONSTER_SPRITE_SIZE 64

// sfTextures need to be kept alive as long as sfSprites are in use, so keep a
// reference to them here.
typedef struct {
	sfImage *font_8x8;

	sfTexture *player_atlas_texture;
	sfTexture *battle_ui_atlas_texture;
	sfTexture *battle_bg_texture;
	sfTexture *monsters_front_texture;
	sfTexture *tile_atlas_texture;

	sfSprite *player_idle;
	sfSprite *battle_button_bg;
	sfSprite *battle_bg;
	sfSprite **monsters_front;
	size_t num_monsters;
	// @TODO: Tiles actually shouldn't be sprites. They should be Vertex
	// Arrays for better performance.
	sfSprite **tiles;
	size_t num_tiles;
} Resources;

// Global variable so each component has access to resources. Importantly, this
// is only mutable at the very start of the game. It should never be modified
// after that.
extern Resources resources;

void resources_init(void);
void resources_destroy(void);

// Throw a texture back at the caller so it can be destroyed after its no longer
// needed.
typedef struct {
	sfTexture *texture;
	sfSprite *sprite;
} DrawPair;

DrawPair draw_text(const char *text, sfColor color, double scale);

#endif
