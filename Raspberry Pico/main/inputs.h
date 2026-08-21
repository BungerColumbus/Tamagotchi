#ifndef _INPUTS_H
#define _INPUTS_H

#include <entity.h>

#define BTN_UP     0
#define BTN_LEFT  1
#define BTN_DOWN   2
#define BTN_RIGHT   3

void inputs_init(void);
void handle_input(Entity *e);

#endif // _INPUTS_H

