#include <genesis.h>

#include "player.h"
#include "resources.h"

int posX;
int posY;

Sprite* player;

u16 PLAYER_init(u16 vramIndex)
{   

    posX = 200;
    posY = 100;

    PAL_setPalette(PAL2, player_car.palette->data, DMA);
    player = SPR_addSprite(&player_car, posX, posY, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));

    return vramIndex;
}

void PLAYER_handleInput(u16 value)
{

    if(value & BUTTON_RIGHT)
    {
        posX += 1;
    }
    else if (value & BUTTON_LEFT)
    {
        posX -= 1;
    }

    if(value & BUTTON_UP)
    {
        posY -= 1;
    }
    else if (value & BUTTON_DOWN)
    {
        posY += 1;
    }

    SPR_setPosition(player, posX, posY);

}
