#ifndef PLATFORMS_H_
#define PLATFORMS_H_
#include "raylib.h"

enum surfaceFlags {
	NONE,
	WATER,
	GROUND,
	ICE,
	SAND,
	FAKE,
	TOP_ONLY,
	DEATH_ZONE
};



typedef struct Surface{
	short flag;
	Rectangle platform;
}Surface;

typedef struct Surfaces {
	int count = 0;
	Surface* surfaces = NULL;
}Surfaces;

#endif