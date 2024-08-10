
//{{BLOCK(instscreen)

//======================================================================
//
//	instscreen, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 232 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 7424 + 2048 = 9984
//
//	Time-stamp: 2024-04-22, 17:19:47
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_INSTSCREEN_H
#define GRIT_INSTSCREEN_H

#define instscreenTilesLen 7424
extern const unsigned short instscreenTiles[3712];

#define instscreenMapLen 2048
extern const unsigned short instscreenMap[1024];

#define instscreenPalLen 512
extern const unsigned short instscreenPal[256];

#endif // GRIT_INSTSCREEN_H

//}}BLOCK(instscreen)
