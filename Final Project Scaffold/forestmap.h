
//{{BLOCK(forestmap)

//======================================================================
//
//	forestmap, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 46 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 1472 + 2048 = 4032
//
//	Time-stamp: 2024-04-08, 21:56:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_FORESTMAP_H
#define GRIT_FORESTMAP_H

#define forestmapTilesLen 1472
extern const unsigned short forestmapTiles[736];

#define forestmapMapLen 2048
extern const unsigned short forestmapMap[1024];

#define forestmapPalLen 512
extern const unsigned short forestmapPal[256];

#endif // GRIT_FORESTMAP_H

//}}BLOCK(forestmap)
