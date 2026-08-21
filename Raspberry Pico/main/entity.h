#ifndef _ENTITY_H
#define _ENTITY_H

#include <stdint.h>

typedef struct { int x, y; } Vec2;

typedef struct {
    Vec2 pos;
    const uint16_t *sprite;
    int w, h;
} Entity;

extern Entity player;
extern Vec2 camera;

#endif // _ENTITY_H