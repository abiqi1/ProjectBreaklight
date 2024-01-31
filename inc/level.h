#ifndef _LEVEL_H_
#define _LEVEL_H_


#define MAP_WIDTH 320
#define MAP_HEIGHT 256
#define SOLID_TILE 1


extern const u16 LEVEL_ARRAY_LENGTH;
extern const u8 LEVEL_COLLISION[1280];
extern u16* levelSpeed;


u16 LEVEL_init(u16 vramIndex);
void startScroll();
void checkCollisionWithLevel();

#endif // _LEVEL_H_