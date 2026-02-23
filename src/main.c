#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <stdio.h>
#include "renderer.h"
#include "input.h"
#include "state_manager.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int window_width = 1280;
    int window_height = 720;
    int window_offset = 0;
    SDL_Window* window = SDL_CreateWindow("game", window_width, window_height, SDL_WINDOW_OPENGL |  SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "failed to create window: %s\n",SDL_GetError());
	return 1;
    }
    
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
	printf("Failed to init GLAD\n");
	return 1;
    }


    glViewport(0, 0, window_width, window_height);


    Batch_Renderer rend;
    StateManager manager;

    init_renderer(&rend, 640, 360, 25000);

    initManager(&manager, GAME);
    int done = 0;
    long LAST = 0;
    float DELTA = 0;
    long NOW = SDL_GetPerformanceCounter();
    while (!done) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	    switch (event.type) {
		case SDL_EVENT_QUIT:
		    done = 1;
		    break;
		case SDL_EVENT_WINDOW_RESIZED:
		    window_width = event.window.data1;
		    window_height = (window_width/16) * 9;
		    window_offset = (event.window.data2 - window_height);
		break;
	    }
	}
	LAST = NOW;
	NOW = SDL_GetPerformanceCounter();
	DELTA = (float)(NOW - LAST) * 1000 / (float)SDL_GetPerformanceFrequency();
	DELTA /= 1000;
	updateMouse(window_width, window_height, rend);
	updateState(&manager, DELTA);
	drawState(manager);
	renderer_update(&rend, window_width, window_height, window_offset, DELTA);
	SDL_GL_SwapWindow(window);
    }

    cleanUpState(&manager);
    cleanup_renderer(&rend);
    SDL_DestroyWindow(window);
    SDL_GL_DestroyContext(gl_context);
}
