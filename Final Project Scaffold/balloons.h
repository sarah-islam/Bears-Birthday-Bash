
//{{BLOCK(balloons)

//======================================================================
//
//	balloons, 256x256@4, 
//	+ palette 256 entries, not compressed
//	+ 123 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 3936 + 2048 = 6496
//
//	Time-stamp: 2024-04-15, 22:05:53
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BALLOONS_H
#define GRIT_BALLOONS_H

#define balloonsTilesLen 3936
extern const unsigned short balloonsTiles[1968];

#define balloonsMapLen 2048
extern const unsigned short balloonsMap[1024];

#define balloonsPalLen 512
extern const unsigned short balloonsPal[256];

#endif // GRIT_BALLOONS_H

//}}BLOCK(balloons)
