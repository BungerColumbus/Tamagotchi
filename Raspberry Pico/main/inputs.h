#ifndef _INPUTS_H
#define _INPUTS_H

#define BTN_UP     0
#define BTN_LEFT  1
#define BTN_DOWN   2
#define BTN_RIGHT   3

extern int offset_x;
extern int offset_y;

void inputs_init(void);
void handle_input(void);

#endif // _INPUTS_H

