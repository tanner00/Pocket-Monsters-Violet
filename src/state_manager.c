#include "include/state_manager.h"

StateManager state_manager;

void state_manager_init(void) {
	state_manager.current_state = OVERWORLD_STATE;
	overworld_init(&state_manager.overworld);
}

void state_manager_destroy(void) {
	overworld_destroy(&state_manager.overworld);
}

void state_manager_update(double dt) {
	State old_state = state_manager.current_state;

	switch (state_manager.current_state) {
	case OVERWORLD_STATE:
		state_manager.current_state =
			overworld_update(&state_manager.overworld, dt);
		break;
	case BATTLE_STATE:
		state_manager.current_state =
			battle_update(&state_manager.battle, dt);
		break;
	}

	if (state_manager.current_state != old_state) {
		state_manager_transition(old_state,
					 state_manager.current_state);
	}
}

void state_manager_draw(sfRenderWindow *window) {
	switch (state_manager.current_state) {
	case OVERWORLD_STATE:
		overworld_draw(&state_manager.overworld, window);
		break;
	case BATTLE_STATE:
		battle_draw(&state_manager.battle, window);
		break;
	}
}

void state_manager_transition(State old_state, State new_state) {
	// Battle states are not persistent
	if (old_state == BATTLE_STATE) {
		battle_destroy(&state_manager.battle);
	}

	if (new_state == BATTLE_STATE) {
		// Generate a new battle state
		// This will be dependent on where the player is, so some clean
		// way to extract that needs to happen
		battle_init(&state_manager.battle);
	}
}
