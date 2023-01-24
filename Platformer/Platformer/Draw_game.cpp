#ifndef DRAW_GAME_H_
#define DRAW_GAME_H_

#include "raylib.h"
#include "player.h"
#include "platforms.h"

void drawBehind(Surfaces surfaces) {
	for (int i = 0; i < surfaces.count; i++) {
		Color color = LIGHTGRAY;
		if (surfaces.surfaces[i].flag == TOP_ONLY) {
			DrawRectangle((int)surfaces.surfaces[i].platform.x, (int)surfaces.surfaces[i].platform.y, (int)surfaces.surfaces[i].platform.width, (int)surfaces.surfaces[i].platform.height, color);
		}
	}
	return;
}

void drawInFront(Surfaces surfaces) {
	for (int i = 0; i < surfaces.count; i++) {
		Color color;
		if (surfaces.surfaces[i].flag == ICE) {
			color = { 0, 196, 255, 255 };
		}
		else if (surfaces.surfaces[i].flag == SAND) {
			color = {255, 255, 196, 255};
		}
		else if (surfaces.surfaces[i].flag == WATER) {
			color = { 0, 128, 255, 128 };
		}
		else {
			color = WHITE;
		}
		if (surfaces.surfaces[i].flag == TOP_ONLY || surfaces.surfaces[i].flag == DEATH_ZONE) {

		}
		else {
			DrawRectangle((int) surfaces.surfaces[i].platform.x, (int) surfaces.surfaces[i].platform.y, (int) surfaces.surfaces[i].platform.width, (int) surfaces.surfaces[i].platform.height, color);
		}
	}
	 
	return;
}

void drawPlayer(Player player) {
	DrawRectangleLines((int)player.position.x, (int)player.position.y, (int)player.size.x, (int)player.size.y, RED);
	return;
}

void drawAI() {

	return;
}

void drawGame(Surfaces surfaces, Player player) {
	BeginDrawing();
		ClearBackground(BLACK);
		BeginMode2D(player.camera);
			drawBehind(surfaces);
			drawPlayer(player);
			drawInFront(surfaces);
			drawAI();
		EndMode2D();
		DrawFPS(10, 10);
	EndDrawing();
	return;
}

#endif