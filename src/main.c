#include <genesis.h>
#include "player.h"

static void handleInput();

int main()
{	
	//HELLO NAJEEB
	u16 ind;
	ind = TILE_USERINDEX;

	SPR_init();
	ind += PLAYER_init(ind);
	
	while(TRUE)
	{   
		
		handleInput();
		SPR_update();
		SYS_doVBlankProcess();
		
	}
	return (0);
}

static void handleInput()
{
	u16 value = JOY_readJoypad(JOY_1);
	PLAYER_handleInput(value);
}
