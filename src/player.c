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

int max_time = 1;

u16* player_health;



Sprite* player;
Sprite* reflection;

u16 PLAYER_init(u16 vramIndex)
{   

    //u16 ind;
    PAL_setPalette(PAL2, blitzer.palette->data, DMA);
    //player = SPR_addSprite(&player_car, fix32ToInt(posX), fix32ToInt(posY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    player = SPR_addSprite(&blitzer, fix32ToInt(posX), fix32ToInt(posY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    reflection = SPR_addSprite(&car_reflection, fix32ToInt(posX), fix32ToInt(posY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    SPR_setAnim(player, 0 );
    SPR_setAnim(reflection, 0);

    player_health = MEM_alloc(sizeof(*player_health));
    *player_health = 3;

    return vramIndex;
}

bool PLAYER_checkHealth(u16* player_health)
{
    if (*player_health == 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void PLAYER_handleReflectionAnim()
{
    
    u32 elapsed_time = getTime(1);

    
    if(elapsed_time % 5 == 0)
    {
        SPR_nextFrame(reflection);
    }

}

void PLAYER_handleInput(u16 value)
{
    if (value & BUTTON_RIGHT || value & BUTTON_LEFT || value & BUTTON_UP || value & BUTTON_DOWN )
    {
        if(value & BUTTON_RIGHT)
        {   
            SPR_setAnim(reflection, 2);
            SPR_setFrame(player, 2);
            player_move_right = TRUE;
            player_move_left = FALSE;
        }
        else if (value & BUTTON_LEFT)
        {   
            SPR_setAnim(reflection, 1);
            SPR_setFrame(player, 1);
            player_move_right = FALSE;
            player_move_left = TRUE;
        }
        else
        {   SPR_setFrame(player, 0);
            SPR_setAnim(reflection, 0);
            player_move_right = FALSE;
            player_move_left = FALSE;
        }

        if(value & BUTTON_UP)
        {
            SPR_setAnim(player, 0);
            player_move_up = TRUE;
            player_move_down = FALSE;
        }
        else if (value & BUTTON_DOWN)   
        {   
            SPR_setAnim(player, 1);
            player_move_up = FALSE;
            player_move_down = TRUE;
        }
        else
        {
            // If both up and down buttons are pressed, don't change vertical movement
            // Or prioritize one direction over the other
            
            player_move_up = FALSE;
            player_move_down = FALSE;
        }
    }
    else
    {
        SPR_setAnim(player, CRUISE_ANI);
        SPR_setFrame(player, CRUISE_ANI);
        SPR_setAnim(reflection, CRUISE_ANI);
        player_move_right = FALSE;
        player_move_left = FALSE;
        player_move_up = FALSE;
        player_move_down = FALSE; 
    }

    checkCollisionWithLevel();
    SPR_setPosition(player, fix32ToInt(posX), fix32ToInt(posY));
    
    if(player_move_left == TRUE)
    {
        SPR_setPosition(reflection, (fix32ToInt(posX) - 4), fix32ToInt(posY));
    }
    else if(player_move_right == TRUE)
    {
        SPR_setPosition(reflection, fix32ToInt(posX) + 5, fix32ToInt(posY));
    }
    else
    {
        SPR_setPosition(reflection, fix32ToInt(posX), fix32ToInt(posY));
    }
    
    
}
