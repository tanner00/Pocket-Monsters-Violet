#include "include/battle.h"
#include "include/resources.h"
// This is literally just for SCREEN_*
#include "include/tilemap.h"

#define BATTLE_SCALE 4.0
#define BATTLE_VIEW                                                            \
	(sfFloatRect) {                                                        \
		0, 0, SCREEN_WIDTH / BATTLE_SCALE,                             \
			SCREEN_HEIGHT / BATTLE_SCALE                           \
	}

void battle_init(Battle *battle) {
	sfView *const camera = sfView_createFromRect(BATTLE_VIEW);
	battle->camera = camera;
}

void battle_destroy(Battle *battle) {}

State battle_update(Battle *battle, double dt) {
	if (sfKeyboard_isKeyPressed(sfKeyE)) {
		return OVERWORLD_STATE;
	}

	return BATTLE_STATE;
}

void battle_draw(Battle *battle, sfRenderWindow *window) {
	sfRenderWindow_setView(window, battle->camera);

	sfSprite_setPosition(resources.battle_bg, (sfVector2f){0, 0});
	sfRenderWindow_drawSprite(window, resources.battle_bg, NULL);

	const int button_bg_height =
		sfSprite_getLocalBounds(resources.battle_button_bg).height;
	sfSprite_setPosition(
		resources.battle_button_bg,
		(sfVector2f){0, BATTLE_VIEW.height - button_bg_height});

	sfRenderWindow_drawSprite(window, resources.battle_button_bg, NULL);

	sfSprite_setPosition(resources.monsters_front[1],
			     (sfVector2f){155, 12});
	sfRenderWindow_drawSprite(window, resources.monsters_front[1], NULL);

	/* unsigned int size = 20; */
	/* sfGlyph glyph = sfFont_getGlyph(resources.font, 'A', size, false); */
	/* sfTexture *bitmap = sfFont_getTexture(resources.font, size); */

	/* sfImage *image = */
	/* 	sfImage_create(glyph.bounds.width, glyph.bounds.height); */
	/* sfImage_copy(sfTexture_copyToImage(bitmap), 0, 0, glyph.textureRect);
	 */
	/* sfText *draw = sfText_create(); */
	/* sfText_setFont(draw, resources.font); */
	/* sfText_setCharacterSize(draw, 24); */
	/* sfText_setString(draw, "Run"); */
	/* sfText_setPosition(draw, (sfVector2f){0, 0}); */
	/* sfRenderWindow_drawText(window, draw, NULL); */
	/* sfText_destroy(draw); */
}
