#ifndef OVERWORLD_H
#define OVERWORLD_H

#include "player.h"
#include "tilemap.h"

#include <SFML/Graphics.h>

typedef struct {
	Player *player;
	Tilemap *map;
	sfView *camera;
} Overworld;

void overworld_init(Overworld *overworld);
void overworld_destroy(Overworld *overworld);

void overworld_update(Overworld *overworld, double dt);
void overworld_draw(Overworld *overworld, sfRenderWindow *window);

#endif
