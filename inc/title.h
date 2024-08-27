#ifndef _TITLE_H_
#define _TITLE_H_

#include <genesis.h>

extern bool returnToTitle;
extern bool startGame;

u16 TITLE_init(u16 vramIndex);
void TITLE_deleteResources();
void TITLE_moveLogo();
bool TITLE_moveTaurus();
void TITLE_startScroll();
s16 TITLE_sprite_init(s16 vramIndex);

bool TITLE_removeTaurus();

typedef struct {
    u16 x;
    u16 y;
    char label[10];
} Option;

void clearMenuText();
void clearDescriptionText();
void clearGameText();
void drawCursor(u16 x, u16 y);
void clearCursor(u16 x, u16 y);
void flashCursor(u16 x, u16 y);
void TITLE_goToMainMenu();
void TITLE_setMenuPalette();
void handleMainMenu();
void handleCarMenu();
void handleControlsMenu();
void handleSoundMenu();
//void handleGameScreen();
//void handlePauseMenu();
void updateCursorPosition(const Option *options, u8 maxIndex, u8 currentIndex);
void resetToInitialScreen();
void resetCursorPositions();  // Resets cursor positions to their default
void updateSoundMenuCursor();
void updateVolumeText();
void updateMainMenuDescription();
void updateCarDescription();
//void updatePauseMenuDescription();
void TITLE_joyEventHandler(u16 joy, u16 changed, u16 state);


#endif