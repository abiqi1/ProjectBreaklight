#include "genesis.h"
#include "level.h"
#include "resources.h"
#include "player.h"


int bgbOffset = 0;
int bgaOffset = 0;
u16* levelSpeed;
int waterSpeedDivide = 2;
u16 bgBaseTileIndex[2];
const u16 LEVEL_ARRAY_LENGTH = 40;
//fix16* speed;
//fix16 waterSpeedDivide = FIX16(0.25);


//tile map with each index representing a tile on the map, 1 is for solid tile (meaning collision), 0 is for tiles that objects can pass through
const u8 LEVEL_COLLISION[1280] =
{
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 
};


u16 LEVEL_init(u16 vramIndex)
{
    u16 ind;

    PAL_setPalette(PAL0, water_pal.data, DMA);
	PAL_setPalette(PAL1, road_pal.data, DMA);

    //load tilesets to vram and keep index
    ind = vramIndex;
    bgBaseTileIndex[0] = ind;
    VDP_loadTileSet(water.tileset, ind, DMA);
    ind += water.tileset->numTile;
    bgBaseTileIndex[1] = ind;
    VDP_loadTileSet(road.tileset, ind, DMA);
    ind += road.tileset->numTile;
    
    // background = MAP_create(&bgb_map, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX)); another way to create backgroud
    // VDP_setPlaneSize(128,128,TRUE); might need this later
    VDP_drawImageEx(BG_B, &water, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, bgBaseTileIndex[0]), 0, 0, FALSE, TRUE);
    VDP_drawImageEx(BG_A, &road, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, bgBaseTileIndex[1]), 0, 0, FALSE, TRUE);
    
    // set scrolling mode
    VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
    
    // Allocate memory for the level speed and assign its address to the pointer - done to avoid copying by value to save memory. probably not required but its safe to avoid high ram usage
    levelSpeed = MEM_alloc(sizeof(levelSpeed));
    *levelSpeed = 7; // now we initialise value of speed to 2
    
    
    return ind;
}

void startScroll()
{
    //reset offset for both planes when limit is reached
    if(bgbOffset <= -256) bgbOffset = 0;
    if(bgaOffset <= -256) bgaOffset = 0;
   
    //VDP_setVerticalScroll(BG_B, bgbOffset -= (*speed/waterSpeedDivide));
    VDP_setVerticalScroll(BG_B, bgbOffset -= 2);
    VDP_setVerticalScroll(BG_A, bgaOffset -= *levelSpeed);
}

void checkCollisionWithLevel()
//this is a mess
{
    //we are taking the top and bottom tiles relative to the player position hitbox and then bit shifting by 3 which is the same as dividing by 8 (but less reosurces used). 
    //we want to divide by 8 because we dont want the pixels but the tiles. 
    s16 player_top_collision_coord_tile = (fix32ToInt(posX) + PLAYER_COLLISION_FROM_TOP) >> 3;
    s16 player_bottom_collision_coord_tile = (fix32ToInt(posY) + PLAYER_COLLISION_FROM_BOTTOM) >> 3;
    s16 blocked_coord;
 
    //controls collision with level when moving right
    if (player_move_right)
    {
        posX += hor_velocity;

        //same as before taking postion of the right tile from the collision box
        s16 player_right_collision_coord_tile = (fix32ToInt(posX) + PLAYER_COLLISION_FROM_RIGHT) >> 3;

        //now we take the top right tile index by taking the right collision tile + the top collision tile times the level array length (40). This gives us the index for the level collision array map
        u16 array_index_top_right = player_right_collision_coord_tile + (player_top_collision_coord_tile * LEVEL_ARRAY_LENGTH);
        u8 array_collision_type_top_right = LEVEL_COLLISION[array_index_top_right];

        //same as above but for the bottom right
        u16 array_index_bottom_right = player_right_collision_coord_tile +(player_bottom_collision_coord_tile * LEVEL_ARRAY_LENGTH);
        u8 array_collision_type_bottom_right = LEVEL_COLLISION[array_index_bottom_right];

        //now check if player has hit a solid tile (1) in top right and bottom right corners and if so push back the player 
        if(array_collision_type_top_right == SOLID_TILE || array_collision_type_bottom_right == SOLID_TILE)
        {
            blocked_coord = (player_right_collision_coord_tile << 3 ) -30 - PLAYER_COLLISION_FROM_RIGHT; // we revert back pixels to move player, extra pusback added to simulate very basic physics
            posX = intToFix32(blocked_coord); 
            posX -= FIX32(0.1); // this is added to prevent player getting caught in a tile and flying offscreen
        }   
    } 
    //same logic applies to the LEFT, UP and DOWN so need for extra comments
    else if (player_move_left)
    {
        posX -= hor_velocity;
        s16 player_left_collision_coord_tile = (fix32ToInt(posX) + PLAYER_COLLISION_FROM_LEFT) >> 3; 

        u16 array_index_top_left = player_left_collision_coord_tile + (player_top_collision_coord_tile * LEVEL_ARRAY_LENGTH);
        u8 array_collision_type_top_left = LEVEL_COLLISION[array_index_top_left];

        u16 array_index_bottom_left = player_left_collision_coord_tile + (player_bottom_collision_coord_tile * LEVEL_ARRAY_LENGTH);
        u8 array_collision_type_bottom_left = LEVEL_COLLISION[array_index_bottom_left];

        if(array_collision_type_top_left == SOLID_TILE || array_collision_type_bottom_left == SOLID_TILE)
        {
            blocked_coord = (player_left_collision_coord_tile << 3) + 38 - PLAYER_COLLISION_FROM_LEFT;
            posX = intToFix32(blocked_coord);
        }
    }

    if(player_move_up)
    {
        posY -= vert_velocity;
        s16 player_top_collision_coord_tile = (fix32ToInt(posY) + PLAYER_COLLISION_FROM_TOP) >> 3;

        s16 player_right_collision_coord_tile = (fix32ToInt(posX) + PLAYER_COLLISION_FROM_RIGHT) >> 3;
        u16 array_index_top_right = player_right_collision_coord_tile + (player_top_collision_coord_tile * LEVEL_ARRAY_LENGTH);
        u8 array_collision_type_top_right = LEVEL_COLLISION[array_index_top_right];

        s16 player_left_collision_coord_tile = (fix32ToInt(posX) + PLAYER_COLLISION_FROM_LEFT) >> 3;
        u16 array_index_top_left = player_left_collision_coord_tile + (player_top_collision_coord_tile * LEVEL_ARRAY_LENGTH);
        u8 array_collision_type_top_left = LEVEL_COLLISION[array_index_top_left];

        if(array_collision_type_top_right == SOLID_TILE || array_collision_type_top_left == SOLID_TILE)
        {
            blocked_coord = (player_top_collision_coord_tile << 3) + 8 - PLAYER_COLLISION_FROM_TOP;
            posY = intToFix32(blocked_coord);
        }   
    }
    else if(player_move_down)
    {
        posY += vert_velocity;
        s16 player_bottom_collision_coord_tile = (fix32ToInt(posY) + PLAYER_COLLISION_FROM_BOTTOM) >> 3;

        s16 player_right_collision_coord_tile = (fix32ToInt(posX) + PLAYER_COLLISION_FROM_RIGHT) >> 3;
        u16 array_index_bottom_right = player_right_collision_coord_tile +(player_bottom_collision_coord_tile * LEVEL_ARRAY_LENGTH);
        u8 array_collision_type_bottom_right = LEVEL_COLLISION[array_index_bottom_right];

        s16 player_left_collision_coord_tile = (fix32ToInt(posX) + PLAYER_COLLISION_FROM_LEFT) >> 3; 
        u16 array_index_bottom_left = player_left_collision_coord_tile + (player_bottom_collision_coord_tile * LEVEL_ARRAY_LENGTH);
        u8 array_collision_type_bottom_left = LEVEL_COLLISION[array_index_bottom_left];

        if(array_collision_type_bottom_right == SOLID_TILE || array_collision_type_bottom_left == SOLID_TILE)
        {
            blocked_coord = (player_bottom_collision_coord_tile << 3) - PLAYER_COLLISION_FROM_BOTTOM;
            posY = intToFix32(blocked_coord);
            posY -= FIX32(0.1);
        }   
    }
}
