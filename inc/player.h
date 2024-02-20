#ifndef _PLAYER_H_
#define _PLAYER_H_


//these here define the hit box of car sprites in terms of pixels
#define PLAYER_COLLISION_FROM_LEFT 16
#define PLAYER_COLLISION_FROM_RIGHT 49
#define PLAYER_COLLISION_FROM_TOP 0
#define PLAYER_COLLISION_FROM_BOTTOM 85

u16 PLAYER_init(u16 vramIndex);
void PLAYER_handleInput(u16 value);
extern fix32 posX;
extern fix32 posY;
extern fix32 vert_velocity;
extern fix32 hor_velocity;
extern bool player_move_right;
extern bool player_move_left;
extern bool player_move_up;
extern bool player_move_down;


#endif // _PLAYER_H_