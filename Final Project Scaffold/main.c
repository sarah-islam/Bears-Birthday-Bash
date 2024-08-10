#include "gba.h"
#include "mode0.h"
#include "sprites.h"
#include "print.h"
#include "game.h"
#include "digitalSound.h"
#include "nekoatsumesong.h"
#include "throw.h"

#include "spritesheet.h"
#include "startscreen.h"
#include "instscreen.h"
#include "histscreen.h"
#include "fishinstscreen.h"
#include "forageinstscreen.h"
#include "pausedscreen.h"
#include "minigamelose.h"

#include "wintileset.h"
#include "wintilesetmap.h"
#include "balloonmap.h"

#include "fishscreentileset.h"
#include "fishscreentilesetmap.h"
#include "cloudmap.h"

#include "outdoortileset.h"
#include "foresttilemap.h"
#include "forestcollisionmap2.h"
#include "mainmaptilemap.h"
#include "mainmapcollisionmap.h"

#include "housetileset.h"
#include "starthousetilemap.h"
#include "starthousecollisionmap.h"
#include "endhousetilemap.h"

unsigned short oldButtons;
unsigned short buttons;

void initialize();

// state function prototypes
void start();
void mainInstructions();
void history();
void game(); // start in house
void mainGame(); // on main map
void fishInstructions();
void fishGame();
void forageInstructions();
void forageGame();
void fishPause();
void foragePause();
void fishWin();
void forageWin();
void fishLose();
void forageLose();
void end(); // end in decorated house

void goToStart();
void goToMainInstructions();
void goToHistory();
void goToGame();
void goToMainGame();
void goToFishInstructions();
void goToFishGame();
void goToForageInstructions();
void goToForageGame();
void goToFishPause();
void goToForagePause();
void goToFishWin();
void goToForageWin();
void goToFishLose();
void goToForageLose();
void goToEnd();

// game function prototypes
void initGame();
void updateGame();
void drawGame();

void initMainGame();
void updateMainGame();
void drawMainGame();

void initFishGame();
void updateFishGame();
void drawFishGame();

void initForageGame();
void updateForageGame();
void drawForageGame();

void initEnd();
void updateEnd();
void drawEnd();

// for win screen balloons
void updateVOff();
void drawBalloons();

// interrupt prototypes (sound, fish game timer)
void setUpInterrupts();
void interruptHandler();

// for balloons
int hOff2;
int vOff2;

// for fish game clouds
int hOff3;
int vOff3;

// for fish game lake
int hOff4;
int vOff4;

// for button in start screen
void initStartButton();
void updateStartButton();
void drawStartButton();

// for history score
void initHistory();
void updateHistory();
void drawHistory();






// SAVING

#define GAMEPAK_RAM  ((u8*)0x0E000000)
void saveToCartridgeRAM();
void loadFromCartridgeRAM();
void saveGame();
void loadGame();
typedef struct {
  int numGamesPlayed;
  int hasSavedBefore;
} SAVE_DATA;
SAVE_DATA saveData;
void saveToCartridgeRAM() {
    u8* saveDataAsCharPointer = &saveData;
    for (unsigned int i = 0; i < sizeof(SAVE_DATA); i++) {
        GAMEPAK_RAM[i] = saveDataAsCharPointer[i];
    }
}
void loadFromCartridgeRAM() {
    u8* saveDataAsCharPointer = &saveData;
    for (unsigned int i = 0; i < sizeof(SAVE_DATA); i++) {
        saveDataAsCharPointer[i] = GAMEPAK_RAM[i];
    }
}
void saveGame() {
    saveData.numGamesPlayed = numGamesPlayed;
    saveToCartridgeRAM();
}
void loadGame() {
    loadFromCartridgeRAM();
    if (saveData.hasSavedBefore == -1) {
        saveData.numGamesPlayed = 0;
        saveData.hasSavedBefore = 1;
    }
    numGamesPlayed = saveData.numGamesPlayed; 
}














// state enum
enum STATE {
    START,
    MAIN_INSTRUCTIONS,
    HISTORY,
    GAME,
    MAIN_GAME,
    FISH_INSTRUCTIONS,
    FISH_GAME,
    FORAGE_INSTRUCTIONS,
    FORAGE_GAME,
    FISH_PAUSE,
    FORAGE_PAUSE,
    FISH_WIN,
    FORAGE_WIN,
    FISH_LOSE,
    FORAGE_LOSE,
    END
} state;


int main() {
    initialize();
    mgba_open();
    
    while (1) {
        // update buttons
        oldButtons = buttons;
        buttons = REG_BUTTONS;

        // check saving is working
        mgba_printf("num games played: %d", numGamesPlayed);

        // state machine
        switch (state) {
            case START:
                REG_DISPCTL &= ~BG_ENABLE(1);
                start();
                break;
            case MAIN_INSTRUCTIONS:
                REG_DISPCTL &= ~BG_ENABLE(1);
                mainInstructions();
                break;
            case HISTORY:
                REG_DISPCTL &= ~BG_ENABLE(1);
                history();
                break;
            case GAME:
                REG_DISPCTL &= ~BG_ENABLE(1);
                game();
                break;
            case MAIN_GAME:
                REG_DISPCTL &= ~BG_ENABLE(1);
                mainGame();
                break;
            case FISH_INSTRUCTIONS:
                REG_DISPCTL &= ~BG_ENABLE(1);
                fishInstructions();
                break;
            case FISH_GAME:
                REG_DISPCTL |= BG_ENABLE(1);
                REG_BG1CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(10) | BG_SIZE_SMALL;
                fishGame();
                break;
            case FORAGE_INSTRUCTIONS:
            REG_DISPCTL &= ~BG_ENABLE(1);
                forageInstructions();
                break;
            case FORAGE_GAME:
            REG_DISPCTL &= ~BG_ENABLE(1);
                forageGame();

                break;
            case FISH_PAUSE:
                REG_DISPCTL &= ~BG_ENABLE(1);
                fishPause();
                break;
            case FORAGE_PAUSE:
            REG_DISPCTL &= ~BG_ENABLE(1);
                foragePause();
                break;
            case FISH_WIN:
                REG_DISPCTL |= BG_ENABLE(1);
                REG_BG1CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(10) | BG_SIZE_SMALL;
                fishWin();
                break;
            case FORAGE_WIN:
                REG_DISPCTL |= BG_ENABLE(1);
                REG_BG1CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(10) | BG_SIZE_SMALL;
                forageWin();
                break;
            case FISH_LOSE:
                REG_DISPCTL &= ~BG_ENABLE(1);
                fishLose();
                break;
            case FORAGE_LOSE:
                REG_DISPCTL &= ~BG_ENABLE(1);
                forageLose();
                break;
            case END:
                REG_DISPCTL &= ~BG_ENABLE(1);
                end();
                break;
        }
        if (state == FORAGE_GAME) {
            REG_BG0HOFF = hOff;
            REG_BG0VOFF = vOff;

        } else if (state == FISH_WIN || state == FORAGE_WIN) {
            REG_BG1HOFF = hOff2;
            REG_BG1VOFF = vOff2;
            REG_BG0HOFF = 0;
            REG_BG0VOFF = 0;

        } else if (state == FISH_GAME) {
            REG_BG0HOFF = hOff4;
            REG_BG0VOFF = vOff4;
            REG_BG1HOFF = hOff3;
            REG_BG1VOFF = vOff3;

        } else {
            REG_BG0HOFF = 0;
            REG_BG0VOFF = 0;
            REG_BG1HOFF = 0;
            REG_BG1VOFF = 0;
        }
    }   
}
void initialize() {
    // set up display control and bg 0
    REG_DISPCTL = MODE(0) | BG_ENABLE(0) | SPRITE_ENABLE;
    REG_BG0CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(8) | BG_SIZE_SMALL | 1;
    // bg 1 starts off
    REG_BG1CNT = 0;

    // spritesheet
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen/2);
    DMANow(3, spritesheetPal, SPRITE_PALETTE, spritesheetPalLen/2);

    // set up sound and interrupts
    setupSounds();
    setUpInterrupts();
    playSoundA(nekoatsumesong_data, nekoatsumesong_length, 1); // looping sound
    
    loadGame();
    goToStart();
}


// START:
void goToStart() {
    initStartButton();
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    state = START;
}
void start() {
    updateStartButton();
    drawStartButton();
    waitForVBlank();
    DMANow(3, &shadowOAM, OAM, 512);
    if (buttonL.x == 56 && buttonL.y == 48 && buttonR.x == 120 && buttonR.y == 48 && BUTTON_PRESSED(BUTTON_A)) {
        goToGame();
    }
    if (buttonL.x == 56 && buttonL.y == 80 && buttonR.x == 120 && buttonR.y == 80 && BUTTON_PRESSED(BUTTON_A)) {
        goToMainInstructions();
    }
    if (buttonL.x == 56 && buttonL.y == 112 && buttonR.x == 120 && buttonR.y == 112 && BUTTON_PRESSED(BUTTON_A)) {
        goToHistory();
    }
}


// MAIN INSTRUCTIONS:
void goToMainInstructions() {
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);
    // main instructions screen
    DMANow(3, instscreenMap, &SCREENBLOCK[8], instscreenMapLen / 2);
    DMANow(3, instscreenPal, BG_PALETTE, instscreenPalLen / 2);
    DMANow(3, instscreenTiles, &CHARBLOCK[0], instscreenTilesLen / 2);
    state = MAIN_INSTRUCTIONS;
}
void mainInstructions() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_B)) {
        goToStart();
    }
}

// HISTORY:
void goToHistory() {
    initHistory();
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);
    state = HISTORY;
}
void history() {
    updateHistory();
    drawHistory();
    waitForVBlank();
    DMANow(3, &shadowOAM, OAM, 512);
    
    if (BUTTON_PRESSED(BUTTON_B)) {
        goToStart();
    }
}


// GAME:
void goToGame() {
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);
    initGame();
    state = GAME;
}
void game() {
    updateGame();
    drawGame();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
    // if player is heading out the door, go to main game
    if (player.x > 175 && player.y < 45) {
        goToMainGame();
        initMainGame();
    }
}


// MAIN GAME:
void goToMainGame() {
    state = MAIN_GAME;
}
void mainGame() {
    updateMainGame();
    drawMainGame();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
    // if lake considered hit and fish game not completed, go to fish instructions
    if (lakeHit == 1 && !fishGameCompleted) {
        goToFishInstructions();
    }
    // if forest considered hit and forage game not completed, go to forage instructions
    if (forestHit == 1 && !forageGameCompleted) {
        goToForageInstructions();
    }

    // if house considered hit and both mini games completed, go to end
    if (houseHit == 1 && numTasksComplete == 2) {
        goToEnd();
        numGamesPlayed++;
    }
}


// FISH INSTRUCTIONS:
void goToFishInstructions() {
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    // fish instructions screen
    DMANow(3, fishinstscreenPal, BG_PALETTE, fishinstscreenPalLen / 2);
    DMANow(3, fishinstscreenTiles, &CHARBLOCK[0], fishinstscreenTilesLen / 2);
    DMANow(3, fishinstscreenMap, &SCREENBLOCK[8], fishinstscreenMapLen / 2);
    state = FISH_INSTRUCTIONS;
}
void fishInstructions() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_A)) {
        goToFishGame();
        initFishGame();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}


// FORAGE INSTRUCTIONS:
void goToForageInstructions() {
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    // forage instructions screen
    DMANow(3, forageinstscreenPal, BG_PALETTE, forageinstscreenPalLen / 2);
    DMANow(3, forageinstscreenTiles, &CHARBLOCK[0], forageinstscreenTilesLen / 2);
    DMANow(3, forageinstscreenMap, &SCREENBLOCK[8], forageinstscreenMapLen / 2);
    state = FORAGE_INSTRUCTIONS;
}
void forageInstructions() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_A)) {
        goToForageGame();
        initForageGame();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}


// FISH GAME:
void goToFishGame() {
    paused = 0;
    hideSprites();
    state = FISH_GAME;
}
void fishGame() {
    updateFishGame();

    // if paused, set timers off
    if (paused) {
        REG_TM2CNT = 0;
        REG_TM3CNT = 0;
    } else {
        // set up timer 2
        REG_TM2D = 65536 - 65536;
        REG_TM2CNT = TIMER_ON | TM_IRQ | TM_FREQ_256;

        // set up timer 3
        REG_TM3D = 65536 - 60;
        REG_TM3CNT = TIMER_ON | TM_IRQ | TM_CASCADE;
    }

    drawFishGame();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToFishPause();
    }

    // win or lose based on score and seconds
    if (fishScore == 15 && !fishGameCompleted) {
        goToFishWin();
        fishGameCompleted = 1;
        numTasksComplete++;
    }
    if (seconds == 59 && !(fishScore == 15)) {
        goToFishLose();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}


// FORAGE GAME:
void goToForageGame() {
    hideSprites();
    state = FORAGE_GAME;
}
void forageGame() {
    updateForageGame();
    drawForageGame();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToForagePause();
    }
    // win or lose based on score and if caught by wolf
    if (forageScore == 20 && !forageGameCompleted) {
        goToForageWin();
        forageGameCompleted = 1;
        numTasksComplete++;
    }
    if (collision(wolf.x, wolf.y, wolf.width, wolf.height, playerF.x, playerF.y, playerF.width, playerF.height)) {
        goToForageLose();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}


// FISH PAUSE:
void goToFishPause() {
    // timer is paused
    paused = 1;
    // set timer 2 off
    if (paused) {
        REG_TM2CNT = 0;
    } else {
        // set up timer 2
        REG_TM2D = 65536 - 65536;
        REG_TM2CNT = TIMER_ON | TM_IRQ | TM_FREQ_256;

        // set up timer 3
        REG_TM3D = 65536 - 60;
        REG_TM3CNT = TIMER_ON | TM_IRQ | TM_CASCADE;
    }
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    // fish pause screen
    DMANow(3, pausedscreenPal, BG_PALETTE, pausedscreenPalLen / 2);
    DMANow(3, pausedscreenTiles, &CHARBLOCK[0], pausedscreenTilesLen / 2);
    DMANow(3, pausedscreenMap, &SCREENBLOCK[8], pausedscreenMapLen / 2);
    state = FISH_PAUSE;
}
void fishPause() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToFishGame();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}


// FORAGE PAUSE:
void goToForagePause() {
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    // forage pause screen
    DMANow(3, pausedscreenPal, BG_PALETTE, pausedscreenPalLen / 2);
    DMANow(3, pausedscreenTiles, &CHARBLOCK[0], pausedscreenTilesLen / 2);
    DMANow(3, pausedscreenMap, &SCREENBLOCK[8], pausedscreenMapLen / 2);
    state = FORAGE_PAUSE;
}
void foragePause() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToForageGame();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}


// FISH WIN:
void goToFishWin() {
    // reset clouds and lake offsets
    hOff3 = 0;
    vOff3 = 0;
    hOff4 = 0;
    vOff4 = 0;

    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    // fish win screen
    DMANow(3, wintilesetPal, BG_PALETTE, wintilesetPalLen / 2);
    DMANow(3, wintilesetTiles, &CHARBLOCK[0], wintilesetTilesLen / 2);
    DMANow(3, wintilesetmapMap, &SCREENBLOCK[8], wintilesetmapMapLen / 2);
    
    // add balloons map in bg 1
    DMANow(3, balloonmapMap, &SCREENBLOCK[10], balloonmapMapLen / 2);

    // reset balloons offsets
    vOff2 = 0;
    hOff2 = 0;
    
    state = FISH_WIN;
}
void fishWin() {
    updateVOff();
    drawBalloons();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToMainGame();
    }
}


// FORAGE WIN:
void goToForageWin() {
    // reset forage game offsets
    hOff = 0;
    vOff = 0;

    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    // forage win screen
    DMANow(3, wintilesetPal, BG_PALETTE, wintilesetPalLen / 2);
    DMANow(3, wintilesetTiles, &CHARBLOCK[0], wintilesetTilesLen / 2);
    DMANow(3, wintilesetmapMap, &SCREENBLOCK[8], wintilesetmapMapLen / 2);
    
    // add balloons map in bg 1
    DMANow(3, balloonmapMap, &SCREENBLOCK[10], balloonmapMapLen / 2);

    // reset balloons offsets
    vOff2 = 0;
    hOff2 = 0;

    state = FORAGE_WIN;
}
void forageWin() {
    updateVOff();
    drawBalloons();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToMainGame();
    }
}


// FISH LOSE:
void goToFishLose() {
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    // fish lose screen
    DMANow(3, minigamelosePal, BG_PALETTE, minigamelosePalLen / 2);
    DMANow(3, minigameloseTiles, &CHARBLOCK[0], minigameloseTilesLen / 2);
    DMANow(3, minigameloseMap, &SCREENBLOCK[8], minigameloseMapLen / 2);
    state = FISH_LOSE;
}
void fishLose() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToFishGame();
        initFishGame();
    }
}


// FORAGE LOSE:
void goToForageLose() {
    hideSprites();
    DMANow(3, &shadowOAM, OAM, 512);

    // forage lose screen
    DMANow(3, minigamelosePal, BG_PALETTE, minigamelosePalLen / 2);
    DMANow(3, minigameloseTiles, &CHARBLOCK[0], minigameloseTilesLen / 2);
    DMANow(3, minigameloseMap, &SCREENBLOCK[8], minigameloseMapLen / 2);
    state = FORAGE_LOSE;
}
void forageLose() {
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToForageGame();
        initForageGame();
    }
}


// END:
void goToEnd() {
    hideSprites();
    initEnd();
    state = END;
}
void end() {
    updateEnd();
    drawEnd();
    waitForVBlank();
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}








/////////////////// GAME LOGIC //////////////////////









// game (starting house) functions
void initGame() {
    // reset all variables when game starts
    fishScore = 0;
    forageScore = 0;
    fishGameCompleted = 0;
    forageGameCompleted = 0;
    numTasksComplete = 0;
    doorHit = 0;
    houseHit = 0;
    forestHit = 0;
    lakeHit = 0;
    wolfStopped = 0;
    seconds = 0;
    minutes = 0;
    paused = 0;

    // house map
    DMANow(3, housetilesetPal, BG_PALETTE, housetilesetPalLen / 2);
    DMANow(3, housetilesetTiles, &CHARBLOCK[0], housetilesetTilesLen / 2);
    DMANow(3, starthousetilemapMap, &SCREENBLOCK[8], starthousetilemapMapLen / 2);

    // initialize sprites
    initPlayer();
    initTextBubble();
}
void updateGame() {
    // update sprites
    updatePlayer();
    updateTextBubble();

    // if door is considered hit, tile modification for door to open
    if (doorHit == 1) {
        SCREENBLOCK[8].tilemap[OFFSET(23, 5, 32)] = TILEMAP_ENTRY_TILEID(107);
        SCREENBLOCK[8].tilemap[OFFSET(24, 5, 32)] = TILEMAP_ENTRY_TILEID(108);
        SCREENBLOCK[8].tilemap[OFFSET(25, 5, 32)] = TILEMAP_ENTRY_TILEID(109);
        SCREENBLOCK[8].tilemap[OFFSET(23, 6, 32)] = TILEMAP_ENTRY_TILEID(127);
        SCREENBLOCK[8].tilemap[OFFSET(24, 6, 32)] = TILEMAP_ENTRY_TILEID(128);
        SCREENBLOCK[8].tilemap[OFFSET(25, 6, 32)] = TILEMAP_ENTRY_TILEID(129);
        SCREENBLOCK[8].tilemap[OFFSET(23, 7, 32)] = TILEMAP_ENTRY_TILEID(147);
        SCREENBLOCK[8].tilemap[OFFSET(24, 7, 32)] = TILEMAP_ENTRY_TILEID(148);
        SCREENBLOCK[8].tilemap[OFFSET(25, 7, 32)] = TILEMAP_ENTRY_TILEID(149);
    }  
}
void drawGame() {
    // draw sprites
    drawPlayer();
    drawTextBubble();
    DMANow(3, shadowOAM, OAM, 128 * 4);
}

// main game (map) functions
void initMainGame() {
    // main map
    DMANow(3, outdoortilesetPal, BG_PALETTE, outdoortilesetPalLen / 2);
    DMANow(3, outdoortilesetTiles, &CHARBLOCK[0], outdoortilesetTilesLen / 2);
    DMANow(3, mainmaptilemapMap, &SCREENBLOCK[8], mainmaptilemapMapLen / 2);
    // initialize player on main map
    initPlayerM();
    initTextBubble2();
    initTextBubble3();
}
void updateMainGame() {
    // main map after state changes
    DMANow(3, outdoortilesetPal, BG_PALETTE, outdoortilesetPalLen / 2);
    DMANow(3, outdoortilesetTiles, &CHARBLOCK[0], outdoortilesetTilesLen / 2);
    DMANow(3, mainmaptilemapMap, &SCREENBLOCK[8], mainmaptilemapMapLen / 2);
    // update player on main map
    updatePlayerM();
    updateTextBubble2();
    updateTextBubble3();
}
void drawMainGame() {
    // draw player on main map
    drawPlayerM();
    drawTextBubble2();
    drawTextBubble3();
    DMANow(3, shadowOAM, OAM, 128 * 4);
}

// fish game functions
void initFishGame() {
    // reset fish game variables
    REG_TM2CNT = 0;
    REG_TM3CNT = 0;

    minutes = 0;
    seconds = 0;
    fishScore = 0;
    fishGameCompleted = 0;
    hOff3 = 0;
    vOff3 = 0;
    vOff4 = 0;
    hOff4 = 0;

    // load fish game screen
    DMANow(3, fishscreentilesetPal, BG_PALETTE, fishscreentilesetPalLen / 2);
    DMANow(3, fishscreentilesetTiles, &CHARBLOCK[0], fishscreentilesetTilesLen / 2);
    DMANow(3, fishscreentilesetmapMap, &SCREENBLOCK[8], fishscreentilesetmapMapLen / 2);

    // add clouds map in bg 1
    DMANow(3, cloudmapMap, &SCREENBLOCK[10], cloudmapMapLen / 2);

    // initialize fish game components
    initTime();
    initFish();
    initMisc();
    initHook();
    initHookAttach();
    initScore();
}
void updateFishGame() {
    // load fish game screen after state changes
    DMANow(3, fishscreentilesetPal, BG_PALETTE, fishscreentilesetPalLen / 2);
    DMANow(3, fishscreentilesetTiles, &CHARBLOCK[0], fishscreentilesetTilesLen / 2);
    DMANow(3, fishscreentilesetmapMap, &SCREENBLOCK[8], fishscreentilesetmapMapLen / 2);

    // add clouds map in bg 1
    DMANow(3, cloudmapMap, &SCREENBLOCK[10], cloudmapMapLen / 2);

    // update offset for clouds and lake
    hOff4 += 1;
    hOff3 -= 2;

    // update fish game components
    updateTime();
    updateFish();
    updateMisc();
    updateHook();
    updateHookAttach();
    updateScore();

    mgba_printf("seconds: %d", seconds);
    mgba_printf("minutes: %d", minutes);
}
void drawFishGame() {
    // draw fish game components
    drawFish();
    drawMisc();
    drawHook();
    drawHookAttach();
    drawScore();

    // set bg offset registers
    REG_BG0HOFF = hOff4;
    REG_BG1HOFF = hOff3;

    DMANow(3, shadowOAM, OAM, 128 * 4);
}

// forage game functions
void initForageGame() {
    // reset forage game variables
    vOff = 0;
    hOff = 0;
    forageScore = 0;
    forageGameCompleted = 0;
    wolfStopped = 0;

    // load forage game screen
    DMANow(3, outdoortilesetPal, BG_PALETTE, outdoortilesetPalLen / 2);
    DMANow(3, outdoortilesetTiles, &CHARBLOCK[0], outdoortilesetTilesLen / 2);
    DMANow(3, foresttilemapMap, &SCREENBLOCK[8], foresttilemapMapLen / 2);

    // initialize forage game components
    initBerriesNuts();
    initPlayerForage();
    initWolf();
    initBait();
    initScore2();
}
void updateForageGame() {
    // load forage game screen after state changes
    DMANow(3, outdoortilesetPal, BG_PALETTE, outdoortilesetPalLen / 2);
    DMANow(3, outdoortilesetTiles, &CHARBLOCK[0], outdoortilesetTilesLen / 2);
    DMANow(3, foresttilemapMap, &SCREENBLOCK[8], foresttilemapMapLen / 2);

    // update forage game components
    updateBerriesNuts();
    updatePlayerForage();
    updateWolf();
    updateBait();
    updateScore2();

    // if player throws bait, play throw sound
    if (BUTTON_PRESSED(BUTTON_B) && wolfStopped == 0) {
        playSoundB(throw_data, throw_length, 0); // simultaneous non-looping sound
        wolfStopped = 1;
    }
}
void drawForageGame() {
    // draw forage game components
    drawBerriesNuts();
    drawPlayerForage();
    drawWolf();
    drawBait();
    drawScore2();
    DMANow(3, shadowOAM, OAM, 128 * 4);
}

// end (ending house) functions
void initEnd() {
    // house map
    DMANow(3, housetilesetPal, BG_PALETTE, housetilesetPalLen / 2);
    DMANow(3, housetilesetTiles, &CHARBLOCK[0], housetilesetTilesLen / 2);
    DMANow(3, endhousetilemapMap, &SCREENBLOCK[8], endhousetilemapMapLen / 2);

    // initialize sprites
    initPlayerE();
    initBella();
    initTextBubble4();
    initTextBubble5();
}
void updateEnd() {
    // update sprites
    updatePlayerE();
    updateBella();
    updateTextBubble4();
    updateTextBubble5();
    saveGame();
}
void drawEnd() {
    // draw sprites
    drawPlayerE();
    drawBella();
    drawTextBubble4();
    drawTextBubble5();
    DMANow(3, shadowOAM, OAM, 128 * 4);
}

// for win screen balloons
void updateVOff() {
    vOff2 += 2;
}
void drawBalloons() {
    REG_BG1VOFF = vOff2;
}










/////////////////// INTERRUPT LOGIC //////////////////////














// timer logic (for fish game)
void setUpInterrupts() {
    // set up timer 2 and 3 and vblank
    REG_IME = 0;
    REG_IE = IRQ_TIMER(2) | IRQ_TIMER(3) | IRQ_VBLANK;
    REG_DISPSTAT = DISPSTAT_VBLANK_IRQ;
    REG_INTERRUPT = interruptHandler;
    REG_IME = 1;

}
void interruptHandler() {
    // turn off interrupt
    REG_IME = 0;
    // only in fish game: timers
    if (state == FISH_GAME || state == FISH_PAUSE) {
        if (REG_IF & IRQ_TIMER(2)) {
            seconds++;
            seconds %= 60;
        }
        if (REG_IF & IRQ_TIMER(3)) {
            minutes++;
            minutes %= 60;
            if (minutes == 1) {
                REG_TM2CNT = 0;
                REG_TM3CNT = 0;
            }
        }

    }
    // for entire game: sounds
    if ((REG_IF & IRQ_VBLANK)) {
        if (soundA.isPlaying) {
            soundA.vBlankCount++;
            // check if sound done
            if (soundA.vBlankCount >= soundA.durationInVBlanks) {
                if (soundA.looping) {
                    playSoundA(soundA.data, soundA.dataLength, soundA.looping);
                }
            }
        }
        if (soundB.isPlaying) {
            soundB.vBlankCount++;
            // check if sound done
            if (soundB.vBlankCount >= soundB.durationInVBlanks) {
                if (soundB.looping) {
                    playSoundB(soundB.data, soundB.dataLength, soundB.looping);
                } else {
                    soundB.isPlaying = 0;
                    dma[2].cnt = 0;
                }
            }
        }
    }
    // reset interrupt
    REG_IF = REG_IF;
    REG_IME = 1;
    
}





/////////////////// START SCREEN BUTTON / HIST LOGIC //////////////////////





void initStartButton() {
    DMANow(3, startscreenPal, BG_PALETTE, startscreenPalLen / 2);
    DMANow(3, startscreenTiles, &CHARBLOCK[0], startscreenTilesLen / 2);
    DMANow(3, startscreenMap, &SCREENBLOCK[8], startscreenMapLen / 2);
    initButtonL();
    initButtonR();
}

void updateStartButton() {
    updateButtonL();
    updateButtonR();
}

void drawStartButton() {
    drawButtonL();
    drawButtonR();
}

void initHistory() {
    DMANow(3, histscreenPal, BG_PALETTE, histscreenPalLen / 2);
    DMANow(3, histscreenTiles, &CHARBLOCK[0], histscreenTilesLen / 2);
    DMANow(3, histscreenMap, &SCREENBLOCK[8], histscreenMapLen / 2);
    initHistoryScore();
}

void updateHistory() {
    updateHistoryScore();
}

void drawHistory() {
    drawHistoryScore();
}
