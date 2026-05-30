#include "input.h"
#include <SDL3/SDL.h>
#include "renderer.h"
#include <stdio.h>

Mouse mouse;
Keys keys;
int keyPressed(key k) {
    const bool *keystate = SDL_GetKeyboardState(NULL);
    return keystate[k];
}

void resetKeys() {
    keys.escape = 0;
    keys.tab = 0;
    keys.r = 0;
}

void updateKeys(SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat) {
        switch (event->key.key) {
            case SDLK_ESCAPE: keys.escape = 1; break;
            case SDLK_TAB: keys.tab = 1; break;
            case SDLK_R: keys.r = 1; break;
        }
    }
}
// capital letters map: T = tab, E = escape, C = control, S = shift
int keyJustPressed(key k) {
    switch (k) {
	case R: return keys.r; break;
	case ESC: return keys.escape; break;
	case TAB: return keys.tab; break;
	default: break;
    }
    return 0;
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
