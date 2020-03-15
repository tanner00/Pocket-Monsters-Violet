#include "include/overworld.h"

#include <assert.h>
#include <stdlib.h>

void overworld_init(Overworld *overworld) {
	Player *const player = malloc(sizeof(*player));
	assert(player);
	overworld->player = player;
	player_init(player, (sfVector2u){30, 20});

	Tilemap *const map = malloc(sizeof(*map));
	assert(map);
	overworld->map = map;
	tilemap_init(map, "res/overworld.png");

	sfView *const camera = sfView_createFromRect(
		(sfFloatRect){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});
	overworld->camera = camera;
}

void overworld_destroy(Overworld *overworld) {
	player_destroy(overworld->player);
	tilemap_destroy(overworld->map);
	sfView_destroy(overworld->camera);
}

void overworld_update(Overworld *overworld, double dt) {
	player_update(overworld->player, overworld->map, dt);
}

void overworld_draw(Overworld *overworld, sfRenderWindow *window) {
	Player *const player = overworld->player;
	Tilemap *const map = overworld->map;
	sfView *const camera = overworld->camera;

	sfView_setCenter(camera, (sfVector2f){player->pos.x, player->pos.y});
	sfRenderWindow_setView(window, camera);

	tilemap_draw(map, window);
	player_draw(player, window);
}
