
// variables:

extern OBJ_ATTR shadowOAM[128];

extern int bubbleHidden;

extern int numGamesPlayed; // number of games played

extern int hOff; // for forage game
extern int vOff; // for forage game

extern int seconds; // for fish game timer
extern int minutes; // for fish game timer

extern int paused; // fish game timer paused or not
extern int fishScore;
extern int fishGameCompleted; // fish game completed or not

extern int wolfStopped; // wolf stopped by bait or not
extern int forageScore;
extern int forageGameCompleted; // forage game completed or not

extern int numTasksComplete;

extern int doorHit; // door collision or not
extern int houseHit; // house collision or not
extern int lakeHit; // lake collision or not
extern int forestHit; // forest collision or not

extern SPRITE player; // player in starting house
extern SPRITE playerM; // player on main map
extern SPRITE playerF; // player in forage game
extern SPRITE playerE; // player in ending house
extern SPRITE bellaBear; // bella bear in ending house
extern SPRITE textBubble; // text bubble for player
extern SPRITE textBubble2;
extern SPRITE textBubble3;
extern SPRITE textBubble4;
extern SPRITE textBubble5;

// fish game sprites
extern SPRITE hook;
extern SPRITE hookAttach[3];
extern SPRITE fish1[15];
extern SPRITE misc1[15];
extern SPRITE time[5];

// forage game sprites
extern SPRITE wolf;
extern SPRITE berriesNuts[20];
extern SPRITE bait;

extern SPRITE score[2]; // for both mini games

// start screen button parts
extern SPRITE buttonL;
extern SPRITE buttonR;

// game history data
extern SPRITE historyScore[2];

// prototypes:

// start screen button parts
void initButtonL();
void updateButtonL();
void drawButtonL();
void initButtonR();
void updateButtonR();
void drawButtonR();

// game history data
void initHistoryScore();
void updateHistoryScore();
void drawHistoryScore();

// player in starting house
void initPlayer();
void drawPlayer();
void updatePlayer();

// player on main map
void initPlayerM();
void drawPlayerM();
void updatePlayerM();

// fish game sprites:

// fish
void initFish();
void updateFish();
void drawFish();

// misc (trash, shells, etc.)
void initMisc();
void updateMisc();
void drawMisc();

// hook
void initHook();
void updateHook();
void drawHook();

// hook attachment
void initHookAttach();
void updateHookAttach();
void drawHookAttach();

// fish score
void initScore();
void updateScore();
void drawScore();

// time displayed
void initTime();
void updateTime();

// forage game sprites:

// player in forage game
void initPlayerForage();
void updatePlayerForage();
void drawPlayerForage();

// wolf
void initWolf();
void updateWolf();
void drawWolf();

// berries and nuts
void initBerriesNuts();
void updateBerriesNuts();
void drawBerriesNuts();

// bait
void initBait();
void updateBait();
void drawBait();

// forage score
void initScore2();
void updateScore2();
void drawScore2();

// end scene sprites:

// player in ending house
void initPlayerE();
void updatePlayerE();
void drawPlayerE();

// bella bear
void initBella();
void updateBella();
void drawBella();


// text bubbles:

// player text bubble (start house)
void initTextBubble();
void updateTextBubble();
void drawTextBubble();

// player text bubble 2 (main map)
void initTextBubble2();
void updateTextBubble2();
void drawTextBubble2();

// player text bubble 3 (main map after both games completed)
void initTextBubble3();
void updateTextBubble3();
void drawTextBubble3();

// bella text bubble 4 (ending house)
void initTextBubble4();
void updateTextBubble4();
void drawTextBubble4();

// player text bubble 5 (ending house)
void initTextBubble5();
void updateTextBubble5();
void drawTextBubble5();


// colorAt functions for collision maps:

inline unsigned char colorAt(int x, int y); // for forage game
inline unsigned char colorAt2(int x, int y); // for houses
inline unsigned char colorAt3(int x, int y); // for main map