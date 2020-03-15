#ifndef PLAYER_H
#define PLAYER_H

#include "tilemap.h"

#include <SFML/Graphics.h>

// These numbers are chosen very specifically
// Any multiple of 120 will do because we use a fixed delta time of 1.0 / 60.0
// to update the game and you have to evenly divide 64 to end on a tile
// boundary. A speed of 60 would be moving 1 pixel per update.
#define WALKING_SPEED 240
#define RUNNING_SPEED 480

typedef struct {
	sfVector2u pos;
	sfVector2u dest;
	// @NOTE: This does not indicate if the player is moving. It indicates
	// how fast the player would be going if they WERE moving.
	// @TODO: Running shoes are not added yet but they will change this
	// value to RUNNING_SPEED
	int speed;
} Player;

void player_init(Player *player, sfVector2u start_tile);
void player_destroy(Player *player);

void player_update(Player *player, Tilemap *tilemap, double dt);
void player_draw(Player *player, sfRenderWindow *window);

#endif
