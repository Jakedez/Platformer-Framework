#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "easydialog.h"
#include "splashscreen.h"
#include <libloaderapi.h>   

#define FONT_SIZE 20

#define MODE_COUNT 2

#define VERSION_NUMBER "1.0.1.5"

// List of player Modes
enum PlayerMode{
    TERRAIN_MODE,
    PLAYER_MODE
};

// List of Surface Flags
enum surfaceFlags {
	NONE,
	WATER,
	GROUND,
	ICE,
	SAND,
    FAKE,
    TOP_ONLY,
    DEATH
};

//Number of total surface flags
#define FLAG_COUNT 8


// Struct with spawn information set
typedef struct Spawns {
	short spawnNumbers;
	Vector2* spawns;
}Spawns;

// Struct with surface information set
typedef struct Surface{
	short flag;
	Rectangle platform;
}Surface;

//struct with Surface Linked-List information set
typedef struct Rectangles{
    Surface rectangle;
    bool isSelected;
    struct Rectangles * next;
    struct Rectangles * prev;
}Rectangles;

// Struct with basic function info
typedef struct Player{
    bool isActive;
    bool hasSaved;
    short mode;
}Player;

// Draws a 2D grid with the origin at (0, 0)
void drawGrid2D(int slices, int spacing, Color color){
    for (int i = 0; i < slices; i++){
        DrawLine(i * spacing, -slices * spacing, i * spacing, slices * spacing, color);
        DrawLine(-i * spacing, -slices * spacing, -i * spacing, slices * spacing, color);
        DrawLine(slices * spacing, i * spacing, -slices * spacing, i * spacing, color);
        DrawLine(slices * spacing, -i * spacing, -slices * spacing, -i * spacing, color);
    }
        DrawLine(-slices * spacing, slices * spacing, slices * spacing, slices * spacing, color);
        DrawLine(-slices * spacing, -slices * spacing, slices * spacing, -slices * spacing, color);
        DrawLine(-slices * spacing, -slices * spacing, slices * -spacing, slices * spacing, color);
        DrawLine(slices * spacing, -slices * spacing, slices * spacing, slices * spacing, color);
    return;
}

// Draws screen according to the appropriate parameters
void drawScreen(Rectangles * list, Player player, Camera2D camera, Rectangles * selected, Rectangles * activeRect, Rectangle spawn, bool scaleShown, bool infoShown, bool showLines){
    Rectangles * rectangles = list;
    Color color;
    BeginDrawing();
    ClearBackground(DARKGRAY);
    BeginMode2D(camera);
    if (camera.zoom < .55){
        DrawRectangle(INT_MIN / 2, -15, INT_MAX, 30, GRAY);
        DrawRectangle(-15, INT_MIN / 2, 30, INT_MAX, GRAY);
        drawGrid2D(1000, 500, GRAY);
    }
    else{
        DrawRectangle(INT_MIN / 2, -1, INT_MAX, 3, GRAY);
        DrawRectangle(-1, INT_MIN / 2, 3, INT_MAX, GRAY);
        drawGrid2D(10000, 50, GRAY);
    }
    while (rectangles != NULL){
        if (rectangles->isSelected){
            color = ORANGE;
        }
        else{
            switch (rectangles->rectangle.flag){
                case GROUND:
                color = LIGHTGRAY;
                break;
                case ICE:
                color = (Color) { 0, 196, 255, 255 };
                break;
                case SAND:
                color = (Color) {255, 255, 196, 255};
                break;
                case FAKE:
                color = (Color) {0, 0, 0, 255};
                break;
                case TOP_ONLY:
                color = (Color) {180, 180, 180, 255};
                break;
                case DEATH:
                color = RED;
                break;
                default:
                color = BLUE;
                break;
            }
        }
        if (rectangles->rectangle.flag == WATER || rectangles->rectangle.flag == FAKE){
            color.a = 128;
        }
        if (rectangles->rectangle.platform.width < 0 && rectangles->rectangle.platform.height < 0){
            DrawRectangle(rectangles->rectangle.platform.x + rectangles->rectangle.platform.width, rectangles->rectangle.platform.y + rectangles->rectangle.platform.height, -rectangles->rectangle.platform.width, -rectangles->rectangle.platform.height, color);
        }
        else if (rectangles->rectangle.platform.height < 0){
            DrawRectangle(rectangles->rectangle.platform.x, rectangles->rectangle.platform.y + rectangles->rectangle.platform.height, rectangles->rectangle.platform.width, -rectangles->rectangle.platform.height, color);
        }
        else if (rectangles->rectangle.platform.width < 0){
            DrawRectangle(rectangles->rectangle.platform.x + rectangles->rectangle.platform.width, rectangles->rectangle.platform.y, -rectangles->rectangle.platform.width, rectangles->rectangle.platform.height, color);
        }
        else{
            DrawRectangle(rectangles->rectangle.platform.x, rectangles->rectangle.platform.y, rectangles->rectangle.platform.width, rectangles->rectangle.platform.height, color);
        }
        rectangles = rectangles->next;
    }
    if (showLines){
        rectangles = list;
        while (rectangles != NULL){
            if (rectangles->rectangle.platform.width < 0 && rectangles->rectangle.platform.height < 0){
                DrawRectangleLines(rectangles->rectangle.platform.x + rectangles->rectangle.platform.width, rectangles->rectangle.platform.y + rectangles->rectangle.platform.height, -rectangles->rectangle.platform.width, -rectangles->rectangle.platform.height, BLACK);
            }
            else if (rectangles->rectangle.platform.height < 0){
                DrawRectangleLines(rectangles->rectangle.platform.x, rectangles->rectangle.platform.y + rectangles->rectangle.platform.height, rectangles->rectangle.platform.width, -rectangles->rectangle.platform.height, BLACK);
            }
            else if (rectangles->rectangle.platform.width < 0){
                DrawRectangleLines(rectangles->rectangle.platform.x + rectangles->rectangle.platform.width, rectangles->rectangle.platform.y, -rectangles->rectangle.platform.width, rectangles->rectangle.platform.height, BLACK);
            }
            else{
                DrawRectangleLines(rectangles->rectangle.platform.x, rectangles->rectangle.platform.y, rectangles->rectangle.platform.width, rectangles->rectangle.platform.height, BLACK);
            }
            rectangles = rectangles->next;
        }
    }
    DrawRectangleLines(spawn.x, spawn.y, spawn.width, spawn.height, RED);
    EndMode2D();
    Rectangle playerScale = {GetScreenWidth() - (int) (GetScreenWidth() / 30), GetScreenHeight() - (int) (GetScreenHeight() / 16.875), -50 * camera.zoom, -100 * camera.zoom};
    
    if (scaleShown){
        DrawLine(playerScale.x, playerScale.y, playerScale.x + playerScale.width, playerScale.y, RED);
        DrawLine(playerScale.x, playerScale.y, playerScale.x, playerScale.y + playerScale.height, RED);
        DrawLine(playerScale.x + playerScale.width, playerScale.y, playerScale.x+ playerScale.width, playerScale.y + playerScale.height, RED);
        DrawLine(playerScale.x + playerScale.width, playerScale.y + playerScale.height, playerScale.x, playerScale.y + playerScale.height, RED);
    }
    if (infoShown){
        if (player.hasSaved){
            DrawText("Data Saved!", 10, 10, FONT_SIZE, WHITE);
        }
        else{
            DrawText("Press Ctrl + S to save your set!", 10, 10, FONT_SIZE, WHITE);
        }
        if (player.isActive){
            selected = activeRect;
        }
        char * mode;
        switch (player.mode){
            case TERRAIN_MODE:
            mode = "Mode: Terrain";
            break;
            case PLAYER_MODE:
            mode = "Mode: Player";
            break;
            default:
            mode = "Mode: Undefined";
        }
        DrawText(mode, 10, 10 + FONT_SIZE * 2, FONT_SIZE, WHITE);

        if (selected != NULL){
            char description[80] = "";
            char xPos[8];
            gcvt((double) selected->rectangle.platform.x, 4, xPos);
            char yPos[8];
            gcvt((double) selected->rectangle.platform.y, 4, yPos);
            char * Xtext = " Width: ";
            char * Ytext = " Height: ";
            char * comma = ", ";
            char xEnd[8];
            gcvt((double) selected->rectangle.platform.width, 4, xEnd);
            char yEnd[8];
            gcvt((double) selected->rectangle.platform.height, 4, yEnd);
            strcat(description, xPos);
            strcat(description, comma);
            strcat(description, yPos);
            strcat(description, Xtext);
            strcat(description, xEnd);
            strcat(description, Ytext);
            strcat(description, yEnd);
            char * flag;
            switch (selected->rectangle.flag){
                case NONE:
                flag = "Flag: None";
                break;
                case WATER:
                flag = "Flag: Water";
                break;
                case GROUND:
                flag = "Flag: Ground";
                break;
                case ICE:
                flag = "Flag: Ice";
                break;
                case SAND:
                flag = "Flag: Sand";
                break;
                case FAKE:
                flag = "Flag: Fake Block";
                break;
                case TOP_ONLY:
                flag = "Flag: Passthrough";
                break;
                case DEATH:
                flag = "Flag: Death Zone";
                break;
                default:
                flag = "Flag: Undefined";
            }
            DrawText(description, 10, 10 + FONT_SIZE * 3, FONT_SIZE, WHITE);
            DrawText(flag, 10, 10 + FONT_SIZE * 4, FONT_SIZE, WHITE);
        }

    }
    
    EndDrawing();
    return;
}


 // Appends a rectangle object at the end of the selected list
Rectangles * addRectangle(Rectangles ** list, Camera2D camera){
        if (*list == NULL){
            (*list) = malloc(sizeof(Rectangles));
            (*list)->next = NULL;
            (*list)->prev = NULL;
            (*list)->rectangle.platform.x = (float) ((int) GetScreenToWorld2D(GetMousePosition(), camera).x);
            (*list)->rectangle.platform.y = (float) ((int) GetScreenToWorld2D(GetMousePosition(), camera).y);
            (*list)->rectangle.platform.width = 0;
            (*list)->rectangle.platform.height = 0;
            (*list)->rectangle.flag = GROUND;
            return *list;
        }
        else{
            Rectangles * set = *list;
            while (set->next != NULL){
                set = set->next;
            }
            set->next = malloc(sizeof(Rectangles));
            set->next->prev = set;
            set = set->next;
            set->next = NULL;
            set->rectangle.platform.x = (float) ((int) GetScreenToWorld2D(GetMousePosition(), camera).x);
            set->rectangle.platform.y = (float) ((int) GetScreenToWorld2D(GetMousePosition(), camera).y);
            set->rectangle.platform.width = 0;
            set->rectangle.platform.height = 0;
            set->rectangle.flag = GROUND;
            return set;
        }
}


// Repositions X and Y positions to nearest "50" on the grid
void snapRectangle(Rectangles * selected, bool * hasSaved){
    if (selected != NULL){
        if (remainderf(selected->rectangle.platform.y, 1) != 0){
            selected->rectangle.platform.y -= remainderf(selected->rectangle.platform.y, 1);
        }
        if (remainderf(selected->rectangle.platform.x, 1) != 0){
            selected->rectangle.platform.x -= remainderf(selected->rectangle.platform.x, 1);
        }
        if ((int) (selected->rectangle.platform.x) % 50  != 0 && selected->rectangle.platform.x > 0){
            if ((int) (selected->rectangle.platform.x) % 50 < 25){
                selected->rectangle.platform.x -= (float) ((int) (selected->rectangle.platform.x) % 50);
            }
            else{
                selected->rectangle.platform.x += (float) (50 - (int) (selected->rectangle.platform.x) % 50);
            }
            *hasSaved = false;
        }
        else if ((int) (selected->rectangle.platform.x) % 50  != 0 && selected->rectangle.platform.x < 0){
            selected->rectangle.platform.x = (float) ((int) selected->rectangle.platform.x);
            if ((int) (selected->rectangle.platform.x) % 50 > -25){
                selected->rectangle.platform.x -= (float) ((int) (selected->rectangle.platform.x) % 50);
            }
            else{
                selected->rectangle.platform.x -= (float) (50 + (int) (selected->rectangle.platform.x) % 50);
            }
            *hasSaved = false;
        }
        if ((int) (selected->rectangle.platform.y) % 50  != 0 && selected->rectangle.platform.y > 0){
            if ((int) (selected->rectangle.platform.y) % 50 < 25){
                selected->rectangle.platform.y -= (float) ((int) (selected->rectangle.platform.y) % 50);
            }
            else{
                selected->rectangle.platform.y += (float) (50 - (int) (selected->rectangle.platform.y) % 50);
            }
            *hasSaved = false;
        }
        else if ((int) (selected->rectangle.platform.y) % 50  != 0 && selected->rectangle.platform.y < 0){
            selected->rectangle.platform.y = (float) ((int) selected->rectangle.platform.y);
            if ((int) (selected->rectangle.platform.y) % 50 > -25){
                selected->rectangle.platform.y -= (float) ((int) (selected->rectangle.platform.y) % 50);
            }
            else{
                selected->rectangle.platform.y -= (float) (50 + (int) (selected->rectangle.platform.y) % 50);
            }
            *hasSaved = false;
        }
    }
    return;
}


// Resizes rectangle Width and height to the nearest "50"
void snapRectangleSize(Rectangles * selected, bool * hasSaved){
    if (selected != NULL){
        if (selected->rectangle.platform.width < 50){
            selected->rectangle.platform.width = 50;
        }
        if (selected->rectangle.platform.height < 50){
            selected->rectangle.platform.height = 50;
        }
        if ((int) (selected->rectangle.platform.width) % 50  != 0){
            selected->rectangle.platform.width = (float) ((int) selected->rectangle.platform.width);
            if ((int) (selected->rectangle.platform.width) % 50 < 25){
                selected->rectangle.platform.width -= (float) ((int) (selected->rectangle.platform.width) % 50);
            }
            else{
                selected->rectangle.platform.width += (float) (50 - (int) (selected->rectangle.platform.width) % 50);
            }
            *hasSaved = false;
        }
        if ((int) (selected->rectangle.platform.height) % 50  != 0){
            selected->rectangle.platform.height = (float) ((int) selected->rectangle.platform.height);
            if ((int) (selected->rectangle.platform.height) % 50 < 25){
                selected->rectangle.platform.height -= (float) ((int) (selected->rectangle.platform.height) % 50);
            }
            else{
                selected->rectangle.platform.height += (float) (50 - (int) (selected->rectangle.platform.height) % 50);
            }
            *hasSaved = false;
        }
    }
    return;
}


// Removes rectangle object in list
void deleteRectangle(Rectangles ** rectangle, Rectangles ** list, int * count, bool * hasSaved){
    Rectangles * item = *rectangle;
    if (*rectangle != NULL){
        if (item->next == NULL && item->prev != NULL){
            item->prev->next = NULL;
            free(item);
        }
        else if (item->next != NULL && item->prev != NULL){
            item->prev->next = item->next;
            item->next->prev = item->prev;
            free(item);
        }
        else if (item->next != NULL && item->prev == NULL){
            *list = item->next;
            item->next->prev = NULL;
            free(item);
        }
        else if (item->next == NULL && item->prev == NULL){
            *list = NULL;
            free(item);
        }
        *rectangle = NULL;
        (*count) -= 1; 
        *hasSaved = false;
    }
    return;
}


// Adds a copy of Selected Rectangle to the list with X and Y positons offset by 20
Rectangles * copyRectangle(Rectangles * selected, Rectangles * list, int * count, bool * hasSaved){
    if (selected != NULL){
        while (list->next != NULL){
            list = list->next;
        }
        list->next = malloc(sizeof(Rectangles));
        list->next->prev = list;
        list = list->next;
        list->next = NULL;
        list->rectangle = selected->rectangle;
        list->rectangle.platform.x += 20;
        list->rectangle.platform.y -= 20;
        (*count) += 1;
        (*hasSaved) = false;
        return list;
    }
    else{
        return NULL;
    }
}


// Modifies Active Rectangle size based on  Mouse Delta
void modifySize(Rectangles * rectangle, Camera2D camera){
    Vector2 delta = GetMouseDelta();
    rectangle->rectangle.platform.width += (float) ((int) delta.x * (1/camera.zoom));
    rectangle->rectangle.platform.height += (float) ((int) delta.y * (1/camera.zoom));
    return;
}


// Unloads all rectangle objects in list from memory
void freeRectangles(Rectangles * list){
    Rectangles * victim;
    do{
        victim = list;
        list = list->next;
        free(victim);
    }while (list != NULL);
    return;
}

// Resizes rectangles when width and height are negative.
void correctRectangle(Rectangles * selected){ //corrects rectangle sizes when width or height is negative
    if (selected != NULL){
        if (selected->rectangle.platform.width < 0){
            selected->rectangle.platform.x = selected->rectangle.platform.x + selected->rectangle.platform.width;
            selected->rectangle.platform.width = -selected->rectangle.platform.width;
        }
        if (selected->rectangle.platform.height < 0){
            selected->rectangle.platform.y = selected->rectangle.platform.y + selected->rectangle.platform.height;
            selected->rectangle.platform.height = -selected->rectangle.platform.height;
        }
    }
    return;
}

// Saves data to a file
void saveData(Rectangles * list, int count, const char * filename, Rectangle spawn){
    FILE * file;
    errno_t result = fopen_s(&file, filename, "wb");
    if (result == 0){
        Vector2 spawnPosition = {spawn.x, spawn.y};
        fwrite(&spawnPosition, sizeof(Vector2), 1, file);
        Rectangles * set = list;
        while (set != NULL){
            correctRectangle(set);
            set = set->next;
        }
        set = list;
        fwrite(&count, sizeof(int), 1, file);
        while (set != NULL){
            fwrite(&set->rectangle, sizeof(Surface), 1, file);
            set = set->next;
        }
        fclose(file);
    }
    
    return;
}


// Sets all rectangles to inactive
void diselect(Rectangles * list){
    while (list != NULL){
    list->isSelected = false;
        list = list->next;
    }
    return;
}


// Flags Selected rectangle as "Selected"
void select(Rectangles * selected){
    if (selected != NULL){
        selected->isSelected = true;
    }
    return;
}


// Marks a Rectangle object as "Selected" if mouse cursor is over it.
Rectangles * choose(Rectangles * list, Camera2D camera){
    Rectangles * selected = NULL;
    while (list != NULL){
        if (GetScreenToWorld2D(GetMousePosition(), camera).x > list->rectangle.platform.x && GetScreenToWorld2D(GetMousePosition(), camera).x < (list->rectangle.platform.x + list->rectangle.platform.width)){
            if (GetScreenToWorld2D(GetMousePosition(), camera).y > list->rectangle.platform.y && GetScreenToWorld2D(GetMousePosition(), camera).y < (list->rectangle.platform.y + list->rectangle.platform.height)){
                selected =  list;
            }
        }
        list = list->next;
    }
    return selected;
}


// Moves a rectangle object's X and Y position by increments of "50"
void snapMovePlatform(Rectangles * selected, bool * hasSaved){
    if (IsKeyPressed(KEY_UP)){
                selected->rectangle.platform.y -= 50;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_DOWN)){
                selected->rectangle.platform.y += 50;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_LEFT)){
                selected->rectangle.platform.x -= 50;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_RIGHT)){
                selected->rectangle.platform.x += 50;
                *hasSaved = false;
            }
}


// Moves Rectangle object's position
void movePlatform(Rectangles * selected, bool * hasSaved, Camera2D camera){
    if (selected != NULL){
        if (IsKeyDown(KEY_LEFT_SHIFT)){
            if (IsKeyPressed(KEY_UP)){
                selected->rectangle.platform.y -= 1;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_DOWN)){
                selected->rectangle.platform.y += 1;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_LEFT)){
                selected->rectangle.platform.x -= 1;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_RIGHT)){
                selected->rectangle.platform.x += 1;
                *hasSaved = false;
            }
        }
        else if (IsKeyDown(KEY_Q)){
            snapMovePlatform(selected, hasSaved);
        }
        else{
            int speed;
            if (IsKeyDown(KEY_SPACE)){
                speed = 5;
            }
            else{
                speed = 2;
            }
            if (IsKeyDown(KEY_UP)){
                selected->rectangle.platform.y -= speed;
                *hasSaved = false;
            }
            if (IsKeyDown(KEY_DOWN)){
                selected->rectangle.platform.y += speed;
                *hasSaved = false;
            }
            if (IsKeyDown(KEY_LEFT)){
                selected->rectangle.platform.x -= speed;
                *hasSaved = false;
            }
            if (IsKeyDown(KEY_RIGHT)){
                selected->rectangle.platform.x += speed;
                *hasSaved = false;
            }
        }
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 delta = GetMouseDelta();
            selected->rectangle.platform.x += (float) ((int) delta.x * (1/camera.zoom));
            selected->rectangle.platform.y += (float) ((int) delta.y * (1/camera.zoom));
            if (delta.x != 0 && delta.y != 0){
                *hasSaved = false;
            }
        }
    }
    return;
}


// Resizes width and height of a rectangle object by incremennts of 50
void snapTransform(Rectangles * selected, bool * hasSaved){
        if (IsKeyPressed(KEY_UP)){
            selected->rectangle.platform.height -= 50;
            *hasSaved = false;
        }
        if (IsKeyPressed(KEY_DOWN)){
            selected->rectangle.platform.height += 50;
            *hasSaved = false;
        }
        if (IsKeyPressed(KEY_LEFT)){
            selected->rectangle.platform.width -= 50;
            *hasSaved = false;
        }
        if (IsKeyPressed(KEY_RIGHT)){
            selected->rectangle.platform.width += 50;
            *hasSaved = false;
        }
    return;
}


// Resizes selected rectangle object width and height
void transformPlatform(Rectangles * selected, bool * hasSaved){
    if (selected != NULL){
        if (IsKeyDown(KEY_LEFT_SHIFT)){
            if (IsKeyPressed(KEY_UP)){
                selected->rectangle.platform.height -= 1;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_DOWN)){
                selected->rectangle.platform.height += 1;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_LEFT)){
                selected->rectangle.platform.width -= 1;
                *hasSaved = false;
            }
            if (IsKeyPressed(KEY_RIGHT)){
                selected->rectangle.platform.width += 1;
                *hasSaved = false;
            }
        }
        else if (IsKeyDown(KEY_Q)){
            snapTransform(selected, hasSaved);
        }
        else{
            int speed;
            if (IsKeyDown(KEY_SPACE)){
                speed = 5;
            }
            else{
                speed = 2;
            }
            if (IsKeyDown(KEY_UP)){
                selected->rectangle.platform.height -= speed;
                *hasSaved = false;
            }
            if (IsKeyDown(KEY_DOWN)){
                selected->rectangle.platform.height += speed;
                *hasSaved = false;
            }
            if (IsKeyDown(KEY_LEFT)){
                selected->rectangle.platform.width -= speed;
                *hasSaved = false;
            }
            if (IsKeyDown(KEY_RIGHT)){
                selected->rectangle.platform.width += speed;
                *hasSaved = false;
            }
        }
    }
    return;
}



// Loads data from file to lists and player spawn
Rectangles * loadData(const char * filename, Rectangles ** list, int * count, Rectangle * spawn){
    FILE * file;
    errno_t result = fopen_s(&file, filename, "rb");
    if (result == 0){
        Vector2 position;
        fread(&position, sizeof(Vector2), 1, file);
        spawn->x = position.x;
        spawn->y = position.y;
        fread(count, sizeof(int), 1, file);
        if (*count > 0){
            *list = malloc(sizeof(Rectangles));
            Rectangles * item = *list;
            for (int i = 0; i < (*count); i++){
                item->isSelected = false;
                if (i == 0){
                    item->prev = NULL;
                }
                fread(&item->rectangle, sizeof(Surface), 1, file);
                if (i == ((*count) - 1)){
                    item->next = NULL;
                }
                else{
                    item->next = malloc(sizeof(Rectangles));
                    item->next->prev = item;
                    item = item->next;
                }
            }
        }
        else{
            *list = NULL;
        }
        fclose(file);
    }
    else{
        *list = NULL;
    }
    return *list;
}


// Moves camera X and Y position
void moveCamera(Camera2D * camera){
    if (!IsKeyDown(KEY_LEFT_CONTROL)){
        if (IsKeyDown(KEY_LEFT_SHIFT)){
            if (IsKeyPressed(KEY_W)){
                camera->target.y -= 1;
            }
            if (IsKeyPressed(KEY_A)){
                camera->target.x -= 1;
            }
            if (IsKeyPressed(KEY_S)){
                camera->target.y += 1;
            }
            if (IsKeyPressed(KEY_D)){
                camera->target.x += 1;
            }
        }
        else{
            float scale = (1/camera->zoom);
            int speed;
            if (IsKeyDown(KEY_SPACE)){
                speed = 10;
            }
            else{
                speed = 5;
            }
            if (IsKeyDown(KEY_W)){
                camera->target.y -= speed * scale;
            }
            if (IsKeyDown(KEY_A)){
                camera->target.x -= speed * scale;
            }
            if (IsKeyDown(KEY_S)){
                camera->target.y += speed * scale;
            }
            if (IsKeyDown(KEY_D)){
                camera->target.x += speed * scale;
            }
        }
    }
    float change = GetMouseWheelMove() / 20;
    camera->zoom += change;
    
    if (camera->zoom < 0.1){
        camera->zoom = 0.1;
    }
    if (camera->zoom > 5){
        camera->zoom = 5;
    }
    return;
}

// Changes the active flag on a selected rectangle object
void changeFlag(Rectangles * selected, bool *hasSaved){
    if (IsKeyPressed(KEY_KP_ADD) || IsKeyPressed(KEY_EQUAL)){
        selected->rectangle.flag++;
        if (selected->rectangle.flag >= FLAG_COUNT){
            selected->rectangle.flag = 0;
        }
        *hasSaved = false;
    }
    if (IsKeyPressed(KEY_KP_SUBTRACT) || IsKeyPressed(KEY_MINUS)){
        selected->rectangle.flag--;
        if (selected->rectangle.flag < 0){
            selected->rectangle.flag = FLAG_COUNT - 1;
        }
        *hasSaved = false;
    }
    return;
}


// Changes active editing mode
void changemode(short * mode, Rectangles ** selected, Rectangles * list, Rectangles ** activeRect, bool * isActive){
    if (IsKeyPressed(KEY_TAB)){
        if (*mode == TERRAIN_MODE){
            *isActive = false;
            correctRectangle(*activeRect);
            correctRectangle(*selected);
            diselect(list);
            *selected = NULL;
            *activeRect = NULL;
        }
        *mode += 1;
        if (*mode >= MODE_COUNT){
            *mode = 0;
        }
    }
    return;
}

//Prints a text file with rectangle data (X, Y, Width, Height)
void printArray(Rectangles * list, int count){
    FILE * file;
    errno_t result = fopen_s(&file, "rectangles.txt", "wt");
    if (result == 0){
        fprintf(file, "Rectangle platforms[%d] = {\n", count);
        while (list != NULL){
            fprintf(file, "    {%f, %f, %f, %f},\n", list->rectangle.platform.x, list->rectangle.platform.y, list->rectangle.platform.width, list->rectangle.platform.height);
            list = list->next;
        }
        fprintf(file, "};");
        fclose(file);
    }
    return;
}

// Modifys Player Spawn Location
void moveSpawn(Rectangle * spawn, Camera2D camera, bool * isSaved){
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
            Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
            Vector2 offset = {spawn->width / 2, spawn->height / 2};
            spawn->x = mousePos.x - offset.x;
            spawn->y = mousePos.y - offset.y;
            *isSaved = false;
        }
        return;
    }


// Selects and opens active file to be opened
void confirmLoad(Rectangles ** list, Rectangles ** selected, bool * hasSaved, char ** filename, char * loadFilename, int * count, Rectangle * spawn, Camera2D * camera, int width, int height){
    bool confirm = true;
    if (!*hasSaved){
        confirm = YesNoDialog("Are You Sure?", "The Current File Hasn't Been Saved Yet! Any Changes Will Be Lost!\nAre you sure you want to load a new file?", GetWindowHandle());
    }
    if (confirm){
        confirm = open_file("Level File (*.level)\0*.level\0", 0, loadFilename, GetWindowHandle());
        *filename = &loadFilename[0];
        if (confirm){
            if (*list != NULL){
                freeRectangles(*list);
            }
            *selected = NULL;
            *list = NULL;
            *list = loadData(*filename, list, count, spawn);
            camera->offset = (Vector2) {width / 2, height / 2};
            camera->target = (Vector2) {width / 2, height / 2};
            camera->zoom = 1;
            *hasSaved = true;
        }
    }
    return;
}

// Confirms a that a file dropped into appication window should be loaded
void confirmDropLoad(Rectangles ** list, Rectangles ** selected, bool * hasSaved, char ** filename, char * loadFilename, int * count, Rectangle * spawn, Camera2D * camera, int width, int height){
    bool confirm = true;
    if (!*hasSaved){
        confirm = YesNoDialog("Are You Sure?", "The Current File Hasn't Been Saved Yet! Any Changes Will Be Lost!\nAre you sure you want to load a new file?", GetWindowHandle());
    }
    if (confirm){
        FilePathList fileinfo = LoadDroppedFiles();
        for (unsigned int i = 0; i < strlen(fileinfo.paths[0]); i++){
            if (!strcmp(".level", &fileinfo.paths[0][i])){
                confirm = true;
                break;
            }
            else{
                confirm = false;
            }
        }
        if (confirm){
            strcpy(loadFilename, fileinfo.paths[0]);
            *filename = &loadFilename[0];
            UnloadDroppedFiles(fileinfo);
            if (*list != NULL){
                freeRectangles(*list);
            }
            *selected = NULL;
            *list = NULL;
            *list = loadData(*filename, list, count, spawn);
            camera->offset = (Vector2) {width / 2, height / 2};
            camera->target = (Vector2) {width / 2, height / 2};
            camera->zoom = 1;
            *hasSaved = true;
        }
        else{
            OKDialog("Load Failed!", "The selected file is not a valid level file.", GetWindowHandle());
            UnloadDroppedFiles(fileinfo);
        }
    }
    else{
        FilePathList fileinfo = LoadDroppedFiles();
        UnloadDroppedFiles(fileinfo);
    }
    return;
}

//sets file end to file extension
void setExtension(char * filename, const char * extension){
    int extLen = strlen(extension); //extension length
    int namelen = strlen(filename); //filename length
    if (strcmp(&filename[namelen-extLen], extension)){ // If the extension location does not contain the asserted extension, extension will be set
        for (int i = 0; i < extLen+1; i++){
            filename[namelen + i] = extension[i];
        }
    }
return;
}

// Ensures camera offset is accurate to window resolution
void setCameraOffset(Camera2D * camera, int width, int height){
    camera->offset = (Vector2) {width / 2, height / 2};
    return;
}

void shiftBlockPosition(Rectangles ** selected, bool * hasSaved){
    if (IsKeyPressed(KEY_PAGE_UP)){
        if ((*selected)->next != NULL){
            Surface * current = &(*selected)->rectangle;
            Surface * shift = &(*selected)->next->rectangle;
            Surface temp = *current;
            *current = *shift;
            *shift = temp;
            (*selected) = (*selected)->next;
            *hasSaved = false;
        }

    }
    else if (IsKeyPressed(KEY_PAGE_DOWN)){
        if ((*selected)->prev != NULL){
            Surface * current = &(*selected)->rectangle;
            Surface * shift = &(*selected)->prev->rectangle;
            Surface temp = *current;
            *current = *shift;
            *shift = temp;
            (*selected) = (*selected)->prev;
            *hasSaved = false;
        }
    }
    return;
}



int main(int argc, char ** argv){
    char * programName = "main.exe";
    char pathName[2048] = "\0";
    GetModuleFileNameA(NULL, pathName, 1023);
    for (unsigned int i = 0; i < strlen(pathName); i++){
        if (!strcmp(programName, &pathName[i])){
            pathName[i] = 0;
            break;
        }
    }
    char splashPath[2048] = "\0";
    strcat(splashPath, pathName);
    strcat(splashPath, "splash.png");
    printf("\n%s\n", splashPath);
    
    beginSplashScreen(splashPath, VERSION_NUMBER, VERSION_BOTTOM);

    Player player;
    Rectangles * list;
    Rectangles * activeRect;
    Rectangles * selected;
    player.isActive = false;
    char * filename;
    char loadFilename[FILENAME_MAX];
    int count = 0;
    activeRect = NULL;
    selected = NULL;
    Rectangle spawn;
    player.hasSaved = true;

    if (argc < 2){
        list = NULL;
        filename = NULL;
        spawn.x = 0;
        spawn.y = 0;
    }
    else{
        filename = argv[1];
        list = loadData(filename, &list, &count, &spawn);
    }
    spawn.width = 50;
    spawn.height = 100;
    WaitTime(3.0);
    endSplashScreen();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(0, 0, "Sapphire");
    SetExitKey(KEY_NULL);
    while (!IsWindowReady()){} //Wait for window initialization before continuing;
    SetWindowPosition(GetScreenWidth() / 4, GetScreenHeight() / 4);
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    int tempWidth = width;
    int tempHeight = height;
    int maxWidth = width;
    int maxHeight = height;
    Vector2 windowPos = GetWindowPosition();
    SetWindowSize(900, 600);
    SetWindowMinSize(600, 400);
    MaximizeWindow();
    bringToFront(GetWindowHandle());
    Camera2D camera;
    setCameraOffset(&camera, width, height);
    camera.target = (Vector2) {width / 2, height / 2};
    camera.rotation = 0;
    camera.zoom = 1;
    player.mode = TERRAIN_MODE;
    SetTargetFPS(60);
    BeginDrawing();
    ClearBackground(DARKGRAY);
    EndDrawing();

    bool isRunning = true;
    bool confirm;
    bool showScale = true;
    bool showInfo = true;
    bool isMaximized = true;
    bool showLines = false;

    while (isRunning && !WindowShouldClose()){
        int width = GetScreenWidth();
        int height = GetScreenHeight();
        
        setCameraOffset(&camera, width, height);
        if (IsWindowFocused()){
            if (IsKeyPressed(KEY_F2)){
                if (showScale){
                    showScale = false;
                }
                else{
                    showScale = true;
                }
            }
            if (IsKeyPressed(KEY_F3)){
                if (showInfo){
                    showInfo = false;
                }
                else{
                    showInfo = true;
                }
            }
            if (IsKeyPressed(KEY_F1)){
                if (showLines){
                    showLines = false;
                }
                else{
                    showLines = true;
                }
            }
            if (IsKeyPressed(KEY_F11)){
                if (IsWindowState(FLAG_WINDOW_UNDECORATED)){
                    ClearWindowState(FLAG_WINDOW_UNDECORATED);
                    SetWindowSize(tempWidth, tempHeight);
                    SetWindowPosition((int) windowPos.x, (int) windowPos.y);
                    WaitTime(.1f);
                    if (isMaximized){
                        MaximizeWindow();
                    }
                    
                    
                }
                else{
                    if (IsWindowMaximized()){
                        isMaximized = true;
                        ClearWindowState(FLAG_WINDOW_MAXIMIZED);
                    }
                    else{
                        isMaximized = false;
                    }
                    
                    tempWidth = width;
                    tempHeight = height;
                    SetWindowSize(maxWidth, maxHeight);
                    windowPos = GetWindowPosition();
                    SetWindowPosition(0, 0);
                    SetWindowState(FLAG_WINDOW_UNDECORATED);
                }
            }
            changemode(&player.mode, &selected, list, &activeRect, &player.isActive);
        
            if (player.mode == TERRAIN_MODE){

                // Creating Blocks
                if (!player.isActive && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)){
                    activeRect = addRectangle(&list, camera);
                    player.isActive = true;
                    count += 1;
                    player.hasSaved = false;
                    selected = NULL;
                }
                else if (player.isActive){
                    modifySize(activeRect, camera);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        player.isActive = false;
                        player.hasSaved = false;
                        correctRectangle(activeRect);
                        activeRect = NULL;
                    }
                }

                shiftBlockPosition(&selected, &player.hasSaved);

                if (!IsKeyDown(KEY_LEFT_CONTROL) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !player.isActive){
                    correctRectangle(selected);
                    selected = choose(list, camera);
                }
                if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_Q)){
                    snapRectangleSize(selected, &player.hasSaved);
                }
                else if (IsKeyPressed(KEY_Q)){
                    snapRectangle(selected, &player.hasSaved);
                }
                if (selected != NULL){
                    changeFlag(selected, &player.hasSaved);
                }
                if (selected != NULL && IsKeyDown(KEY_LEFT_ALT)){
                    transformPlatform(selected, &player.hasSaved);
                }
                else if(selected != NULL){
                    movePlatform(selected, &player.hasSaved, camera);
                    if (IsKeyPressed(KEY_DELETE) || IsKeyPressed(KEY_BACKSPACE)){
                        deleteRectangle(&selected, &list, &count, &player.hasSaved);
                    }
                }
                if (IsKeyDown(KEY_LEFT_CONTROL)){
                    if (IsKeyPressed(KEY_N)){
                        selected = copyRectangle(selected, list, &count, &player.hasSaved);
                    }
                    if (IsKeyPressed(KEY_P)){
                        printArray(list, count);
                    }
                }
                diselect(list);
                select(selected);
            }
            else if (player.mode == PLAYER_MODE){
                moveSpawn(&spawn, camera, &player.hasSaved);
            }

            moveCamera(&camera);
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O)){
                confirmLoad(&list, &selected, &player.hasSaved, &filename, loadFilename, &count, &spawn, &camera, width, height);
            }
            if (IsFileDropped()){
                confirmDropLoad(&list, &selected, &player.hasSaved, &filename, loadFilename, &count, &spawn, &camera, width, height);
            }
            if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_S)){
                    confirm = save_file("Level File (*.level)\0*.level\0", 0, loadFilename, GetWindowHandle());
                    if (confirm){
                        setExtension(loadFilename, ".level");
                        filename = &loadFilename[0];
                        saveData(list, count, filename, spawn);
                        player.hasSaved = true;
                    }
            }
            else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)){
                if (filename == NULL){
                    confirm = save_file("Level File (*.level)\0*.level\0", 0, loadFilename, GetWindowHandle());
                    if (confirm){
                        setExtension(loadFilename, ".level");
                        filename = &loadFilename[0];
                        saveData(list, count, filename, spawn);
                        player.hasSaved = true;
                    }
                }
                else{
                    saveData(list, count, filename, spawn);
                    player.hasSaved = true;
                }
            }
            if (IsKeyPressed(KEY_ESCAPE)){
                if (YesNoDialog("", "Would you like to exit?", GetWindowHandle())){
                    isRunning = false;
                }
            }

        }

        drawScreen(list, player, camera, selected, activeRect, spawn, showScale, showInfo, showLines);
        
        if ((!isRunning || WindowShouldClose())){
            if (!player.hasSaved){
                isRunning = false;
                short checkConfirm;
                checkConfirm = YesNoCancelDialog("Do you want to save?", "Your work is not currently saved, would you like to save before exiting?", GetWindowHandle());
                switch (checkConfirm){
                    case YES:
                        if (filename == NULL){
                            confirm = save_file("Level File (*.level)\0*.level\0", 0, loadFilename, GetWindowHandle());
                            if (confirm){
                                setExtension(loadFilename, ".level");
                                filename = &loadFilename[0];
                                saveData(list, count, filename, spawn);
                                player.hasSaved = true;
                            }
                            else{
                                isRunning = true;
                            }
                        }
                        else{
                            saveData(list, count, filename, spawn);
                            player.hasSaved = true;
                        }
                        break;
                    case NO:
                        break;
                    case CANCEL:
                        //overflows to default
                    default:
                        isRunning = true;
                    break;
                }

            }
            else{
                isRunning = false;
            }
        }
    }
    CloseWindow();
    if (list != NULL){
        freeRectangles(list);
    }
    return EXIT_SUCCESS;
}