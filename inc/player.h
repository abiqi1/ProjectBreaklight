#ifndef _PLAYER_H_
#define _PLAYER_H_


//these here define the hit box of car sprites in terms of pixels
#define PLAYER_COLLISION_FROM_LEFT 16
#define PLAYER_COLLISION_FROM_RIGHT 48
#define PLAYER_COLLISION_FROM_TOP 0
#define PLAYER_COLLISION_FROM_BOTTOM 85

#define RELFECTION_ANI_DEFAULT 60
#define RELFECTION_SPEED_DEFAULT 6


#define CRUISE_ANI 0
#define BREAK_ANI 1
#define MOVE_LEFT_ANI 1
#define MOVE_RIGHT_ANI 2

extern fix32 posX;
extern fix32 posY;
extern fix32 vert_velocity;
extern fix32 hor_velocity;
extern bool player_move_right;
extern bool player_move_left;
extern bool player_move_up;
extern bool player_move_down;
extern u16* player_health;



u16 PLAYER_init(u16 vramIndex);
void PLAYER_handleInput(u16 value);
bool PLAYER_checkHealth(u16* player_health);
void PLAYER_handleReflectionAnim();
void PLAYER_manageHealth(u16* graceperiod);


#endif // _PLAYER_H_