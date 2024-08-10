
//{{BLOCK(fishgamescreen)

//======================================================================
//
//	fishgamescreen, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 3 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 96 + 2048 = 2656
//
//	Time-stamp: 2024-03-30, 19:06:37
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FISHGAMESCREEN_H
#define GRIT_FISHGAMESCREEN_H

#define fishgamescreenTilesLen 96
extern const unsigned short fishgamescreenTiles[48];

#define fishgamescreenMapLen 2048
extern const unsigned short fishgamescreenMap[1024];

#define fishgamescreenPalLen 512
extern const unsigned short fishgamescreenPal[256];

#endif // GRIT_FISHGAMESCREEN_H

//}}BLOCK(fishgamescreen)
