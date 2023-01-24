#ifndef INCLUDE_H_
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "platforms.h"
#include "player.h"


//Draw Game Declarations
void drawInFront(Surfaces surfaces);
void drawBehind(Surfaces surfaces);
void drawPlayer(Player player);
void drawAI(void);
void drawGame(Surfaces surfaces, Player player);

//Player Collision Declarations
enum Collision {
	NO_COLLISION,
	LEFT_COLLISION,
	RIGHT_COLLISION,
	TOP_COLLISION,
	BOTTOM_COLLISION
};
short checkCollisionSide(Player* player, Rectangle platform);
short checkCollisionTop(Player* player, Rectangle platform);
void resetPositionVertical(Player* player, Surface platform, short verticalResult);
void resetPositionHorizontal(Player* player, Surface platform, short horizontalResult);
void checkPlayerCollision(Player* player, Surfaces surfaces);

//Level Declarations
typedef struct Spawns {
	short spawnNumbers;
	Vector2* spawns = NULL;
}Spawns;

void* xrealloc(void * block, size_t size) {
	void* pointer = realloc(block, size);
	if (pointer == NULL) {
		exit(EXIT_FAILURE);
	}
	else {
		return pointer;
	}
}

void* xmalloc(size_t size) {
	void* pointer = malloc(size);
	if (pointer == NULL) {
		exit(EXIT_FAILURE);
	}
	else {
		return pointer;
	}
}

class Level {
public:
	Surfaces surfaces;
	Vector2 playerStart;
	Spawns spawns;
	void loadLevel(const char* filename, Vector2 * position) {
		errno_t result;
		FILE* file;
		result = fopen_s(&file, filename, "rb");
		if (result == 0) {
			fread(&playerStart, sizeof(Vector2), 1, file);
			*position = playerStart;
			fread(&surfaces.count, sizeof(int), 1, file);
			surfaces.surfaces = (Surface*) xrealloc(surfaces.surfaces, sizeof(Surface) * surfaces.count);
			for (int i = 0; i < surfaces.count; i++) {
				fread(&surfaces.surfaces[i], sizeof(Surface), 1, file);
			}
			fclose(file);
		}
		return;
	}
};


#endif