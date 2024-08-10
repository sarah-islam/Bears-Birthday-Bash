
//{{BLOCK(histscreen)

//======================================================================
//
//	histscreen, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 84 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 2688 + 2048 = 5248
//
//	Time-stamp: 2024-04-22, 21:47:58
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_HISTSCREEN_H
#define GRIT_HISTSCREEN_H

#define histscreenTilesLen 2688
extern const unsigned short histscreenTiles[1344];

#define histscreenMapLen 2048
extern const unsigned short histscreenMap[1024];

#define histscreenPalLen 512
extern const unsigned short histscreenPal[256];

#endif // GRIT_HISTSCREEN_H

//}}BLOCK(histscreen)
