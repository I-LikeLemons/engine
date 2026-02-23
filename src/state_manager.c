#include <state_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <renderer.h>

void initManager(StateManager *sm, states s) {
    sm->state = s;
}

void updateState(StateManager *sm, float delta) {
    switch (sm->state) {
	case GAME:
	    break;
	case MENU:
	    break;
    };
}

void drawState(StateManager sm) {
    switch (sm.state) {
	case GAME:
	    break;
	case MENU:
	    break;
    };

    quad_draw(0,0,100,100,0,1,1,0,0,WHITE);
}

void cleanUpState(StateManager *sm) {
}
