#include <genesis.h>
#include "player.h"
#include "level.h"

bool main_game_loop = TRUE;
u16 ani_timer = 1;
static void handleInput();
static void check_game_loop();


int main()
{	
	u16 ind;
	ind = TILE_USERINDEX;

	SPR_init();
	ind += LEVEL_init(ind);
	ind += PLAYER_init(ind);

	
	
	while(main_game_loop)
	{   
		startScroll();
		handleInput();
		PLAYER_handleReflectionAnim();
		SPR_update();
		SYS_doVBlankProcess();
		//check_game_loop();
	}

	//free used mem
	MEM_free(levelSpeed);
	MEM_free(player_health);

	return (0);
}

static void handleInput()
{
	u16 value = JOY_readJoypad(JOY_1);
	PLAYER_handleInput(value);
}

static void check_game_loop()
{
	
	if ( PLAYER_checkHealth(player_health) != TRUE )
	{
		main_game_loop = FALSE;
	}
	
}