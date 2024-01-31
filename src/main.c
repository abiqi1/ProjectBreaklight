#include <genesis.h>
#include "player.h"
#include "level.h"


static void handleInput();

int main()
{	
	u16 ind;
	ind = TILE_USERINDEX;

	SPR_init();
	ind += LEVEL_init(ind);
	ind += PLAYER_init(ind);
	
	while(TRUE)
	{   
		startScroll();
		handleInput();
		

		SPR_update();
		SYS_doVBlankProcess();
	}

	//free used mem
	MEM_free(levelSpeed);

	return (0);
}

static void handleInput()
{
	u16 value = JOY_readJoypad(JOY_1);
	PLAYER_handleInput(value);
}
