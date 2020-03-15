#ifndef RESOURCES_H
#define RESOURCES_H

#include <SFML/Graphics.h>

// All resources live for the entirety of the game which isn't a big deal
// because the game is so small.

// sfTextures need to be kept alive as long as sfSprites are in use, so keep a
// reference to them here.
typedef struct {
	sfTexture *player_atlas;
	sfTexture *tile_atlas;

	sfSprite *player_sprite;
	// @TODO: Tiles actually shouldn't be sprites. They should be Vertex
	// Arrays for better performance.
	sfSprite **tile_sprites;

	size_t num_tile_sprites;
} Resources;

// Global variable so each component has access to resources. Importantly, this
// is only mutable at the very start of the game. It should never be modified
// after that.
extern Resources resources;

void resources_init(void);
void resources_destroy(void);

#endif
