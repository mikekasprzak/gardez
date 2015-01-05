// - ----------------------------------------------------------------------------------------- - //
#include <c64.h>
#include <string.h>
// - ----------------------------------------------------------------------------------------- - //
#include "vickers.h"
// - ----------------------------------------------------------------------------------------- - //

extern char Music[];

extern void SIDInit();
extern void SIDStep();
/*
//#define SIDPlay() {
void SIDInit( const char Song ) {
	__asm__("lda #0");
	__asm__("tax");
	__asm__("tay");
	__asm__("jsr #$1000");
}
//_Music+$00");//,Music+0x7C+2);//$400");//%w", 0x40D);//0x1000);
//}

void SIDStep() {
	__asm__("jsr #$1012");//, 0x1013);// 0x1006);
}*/
	
// - ----------------------------------------------------------------------------------------- - //
extern char SpriteColor[2];
extern char SpriteData[];
// - ----------------------------------------------------------------------------------------- - //
extern char CharColor[4];
extern char CharData[];
extern char TileData[];
extern char TileAttribData[];
// - ----------------------------------------------------------------------------------------- - //

// - ----------------------------------------------------------------------------------------- - //
struct sMapHeader {
	unsigned char Width;
	unsigned char Height;
};
// - ----------------------------------------------------------------------------------------- - //
extern struct sMapHeader MapHeader;
extern char MapData[];
// - ----------------------------------------------------------------------------------------- - //

// - ----------------------------------------------------------------------------------------- - //
#define Mul9(_In) (((_In)<<3)+(_In))
// - ----------------------------------------------------------------------------------------- - //

// - ----------------------------------------------------------------------------------------- - //
	extern char Music[];

int main(void) {
	VICSetAddr( 0, 2 );	// Fun Mode (shows neat garbage) //

	memcpy((void*)(0x4000),(void*)&Music,1024*8);

	SIDInit();

	// Load Sprites //
	memcpy((void*)(MySpriteBase),(void*)&SpriteData,32*64);
	VIC.spr_mcolors[0] = SpriteColor[0];//COLOR_BLACK;
	VIC.spr_mcolors[1] = SpriteColor[1];//COLOR_WHITE;

	// Load Chars //
	memcpy((void*)(MyCharBase),(void*)&CharData,24*8);
	VIC.bgcolor[0] = CharColor[0];
	VIC.bgcolor[1] = CharColor[1];
	VIC.bgcolor[2] = CharColor[2];
	VIC.bgcolor[3] = CharColor[3];

	// Load Map //
	//memset((void*)0xD800,COLOR_BROWN,1024); // 1000 actually //
	memset((void*)MyScreenBase,0,1024);
	{
		char x, y;
		char w = MapHeader.Width;
		char h = MapHeader.Height;
		
		// Screen Width //
		const unsigned char sw = 40;
		const unsigned char sw2 = 40+40;
		
		for ( y = 0; y < h; y++ ) {	
			char Line = w*y;
			unsigned short SLine = 40*y;
			
			for ( x = 0; x < w; x++ ) {
				unsigned char c = MapData[Line+x];
				unsigned char* TileAddr = &TileData[Mul9(c)];
				unsigned char* Out = (unsigned char*)MyScreenBase+(SLine+SLine+SLine)+(x+x+x);
				Out[0] = TileAddr[0];
				Out[1] = TileAddr[1];
				Out[2] = TileAddr[2];
				Out[sw+0] = TileAddr[3];
				Out[sw+1] = TileAddr[4];
				Out[sw+2] = TileAddr[5];
				Out[sw2+0] = TileAddr[6];
				Out[sw2+1] = TileAddr[7];
				Out[sw2+2] = TileAddr[8];
				
				Out = (unsigned char*)0xD800+(SLine+SLine+SLine)+(x+x+x);
				Out[0] = TileAttribData[TileAddr[0]];
				Out[1] = TileAttribData[TileAddr[1]];
				Out[2] = TileAttribData[TileAddr[2]];
				Out[sw+0] = TileAttribData[TileAddr[3]];
				Out[sw+1] = TileAttribData[TileAddr[4]];
				Out[sw+2] = TileAttribData[TileAddr[5]];
				Out[sw2+0] = TileAttribData[TileAddr[6]];
				Out[sw2+1] = TileAttribData[TileAddr[7]];
				Out[sw2+2] = TileAttribData[TileAddr[8]];
			}
		}
	}
	

	VICSetPage( 2 );
	VICSetAddr( 0, 1 );

	VIC.bordercolor = COLOR_ORANGE;
	VIC.ctrl2 = (VIC.ctrl2 & (~0x10)) | 0x10;	// Enable Multicolor Mode //

	// Scroll Registers //
	VIC.ctrl1 = (VIC.ctrl1 & (~0x7)) | 6; 		// Y Scroll //
	VIC.ctrl2 = (VIC.ctrl2 & (~0x7)) | 4;		// X Scroll //

	// Sprite Setup //
	SprInit();

	{
		short x = 50+(24*5)+4;
		short y = 50+(24*3)+6;
		char Facing = 0;
		char Firing = 0;
		
		short FireX = 0;
		short FireY = 0;
		short FireLife = 0;
		short FireVelX = 0;
		short FireVelY = 0;
		
		while (1) {
			// Step //
			{
				unsigned char Input = PadGet1();
				if ( Input & PAD_LEFT ) {
					x--;
					Facing = 0;
				}
				else if ( Input & PAD_RIGHT ) {
					x++;
					Facing = 1;
				}
				
				Firing = Input & PAD_FIRE;
			}
			
			if ( FireLife == 0 ) {
				if ( Firing ) {
					FireLife = 60;
					FireX = x;
					FireY = y;
					
					FireVelX = Facing ? +2 : -2;
					FireX += FireVelX << 3;
					FireVelY = 0;
				}
			}
			else {
				FireX += FireVelX;
				FireY += FireVelY;
				FireLife--;
			}
			
			SIDStep();
			
			// Draw //	
			VICWaitNotVBlank();
			VICWaitVBlank();
			
			if ( FireLife > 0 ) {
				SprAlloc( 16 + ((FireVelX > 0) ? 1 : 0), FireX, FireY );
			}
			
			SprAlloc( (2*Facing)+(Firing?12:0), x,y );
			
			SprDraw();
		}
	}

	return 0;
}
