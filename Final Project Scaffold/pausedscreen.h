
//{{BLOCK(pausedscreen)

//======================================================================
//
//	pausedscreen, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 162 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 5184 + 2048 = 7744
//
//	Time-stamp: 2024-04-22, 17:35:17
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PAUSEDSCREEN_H
#define GRIT_PAUSEDSCREEN_H

#define pausedscreenTilesLen 5184
extern const unsigned short pausedscreenTiles[2592];

#define pausedscreenMapLen 2048
extern const unsigned short pausedscreenMap[1024];

#define pausedscreenPalLen 512
extern const unsigned short pausedscreenPal[256];

#endif // GRIT_PAUSEDSCREEN_H

//}}BLOCK(pausedscreen)
