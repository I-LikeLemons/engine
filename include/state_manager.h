#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H 

typedef enum {
    MENU,
    GAME,
} states;


typedef struct {
    states state;
} StateManager;

void initManager(StateManager* sm, states s);
void updateState(StateManager* sm, float delta);
void drawState(StateManager sm);
void changeState(StateManager *sm, states s);
void cleanUpState(StateManager *sm);
#endif
