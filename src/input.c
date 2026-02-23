#include "input.h"
#include "SDL3/SDL_mouse.h"
#include <SDL3/SDL.h>
#include "renderer.h"

Mouse mouse;
int keyPressed(key k) {
    const bool *keystate = SDL_GetKeyboardState(NULL);
    return keystate[k];
}

void updateMouse(float w, float h, Batch_Renderer r) {
    SDL_MouseButtonFlags input_state = SDL_GetMouseState(&mouse.x, &mouse.y);
    float scale_x, scale_y;
    scale_x = r.window_width/w;
    scale_y = r.window_height/h;
    mouse.x *= scale_x;
    mouse.y *= scale_y;

    if (input_state & SDL_BUTTON_LMASK) {
	mouse.left_pressed = 1; 
    } else {
	mouse.left_pressed = 0; 
    }
    if (input_state & SDL_BUTTON_RMASK) {
	mouse.right_pressed = 1; 
    } else {
	mouse.right_pressed = 0; 
    }
}
