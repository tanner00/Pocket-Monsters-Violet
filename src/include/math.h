#ifndef MATH_H
#define MATH_H

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

#endif
