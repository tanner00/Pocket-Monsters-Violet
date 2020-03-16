#include "include/overworld.h"
#include "include/resources.h"
#include "include/state_manager.h"

#include <SFML/Graphics.h>
#include <SFML/Window.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {

	// @TODO: replace this with better RNG
	srand(time(NULL));

	const sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT, 32};
	sfRenderWindow *window = sfRenderWindow_create(
		mode, "Pocket Monsters: Violet", sfResize | sfClose, NULL);
	// @TODO: add option for fullscreen
	sfRenderWindow_setVerticalSyncEnabled(window, true);

	resources_init();
	state_manager_init();

	// Each update moves the game a deterministic 1.0 / 60.0 seconds into
	// the future
	// This prevents things like a slow computer from completely ruining the
	// logic of the game like tile movement
	// https://gafferongames.com/post/fix_your_timestep/
	const double dt = 1.0 / 60.0;
	double accumulator = 0.0;
	sfClock *const dt_clock = sfClock_create();
	while (sfRenderWindow_isOpen(window)) {
		sfEvent event;
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(window);
			}
		}

		// Adds the amount of time since the last frame
		accumulator +=
			sfTime_asSeconds(sfClock_getElapsedTime(dt_clock));
		sfClock_restart(dt_clock);

		// Simulate the world until it's caught up
		while (accumulator >= dt) {
			state_manager_update(dt);
			accumulator -= dt;
		}

		sfRenderWindow_clear(window, sfBlack);

		state_manager_draw(window);

		sfRenderWindow_display(window);
	}

	state_manager_destroy();
	sfClock_destroy(dt_clock);
	sfRenderWindow_destroy(window);

	return 0;
}
