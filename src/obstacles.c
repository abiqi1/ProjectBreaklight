#ifndef EFFECTS_H
#define EFFECTS_H

#include <genesis.h>
#include "obstacles.h"
#include "resources.h"


#define Y_COORD_LIMIT 220
#define Y_SPAWN_COORD -70
#define SPAWN_RATE 100

#define PLAYER_OBSTACLE_WIDTH 30
#define PLAYER_OBSTACLE_LENGTH 55
#define ITEM_COUNTER_DEFAULT 900


ObstacleData obsDataList[] = {
    { 6,  &cars_sheet,  PAL2, "whiteCar",  30, 55 },
    { 7,  &cars_sheet,  PAL2, "police",    30, 55 },
    { 8,  &cars_sheet,  PAL2, "ambulance", 30, 55 },
    { 9,  &cars_sheet,  PAL2, "taxi",      30, 55 },
    { 10, &cars_sheet,  PAL2, "blueCar",   30, 55 },
    { 11, &cars_sheet,  PAL2, "greyCar",   30, 55 },
    { 3,  &items_sheet, PAL3, "barrier",   30, 61 },
    { 4,  &items_sheet, PAL3, "oil",       30, 61 }
  };

ObstacleData itemDataList[] = {
    { 0,  &items_sheet, PAL3, "heart",     30, 61 },
    { 1,  &items_sheet, PAL3, "armour",    30, 61 },
    { 2,  &items_sheet, PAL3, "nitro",     30, 61 },
};

int xSpawnCoords[5] = {50, 88, 128, 168, 207};
Obstacle obstacleList[20];
Effect effectList[5];
int obstacleListIndex = -1;
int effectListIndex = -1;
u16 spawn_counter = SPAWN_RATE;
int effect_delete_counter = 30;

int* itemCounter;



//random() % (max - min + 1) + min

void OBSTACLES_counterInit()
{
    itemCounter = MEM_alloc(sizeof(*itemCounter));
    *itemCounter = ITEM_COUNTER_DEFAULT;
}

u16 OBSTACLES_generator(u16 vramIndex)
{   
    spawn_counter--;
    --*itemCounter;

    if(spawn_counter == 0)
    {
        pickRandObj();
        spawn_counter = SPAWN_RATE;
    }
    OBSTACLES_handleMovement();
    return vramIndex;
}

void updateEffects(Effect e) 
{
    effectListIndex++;
    effectList[effectListIndex] = e;
}

void deleteEffect() 
{
    Effect* e;
    for (int i = 0; i <= effectListIndex; i++) 
    {
        e = &effectList[i];

        if (e->timer > 0) 
        {
            e->timer--;
        } 
        else 
        {
            SPR_releaseSprite(e->o);
            for(int j = i; j <= effectListIndex; j++)
            {
                effectList[j] = effectList[j + 1];
            }
            effectListIndex--;
            i--; 
        }
    }
}

void OBSTACLES_handleMovement()
{   
    Obstacle* o;
    for(int i = 0; i <=obstacleListIndex; i++ )
    {
        o = &obstacleList[i];
        if(strcmp(o->data->name, "police") == 0)
        {
            updateObstaclePosition(o);
        }
        else
        {
            o->obsY += FIX32(1);
            SPR_setPosition(o->obs, fix32ToInt(o->obsX), fix32ToInt(o->obsY));
        }
    }
}

void updateObstaclePosition(Obstacle* o) 
{
    if (fix32ToInt(posX) < fix32ToInt(o->obsX)) {
        o->obsX -= FIX32(0.5);
    }
    // If player is to the right of the obstacle, move obstacle right
    else if (fix32ToInt(posX) > fix32ToInt(o->obsX)) {
        o->obsX += FIX32(0.5);
    }
    o->obsY+=FIX32(1);
    SPR_setPosition(o->obs, fix32ToInt(o->obsX), fix32ToInt(o->obsY));
}

void OBSTACLES_deleteFromList()
{
    Obstacle* o;
    for(int i = 0; i <= obstacleListIndex; i++)
    {
        o = &obstacleList[i];
        if(fix32ToInt(o->obsY) > Y_COORD_LIMIT)
        {
            releaseSprite(o, i);
        }
    }
    deleteEffect();
}

void OBSTACLES_handleCollision(u16* graceperiod)
{
    effect_delete_counter--;
    Obstacle* o;
    for(int i = 0; i <= obstacleListIndex; i++)
    {
            //                     int hit = 1001;
            //         char str_Region[2] = "0";
            //     sprintf(str_Region,"%d",hit);
            //  VDP_clearTextAreaBG(BG_B,15,10,2,1);
            //     VDP_drawTextBG(BG_B,str_Region,15,10);


        o = &obstacleList[i];
        if(fix32ToInt(posX) < fix32ToInt(o->obsX) + o->data->width &&
           fix32ToInt(posX) + PLAYER_OBSTACLE_WIDTH > fix32ToInt(o->obsX) &&
           fix32ToInt(posY) < fix32ToInt(o->obsY) + o->data->length &&
           fix32ToInt(posY) + PLAYER_OBSTACLE_LENGTH > fix32ToInt(o->obsY)
        )
        {
            if(strcmp(o->data->name, "armour") == 0)
            {
                Sprite* armourEffect = SPR_addSprite(&effects_sheet, fix32ToInt(o->obsX), fix32ToInt(o->obsY), TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                SPR_setAnim(armourEffect, 1);
                releaseSprite(o, i);
                Effect e = {armourEffect, 30};
                updateEffects(e);
            }
            else if(strcmp(o->data->name, "heart") == 0)
            {
                Sprite* heartEffect = SPR_addSprite(&effects_sheet, fix32ToInt(o->obsX), fix32ToInt(o->obsY), TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                SPR_setAnim(heartEffect, 0);
                releaseSprite(o, i);
                Effect e = {heartEffect, 30};
                updateEffects(e);
            }
            else if(strcmp(o->data->name, "nitro") == 0)
            {   
                Sprite* nitroEffect = SPR_addSprite(&effects_sheet, fix32ToInt(posX), fix32ToInt(posY), TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                SPR_setAnim(nitroEffect, 3);
                SPR_setAlwaysOnTop(nitroEffect);
                releaseSprite(o, i);
                Effect e = {nitroEffect, 30};
                updateEffects(e);
            }
            // else if(strcmp(o->data->name, "oil"))
            // {

            // }
            // else if(strcmp(o->data->name, "barrier"))
            // {

            // }
            else
            {
                //PLAYER_manageHealth(graceperiod);
                Sprite* explosion = SPR_addSprite(&effects_sheet, fix32ToInt(o->obsX), fix32ToInt(o->obsY), TILE_ATTR(PAL3, FALSE, FALSE, FALSE));
                SPR_setAnim(explosion, 7);
                SPR_setAlwaysOnTop(explosion);
                releaseSprite(o, i);
       

                Effect e = {explosion, 30};
                updateEffects(e);
                //SPR_releaseSprite(explosion);// Adjust loop counter since we removed an element
            }
        }
    }
}



void releaseSprite(Obstacle* o, int i)
{
    SPR_releaseSprite(o->obs);
    for(int j = i; j <= obstacleListIndex; j++)
    {
        obstacleList[j] = obstacleList[j + 1];
    }
    obstacleListIndex--;
    i--; // Adjust loop counter since we removed an element
}

void pickRandObj()
{
    setRandomSeed(random());
    s32 rand_x_spawn = xSpawnCoords[random() % (4 - 0 + 1) + 0];
    ObstacleData* newObsD;

    if (obstacleListIndex <= 20)
    {
        if (check_item_counter() == TRUE)
        {
            u16 rand_obstacle_data_index = ((random() % (2 - 0 + 1)) + 0);
            newObsD = &itemDataList[rand_obstacle_data_index]; //select from items list
        }
        else
        {
             u16 rand_obstacle_data_index = ((random() % (7 - 0 + 1)) + 0);
             newObsD = &obsDataList[rand_obstacle_data_index]; //select from obstacle list
        }

        Sprite* obstacle = SPR_addSprite(newObsD->sheet, rand_x_spawn, Y_SPAWN_COORD, TILE_ATTR(newObsD->pal, FALSE, FALSE, FALSE));
        SPR_setAnim(obstacle, newObsD->sheetIndRef);
        Obstacle randObs = {obstacle, newObsD, NULL, NULL };
        randObs.obsX = intToFix32(rand_x_spawn);
        randObs.obsY = intToFix32(Y_SPAWN_COORD);
        obstacleListIndex++;
        obstacleList[obstacleListIndex] = randObs;
    }
}

bool check_item_counter()
{
    bool status = FALSE;

    if (*itemCounter < 0) 
    {
        *itemCounter = ITEM_COUNTER_DEFAULT;
        status = TRUE;
    } 
    return status;
}

#endif // EFFECTS_H