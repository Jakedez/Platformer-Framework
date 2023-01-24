#ifndef PLAYER_H_
#define PLAYER_H_
#include "raylib.h"
#include "platforms.h"

//Player Collision Declarations


typedef struct Bindings {
	short lookUp = KEY_W;
	short moveLeft = KEY_A;
	short lookDown = KEY_S;
	short moveRight = KEY_D;
	short jump = KEY_SPACE;
}Bindings;


typedef struct Player {
	Vector2 position = {400, 950};
	Vector2 lastPos{};
	float jumpPos = position.y;
	Vector2 size {50, 100};
	Vector2 velocity = {0, 0};
	Bindings bindings;
	Camera2D camera;
	float jumpMultiplier = 1.1f;
	float terminalVelocity = 600;
	bool hasTerminalGravity = false;
	float terminalGravity = 500;
	float acceleration = .3f;
	float deceleration = .3f;
	bool isGrounded = false;
	bool hasJumped = false;
	bool queueDownJump = false;
	bool hasDownJumped = false;
	bool isDead = false;
	float jumpHeight = 800;
	float gravity = 3000;
	short surfaceFlag = NONE;
	short spatialFlag = NONE;
	short wallFlag = NONE;
	short prevSpatialFlag = NONE;
	short prevSurfaceFlag = NONE;
	float waterbuffer = 0;
	float deltaTime = 0;

	void setPlayer() {
		jumpPos = position.y;
		velocity = { 0, 0 };
		jumpMultiplier = 1.1f;
		terminalVelocity = 600;
		hasTerminalGravity = false;
		terminalGravity = 500;
		acceleration = .3f;
		deceleration = .3f;
		isGrounded = false;
		hasJumped = false;
		queueDownJump = false;
		hasDownJumped = false;
		isDead = false;
		jumpHeight = 800;
		gravity = 3000;
		surfaceFlag = NONE;
		spatialFlag = NONE;
		wallFlag = NONE;
		prevSpatialFlag = NONE;
		prevSurfaceFlag = NONE;
		waterbuffer = 0;
		deltaTime = 0;
	}
	void getDelta() {
		deltaTime = GetFrameTime();
		return;
	}
	void move(void) {
		lastPos = position;
		position.x += velocity.x * deltaTime;
		position.y += velocity.y * deltaTime;
		if (hasJumped) {
      			if (jumpPos - position.y > (size.y * jumpMultiplier)) {
				position.y = jumpPos - (size.y * jumpMultiplier);
				velocity.y = 0;
			}
		}
		return;
	}
	void initCamera() {
		camera.offset.x = ((float)GetScreenWidth() / 2.f) + ((float)size.x / 2.f);
		camera.offset.y = ((float)GetScreenHeight() / 2.f) + ((float)size.y / 2.f);
		camera.rotation = 0.0f;
		camera.target.x = position.x;
		camera.target.y = position.y;
		camera.zoom = 2.f;
	}
	void activateFlags() {
		if (!isGrounded) {
			surfaceFlag = NONE;
		}
		if (surfaceFlag == GROUND || surfaceFlag == TOP_ONLY) {
			acceleration = .3f;
			deceleration = .3f;
			jumpHeight = 800.f;
			terminalVelocity = 600;
			jumpMultiplier = 1.1f;
		}
		else if (surfaceFlag == ICE) {
			acceleration = 2.5f;
			deceleration = 2.5f;
			jumpHeight = 800.f;
			terminalVelocity = 800;
			jumpMultiplier = 1.1f;
		}
		else if (surfaceFlag == NONE) {
			acceleration = 3.f;
			//deceleration = 1.f;
		}
		else if (surfaceFlag == SAND) {
			acceleration = .3f;
			deceleration = .1f;
			jumpHeight = 600.f;
			terminalVelocity = 300;
			jumpMultiplier = 0.6f;
		}
		else {
			acceleration = .3f;
			jumpHeight = 800.f;
			terminalVelocity = 600;
		}
		if (spatialFlag == WATER) {
			if (prevSpatialFlag != WATER) {
				velocity.y /= 2.5;
				velocity.x /= 2.5;
				jumpHeight = 0;
				waterbuffer = 0;
			}
			if (waterbuffer < 1.f) {
				waterbuffer += deltaTime * 6;
				if (waterbuffer > 1.f) {
					waterbuffer = 1.f;
				}
			}
			if (waterbuffer == 1.f) {
				jumpHeight = 1200.f;
			}
			hasTerminalGravity = true;
			gravity = 400;
			acceleration = .5f;
			deceleration = .5f;
			terminalVelocity = 400;
			isGrounded = true;
			if (velocity.y < -800) {
				velocity.y = -800;
			}
		}
		else {
			hasTerminalGravity = false;
			gravity = 3000;
		}
		prevSpatialFlag = spatialFlag;
		prevSurfaceFlag = surfaceFlag;
		return;
	}
	void controlX() {
		if (IsKeyDown(bindings.moveLeft)) {
			velocity.x -= (terminalVelocity/acceleration) * deltaTime;
		}
		if (IsKeyDown(bindings.moveRight)) {
			velocity.x += (terminalVelocity/acceleration) * deltaTime;
		}
		return;
	}

	void checkDownJump() {
		if (queueDownJump) {
			if ((surfaceFlag == TOP_ONLY || prevSurfaceFlag == TOP_ONLY) && ((surfaceFlag == TOP_ONLY || surfaceFlag == NONE) && (prevSurfaceFlag == TOP_ONLY || prevSurfaceFlag == NONE))) {
				position.y += 1;
				hasDownJumped = true;
				//checkplayer
			}
			else {
				velocity.y = -jumpHeight;
				jumpPos = position.y;
				hasJumped = true;
			}
			queueDownJump = false;
		}
		else {
		}
	}

	void controlJump() {
		if (isGrounded == true) {
			hasJumped = false;
		}
		if (spatialFlag != WATER) {
			if (IsKeyPressed(bindings.jump) && isGrounded) {
				if (!IsKeyDown(bindings.lookDown)) {
					velocity.y = -jumpHeight;
					jumpPos = position.y;
					hasJumped = true;
				}
				else {
					queueDownJump = true;
				}
			}
		}
		else {
			if (IsKeyDown(bindings.jump) && isGrounded) {
				
				velocity.y -= jumpHeight * deltaTime;
				jumpPos = position.y;
				hasJumped = true;
	
				
			}
		}
		
		return;
	}
	void doGravity() {
		velocity.y += gravity * deltaTime;
		if (hasTerminalGravity && velocity.y > terminalGravity){
			velocity.y = terminalGravity;
		}
		else if (hasTerminalGravity && velocity.y < -terminalGravity) {
			velocity.y = -terminalGravity;
		}
		return;
	}
	void lowerVelocity() {
		if (velocity.x > 0 && (!IsKeyDown(bindings.moveRight) || IsKeyDown(bindings.moveLeft))) {
			velocity.x -= (terminalVelocity/deceleration) * deltaTime;
			if (velocity.x < 0) {
				velocity.x = 0;
			}
		}
		if (velocity.x < 0 && (!IsKeyDown(bindings.moveLeft) || IsKeyDown(bindings.moveRight))) {
			velocity.x += (terminalVelocity/deceleration) * deltaTime;
			if (velocity.x > 0) {
				velocity.x = 0;
			}
		}
		if (velocity.x > terminalVelocity) {
			velocity.x = terminalVelocity;
		}
		else if (velocity.x < -terminalVelocity) {
			velocity.x = -terminalVelocity;
		}
		return;
	}
	void setCameraPosition(float speed) {
		if (camera.target.x > position.x) {
			camera.target.x -= speed * (camera.target.x - position.x) * deltaTime;
			if (camera.target.x < position.x) {
				camera.target.x = position.x;
			}
		}
		else if (camera.target.x < position.x) {
			camera.target.x -= speed * (camera.target.x - position.x) * deltaTime;
			if (camera.target.x > position.x) {
				camera.target.x = position.x;
			}
		}
		if (camera.target.y > position.y) {
			camera.target.y -= speed * (camera.target.y - position.y) * deltaTime;
			if (camera.target.y < position.y) {
				camera.target.y = position.y;
			}
		}
		else if (camera.target.y < position.y) {
			camera.target.y -= speed * (camera.target.y - position.y) * deltaTime;
			if (camera.target.y > position.y) {
				camera.target.y = position.y;
			}
		}
	}
	void resetPlayer(Vector2 startPoint) {
		if (isDead) {
			setPlayer();
			position = startPoint;
			lastPos = startPoint;
			camera.target = startPoint;
			return;
		}
	}
	void checkDeath(Vector2 startPoint) {
		if (spatialFlag == DEATH_ZONE) {
			isDead = true;
		}
	}
}Player;

#endif