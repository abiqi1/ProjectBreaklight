
#include "title.h"

#include <genesis.h>
#include "level.h"
#include "resources.h"


#define LOGOX 40
#define LEFT_WHEEL_Y 169
#define RIGHT_WHEEL_Y 167
#define LEFT_WHEEL_OFFSET 26
#define RIGHT_WHEEL_OFFSET 181
#define TAURUSX 256
#define LOGOYTOP -126
#define LOGOYBOTTOM -63
#define offsetValue 63


s16* BgOffsetRoad;

u16 bgBaseTileIndex[2];
Sprite* logoArrayTop[4];
int logoArrayLength = sizeof(logoArrayTop) / sizeof(logoArrayTop[0]);
Sprite* logoArrayBottom[4];
Sprite* taurusArray[4];
int taurusArrayLength = sizeof(taurusArray) / sizeof(taurusArray[0]);
fix32 taurusPosX = FIX32(100);
fix32 taurusPosY = FIX32(100);
Sprite* taurus;

Sprite* left_wheel;
Sprite* right_wheel;

s16 road_tile_ind = 52;

s16 scrollValuesA[28] = {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 };
s16 scrollValuesB[28] = {0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0 };

int rowA = 0;
int rowB = 0;


// --------------------------------------------------------------------

//title stuff


// Define menu option indices
#define NUM_MAIN_OPTIONS 4
#define NUM_CAR_OPTIONS 7
#define NUM_VOLUME_LEVELS 5
#define NUM_PAUSE_OPTIONS 5

// Menu states
typedef enum {
    MAIN_MENU,
    CAR_MENU,
    CONTROLS_MENU,
    SOUND_MENU,
} MenuState;

MenuState currentMenu; 


// Main Menu Options
const Option mainOptions[NUM_MAIN_OPTIONS] = {
    {11, 9, "Play"},
    {11, 10, "Controls"},
    {11, 11, "Sound"},
    {11, 12, "Quit"},
};

// Car Select Menu Options
const Option carOptions[NUM_CAR_OPTIONS] = {
    {11, 9, "Fang"},
    {11, 10, "Armitage"},
    {11, 11, "Blitzer"},
    {11, 12, "Rapide"},
    {11, 13, "Taurus"},
    {11, 14, "Ferossa"},
    {11, 15, "Samurai"},
};

// Sound Menu Volume Levels
const Option volumeLevels[NUM_VOLUME_LEVELS] = {
    {10, 11, "0"},
    {15, 11, "25"},
    {19, 11, "50"},
    {23, 11, "75"},
    {27, 11, "100"},
};

// Global variables for cursor, index, and volume settings
u8 mainMenuIndex = 0;      // Cursor position in the Main Menu
u8 carMenuIndex = 0;       // Cursor position in the Car Menu
u8 soundMenuIndex = 0;     // Cursor position in the Sound Menu
u8 pauseMenuIndex = 0;     // Cursor position in the Pause Menu
u8 currentVolumeMusic = 2; // Default to "50"
u8 currentVolumeFX = 2;    // Default to "50"
u8 isVolumeSelecting = 0;  // Flag to check if we're adjusting volume
u8 selectedVolumeIndex;    // Holds the index of the selected volume level
bool carSelected = FALSE;  // Flag to indicate a car has been selected

bool returnToTitle = false;
bool startGame = false;

Sprite* carSelect;


void TITLE_joyEventHandler(u16 joy, u16 changed, u16 state) {
    if (joy != JOY_1) return;

    switch (currentMenu) {
        // case TITLE_SEQUENCE:
        //     if (state & BUTTON_START) {
        //         clearMenuText();
        //         clearDescriptionText(); // Ensure description text is also cleared
        //         currentMenu = TITLE_SCREEN;  // Transition to the title screen
        //         resetToInitialScreen();
        //     }
        //     break;

        // case TITLE_SCREEN:
        //     if (state & BUTTON_START) {
        //         clearMenuText();
        //         clearDescriptionText(); // Ensure description text is also cleared
        //         currentMenu = MAIN_MENU;  // Transition to the main menu
        //         handleMainMenu();
        //     }
        //     break;

        case MAIN_MENU:
            if (state & BUTTON_DOWN) {
                if (mainMenuIndex < NUM_MAIN_OPTIONS - 1) mainMenuIndex++;
                updateCursorPosition(mainOptions, NUM_MAIN_OPTIONS, mainMenuIndex);
                updateMainMenuDescription();  // Update the description text
            } else if (state & BUTTON_UP) {
                if (mainMenuIndex > 0) mainMenuIndex--;
                updateCursorPosition(mainOptions, NUM_MAIN_OPTIONS, mainMenuIndex);
                updateMainMenuDescription();  // Update the description text
            } else if (state & BUTTON_A) {
                flashCursor(mainOptions[mainMenuIndex].x - 2, mainOptions[mainMenuIndex].y);

                clearMenuText();
                clearDescriptionText(); // Ensure description text is also cleared
                
                switch (mainMenuIndex) {
                    case 0: currentMenu = CAR_MENU; handleCarMenu(); break;
                    case 1: currentMenu = CONTROLS_MENU; handleControlsMenu(); break;
                    case 2: currentMenu = SOUND_MENU; handleSoundMenu(); break;
                    case 3:  // Quit
                        resetCursorPositions();  // Reset all cursor positions
                        clearMenuText();
                        clearDescriptionText(); // Ensure description text is also cleared
                        clearGameText();  // Clear the game screen
                        returnToTitle = TRUE;
                        break;
                }
            }
            break;

        case CAR_MENU:
            if (!carSelected) {
                if (state & BUTTON_DOWN) {
                    if (carMenuIndex < NUM_CAR_OPTIONS - 1) carMenuIndex++;
                    updateCursorPosition(carOptions, NUM_CAR_OPTIONS, carMenuIndex);
                    updateCarDescription();  // Update the car description text
                } else if (state & BUTTON_UP) {
                    if (carMenuIndex > 0) carMenuIndex--;
                    updateCursorPosition(carOptions, NUM_CAR_OPTIONS, carMenuIndex);
                    updateCarDescription();  // Update the car description text
                } else if (state & BUTTON_A) {
                    flashCursor(carOptions[carMenuIndex].x - 2, carOptions[carMenuIndex].y);
                    carSelected = TRUE;  // Set the flag indicating a car has been selected

                    // Display "Start" with "< >" for confirmation
                    VDP_drawText("Start", 25, 15);
                    VDP_drawText(">", 24, 15);
                    VDP_drawText("<", 30, 15);

                    clearCursor(carOptions[carMenuIndex].x - 2, carOptions[carMenuIndex].y);
                } else if (state & BUTTON_B) {
                    clearMenuText();
                    clearDescriptionText(); // Ensure description text is also cleared
                    currentMenu = MAIN_MENU;
                    handleMainMenu();
                }
            } else {
                if (state & BUTTON_START) {
                    clearMenuText();
                    clearDescriptionText();
                    //currentMenu = GAME_SCREEN; // this is where we start the game
                    startGame = TRUE;
                    //handleGameScreen();
                } else if (state & BUTTON_B) {
                    carSelected = FALSE;  // Allow re-selection of a car
                    clearMenuText();
                    clearDescriptionText();
                    handleCarMenu();
                }
            }
            break;

        case CONTROLS_MENU:
            if (state & BUTTON_B) {
                clearMenuText();
                clearDescriptionText(); // Ensure description text is also cleared

                currentMenu = MAIN_MENU;
                handleMainMenu();
                }
            break;

        case SOUND_MENU:
            if (isVolumeSelecting) {
                if (state & BUTTON_LEFT && selectedVolumeIndex > 0) {
                    selectedVolumeIndex--;
                    updateSoundMenuCursor();
                } else if (state & BUTTON_RIGHT && selectedVolumeIndex < NUM_VOLUME_LEVELS - 1) {
                    selectedVolumeIndex++;
                    updateSoundMenuCursor();
                } else if (state & BUTTON_A) {
                    flashCursor(volumeLevels[selectedVolumeIndex].x - 1, soundMenuIndex == 0 ? 11 : 17);
                    if (soundMenuIndex == 0) currentVolumeMusic = selectedVolumeIndex;
                    else if (soundMenuIndex == 1) currentVolumeFX = selectedVolumeIndex;
                    updateVolumeText();
                    isVolumeSelecting = 0;
                    updateSoundMenuCursor();
                } else if (state & BUTTON_B) {
                    isVolumeSelecting = 0;
                    updateSoundMenuCursor();
                }
            } else {
                if (state & BUTTON_DOWN && soundMenuIndex < 1) {
                    soundMenuIndex++;
                    selectedVolumeIndex = currentVolumeFX;
                    updateSoundMenuCursor();
                } else if (state & BUTTON_UP && soundMenuIndex > 0) {
                    soundMenuIndex--;
                    selectedVolumeIndex = currentVolumeMusic;
                    updateSoundMenuCursor();
                } else if (state & BUTTON_A) {
                    flashCursor(8, soundMenuIndex == 0 ? 9 : 15);
                    isVolumeSelecting = 1;
                    updateSoundMenuCursor();
                } else if (state & BUTTON_B) {
                    clearMenuText();
                    clearDescriptionText(); // Ensure description text is also cleared
                    currentMenu = MAIN_MENU;
                    handleMainMenu();
                    }
            }
            break;
    }
}

void TITLE_goToMainMenu() 
{
    clearMenuText();
    clearDescriptionText();
    currentMenu = MAIN_MENU;
    handleMainMenu();
}

void TITLE_setMenuPalette()
{
    PAL_setPalette(PAL0, title_menu.palette->data, DMA); 
    VDP_setTextPalette(PAL1);
    PAL_setColor(31, RGB24_TO_VDPCOLOR(0xFFFFFF));
    PAL_setPalette(PAL2, cars_sheet.palette->data, DMA);

    // spawn car selection sprites and initialise them to be hidden at first
    carSelect = SPR_addSprite(&cars_sheet, 188, 55, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    SPR_setVisibility(carSelect, HIDDEN );
}

// Reset to the initial screen with "Press Start!"
// void resetToInitialScreen() {
//     VDP_drawText("Press Start!", 14, 15);
//     currentMenu = TITLE_SCREEN;
//     carSelected = FALSE;  // Reset car selection
//     //gamePaused = FALSE;   // Ensure game is not paused
//     //counter = 0;          // Reset counter
// }

// Clear all text in the main menu area, expanded to cover an extra line below
void clearMenuText() {
    VDP_clearTextArea(8, 6, 24, 13);
}

// Clear the description area
void clearDescriptionText() {
    VDP_clearTextArea(9, 18, 22, 4);
}

// Clear the game screen
void clearGameText() {
    VDP_clearTextArea(0, 0, 40, 1);  // Clear the car name and counter
}

// Reset all cursor positions to their default
void resetCursorPositions() {
    mainMenuIndex = 0;
    carMenuIndex = 0;
    soundMenuIndex = 0;
    pauseMenuIndex = 0;
}

// Draw cursor at the specified position
void drawCursor(u16 x, u16 y) {
    VDP_drawText(">", x, y);
}

// Clear cursor from the specified position
void clearCursor(u16 x, u16 y) {
    VDP_clearText(x, y, 1);
}

// Flash the cursor at the specified position with a slower effect
void flashCursor(u16 x, u16 y) {
    for (int i = 0; i < 3; i++) {
        clearCursor(x, y);
        SYS_doVBlankProcess();
        SYS_doVBlankProcess(); // Additional wait to slow down the flashing
        drawCursor(x, y);
        SYS_doVBlankProcess();
        SYS_doVBlankProcess(); // Additional wait to slow down the flashing
    }
}

// Update cursor position and clear the previous one
void updateCursorPosition(const Option *options, u8 maxIndex, u8 currentIndex) {
    for (u8 i = 0; i < maxIndex; i++) {
        clearCursor(options[i].x - 2, options[i].y);
    }
    drawCursor(options[currentIndex].x - 2, options[currentIndex].y);
}

// Handle the Main Menu
void handleMainMenu() {
    SPR_setVisibility(carSelect, HIDDEN );
    VDP_drawText("Main Menu", 9, 7);
    VDP_drawImageEx(BG_B, &title_menu, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USER_INDEX ), 8, 6, FALSE, TRUE);
    for (u16 i = 0; i < NUM_MAIN_OPTIONS; i++) {
        VDP_drawText(mainOptions[i].label, mainOptions[i].x, mainOptions[i].y);
    }
    updateCursorPosition(mainOptions, NUM_MAIN_OPTIONS, mainMenuIndex);
    updateMainMenuDescription();  // Initial description
}

// Handle the Car Menu (still titled "Car Select")
void handleCarMenu() {
    VDP_drawText("Car Select", 9, 7);
    VDP_drawImageEx(BG_B, &pause_menu, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USER_INDEX ), 8, 6, FALSE, TRUE);
    for (u16 i = 0; i < NUM_CAR_OPTIONS; i++) {
        VDP_drawText(carOptions[i].label, carOptions[i].x, carOptions[i].y);
    }
    updateCursorPosition(carOptions, NUM_CAR_OPTIONS, carMenuIndex);

    updateCarDescription();  // Initial car description
}

// Handle the Controls Menu
void handleControlsMenu() {
    VDP_drawImageEx(BG_B, &controls_menu, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USER_INDEX ), 8, 6, FALSE, TRUE);
    VDP_drawText("Controls Menu", 9, 7);
    VDP_drawText("Gas", 14, 12);
    VDP_drawText("Brake", 13, 16);
    VDP_drawText("Right", 17, 14);
    VDP_drawText("Left", 10, 14);
    VDP_drawText("Boost", 25, 12);
    VDP_drawText("Start", 21, 10);
    VDP_drawText("Gas", 22, 16);
    VDP_drawText("Brake", 24, 15);
}

// Handle the Sound Menu
void handleSoundMenu() {
    soundMenuIndex = 0;
    isVolumeSelecting = 0;
    selectedVolumeIndex = (soundMenuIndex == 0) ? currentVolumeMusic : currentVolumeFX;

    VDP_drawImageEx(BG_B, &sound_menu, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USER_INDEX ), 8, 6, FALSE, TRUE);
    VDP_drawText("Sound Menu", 9, 7);
    VDP_drawText("Music =", 10, 9);
    VDP_drawText(volumeLevels[currentVolumeMusic].label, 18, 9);
    VDP_drawText("FX =", 10, 15);
    VDP_drawText(volumeLevels[currentVolumeFX].label, 18, 15);

    for (u8 i = 0; i < NUM_VOLUME_LEVELS; i++) {
        VDP_drawText(volumeLevels[i].label, volumeLevels[i].x, 11);
        VDP_drawText(volumeLevels[i].label, volumeLevels[i].x, 17);
    }

    updateSoundMenuCursor();
}

// Update the cursor position in the Sound Menu
void updateSoundMenuCursor() {
    clearCursor(8, 9);
    clearCursor(8, 15);
    for (u8 i = 0; i < NUM_VOLUME_LEVELS; i++) {
        clearCursor(volumeLevels[i].x - 1, 11);
        clearCursor(volumeLevels[i].x - 1, 17);
    }

    if (isVolumeSelecting) {
        drawCursor(volumeLevels[selectedVolumeIndex].x - 1, soundMenuIndex == 0 ? 11 : 17);
    } else {
        drawCursor(8, soundMenuIndex == 0 ? 9 : 15);
    }
}

// Update the displayed volume next to "Music =" or "FX ="
void updateVolumeText() {
    VDP_clearText(18, 9, 4);
    VDP_clearText(18, 15, 4);
    VDP_drawText(volumeLevels[currentVolumeMusic].label, 18, 9);
    VDP_drawText(volumeLevels[currentVolumeFX].label, 18, 15);
}

// Update the description text based on the current main menu selection
void updateMainMenuDescription() {
    clearDescriptionText();  // Clear the previous description

    switch (mainMenuIndex) {
        case 0:  // Play (Car Select)
            VDP_drawText("Live from Channel 4,", 9, 18);
            VDP_drawText("supercar robbery in", 9, 19);
            VDP_drawText("progress!", 9, 20);
            break;
        case 1:  // Controls
            VDP_drawText("Wait, how do you", 9, 18);
            VDP_drawText("drive again?!", 9, 19);
            break;
        case 2:  // Sound
            VDP_drawText("Turn it up!", 9, 18);
            break;
        case 3:  // Quit
            VDP_drawText("Out of gas -", 9, 18);
            VDP_drawText("and time", 9, 19);
            break;
    }
}

// Update the description text based on the current car selection
void updateCarDescription() {
    clearDescriptionText();  // Clear the previous description
    SPR_setVisibility(carSelect, VISIBLE );

    switch (carMenuIndex) {
        case 0:  // Fang
            VDP_drawText("Suspect seen", 9, 18);
            VDP_drawText("fleeing a scrap-", 9, 19);
            VDP_drawText("yard in a shitbox", 9, 20);
            SPR_setAnim(carSelect, 0);
            break;
        case 1:  // Armitage
            VDP_drawText("007 ain't got shit", 9, 18);
            VDP_drawText("on this vigilante!", 9, 19);
            SPR_setAnim(carSelect, 1);
            break;
        case 2:  // Blitzer
            VDP_drawText("German excellence in", 9, 18);
            VDP_drawText("a tight package -", 9, 19);
            VDP_drawText("perfect for gapping!", 9, 20);
            SPR_setAnim(carSelect, 3);
            break;
        case 3:  // Rapide
            VDP_drawText("Romanian sociallite", 9, 18);
            VDP_drawText("robbed in broad", 9, 19);
            VDP_drawText("daylight!", 9, 20);
            SPR_setAnim(carSelect, 2);
            break;
        case 4:  // Taurus
            VDP_drawText("Who needs horses when", 9, 18);
            VDP_drawText("you can have BULLS!", 9, 19);
            SPR_setAnim(carSelect, 4);
            break;
        case 5:  // Ferossa
            VDP_drawText("Sometimes maybe good,", 9, 18);
            VDP_drawText("Sometimes maybe shit!", 9, 19);
            SPR_setAnim(carSelect, 5);
            break;
        case 6:  // Samurai
            VDP_drawText("In Yakuza, Mechanic.", 9, 18);
            VDP_drawText("No.1, Steady Hand!", 9, 19);
            VDP_drawText("Mistake! Car stolen!", 9, 20);
            SPR_setAnim(carSelect, 6);
            break;
    }
}

// --------------------------------------------------------------------




u16 TITLE_init(u16 vramIndex)
{   
    
    PAL_setPalette(PAL0, sky_water.palette->data, DMA);
    PAL_setPalette(PAL1, bridge_road.palette->data, DMA);
    u16 ind;
    ind = vramIndex;

    bgBaseTileIndex[0] = ind;
    VDP_loadTileSet(sky_water.tileset, ind, DMA);
    ind += road_title.tileset->numTile;
    VDP_drawImageEx(BG_B, &sky_water, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, bgBaseTileIndex[0]), -12, 0, FALSE, TRUE);

    bgBaseTileIndex[1] = ind;
    VDP_loadTileSet(bridge_road.tileset, ind, DMA);
    ind += bridge_road.tileset->numTile;
    VDP_drawImageEx(BG_A, &bridge_road, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, bgBaseTileIndex[1]), 0, 0, FALSE, TRUE);

    return ind;
    
}

s16 TITLE_sprite_init(s16 vramIndex)
{
    int logoOffset = LOGOX;
    int taurusOffset = TAURUSX;
    PAL_setPalette(PAL2, logo.palette->data, DMA);
   
    for(int i = 0; i < logoArrayLength; i++, logoOffset += offsetValue, taurusOffset += offsetValue)
    {
        Sprite* title = SPR_addSprite(&logo, logoOffset , LOGOYTOP, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
        logoArrayTop[i] = title;
        SPR_setFrame(logoArrayTop[i], i);

        Sprite* titleB = SPR_addSprite(&logo, logoOffset , LOGOYBOTTOM, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
        logoArrayBottom[i] = titleB;
        SPR_setAnimAndFrame(logoArrayBottom[i], 1, i);

        Sprite* taurus = SPR_addSprite(&taurus_main, taurusOffset, 150, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));  //taurusOffset
        taurusArray[i] = taurus;
        SPR_setAlwaysAtBottom(taurus);
        SPR_setFrame(taurusArray[i], i);
        
    }

    int car_0 = SPR_getPositionX(taurusArray[0]);
    left_wheel = SPR_addSprite(&wheels, car_0 + LEFT_WHEEL_OFFSET, LEFT_WHEEL_Y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE)); 
    SPR_setAlwaysOnTop(left_wheel);

    right_wheel = SPR_addSprite(&wheels, car_0 + RIGHT_WHEEL_OFFSET, RIGHT_WHEEL_Y, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    SPR_setAlwaysOnTop(right_wheel); 

    return vramIndex;
}

bool TITLE_moveTaurus()
{
    s16 x_taurus = SPR_getPositionX(taurusArray[0]);

    if( x_taurus > (s16)40 ) // && y_bottom_check < (s16)68
    {
        for(int i = 0; i < taurusArrayLength ; i++)
        {   
            s16 x_taurus = SPR_getPositionX(taurusArray[i]) - 5;
            s16 y_taurus = SPR_getPositionY(taurusArray[i]);


            SPR_setPosition(taurusArray[i], x_taurus, y_taurus);
        }
        s16 left_wheel_x = SPR_getPositionX(left_wheel) - 5;
        s16 left_wheel_y = SPR_getPositionY(left_wheel);
        s16 right_wheel_x = SPR_getPositionX(right_wheel) - 5;
        s16 right_wheel_y = SPR_getPositionY(right_wheel);

        SPR_setPosition(left_wheel, left_wheel_x, left_wheel_y);
        SPR_setPosition(right_wheel, right_wheel_x, right_wheel_y);
    }
    else
    {   
        return FALSE;
    }

    return TRUE;
}


bool TITLE_removeTaurus()
{
    s16 x_taurus = SPR_getPositionX(taurusArray[0]);


    for(int i = 0; i < taurusArrayLength ; i++)
    {  
    s16 x_taurus = SPR_getPositionX(taurusArray[i]) - 10;
    s16 y_taurus = SPR_getPositionY(taurusArray[i]);
    SPR_setPosition(taurusArray[i], x_taurus, y_taurus);
    }
    
    s16 left_wheel_x = SPR_getPositionX(left_wheel) - 10;
    s16 left_wheel_y = SPR_getPositionY(left_wheel);
    s16 right_wheel_x = SPR_getPositionX(right_wheel) - 10;
    s16 right_wheel_y = SPR_getPositionY(right_wheel);

    SPR_setPosition(left_wheel, left_wheel_x, left_wheel_y);
    SPR_setPosition(right_wheel, right_wheel_x, right_wheel_y);
    
    if(x_taurus < -180)
    {
        for(int i = 0; i < taurusArrayLength; i++)
        {
            SPR_releaseSprite(taurusArray[i]);
        }
        
        SPR_releaseSprite(left_wheel);
        SPR_releaseSprite(right_wheel);

        return TRUE;
    }

    return FALSE;

}


void TITLE_moveLogo()
{   

    s16 y_top_check = SPR_getPositionY(logoArrayTop[0]);

    if( y_top_check < (s16)10 ) // && y_bottom_check < (s16)68
    {
        for(int i = 0; i < logoArrayLength; i++)
        {   
            s16 x_pos_top = SPR_getPositionX(logoArrayTop[i]);
            s16 y_pos_top = SPR_getPositionY(logoArrayTop[i]) + 3;
            s16 x_pos_bottom = SPR_getPositionX(logoArrayBottom[i]);
            s16 y_pos_bottom = SPR_getPositionY(logoArrayBottom[i]) + 3;
            SPR_setPosition(logoArrayTop[i], x_pos_top, y_pos_top);
            SPR_setPosition(logoArrayBottom[i], x_pos_bottom, y_pos_bottom);
        }
    }

}

void TITLE_deleteResources()
{
    for(int i = 0; i < logoArrayLength; i++)
    {
        SPR_releaseSprite(logoArrayTop[i]);
        SPR_releaseSprite(logoArrayBottom[i]);
    }

}

void TITLE_startScroll()
{

    VDP_setHorizontalScrollTile(BG_A, 0, scrollValuesA, 28, CPU );

    for(rowA = 0; rowA < 28; rowA++)
    {
        if(rowA <= 16 )
        {
            scrollValuesA[rowA] = scrollValuesA[rowA] + 1;
        }
        else
        {
            scrollValuesA[rowA] = scrollValuesA[rowA] + 5;
        }
        //if(scrollValuesA[rowA] >= 256) scrollValuesA[rowA] = 0;

    }

    VDP_setHorizontalScrollTile(BG_B, 0, scrollValuesB, 28, CPU );
    for(rowB = 0; rowB < 28; rowB++)
    {
        if(rowB <= 15)
        {
            scrollValuesB[rowB] = scrollValuesB[rowB] + 1;
        }
        else if(rowB > 15 && rowB <= 18)
        {
            scrollValuesB[rowB] = scrollValuesB[rowB] + 2;
        }
        else
        {
            scrollValuesB[rowB] = scrollValuesB[rowB] + 3;
        }
        //if(scrollValuesB[rowB] >= 256) scrollValuesB[rowB] = 0;

    }
    
}