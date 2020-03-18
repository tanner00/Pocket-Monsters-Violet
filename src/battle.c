#include "include/battle.h"
// This is literally just for SCREEN_*
#include "include/math.h"
#include "include/tilemap.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Battles are scaled by 4x
const sfFloatRect BATTLE_VIEW = {0, 0, SCREEN_WIDTH / 4.0, SCREEN_HEIGHT / 4.0};

// These positions are sfVector2fs because its what *_setPosition uses

// These positions on the screen are determined by
// res/battle_ui.png. They have no reason outside of
// that.
const sfVector2f move_button_bg_pos[] = {
	{3, 118},
	{95, 118},
	{3, 139},
	{95, 139},
};

// The button positions for the strings: "Items", "Switch", and "Run",
// respectively. I picked a position I wanted them to right-align to (235) and
// subtracted the pixel width of their rendered strings (the formula for the
// width of text is length = 8 * num_chars because every character at scale=1.0
// is 8 pixels wide)
// The y value for the postions is pretty arbitrary other than the fact that it
// looks nice and evenly spaced (118 is the top of the gray bottom right bit of
// res/button_ui.png)
const sfVector2f action_button_pos[] = {
	{235 - 8 * 5, 118 + 4},
	{235 - 8 * 6, 118 + 4 + 12},
	{235 - 8 * 3, 118 + 4 + 24},
};

// @TODO: when monsters are switched these buffers need to be updated.
void battle_init(Battle *battle) {
	sfView *const camera = sfView_createFromRect(BATTLE_VIEW);
	assert(camera);
	battle->camera = camera;

	// Each monster has at most 4 moves
	DrawableText *moves_text = malloc(4 * sizeof(*moves_text));
	assert(moves_text);
	battle->moves_text = moves_text;
	for (size_t i = 0; i < 4; ++i) {
		// get current pokemon's moves
		const char *move_text = "Fire Blast";
		const sfVector2u move_text_bounds =
			(sfVector2u){strlen(move_text) * 8, 8};

		const sfVector2f button_bg_pos = move_button_bg_pos[i];
		const sfIntRect button_bg_bounds = {button_bg_pos.x,
						    button_bg_pos.y, 89, 18};

		moves_text[i] = draw_text(move_text, sfWhite, 1.0);
		battle->moves_text_pos[i] =
			center_within_rect(button_bg_bounds, move_text_bounds);
	}

	battle->selected_button = 0;
	battle->button_cooldown = 0;
}

void battle_destroy(Battle *battle) {
	for (size_t i = 0; i < 4; ++i) {
		sfTexture_destroy(battle->moves_text[i].texture);
		sfSprite_destroy(battle->moves_text[i].sprite);
	}
	free(battle->moves_text);
	sfView_destroy(battle->camera);
}

// I know this might look weird at first, don't fret. I will comment it for you.
// If you don't like this approach, imagine the magnitude of if-statements
// that would implement this same logic. Maybe that will make you feel better.

// The battle interface looks roughly like this:
// m1 m2  o1
//        o2
// m3 m4  o3
// where the m prefix means move and the o prefix means option

// m1: 0th index
// m2: 1st index
// m3: 2nd index
// m4: 3rd index
// o1: 4th index
// o2: 5th index
// o3: 6th index

// left: 0th index of the inner arrays
// right: 1st index of the inner arrays
// up: 2nd index of the inner arrays
// down: 3rd index of the inner arrays

// I want to be able to move around these keys and select them. This array tells
// me for what key I press and for what location I'm currently at, where to go
// next.
// For example: If I'm at o2, and I press down (this is index [5][3] in the
// array), I should go to the value 6... which is o3. Correct!

// To implement the "don't go anywhere" logic (i.e.: you shouldn't go anywhere
// if you press right at o1) just map back to your starting index.

// This array is [button I'm currently on (0...6)][key I pressed (0...3)]
const int key_to_dest[][4] = {{0, 1, 0, 2}, {0, 4, 1, 3}, {2, 3, 0, 2},
			      {2, 6, 1, 3}, {1, 4, 4, 5}, {1, 5, 4, 6},
			      {3, 6, 5, 6}};
// Footnote: I'll prove that this was the way to go. For every number in this
// array, if it's number doesn't match up with its outter index, that would be
// another if statement. That would add 17 if statements to this code. The table
// driven approach is easier to read AND more efficient.

State battle_update(Battle *battle, double dt) {
	if (!battle->button_cooldown) {
		const int left = sfKeyboard_isKeyPressed(sfKeyLeft);
		const int right = sfKeyboard_isKeyPressed(sfKeyRight);
		const int up = sfKeyboard_isKeyPressed(sfKeyUp);
		const int down = sfKeyboard_isKeyPressed(sfKeyDown);
		const bool any = left || right || up || down;

		if (left) {
			battle->selected_button =
				key_to_dest[battle->selected_button][0];
		} else if (right) {
			battle->selected_button =
				key_to_dest[battle->selected_button][1];
		} else if (up) {
			battle->selected_button =
				key_to_dest[battle->selected_button][2];
		} else if (down) {
			battle->selected_button =
				key_to_dest[battle->selected_button][3];
		}

		if (any) {
			battle->button_cooldown = 10;
		}
	} else {
		--battle->button_cooldown;
	}

	return BATTLE_STATE;
}

void battle_draw(Battle *battle, sfRenderWindow *window) {
	sfRenderWindow_setView(window, battle->camera);

	// Draw the background of the battle. Right now this is always a
	// grassy looking area. Eventually this will be dependent on
	// where in the world the player is.
	sfSprite_setPosition(resources.battle_bg, (sfVector2f){0, 0});
	sfRenderWindow_drawSprite(window, resources.battle_bg, NULL);

	sfSprite_setPosition(resources.battle_button_bg, (sfVector2f){0, 0});
	sfRenderWindow_drawSprite(window, resources.battle_button_bg, NULL);

	// Moves have a background color depending on their type.
	for (size_t i = 0; i < 4; ++i) {
		sfRectangleShape_setFillColor(resources.move_button, sfRed);

		sfRectangleShape_setPosition(resources.move_button,
					     move_button_bg_pos[i]);

		sfRenderWindow_drawRectangleShape(window, resources.move_button,
						  NULL);
	}

	for (size_t i = 0; i < 4; ++i) {
		const DrawableText move_text = battle->moves_text[i];

		sfSprite_setColor(move_text.sprite, sfWhite);
		if (battle->selected_button == i) {
			sfSprite_setColor(move_text.sprite, sfYellow);
		}

		sfSprite_setPosition(move_text.sprite,
				     battle->moves_text_pos[i]);
		sfRenderWindow_drawSprite(window, move_text.sprite, NULL);
	}

	for (size_t i = 0; i < 3; ++i) {
		const DrawableText action_text =
			resources.battle_action_text[i];

		sfSprite_setColor(action_text.sprite, sfWhite);
		if (battle->selected_button == i + 4) {
			sfSprite_setColor(action_text.sprite, sfYellow);
		}

		sfSprite_setPosition(action_text.sprite, action_button_pos[i]);
		sfRenderWindow_drawSprite(window, action_text.sprite, NULL);
	}

	// @NOTE: Just a test monster sprite
	sfSprite_setPosition(resources.monsters_front[1],
			     (sfVector2f){155, 12});
	sfRenderWindow_drawSprite(window, resources.monsters_front[1], NULL);
}
