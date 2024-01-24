#include "genesis.h"
#include "level.h"
#include "resources.h"


//Map* background;

//u32 dir = 0;
int offset = 0;
int* speed;

u16 LEVEL_init(u16 vramIndex)
{
    PAL_setPalette(PAL0, palette_all.data,DMA);
    u16 ind;
    ind = TILE_USERINDEX;
    VDP_loadTileSet(&bga_tileset, ind, DMA);
    ind += bga_tileset.numTile;
    
    
    //background = MAP_create(&bgb_map, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX)); another way to create backgroud
    //VDP_setPlaneSize(128,128,TRUE); might need this later
    VDP_drawImageEx(BG_B, &bga_road, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX), 0, 0, FALSE, TRUE);
    VDP_setScrollingMode(HSCROLL_PLANE,VSCROLL_PLANE);
    
    //// Allocate memory for the integer and assign its address to the pointer - done to avpid copying by value to save memory. probably not required but its safe to avoid high ram usage
    speed = (int*)malloc(sizeof(int));
    *speed = 1; // now we initialise value of speed by 1
    
    return ind;

}

void start_scroll()
{
    if(offset <= -256) offset = 0;
   
    VDP_setVerticalScroll(BG_B, offset-= *speed );
    
    //ignore these - method to use when using map object rather the just drawing image
    //dir -= 1;
    //MAP_scrollTo(background, 0, dir);
}







