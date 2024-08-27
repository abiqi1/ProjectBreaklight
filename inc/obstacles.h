#ifndef _OBSTACLES_H_
#define _OBSTACLES_H_

#include "player.h"

typedef struct Effect
{
    Sprite* o;
    u16 timer;

} Effect;


typedef struct ObstacleData 
{
    u8 sheetIndRef;
    const SpriteDefinition* sheet;
    u8 pal;
    char name[15];
    u8 width;
    u8 length;

} ObstacleData;


typedef struct Obstacle
{
    Sprite* obs;
    ObstacleData* data;
    fix32 obsX;
    fix32 obsY;

} Obstacle;

Sprite* nitroEffect;
Sprite* heartEffect;
Sprite* armourEffect;

u16 OBSTACLES_generator(u16 vramIndex);
extern void OBSTACLES_handleMovement();
extern void OBSTACLES_counterInit();
void OBSTACLES_deleteFromList();
void OBSTACLES_handleCollision(u16* graceperiod);
void updateEffects(Effect e);
void deleteEffect();
void releaseSprite(Obstacle* o, int i);
void updateObstaclePosition(Obstacle* o);
bool check_item_counter();
void pickRandObj();



#endif