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

u16 relection_ani_counter = RELFECTION_ANI_DEFAULT;
u16 relection_speed = RELFECTION_SPEED_DEFAULT;

u16* player_health;


Sprite* player;
Sprite* reflection;
Sprite* sparks;

u16 PLAYER_init(u16 vramIndex)
{   
    PAL_setPalette(PAL2, blitzer.palette->data, DMA);
    PAL_setPalette(PAL3, effects_sheet.palette->data, DMA);
    sparks = SPR_addSprite(&effects_sheet, player->x, player->y, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
    player = SPR_addSprite(&blitzer, fix32ToInt(posX), fix32ToInt(posY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    reflection = SPR_addSprite(&car_reflection, fix32ToInt(posX), fix32ToInt(posY), TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    SPR_setVisibility(sparks, HIDDEN );
    
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
    relection_ani_counter--;

    if(relection_ani_counter % relection_speed == 0)
    {
        SPR_nextFrame(reflection);
        relection_ani_counter = RELFECTION_ANI_DEFAULT;
    }
}

void PLAYER_handleInput(u16 value)
{
    if (value & BUTTON_RIGHT || value & BUTTON_LEFT || value & BUTTON_UP || value & BUTTON_DOWN )
    {
        if(value & BUTTON_RIGHT)
        {   
            SPR_setAnim(reflection, 2);
            SPR_setAnim(sparks, 2);
            SPR_setFrame(player, 2);
            SPR_setVisibility(sparks,  HIDDEN );
            player_move_right = TRUE;
            player_move_left = FALSE;
        }
        else if (value & BUTTON_LEFT)
        {   
            SPR_setAnim(reflection, 1);
            SPR_setAnim(sparks, 1);
            SPR_setFrame(player, 1);
            SPR_setVisibility(sparks,  HIDDEN );
            player_move_right = FALSE;
            player_move_left = TRUE;
        }
        else
        {   
            SPR_setFrame(player, 0);
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

    LEVEL_checkCollisionWithLevel(sparks);
    SPR_setPosition(player, fix32ToInt(posX), fix32ToInt(posY));
    
    if(player_move_left == TRUE)
    {
        SPR_setPosition(reflection, (fix32ToInt(posX) - 4), fix32ToInt(posY)); // this needs to change later so that all car reflections can be adjusted correctly.
        SPR_setPosition(sparks, fix32ToInt(posX), fix32ToInt(posY));
    }
    else if(player_move_right == TRUE)
    {
        SPR_setPosition(reflection, fix32ToInt(posX) + 5, fix32ToInt(posY));
        SPR_setPosition(sparks, fix32ToInt(posX), fix32ToInt(posY));
    }
    else
    {
        SPR_setPosition(reflection, fix32ToInt(posX), fix32ToInt(posY));
        SPR_setPosition(sparks, fix32ToInt(posX), fix32ToInt(posY));
    }
}
