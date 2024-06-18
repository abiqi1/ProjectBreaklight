#include <genesis.h>
#include "player.h"
#include "level.h"
#include "obstacles.h"




bool main_game_loop = TRUE;



static void handleInput();
static void check_game_loop();


int main()
{	
	u16 ind;
	ind = TILE_USER_INDEX;
	u16* grace_period;
	grace_period = MEM_alloc(sizeof(*grace_period));

	SPR_init();
	ind += LEVEL_init(ind);
	ind += PLAYER_init(ind);
	OBSTACLES_counterInit();

	
	
	while(main_game_loop)
	{   
		grace_period--;
		LEVEL_startScroll();
		handleInput();
		PLAYER_handleReflectionAnim();
		ind += OBSTACLES_generator(ind);
		OBSTACLES_handleMovement();
		OBSTACLES_handleCollision(grace_period);
		OBSTACLES_deleteFromList();
		SPR_update();
		SYS_doVBlankProcess();
		check_game_loop();
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
	if ( !PLAYER_checkHealth(player_health) )
	{
		main_game_loop = FALSE;
	}
}