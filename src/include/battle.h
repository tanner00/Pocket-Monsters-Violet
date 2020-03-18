#ifndef BATTLE_H
#define BATTLE_H

#include "resources.h"
#include "states.h"

#include <SFML/Graphics.h>

typedef struct {
	sfView *camera;

	DrawableText *moves_text;
	sfVector2f moves_text_pos[4];

	size_t selected_button;
	size_t button_cooldown;
} Battle;

void battle_init(Battle *battle);
void battle_destroy(Battle *battle);

State battle_update(Battle *battle, double dt);
void battle_draw(Battle *battle, sfRenderWindow *window);

#endif
