#ifndef MATH_H
#define MATH_H

#include <SFML/Graphics.h>

#include <stdlib.h>

// These macros avoid the double evaluation problem that occurs in the naive
// implementations of these functions
#define MAX(a, b)                                                              \
	({                                                                     \
		__typeof__(a) _a = (a);                                        \
		__typeof__(b) _b = (b);                                        \
		_a > _b ? _a : _b;                                             \
	})

#define MIN(a, b)                                                              \
	({                                                                     \
		__typeof__(a) _a = (a);                                        \
		__typeof__(b) _b = (b);                                        \
		_a < _b ? _a : _b;                                             \
	})

// Generates a random number in the range [a, b]
inline int random_range(int a, int b) { return (rand() % (b - a + 1)) + a; }

// This returns an sfVector2f because they are required by sfRenderWindow_draw*
// @TODO: comment this formula
inline sfVector2f center_within_rect(sfIntRect bounds, sfVector2u inner) {
	sfVector2u centered;
	centered.x = bounds.left + bounds.width / 2 - inner.x / 2;
	centered.y = bounds.top + inner.y / 2;
	return (sfVector2f){centered.x, centered.y};
}

#endif
