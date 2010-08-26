#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#define ARRDEFAULTSIZE 50

#define ARRCHECK(p, type, size, max) do {\
	if (size >= max) { \
		while (size >= max) max += ARRDEFAULTSIZE; \
		p = realloc(p, max * sizeof(type)); \
	} \
} while (0)

#endif
