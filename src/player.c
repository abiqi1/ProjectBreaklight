#include <genesis.h>

#include "player.h"
#include "resources.h"
#include "level.h"




bool player_move_right = FALSE;
bool player_move_left = FALSE;
bool player_move_up = FALSE;
bool player_move_down = FALSE;
fix32 posX = FIX32(100);
fix32 posY = FIX32(100);
fix32 vert_velocity = FIX32(2);
fix32 hor_velocity = FIX32(3);





Sprite* player;

u16 PLAYER_init(u16 vramIndex)
{   

    PAL_setPalette(PAL2, player_car.palette->data, DMA);
    player = SPR_addSprite(&player_car, fix32ToInt(posX), fix32ToInt(posY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    SPR_setAnim(player, 0);

    return vramIndex;
}


void PLAYER_handleInput(u16 value)
{
    if(value & BUTTON_RIGHT)
    {
        player_move_right = TRUE;
        player_move_left = FALSE;
    }
    else if (value & BUTTON_LEFT)
    {
        player_move_right = FALSE;
        player_move_left = TRUE;
    }
    else
    {
        player_move_right = FALSE;
        player_move_left = FALSE;
    }

    if(value & BUTTON_UP)
    {
        player_move_up = TRUE;
        player_move_down = FALSE;
        //posY -= 1;
    }
    else if (value & BUTTON_DOWN)
    {
        //posY += 1;
        player_move_up = FALSE;
        player_move_down = TRUE;
    }
    else
    {
        player_move_up = FALSE;
        player_move_down = FALSE; 
    }

    checkCollisionWithLevel();
    SPR_setPosition(player, fix32ToInt(posX), fix32ToInt(posY));
}
