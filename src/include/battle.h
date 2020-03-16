#ifndef BATTLE_H
#define BATTLE_H

#include "states.h"

#include <SFML/Graphics.h>

typedef struct {
	sfView *camera;
} Battle;

void battle_init(Battle *battle);
void battle_destroy(Battle *battle);

State battle_update(Battle *battle, double dt);
void battle_draw(Battle *battle, sfRenderWindow *window);

#endif
