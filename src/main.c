#include "include/overworld.h"
#include "include/resources.h"

#include <SFML/Graphics.h>
#include <SFML/Window.h>

#include <stdbool.h>
#include <stdio.h>

int main(void) {

	const sfVideoMode mode = {SCREEN_WIDTH, SCREEN_HEIGHT, 32};
	sfRenderWindow *window = sfRenderWindow_create(
		mode, "Pocket Monsters: Violet", sfClose, NULL);
	sfRenderWindow_setVerticalSyncEnabled(window, true);

	resources_init();

	Overworld overworld;
	overworld_init(&overworld);

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
			overworld_update(&overworld, dt);
			accumulator -= dt;
		}

		sfRenderWindow_clear(window, sfWhite);

		overworld_draw(&overworld, window);

		sfRenderWindow_display(window);
	}

	overworld_destroy(&overworld);
	sfClock_destroy(dt_clock);
	sfRenderWindow_destroy(window);

	return 0;
}
