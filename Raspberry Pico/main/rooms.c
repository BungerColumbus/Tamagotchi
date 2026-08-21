#include <stddef.h>
#include "rooms.h"
#include "sd_commands.h"

// *room_at points towards the room with pos x and y by literally checking every room and seeing if they have those 2 coordinates you put as params.
const Room *room_at(int x, int y)
{
    for (size_t i = 0; i < ROOM_COUNT; i++)
        if (rooms[i].pos.x == x && rooms[i].pos.y == y)
            return &rooms[i];
    return NULL;
}

void change_room(Edge edge, uint16_t active_background[20480], Vec2 current_room)
{
    // We initialize the target with the current_room, current_room is always 0, 0. 
    // Afterwards depending on the edge that was chosen we change the x OR y accordingly
    Vec2 target = current_room;
    switch (edge) {
        case EDGE_LEFT:   target.x -= 1; break;
        case EDGE_RIGHT:  target.x += 1; break;
        case EDGE_TOP:    target.y -= 1; break;
        case EDGE_BOTTOM: target.y += 1; break;
        default: return;
    }

    // *r points towards the room_at target (x, y). We add the EDGE that is being crossed to the correct coordinate. 
    const Room *r = room_at(target.x, target.y);
    if (r == NULL) {
        // No room that way, shove the player back on screen
        if (edge == EDGE_LEFT)   player.pos.x = 0;
        if (edge == EDGE_RIGHT)  player.pos.x = SCREEN_WIDTH  - player.w/2;
        if (edge == EDGE_TOP)    player.pos.y = 0;
        if (edge == EDGE_BOTTOM) player.pos.y = SCREEN_HEIGHT - player.h/2;
        // If there is a room ignore everthing and return 
        return;
    }

    // If there is a room on the other side we teleport the player on the other side AND
    switch (edge) {
        case EDGE_LEFT:   player.pos.x = SCREEN_WIDTH - player.w/2; break;
        case EDGE_RIGHT:  player.pos.x = 0;                       break;
        case EDGE_TOP:    player.pos.y = SCREEN_HEIGHT - player.h/2; break;
        case EDGE_BOTTOM: player.pos.y = 0;                        break;
        default: break;
    }

    // We read the name of room r and put that as the active_background.
    read_bmp_to_array(r->room_name, active_background);
    // The current room is now the target room.
    current_room = target;


}