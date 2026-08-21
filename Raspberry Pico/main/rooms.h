#ifndef _ROOMS_H
#define _ROOMS_H

#include "entity.h"
#include "images.h"

// A room is literally a 2D position and the name of the .bmp file
typedef struct {
    Vec2 pos;
    const char *room_name;
} Room;


const Room *room_at(int x, int y);

// A static, constant array of Rooms
static const Room rooms[] = {
    { { 0, 0}, "mainroom.bmp" },
    { {-1, 0}, "leftroom.bmp" },
};

// Number of rooms
#define ROOM_COUNT (sizeof(rooms))

// Defines 4 edges and the case when there is no edge (the player ain't crossing an edge)
typedef enum { EDGE_NONE, EDGE_LEFT, EDGE_RIGHT, EDGE_TOP, EDGE_BOTTOM } Edge;

// Checks if an edge was crossed, each if statement checks if the pos of the player crossed a boundary
static Edge edge_crossed(const Entity *e)
{
    if (e->pos.x + e->w/2 <= 0)             return EDGE_LEFT;
    if (e->pos.x >= SCREEN_WIDTH)         return EDGE_RIGHT;
    if (e->pos.y + e->h <= 0)             return EDGE_TOP;
    if (e->pos.y >= SCREEN_HEIGHT)        return EDGE_BOTTOM;
    return EDGE_NONE;
}

// The whole change room void.
void change_room(Edge edge, uint16_t active_background[20480], Vec2 current_room);

#endif // _ROOMS_H