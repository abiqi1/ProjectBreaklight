#include <genesis.h>
#include "player.h"
#include "level.h"
#include "obstacles.h"
#include "title.h"




enum GAME_STATE
{
	STATE_TITLE,
    STATE_PLAY,
	STATE_PRELOAD,
	STATE_TITLE_MENU
};
enum GAME_STATE currentState;
void basicInit();
void processStatetTitle();
void processStatePlay();
void processStatePreload();
void processTitleMenu();
void joyHandlerMenu(u16 joy, u16 changed, u16 state);

static void handleInput();


int main()
{	

	void basicInit();
	while(1)
    {
        switch(currentState){
            case STATE_TITLE:{
                processStatetTitle();
                break;
            }
			case STATE_TITLE_MENU:{
				processTitleMenu();
			}
			case STATE_PRELOAD:{
				processStatePreload();
				break;
			}
            case STATE_PLAY:{
                processStatePlay();
                break;
            }
        }
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

void basicInit()
{
    JOY_init();
    currentState = STATE_TITLE;
}


void processStatePreload()
{	
	JOY_init();
	u16 playInd;
	playInd = TILE_USER_INDEX;

    playInd += LEVEL_init(playInd);

	while(currentState == STATE_PRELOAD)
	{
		LEVEL_startScroll();
		SYS_doVBlankProcess();
	}


}

void processStatePlay()
{		
		u16 ind;
		ind = TILE_USER_INDEX;
		u16* grace_period;
		grace_period = MEM_alloc(sizeof(*grace_period));

		ind += LEVEL_init(ind);
		ind += PLAYER_init(ind);
		OBSTACLES_counterInit();

		while(currentState == STATE_PLAY)
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
		}
}


void processStatetTitle()
{
	int offset = 0;
	VDP_setPlaneSize(64,64, TRUE);
	VDP_setScrollingMode(HSCROLL_TILE,VSCROLL_PLANE);
	
	u16 ind;
	ind = TILE_USER_INDEX;
	SPR_initEx(980);
	ind += TITLE_init(ind);
	waitTick(1000);

	while(TRUE)
	{
		VDP_setVerticalScroll(BG_B, offset +=1);
		VDP_setVerticalScroll(BG_A, offset +=1);
		SYS_doVBlankProcess();
		if(offset > 287){
			break;
		}
	}

	VDP_drawText("               PRESS START!               ", 30, 52);

	ind += TITLE_sprite_init(ind);

	JOY_setEventHandler(joyHandlerMenu);


	while(currentState == STATE_TITLE)
	{	
		TITLE_startScroll();
		TITLE_moveTaurus();
		TITLE_moveLogo();
		SPR_update();
        SYS_doVBlankProcess();
    }

}

void processTitleMenu()
{	

	// Remove title and taurus before proceeding to menu
    VDP_clearTextLineBG(BG_A, 52);
	waitTick(100);
	while(TRUE)
	{	
		TITLE_startScroll();
		bool exit = TITLE_removeTaurus();
		SPR_update();
        SYS_doVBlankProcess();
		if(exit)
			break;
    }

	TITLE_deleteResources();
	VDP_resetScreen();


	JOY_setEventHandler(TITLE_joyEventHandler); 

	TITLE_setMenuPalette();

	TITLE_goToMainMenu();

	while(currentState == STATE_TITLE_MENU)
	{	
		SPR_update();
        SYS_doVBlankProcess();	
		if(returnToTitle)
		{
		   currentState = STATE_TITLE;
		   returnToTitle = false;
		}
		else if(startGame)
		{	
				VDP_resetScreen();
				VDP_init();
				JOY_reset();
				startGame = false; //reset value
				currentState = STATE_PRELOAD;
		}
	}
}

void joyHandlerMenu(u16 joy, u16 changed, u16 state)
{
    if (joy == JOY_1)
    {
        if (state & BUTTON_START)
        {
			JOY_reset();
            currentState = STATE_TITLE_MENU;
        }
    }
}