#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include "battle.h"
#include "overworld.h"
#include "states.h"

#include <SFML/Graphics.h>

typedef struct {
	State current_state;

	Overworld overworld;
	Battle battle;
} StateManager;

extern StateManager state_manager;

void state_manager_init(void);
void state_manager_destroy(void);

void state_manager_update(double dt);
void state_manager_draw(sfRenderWindow *window);

void state_manager_transition(State old_state, State new_state);

#endif
