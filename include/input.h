#ifndef INPUT_H
#define INPUT_H
#include <SDL3/SDL.h>
#include "renderer.h"


typedef enum {
    W = SDL_SCANCODE_W,
    A = SDL_SCANCODE_A,
    S = SDL_SCANCODE_S,
    D = SDL_SCANCODE_D,
    E = SDL_SCANCODE_E,
    R = SDL_SCANCODE_R,
    SPACE = SDL_SCANCODE_SPACE,
    ESC = SDL_SCANCODE_ESCAPE,
    TAB = SDL_SCANCODE_TAB
} key;

typedef struct {
    float x;
    float y;
    int left_pressed;
    int right_pressed;
} Mouse;

typedef struct {
    int r, tab, escape;
} Keys;

extern Mouse mouse;
extern Keys keys;

void resetKeys();
void updateKeys(SDL_Event *event);
int keyPressed(key k);
int keyJustPressed(key k);

void updateMouse(float w, float h, Batch_Renderer r);
#endif
