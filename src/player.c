#include "include/player.h"
#include "include/math.h"
#include "include/resources.h"

#include <assert.h>

void player_init(Player *player, sfVector2u start_tile) {
	player->pos.x = start_tile.x * TILE_SIZE;
	player->pos.y = start_tile.y * TILE_SIZE;
	player->dest = player->pos;
	player->speed = WALKING_SPEED;
}

void player_destroy(Player *player) {}

State player_update(Player *player, const Tilemap *tilemap, double dt) {
	// velocity_normal tells which direction the player is going. You could
	// avoid a vector for this but it simplifies if-else logic into vector
	// multiplication.
	static sfVector2i velocity_normal = {0, 0};

	// If the player has not reached the destination, keep moving them in
	// that direction and don't accept input.
	// This implements for tile-by-tile movement like in older Pokémon
	// games.
	const bool moving = player->dest.x != player->pos.x ||
			    player->dest.y != player->pos.y;

	const bool left = sfKeyboard_isKeyPressed(sfKeyLeft);
	const bool right = sfKeyboard_isKeyPressed(sfKeyRight);
	const bool up = sfKeyboard_isKeyPressed(sfKeyUp);
	const bool down = sfKeyboard_isKeyPressed(sfKeyDown);

	if (!moving) {
		velocity_normal = (sfVector2i){0, 0};
		if (left) {
			velocity_normal.x = -1;
			player->dest.x = player->pos.x - TILE_SIZE;
		} else if (right) {
			velocity_normal.x = 1;
			player->dest.x = player->pos.x + TILE_SIZE;
		} else if (up) {
			velocity_normal.y = -1;
			player->dest.y = player->pos.y - TILE_SIZE;
		} else if (down) {
			velocity_normal.y = 1;
			player->dest.y = player->pos.y + TILE_SIZE;
		}

		// Find what tile the player will be moving to.
		// If that tile is solid don't allow the player to move there.
		sfVector2u dest_tile_coords = to_tile_coords(player->dest);
		TileId dest_tile = tilemap_get_tile(tilemap, dest_tile_coords);
		if (is_tile_solid(dest_tile)) {
			player->dest = player->pos;
			velocity_normal = (sfVector2i){0, 0};
		}
	}

	// Move the player if they have a velocity
	player->pos.x += player->speed * velocity_normal.x * dt;
	player->pos.y += player->speed * velocity_normal.y * dt;

	// If the player was considered moving before their location was
	// updated and has now reached the destination tile after their position
	// is updated, the player is considered to have just stopped.
	if (moving && player->pos.x == player->dest.x &&
	    player->pos.y == player->dest.y) {
		TileId tile_on =
			tilemap_get_tile(tilemap, to_tile_coords(player->pos));

		// @TODO: Figure out what I want the encounter rate to be
		if (tile_on == TALL_GRASS && random_range(1, 10) == 10) {
			return BATTLE_STATE;
		}
	}

	return OVERWORLD_STATE;
}

void player_draw(Player *player, sfRenderWindow *window) {
	// The sprite position is derived from player->pos (a sfVector2u)
	// because floats are too imprecise for tile movement and collision
	const sfVector2f new_position =
		(sfVector2f){(float)player->pos.x, (float)player->pos.y};
	sfSprite_setPosition(resources.player_idle, new_position);

	sfRenderWindow_drawSprite(window, resources.player_idle, NULL);
}
