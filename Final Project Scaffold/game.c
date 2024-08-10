#include "gba.h"
#include "mode0.h"
#include "sprites.h"
#include "spritesheet.h"
#include "game.h"

#include "mainmaptilemap.h"
#include "mainmapcollisionmap.h"

#include "foresttilemap.h"
#include "forestcollisionmap2.h"

#include "starthousetilemap.h"
#include "starthousecollisionmap.h"

OBJ_ATTR shadowOAM[128];

SPRITE player;
SPRITE playerM;

SPRITE hook;
SPRITE hookAttach[3];
SPRITE fish1[15];
SPRITE misc1[15];
SPRITE score[2];
SPRITE time[5];

SPRITE playerF;
SPRITE wolf;
SPRITE berriesNuts[20];
SPRITE bait;

SPRITE playerE;
SPRITE bellaBear;

SPRITE buttonL;
SPRITE buttonR;

SPRITE textBubble;
SPRITE textBubble2;
SPRITE textBubble3;
SPRITE textBubble4;
SPRITE textBubble5;

SPRITE historyScore[2];
int numGamesPlayed;

int hOff = 0;
int vOff = 0;

int paused = 0;
int minutes = 0;
int seconds = 0;
int fishScore = 0;
int fishGameCompleted = 0;

int wolfStopped = 0;
int forageScore = 0;
int forageGameCompleted = 0;

int numTasksComplete = 0;

int doorHit = 0;
int houseHit = 0;
int lakeHit = 0;
int forestHit = 0;

typedef enum {RIGHT, LEFT} DIRECTION;

// START SCREEN BUTTON LOGIC

void initButtonL() {
    buttonL.width = 32;
    buttonL.height = 64;
    buttonL.x = 56;
    buttonL.y = 48;
    buttonL.oamIndex = 126;
}
void updateButtonL() {
    if ((buttonL.y < 112) && BUTTON_PRESSED(BUTTON_DOWN)) {
        buttonL.y += 32;
    }
    if ((buttonL.y > 48) && BUTTON_PRESSED(BUTTON_UP)) {
        buttonL.y -= 32;
    }
}
void drawButtonL() {
    shadowOAM[buttonL.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_WIDE | ATTR0_Y(buttonL.y);
    shadowOAM[buttonL.oamIndex].attr1 = ATTR1_LARGE | ATTR1_X(buttonL.x);
    shadowOAM[buttonL.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(15, 20));
}

void initButtonR() {
    buttonR.width = 32;
    buttonR.height = 64;
    buttonR.x = 120;
    buttonR.y = 48;
    buttonR.oamIndex = 127;
}

void updateButtonR() {
    if ((buttonR.y < 112) && BUTTON_PRESSED(BUTTON_DOWN)) {
        buttonR.y += 32;
    }
    if ((buttonR.y > 48) && BUTTON_PRESSED(BUTTON_UP)) {
        buttonR.y -= 32;
    }
}

void drawButtonR() {
    shadowOAM[buttonR.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_WIDE | ATTR0_Y(buttonR.y);
    shadowOAM[buttonR.oamIndex].attr1 = ATTR1_LARGE | ATTR1_X(buttonR.x);
    shadowOAM[buttonR.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(15, 24));
}


// GAME HISTORY DATA LOGIC

void initHistoryScore() {
    for (int i = 0; i < 2; i++) {
        historyScore[i].x = 10 + (i * 20);
        historyScore[i].y = 10;
        historyScore[i].width = 16;
        historyScore[i].height = 16;
        historyScore[i].oamIndex = 124 + i;
    }
    
}
void updateHistoryScore() {
    
}
void drawHistoryScore() {
    shadowOAM[124].attr0 = ATTR0_REGULAR | ATTR0_Y(74) | ATTR0_SQUARE;
    shadowOAM[124].attr1 = ATTR1_X(105) | ATTR1_SMALL;
    shadowOAM[124].attr2 = ATTR2_PALROW(2) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((numGamesPlayed / 10) % 10) * 2, (((numGamesPlayed / 10) / 10) * 2) + 30);
    //shadowOAM[124].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(0, 30);

    shadowOAM[125].attr0 = ATTR0_REGULAR | ATTR0_Y(74) | ATTR0_SQUARE;
    shadowOAM[125].attr1 = ATTR1_X(115) | ATTR1_SMALL;
    shadowOAM[125].attr2 = ATTR2_PALROW(2) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((numGamesPlayed % 10) % 10) * 2, (((numGamesPlayed % 10) / 10) * 2) + 30);
    //shadowOAM[125].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(2, 30);
    mgba_printf("draw");
}

// HOUSE LOGIC

// house player logic
void initPlayer() {
    player.x = 30;
    player.y = 75;
    player.xVel = 1;
    player.yVel = 1;
    player.width = 15;
    player.height = 20;
    player.oamIndex = 37;

    player.isAnimating = 0;
    player.currentFrame = 0;
    player.numFrames = 5;
    player.timeUntilNextFrame = 10;
    player.direction = RIGHT;
}
void updatePlayer() {
    // animation + movement based on bounds and collision map
    player.isAnimating = 0;

    int leftX = player.x;
    int rightX = player.x + player.width - 1;
    int topY = player.y;
    int bottomY = player.y + player.height - 1;

    if (BUTTON_HELD(BUTTON_UP)) {
        player.isAnimating = 1;
        if (player.y < 0) {
            player.y = 0;
        }
        if (colorAt2(leftX, topY - player.yVel) && colorAt2(rightX, topY - player.yVel)) {
            player.y -= player.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_DOWN)) {
        player.isAnimating = 1;
        if (player.y > (160 - player.height)) {
            player.y = 160 - player.height;
        }
        if (colorAt2(leftX, bottomY + player.yVel) && colorAt2(rightX, bottomY + player.yVel)) {
            player.y += player.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_LEFT)) {
        player.direction = LEFT;
        player.isAnimating = 1;
        if (player.x < 0) {
            player.x = 0;
        }
        if (colorAt2(leftX - player.xVel, topY) && colorAt2(leftX - player.xVel, bottomY)) {
            player.x -= player.xVel;
        }
    }
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        player.direction = RIGHT;
        player.isAnimating = 1;
        if (player.x > (240 - player.width)) {
            player.x = 240 - player.width;
        }
        if (colorAt2(rightX + player.xVel, topY) && colorAt2(rightX + player.xVel, bottomY)) {
            player.x += player.xVel;
        }
    }
    // if color at player position is red (2), door is hit
    if (colorAt2(leftX, topY) == 2 || colorAt2(rightX, topY) == 2 || colorAt2(leftX, bottomY) == 2 || colorAt2(rightX, bottomY) == 2) {
        doorHit = 1;
    }

    if (player.isAnimating) {
        player.timeUntilNextFrame--;
        if (player.timeUntilNextFrame == 0) {
            player.currentFrame = (player.currentFrame + 1) % player.numFrames;
            player.timeUntilNextFrame = 10;
        }
    } else {
        player.currentFrame = 0;
    }
}
void drawPlayer() {
    shadowOAM[player.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_TALL | ATTR0_Y(player.y);
    shadowOAM[player.oamIndex].attr1 = ATTR1_MEDIUM | ATTR1_X(player.x);
    if (player.direction == LEFT) {
        shadowOAM[player.oamIndex].attr1 |= ATTR1_HFLIP;
    }
    shadowOAM[player.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(1, player.currentFrame * 4));
}

// player text bubble logic
void initTextBubble() {
    textBubble.x = player.x + 20;
    textBubble.y = player.y - 20;
    textBubble.width = 64;
    textBubble.height = 32;
    textBubble.oamIndex = 80;
}

void updateTextBubble() {
    // movement based on player
    textBubble.x = player.x + 20;
    textBubble.y = player.y - 20;
}

void drawTextBubble() {
    if (doorHit == 1) {
        shadowOAM[textBubble.oamIndex].attr0 = ATTR0_HIDE;
    } else {
    shadowOAM[textBubble.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_WIDE | ATTR0_Y(textBubble.y);
    shadowOAM[textBubble.oamIndex].attr1 = ATTR1_LARGE | ATTR1_X(textBubble.x);
    shadowOAM[textBubble.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(4, 8));
    }
}

// MAIN MAP LOGIC

// main map player logic
void initPlayerM() {
    playerM.x = 30;
    playerM.y = 75;
    playerM.xVel = 1;
    playerM.yVel = 1;
    playerM.width = 15;
    playerM.height = 20;
    playerM.oamIndex = 37;

    playerM.isAnimating = 0;
    playerM.currentFrame = 0;
    playerM.numFrames = 5;
    playerM.timeUntilNextFrame = 10;
    playerM.direction = RIGHT;
}
void updatePlayerM() {
    // animation + movement based on bounds and collision map
    playerM.isAnimating = 0;

    int leftX = playerM.x;
    int rightX = playerM.x + playerM.width - 1;
    int topY = playerM.y;
    int bottomY = playerM.y + playerM.height - 1;

    if (BUTTON_HELD(BUTTON_UP)) {
        playerM.isAnimating = 1;
        if (playerM.y < 0) {
            playerM.y = 0;
        }
        if (colorAt3(leftX, topY - playerM.yVel) && colorAt3(rightX, topY - playerM.yVel)) {
            playerM.y -= playerM.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_DOWN)) {
        playerM.isAnimating = 1;
        if (playerM.y > (160 - playerM.height)) {
            playerM.y = 160 - playerM.height;
        }
        if (colorAt3(leftX, bottomY + playerM.yVel) && colorAt3(rightX, bottomY + playerM.yVel)) {
            playerM.y += playerM.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_LEFT)) {
        playerM.direction = LEFT;
        playerM.isAnimating = 1;
        if (playerM.x < 0) {
            playerM.x = 0;
        }
        if (colorAt3(leftX - playerM.xVel, topY) && colorAt3(leftX - playerM.xVel, bottomY)) {
            playerM.x -= playerM.xVel;
        }
    }
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        playerM.direction = RIGHT;
        playerM.isAnimating = 1;
        if (playerM.x > (240 - playerM.width)) {
            playerM.x = 240 - playerM.width;
        }
        if (colorAt3(rightX + playerM.xVel, topY) && colorAt3(rightX + playerM.xVel, bottomY)) {
            playerM.x += playerM.xVel;
        }
    }
    // if color at player position is red (2) and tasks completed is 2, house is considered hit
    if (colorAt3(leftX, topY) == 2 || colorAt3(rightX, topY) == 2 || colorAt3(leftX, bottomY) == 2 || colorAt3(rightX, bottomY) == 2) {
        if (numTasksComplete == 2) {
            houseHit = 1;
        }
    }
    // if color at player position is green (3) and forage game not completed, forest is considered hit
    if (colorAt3(leftX, topY) == 3 || colorAt3(rightX, topY) == 3 || colorAt3(leftX, bottomY) == 3 || colorAt3(rightX, bottomY) == 3) {
        if (forageGameCompleted == 0) {
            forestHit = 1;
        }
    }
    // if color at player position is blue (4) and fish game not completed, lake is considered hit
    if (colorAt3(leftX, topY) == 4 || colorAt3(rightX, topY) == 4 || colorAt3(leftX, bottomY) == 4 || colorAt3(rightX, bottomY) == 4) {
        if (fishGameCompleted == 0) {
            lakeHit = 1;
        }
    }

    if (playerM.isAnimating) {
        playerM.timeUntilNextFrame--;
        if (playerM.timeUntilNextFrame == 0) {
            playerM.currentFrame = (playerM.currentFrame + 1) % playerM.numFrames;
            playerM.timeUntilNextFrame = 10;
        }
    } else {
        playerM.currentFrame = 0;
    }
}
void drawPlayerM() {
    shadowOAM[playerM.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_TALL | ATTR0_Y(playerM.y);
    shadowOAM[playerM.oamIndex].attr1 = ATTR1_MEDIUM | ATTR1_X(playerM.x);
    if (playerM.direction == LEFT) {
        shadowOAM[playerM.oamIndex].attr1 |= ATTR1_HFLIP;
    }
    shadowOAM[playerM.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(1, playerM.currentFrame * 4));
}

// player text bubble logic
void initTextBubble2() {
    textBubble2.x = playerM.x + 20;
    textBubble2.y = playerM.y - 20;
    textBubble2.width = 64;
    textBubble2.height = 32;
    textBubble2.oamIndex = 81;
}
void updateTextBubble2() {
    // movement based on player
    textBubble2.x = playerM.x + 20;
    textBubble2.y = playerM.y - 20;
}
void drawTextBubble2() {
    if (lakeHit == 1 || forestHit == 1) {
        shadowOAM[textBubble2.oamIndex].attr0 = ATTR0_HIDE;
    } else {
        shadowOAM[textBubble2.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_WIDE | ATTR0_Y(textBubble2.y);
        shadowOAM[textBubble2.oamIndex].attr1 = ATTR1_LARGE | ATTR1_X(textBubble2.x);
        shadowOAM[textBubble2.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(4, 12));
    }
}

// player text bubble logic
void initTextBubble3() {
    textBubble3.x = playerM.x + 20;
    textBubble3.y = playerM.y - 20;
    textBubble3.width = 64;
    textBubble3.height = 32;
    textBubble3.oamIndex = 82;
}

void updateTextBubble3() {
    // movement based on player
    textBubble3.x = playerM.x + 20;
    textBubble3.y = playerM.y - 20;
}

void drawTextBubble3() {
    if (numTasksComplete == 2) {
        shadowOAM[textBubble3.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_WIDE | ATTR0_Y(textBubble3.y);
        shadowOAM[textBubble3.oamIndex].attr1 = ATTR1_LARGE | ATTR1_X(textBubble3.x);
        shadowOAM[textBubble3.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(4, 16));
    } else {
        shadowOAM[textBubble3.oamIndex].attr0 = ATTR0_HIDE;
    }
}











/////////////////// FISH GAME LOGIC //////////////////////











// fish logic
void initFish() {
    for (int i = 0; i < 15; i++) {
        fish1[i].x = 10 + (i % 5) * 50;
        fish1[i].y = 45 + (i / 5) * 35;
        fish1[i].xVel = 1;
        fish1[i].yVel = 1;
        fish1[i].width = 16;
        fish1[i].height = 16;
        fish1[i].oamIndex = i;
        fish1[i].active = 1;
    }
}
void updateFish() {
    for (int i = 0; i < 15; i++) {
        // movement when button not held
        if (fish1[i].active && !BUTTON_HELD(BUTTON_B)) {
            fish1[i].x += fish1[i].xVel;
            if (fish1[i].x > 240) {
                fish1[i].x = 0;
            }
            // collision with hook
            if (collision(fish1[i].x, fish1[i].y, fish1[i].width, fish1[i].height, hook.x, hook.y, hook.width, hook.height)) {
                fish1[i].active = 0;
                hook.y = 0;
                hook.isMovingDown = 0;
                fishScore++;
            }
        }
        // movement when button held (moves towards hook)
        if (fish1[i].active && BUTTON_HELD(BUTTON_B)) {
            if (fish1[i].x < hook.x) {
                fish1[i].x += fish1[i].xVel;
            }
            if (fish1[i].x > hook.x) {
                fish1[i].x -= fish1[i].xVel;
            }
            if (fish1[i].y < hook.y) {
                fish1[i].y += fish1[i].yVel;
            }
            if (fish1[i].y > hook.y) {
                fish1[i].y -= fish1[i].yVel;
            }
            // collision with hook
            if (collision(fish1[i].x, fish1[i].y, fish1[i].width, fish1[i].height, hook.x, hook.y, hook.width, hook.height)) {
                fish1[i].active = 0;
                hook.y = 0;
                hook.isMovingDown = 0;
                fishScore++;
            }
        }
        // hide when inactive
        if (fish1[i].active == 0) {
            shadowOAM[fish1[i].oamIndex].attr0 = ATTR0_HIDE;
        }
    }
}
void drawFish() {
    for (int i = 0; i < 15; i++) {
        if (fish1[i].active) {
            int tileID = 8 + (i % 3) * 2;

            shadowOAM[fish1[i].oamIndex].attr0 = ATTR0_REGULAR | ATTR0_SQUARE | ATTR0_Y(fish1[i].y);
            shadowOAM[fish1[i].oamIndex].attr1 = ATTR1_SMALL | ATTR1_HFLIP | ATTR1_X(fish1[i].x);
            shadowOAM[fish1[i].oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(tileID, 0));
        }
    }
}

// misc items logic
void initMisc() {
    for (int i = 0; i < 10; i++) {
        misc1[i].x = 35 + (i % 5) * 50;
        misc1[i].y = 60 + (i / 5) * 40;
        misc1[i].xVel = 1;
        misc1[i].yVel = 0;
        misc1[i].width = 16;
        misc1[i].height = 16;
        misc1[i].oamIndex = i + 15;
        misc1[i].active = 1;
    }
}
void updateMisc() {
    // movement
    for (int i = 0; i < 10; i++) {
        if (misc1[i].active) {
            misc1[i].x += misc1[i].xVel;
            if (misc1[i].x > 240) {
                misc1[i].x = 0;
            }
            // collision with hook
            if (collision(misc1[i].x, misc1[i].y, misc1[i].width, misc1[i].height, hook.x, hook.y, hook.width, hook.height)) {
                misc1[i].active = 0;
                hook.y = 0;
                hook.isMovingDown = 0;
            }
        }
        // hide when inactive
        if (misc1[i].active == 0) {
            shadowOAM[misc1[i].oamIndex].attr0 = ATTR0_HIDE;
        }
    }
}
void drawMisc() {
    for (int i = 0; i < 10; i++) {
        if (misc1[i].active) {
            int tileID = 8 + (i % 3) * 2;

            shadowOAM[misc1[i].oamIndex].attr0 = ATTR0_REGULAR | ATTR0_SQUARE | ATTR0_Y(misc1[i].y);
            shadowOAM[misc1[i].oamIndex].attr1 = ATTR1_SMALL | ATTR1_X(misc1[i].x);
            shadowOAM[misc1[i].oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(tileID, 2));
        }
    }
}

// hook logic (both hook and hook attachments)
void initHook() {
    hook.oldX = 100;
    hook.oldY = 0;
    hook.x = 100;
    hook.y = 0;
    hook.xVel = 0;
    hook.yVel = 1;
    hook.width = 16;
    hook.height = 32;
    hook.oamIndex = 25;
}
void initHookAttach() {
    for (int i = 0; i < 3; i++) {
        hookAttach[i].oldX = 100;
        hookAttach[i].oldY = 0 - hook.height - (32 * i);
        hookAttach[i].x = 100;
        hookAttach[i].y = 0 - hook.height - (32 * i);
        hookAttach[i].xVel = 0;
        hookAttach[i].yVel = 1;
        hookAttach[i].width = 16;
        hookAttach[i].height = 32;
        hookAttach[i].oamIndex = 26 + i;
    }
}
void updateHook() {
    // movement
    hook.oldX = hook.x;
    hook.oldY = hook.y;
    // delay used for hook drop down
    int delay = 2;
    // bounds
    if (hook.x > 240 - hook.width) {
        hook.x = 240 - hook.width;
    }
    if (hook.x < 0) {
        hook.x = 0;
    }
    // hook drop down movement
    if (BUTTON_PRESSED(BUTTON_DOWN)) {
        hook.yVel = 1;
        hook.yCounter = 0;
        hook.isMovingDown = 1;
    }
    if (hook.isMovingDown) {
        hook.yCounter++;
        if (hook.yCounter >= delay) {
            hook.y += hook.yVel;
            hook.yCounter = 0;
            for (int i = 0; i < 15; i++) {
                if (fish1[i].active) {
                    // collision with fish so goes back to top
                    if (collision(fish1[i].x, fish1[i].y, fish1[i].width, fish1[i].height, hook.x, hook.y, hook.width, hook.height)) {
                        fish1[i].active = 0;
                        hook.y = 0;
                        hook.isMovingDown = 0;
                        fishScore++;
                    }
                }
            }
            for (int i = 0; i < 10; i++) {
                if (misc1[i].active) {
                    // collision with misc so goes back to top
                    if (collision(misc1[i].x, misc1[i].y, misc1[i].width, misc1[i].height, hook.x, hook.y, hook.width, hook.height)) {
                        misc1[i].active = 0;
                        hook.y = 0;
                        hook.isMovingDown = 0;
                    }
                }
            }
            // hit bound so go back to top
            if (hook.y >= 160 - hook.height) {
                hook.isMovingDown = 0;
            }
        }
    }
    // back and forth movement
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        hook.x += 1;
    }
    if (BUTTON_HELD(BUTTON_LEFT)) {
        hook.x -= 1;
    }
    // hide
    if (fishScore == 15) {
        shadowOAM[hook.oamIndex].attr0 = ATTR0_HIDE;
    }
}
void updateHookAttach() {
    // movement based on hook
    for (int i = 0; i < 3; i++) {
        hookAttach[i].oldX = hookAttach[i].x;
        hookAttach[i].oldY = hookAttach[i].y;
        hookAttach[i].x = hook.x;
        hookAttach[i].y = hook.y - hook.height - (32 * i);
        // hide
        if (fishScore == 15) {
            shadowOAM[hookAttach[i].oamIndex].attr0 = ATTR0_HIDE;
        }
    }
}
void drawHook() {
    shadowOAM[hook.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_TALL | ATTR0_Y(hook.y);
    shadowOAM[hook.oamIndex].attr1 = ATTR1_MEDIUM | ATTR1_X(hook.x);
    shadowOAM[hook.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(5, 4));

    // palette modification
    if (BUTTON_HELD(BUTTON_B)) {
        shadowOAM[hook.oamIndex].attr2 = ATTR2_PALROW(1) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(5, 4));
    }
}
void drawHookAttach() {
    for (int i = 0; i < 3; i++) {
        shadowOAM[hookAttach[i].oamIndex].attr0 = ATTR0_REGULAR | ATTR0_TALL | ATTR0_Y(hookAttach[i].y);
        shadowOAM[hookAttach[i].oamIndex].attr1 = ATTR1_MEDIUM | ATTR1_X(hookAttach[i].x);
        shadowOAM[hookAttach[i].oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(5, 1));

        // palette modification
        if (BUTTON_HELD(BUTTON_B)) {
            shadowOAM[hookAttach[i].oamIndex].attr2 = ATTR2_PALROW(1) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(5, 1));
        }
    }
}

// score logic
void initScore() {
    for (int i = 0; i < 2; i++) {
        score[i].x = 10 + (i * 10);
        score[i].y = 10;
        score[i].width = 8;
        score[i].height = 8;
        score[i].oamIndex = 30 + i;
    }
}
void updateScore() {
    // hide
    if (fishScore == 15) {
        shadowOAM[30].attr0 = ATTR0_HIDE;
        shadowOAM[31].attr0 = ATTR0_HIDE;
    }
}
void drawScore() {
    shadowOAM[30].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
    shadowOAM[30].attr1 = ATTR1_X(210) | ATTR1_SMALL;
    shadowOAM[30].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((fishScore / 10) % 10) * 2, (((fishScore / 10) / 10) * 2) + 30);

    shadowOAM[31].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
    shadowOAM[31].attr1 = ATTR1_X(220) | ATTR1_SMALL;
    shadowOAM[31].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((fishScore % 10) % 10) * 2, (((fishScore % 10) / 10) * 2) + 30);
}

// displayed time logic
void initTime() {
    for (int i = 0; i < 5; i++) {
        time[i].x = 10 + (i * 10);
        time[i].y = 10;
        time[i].width = 8;
        time[i].height = 8;
        time[i].oamIndex = 32 + i;
    }
}
void updateTime() {
        shadowOAM[32].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
        shadowOAM[32].attr1 = ATTR1_X(10) | ATTR1_SMALL;
        shadowOAM[32].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((minutes / 10) % 10) * 2, (((minutes / 10) / 10) * 2) + 30);

        shadowOAM[33].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
        shadowOAM[33].attr1 = ATTR1_X(20) | ATTR1_SMALL;
        shadowOAM[33].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((minutes % 10) % 10) * 2, (((minutes % 10) / 10) * 2) + 30);

        shadowOAM[34].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
        shadowOAM[34].attr1 = ATTR1_X(30) | ATTR1_SMALL;
        shadowOAM[34].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(20, 30);

        shadowOAM[35].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
        shadowOAM[35].attr1 = ATTR1_X(40) | ATTR1_SMALL;
        shadowOAM[35].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((seconds / 10) % 10) * 2, (((seconds / 10) / 10) * 2) + 30);

        shadowOAM[36].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
        shadowOAM[36].attr1 = ATTR1_X(50) | ATTR1_SMALL;
        shadowOAM[36].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((seconds % 10) % 10) * 2, (((seconds % 10) / 10) * 2) + 30);
    
}












/////////////////// FORAGE GAME LOGIC //////////////////////










// forage player logic
void initPlayerForage() {
    playerF.x = 23;
    playerF.y = 30;
    playerF.width = 15;
    playerF.height = 20;
    playerF.xVel = 2;
    playerF.yVel = 2;
    playerF.oamIndex = 38;

    playerF.isAnimating = 0;
    playerF.currentFrame = 0;
    playerF.numFrames = 5;
    playerF.timeUntilNextFrame = 10;
    playerF.direction = RIGHT;
}
void updatePlayerForage() {
    // animation + movement based on bounds and collision map
    playerF.isAnimating = 0;
    int leftX = playerF.x;
    int rightX = playerF.x + playerF.width - 1;
    int topY = playerF.y;
    int bottomY = playerF.y + playerF.height - 1;

    if (BUTTON_HELD(BUTTON_UP)) {
        playerF.isAnimating = 1;
        if (playerF.y < 0) {
            playerF.y = 0;
        }
        if (colorAt(leftX, topY - playerF.yVel) && colorAt(rightX, topY - playerF.yVel)) {
            playerF.y -= playerF.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_DOWN)) {
        playerF.isAnimating = 1;
        if (playerF.y > (256 - playerF.height)) {
            playerF.y = 256 - playerF.height;
        }
        if (colorAt(leftX, bottomY + playerF.yVel) && colorAt(rightX, bottomY + playerF.yVel)) {
            playerF.y += playerF.yVel;
        }
    }
    if (BUTTON_HELD(BUTTON_LEFT)) {
        playerF.direction = LEFT;
        playerF.isAnimating = 1;
        if (playerF.x < 0) {
            playerF.x = 0;
        }
        if (colorAt(leftX - playerF.xVel, topY) && colorAt(leftX - playerF.xVel, bottomY)) {
            playerF.x -= playerF.xVel;
        }
    }
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        playerF.direction = RIGHT;
        playerF.isAnimating = 1;
        if (playerF.x > (256 - playerF.width)) {
            playerF.x = 256 - playerF.width;
        }
        if (colorAt(rightX + playerF.xVel, topY) && colorAt(rightX + playerF.xVel, bottomY)) {
            playerF.x += playerF.xVel;
        }
    }

    if (playerF.isAnimating) {
        playerF.timeUntilNextFrame--;
        if (playerF.timeUntilNextFrame == 0) {
            playerF.currentFrame = (playerF.currentFrame + 1) % playerF.numFrames;
            playerF.timeUntilNextFrame = 10;
        }
    } else {
        playerF.currentFrame = 0;
    }

    // offset bounds
    hOff = playerF.x - ((SCREENWIDTH - playerF.width) / 2);
    vOff = playerF.y - ((SCREENHEIGHT - playerF.height) / 2);

    if (hOff < 0) {
        hOff = 0;
    }
    if (hOff > (256 - SCREENWIDTH)) {
        hOff = 256 - SCREENWIDTH;
    }
    if (vOff < 0) {
        vOff = 0;
    }
    if (vOff > (256 - SCREENHEIGHT)) {
        vOff = 256 - SCREENHEIGHT;
    }

}
void drawPlayerForage() {
    shadowOAM[playerF.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_TALL | ATTR0_Y(playerF.y - vOff);
    shadowOAM[playerF.oamIndex].attr1 = ATTR1_MEDIUM | ATTR1_X(playerF.x - hOff);
    if (playerF.direction == LEFT) {
        shadowOAM[playerF.oamIndex].attr1 |= ATTR1_HFLIP;
    }
    shadowOAM[playerF.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(1, playerF.currentFrame * 4));
}

// wolf logic
void initWolf() {
    wolf.x = -20;
    wolf.y = 30;
    wolf.width = 21;
    wolf.height = 27;
    wolf.xVel = 1;
    wolf.yVel = 1;
    wolf.oamIndex = 39;
    wolf.isAnimating = 0;
    wolf.currentFrame = 0;
    wolf.numFrames = 4;
    wolf.timeUntilNextFrame = 10;
    wolf.direction = RIGHT;
}
void updateWolf() {
    // wolf animation + movement based on player position and collision map
    wolf.isAnimating = 0;

    int dx = playerF.x - wolf.x;
    int dy = playerF.y - wolf.y;

    int leftX = wolf.x;
    int rightX = wolf.x + wolf.width - 1;
    int topY = wolf.y;
    int bottomY = wolf.y + wolf.height - 1;

    if (dx < 0 && colorAt(leftX - wolf.xVel, topY) && colorAt(leftX - wolf.xVel, bottomY)) {
        wolf.x -= wolf.xVel;
        wolf.direction = LEFT;
        wolf.isAnimating = 1;
    } else if (dx > 0 && wolf.x + wolf.width < 256 && colorAt(rightX + wolf.xVel, topY) && colorAt(rightX + wolf.xVel, bottomY)) {
        wolf.x += wolf.xVel;
        wolf.direction = RIGHT;
        wolf.isAnimating = 1;
    }
    if (dy < 0 && colorAt(leftX, topY - wolf.yVel) && colorAt(rightX, topY - wolf.yVel)) {
        wolf.y -= wolf.yVel;
        wolf.isAnimating = 1;
    } else if (dy > 0 && wolf.y + wolf.height < 256 && colorAt(leftX, bottomY + wolf.yVel) && colorAt(rightX, bottomY + wolf.yVel)) {
        wolf.y += wolf.yVel;
        wolf.isAnimating = 1;
    }

    if (wolf.isAnimating) {
        wolf.timeUntilNextFrame--;
        if (wolf.timeUntilNextFrame == 0) {
            wolf.currentFrame = (wolf.currentFrame + 1) % wolf.numFrames;
            wolf.timeUntilNextFrame = 10;
        }
    } else {
        wolf.currentFrame = 0;
    }

    // wolf stops following player when bait is thrown
    if (wolfStopped == 1) {
        wolf.xVel = 0;
        wolf.yVel = 0;
        wolf.isAnimating = 0;
        wolf.currentFrame = 0;
        wolf.direction = LEFT;
    }
}
void drawWolf() {
    shadowOAM[wolf.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_SQUARE | ATTR0_Y(wolf.y - vOff);
    shadowOAM[wolf.oamIndex].attr1 = ATTR1_MEDIUM | ATTR1_X(wolf.x - hOff);
    if (wolf.direction == LEFT) {
        shadowOAM[wolf.oamIndex].attr1 |= ATTR1_HFLIP;
    }
    shadowOAM[wolf.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(1) | (ATTR2_TILEID(15, wolf.currentFrame * 4));
}

// berries and nuts logic
void initBerriesNuts() {
    // berries/nuts 8x8 sprites should be positioned at random locations that are not on the collision maps color 0 (black)
    for (int i = 0; i < 20; i++) {
        int x;
        int y;
        int isValidLocation;
        do {
            x = rand() % 256;
            y = rand() % 256;
            isValidLocation = 1;
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    if (colorAt(x + col, y + row) == 0) {
                        isValidLocation = 0;
                        break;
                    }
                }
                if (!isValidLocation) break;
            }
        } while (!isValidLocation);
        berriesNuts[i].x = x;
        berriesNuts[i].y = y;
        berriesNuts[i].width = 8;
        berriesNuts[i].height = 8;
        berriesNuts[i].oamIndex = 40 + i;
        berriesNuts[i].active = 1;
    }
}
void updateBerriesNuts() {
    // berries/nuts collision with player
    for (int i = 0; i < 20; i++) {
        if (berriesNuts[i].active) {
            if (collision(berriesNuts[i].x, berriesNuts[i].y, berriesNuts[i].width, berriesNuts[i].height, playerF.x, playerF.y, playerF.width, playerF.height)) {
                berriesNuts[i].active = 0;
                forageScore++;
            }
        }
        // hide
        if (berriesNuts[i].active == 0) {
            shadowOAM[berriesNuts[i].oamIndex].attr0 = ATTR0_HIDE;
        }
    }

}
void drawBerriesNuts() {
    for (int i = 0; i < 20; i++) {
        if (berriesNuts[i].active) {
            int tileID = 8 + (i % 2) * 1;
            shadowOAM[berriesNuts[i].oamIndex].attr0 = ATTR0_REGULAR | ATTR0_SQUARE | ATTR0_Y(berriesNuts[i].y - vOff);
            shadowOAM[berriesNuts[i].oamIndex].attr1 = ATTR1_TINY | ATTR1_X(berriesNuts[i].x - hOff);
            shadowOAM[berriesNuts[i].oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(tileID, 5));
        }
    }
}

// bait logic
void initBait() {
    bait.x = 0;
    bait.y = 0;
    bait.height = 16;
    bait.width = 16;
    bait.xVel = 1;
    bait.yVel = 1;
    bait.oamIndex = 72;
    bait.active = 0;
}
void updateBait() {
    // bait movement based on player to wolf position
    if (BUTTON_PRESSED(BUTTON_B)) {
        if (!bait.active) {
            bait.active = 1;
            bait.x = playerF.x + 5;
            bait.y = playerF.y + 5;
        }
    }
    if (bait.active) {
        if (bait.x < (wolf.x + 10)) {
            bait.x += bait.xVel;
        }
        if (bait.x > wolf.x) {
            bait.x -= bait.xVel;
        }
        if (bait.y < (wolf.y + 10)) {
            bait.y += bait.yVel;
        }
        if (bait.y > wolf.y) {
            bait.y -= bait.yVel;
        }
        // if collision with bait, wolf stops following player
        if (collision(wolf.x, wolf.y, wolf.width, wolf.height, bait.x, bait.y, bait.width, bait.height)) {
            wolfStopped = 1;
        }
    }
}
void drawBait() {
    if (bait.active) {
        shadowOAM[bait.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_SQUARE | ATTR0_Y(bait.y - vOff);
        shadowOAM[bait.oamIndex].attr1 = ATTR1_SMALL | ATTR1_X(bait.x - hOff);
        shadowOAM[bait.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(10, 5));
    }
}

// forage score logic
void initScore2() {
    for (int i = 0; i < 2; i++) {
        score[i].x = 10 + (i * 10);
        score[i].y = 10;
        score[i].width = 8;
        score[i].height = 8;
        score[i].oamIndex = 30 + i;
    }
}
void updateScore2() {
    // hide
    if (forageScore == 20) {
        shadowOAM[30].attr0 = ATTR0_HIDE;
        shadowOAM[31].attr0 = ATTR0_HIDE;
    }
}
void drawScore2() {
    shadowOAM[30].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
    shadowOAM[30].attr1 = ATTR1_X(210) | ATTR1_SMALL;
    shadowOAM[30].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((forageScore / 10) % 10) * 2, (((forageScore / 10) / 10) * 2) + 30);

    shadowOAM[31].attr0 = ATTR0_Y(140) | ATTR0_SQUARE;
    shadowOAM[31].attr1 = ATTR1_X(220) | ATTR1_SMALL;
    shadowOAM[31].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | ATTR2_TILEID(((forageScore % 10) % 10) * 2, (((forageScore % 10) / 10) * 2) + 30);
}










/////////////////// END GAME LOGIC //////////////////////










// ending house bella logic
void initBella() {
    bellaBear.x = 130;
    bellaBear.y = 70;
    bellaBear.width = 15;
    bellaBear.height = 32;
    bellaBear.oamIndex = 78;
    bellaBear.isAnimating = 0;
    bellaBear.direction = LEFT;
}

void updateBella() {
}

void drawBella() {
    shadowOAM[bellaBear.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_TALL | ATTR0_Y(bellaBear.y);
    shadowOAM[bellaBear.oamIndex].attr1 = ATTR1_MEDIUM | ATTR1_X(bellaBear.x);
    if (bellaBear.direction == LEFT) {
        shadowOAM[bellaBear.oamIndex].attr1 |= ATTR1_HFLIP;
    }
    shadowOAM[bellaBear.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(23, 0));
}

// ending house player logic
void initPlayerE() {
    playerE.x = 95;
    playerE.y = 70;
    playerE.width = 16;
    playerE.height = 32;
    playerE.oamIndex = 79;
}
void updatePlayerE() {
}
void drawPlayerE() {
    shadowOAM[playerE.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_TALL | ATTR0_Y(playerE.y);
    shadowOAM[playerE.oamIndex].attr1 = ATTR1_MEDIUM | ATTR1_X(playerE.x);
    shadowOAM[playerE.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(20, 0));
}

// bella text bubble logic
void initTextBubble4() {
    textBubble4.x = bellaBear.x + 20;
    textBubble4.y = bellaBear.y - 20;
    textBubble4.width = 64;
    textBubble4.height = 32;
    textBubble4.oamIndex = 83;
}

void updateTextBubble4() {
}

void drawTextBubble4() {
    shadowOAM[textBubble4.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_WIDE | ATTR0_Y(textBubble4.y);
    shadowOAM[textBubble4.oamIndex].attr1 = ATTR1_LARGE | ATTR1_X(textBubble4.x);
    shadowOAM[textBubble4.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(4, 20));
}

// player text bubble logic
void initTextBubble5() {
    textBubble5.x = playerE.x - 66;
    textBubble5.y = playerE.y - 20;
    textBubble5.width = 64;
    textBubble5.height = 32;
    textBubble5.oamIndex = 84;
}

void updateTextBubble5() {
    // movement based on player
    textBubble5.x = playerE.x - 66;
    textBubble5.y = playerE.y - 20;
}

void drawTextBubble5() {
    shadowOAM[textBubble5.oamIndex].attr0 = ATTR0_REGULAR | ATTR0_WIDE | ATTR0_Y(textBubble5.y);
    shadowOAM[textBubble5.oamIndex].attr1 = ATTR1_LARGE | ATTR1_X(textBubble5.x);
    shadowOAM[textBubble5.oamIndex].attr2 = ATTR2_PALROW(0) | ATTR2_PRIORITY(0) | (ATTR2_TILEID(4, 24));
}

// collision map logic

// forest
inline unsigned char colorAt(int x, int y){
    return ((unsigned char *) forestcollisionmap2Bitmap) [OFFSET(x, y, 256)];
}
// house
inline unsigned char colorAt2(int x, int y) {
    return ((unsigned char *) starthousecollisionmapBitmap) [OFFSET(x, y, 256)];
}
// main map
inline unsigned char colorAt3(int x, int y) {
    return ((unsigned char *) mainmapcollisionmapBitmap) [OFFSET(x, y, 256)];
}