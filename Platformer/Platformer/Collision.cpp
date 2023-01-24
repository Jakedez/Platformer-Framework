#ifndef COLLISION_CPP_
#define COLLISION_CPP_
#include "raylib.h"
#include "player.h"
#include "platforms.h"

enum Collision {
	NO_COLLISION,
	LEFT_COLLISION,
	RIGHT_COLLISION,
	TOP_COLLISION,
	BOTTOM_COLLISION
};

short checkCollisionSide(Player * player, Rectangle platform) {
	short collision = NO_COLLISION;
	if (!CheckCollisionRecs({player->lastPos.x, player->position.y, player->size.x, player->size.y}, platform)) {
		if (player->position.x > player->lastPos.x) {
			collision = RIGHT_COLLISION;
		}
		else if (player->position.x < player->lastPos.x) {
			collision = LEFT_COLLISION;
		}
	}
	return collision;
}

short checkCollisionTop(Player* player, Rectangle platform) {
	short collision = NO_COLLISION;
	if (!CheckCollisionRecs({ player->position.x, player->lastPos.y, player->size.x, player->size.y }, platform)) {
		if (player->position.y > player->lastPos.y) {
			collision = BOTTOM_COLLISION;
		}
		else if (player->position.y < player->lastPos.y) {
			collision = TOP_COLLISION;
		}
	}
	return collision;
}



void resetPositionVertical(Player* player, Surface platform, short verticalResult) {
	if ((platform.flag != WATER && platform.flag != DEATH_ZONE) && platform.flag != FAKE) {
		if (verticalResult == BOTTOM_COLLISION) {
			player->isGrounded = true;
			player->position.y = platform.platform.y - player->size.y;
			player->velocity.y = 0;
			player->surfaceFlag = platform.flag;
		}
		else if (verticalResult == TOP_COLLISION && platform.flag != TOP_ONLY) {
			player->surfaceFlag = platform.flag;
			player->position.y = platform.platform.y + platform.platform.height;
			player->velocity.y = 0;
		}
	}
	return;
}

void resetPositionHorizontal(Player* player, Surface platform, short horizontalResult) {
	if ((platform.flag != WATER && platform.flag != DEATH_ZONE) && platform.flag != FAKE && platform.flag != TOP_ONLY) {
		if (horizontalResult == LEFT_COLLISION) {
			player->position.x = platform.platform.x + platform.platform.width;
			player->velocity.x = 0;
		}
		else if (horizontalResult == RIGHT_COLLISION) {
			player->position.x = platform.platform.x - player->size.x;
			player->velocity.x = 0;
		}
	}
}



void checkPlayerCollision(Player * player, Surfaces surfaces) {
	short verticalCollision = NO_COLLISION;
	short horizontalCollision = NO_COLLISION;
	player->spatialFlag = NONE;
	player->surfaceFlag = NONE;
	for (int i = 0; i < surfaces.count; i++) {
		if (player->hasDownJumped && surfaces.surfaces[i].flag == TOP_ONLY) {

		}
		else{
			verticalCollision = NO_COLLISION;
			horizontalCollision = NO_COLLISION;
			if (CheckCollisionRecs({ player->position.x, player->position.y, player->size.x, player->size.y }, surfaces.surfaces[i].platform)) {
				verticalCollision = checkCollisionTop(player, surfaces.surfaces[i].platform);
				if (verticalCollision) {
					resetPositionVertical(player, surfaces.surfaces[i], verticalCollision);
				}
				if (surfaces.surfaces[i].flag == WATER || surfaces.surfaces[i].flag == DEATH_ZONE) {
					player->spatialFlag = surfaces.surfaces[i].flag;
				}
			}
			else {
			
			}
			if (CheckCollisionRecs({ player->position.x, player->position.y, player->size.x, player->size.y }, surfaces.surfaces[i].platform) && !player->hasDownJumped) {
				horizontalCollision = checkCollisionSide(player, surfaces.surfaces[i].platform);
				if (horizontalCollision) {
					resetPositionHorizontal(player, surfaces.surfaces[i], horizontalCollision);
				}
				if (surfaces.surfaces[i].flag == WATER || surfaces.surfaces[i].flag == DEATH_ZONE) {
					if (player->spatialFlag != DEATH_ZONE) {
						player->spatialFlag = surfaces.surfaces[i].flag;
					}
				}
			}
		}
		
	}
	if (player->lastPos.y != player->position.y) {
		player->isGrounded = false;
	}
	if (player->hasDownJumped) {
		player->hasDownJumped = false;
	}
	return;
}
#endif