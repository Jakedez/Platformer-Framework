#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "include.h"

class Director {
private:
	Level level{};
	Player player{};
	void getInput() {
		player.controlX();
		player.controlJump();
		return;
	}
	void updateGame() {
		player.lowerVelocity();
		player.move();
		player.doGravity();
		player.checkDeath(level.playerStart);
		if (player.isDead) {
			player.resetPlayer(level.playerStart);
		}
		checkPlayerCollision(&player, level.surfaces);
		player.checkDownJump();
		if (player.hasDownJumped) {
			checkPlayerCollision(&player, level.surfaces);
		}
		player.activateFlags();
		player.setCameraPosition(10);
		return;
	}
public:
	void directGame(const char * filename) {
		level.surfaces.count = 0;
		level.surfaces.surfaces = (Surface *) malloc(0);
		level.loadLevel(filename, &player.position);
		player.initCamera();
		while (!WindowShouldClose()) {
			getInput();
			updateGame();
			drawGame(level.surfaces, player);		
			
			player.getDelta();
		}
		return;
	}
};


int main(int argc, char** argv) {
	InitWindow(1920, 1080, "Test");
	//ToggleFullscreen();
	SetTargetFPS(0);
	SetWindowState(FLAG_VSYNC_HINT);
	const char* filename;
	if (argc < 2) {
		filename = "Rectangles.level";
	}
	else {
		filename = argv[1];
	}
	Director director;
	director.directGame(filename);
	return EXIT_SUCCESS;
}