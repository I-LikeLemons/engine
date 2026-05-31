#include <state_manager.h>
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

}

void cleanUpState(StateManager *sm) {
}
