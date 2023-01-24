#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include "pthread.h"
#include <stdbool.h>
#include <string.h>

// Vector 2 implementation with Integers instead of Floating Points
typedef struct vector2Int{
    int x, y;
}vector2Int;

typedef void* (*function_Pointer) (void *);

pthread_t g_splashThread;

bool g_finished;

const char * g_splashFile;


const char * g_versionNumber;

#define VERSION_TOP 1
#define VERSION_BOTTOM 2

int g_versionPos;

//Runs Splash Screen
void * splashScreen(void * arg){
    Image splashImage = LoadImage(g_splashFile);
    vector2Int imageSize = {splashImage.width, splashImage.height};
    InitWindow(imageSize.x, imageSize.y, "");
    SetWindowState(FLAG_WINDOW_UNDECORATED);
    if (g_versionNumber != NULL){
        
        int fontsize = (int) ((((double) imageSize.x / 60.0) + ((double) imageSize.y / 33.8)) / 2);
        char versionText[240] = "Version ";
        strcat(versionText, g_versionNumber);
        int versionY;
        switch (g_versionPos){
            case VERSION_TOP:
                versionY = (int) ((double) fontsize * 0.5);
                break;
            case VERSION_BOTTOM:
                versionY = imageSize.y - (int) ((double) fontsize * 1.5);
                break;
            default:
                versionY = 0;
                break;
        }
        ImageDrawText(&splashImage, versionText, imageSize.x / 35, versionY, fontsize, (Color) {255, 255, 255, 128});
    }
    Texture2D splash = LoadTextureFromImage(splashImage);
    while (!g_finished){
        ClearBackground(BLACK);
        BeginDrawing();{
            DrawTexture(splash, 0, 0, WHITE);
        }
        EndDrawing();
    }
    UnloadImage(splashImage);
    CloseWindow();
    ClearWindowState(FLAG_WINDOW_UNDECORATED);
    return NULL;
}

function_Pointer function = splashScreen;

// Initializes splash screen until given the command to terminate
void beginSplashScreen(const char * filename, const char * version, int versionPos){
    g_splashFile = filename;
    g_versionNumber = version;
    g_finished = false;
    g_versionPos = versionPos;
    pthread_create(&g_splashThread, NULL, function, NULL);
    return;
}

// Terminates active splash screen
void endSplashScreen(){
    g_finished = true;
    pthread_join(g_splashThread, NULL);
    return;
}