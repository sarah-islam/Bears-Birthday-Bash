
//{{BLOCK(startscreen)

//======================================================================
//
//	startscreen, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 126 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 32 + 4032 + 2048 = 6112
//
//	Time-stamp: 2024-04-15, 19:10:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_STARTSCREEN_H
#define GRIT_STARTSCREEN_H

#define startscreenTilesLen 4032
extern const unsigned short startscreenTiles[2016];

#define startscreenMapLen 2048
extern const unsigned short startscreenMap[1024];

#define startscreenPalLen 32
extern const unsigned short startscreenPal[16];

#endif // GRIT_STARTSCREEN_H

//}}BLOCK(startscreen)
