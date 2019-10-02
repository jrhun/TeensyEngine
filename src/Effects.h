#pragma once


//effects

#include "Noise.h"

int _XY(int x, int y) {
	if (x >= 0 and x < SCREEN_WIDTH and y >= 0 and y < SCREEN_HEIGHT)
		return x + y * SCREEN_WIDTH;
	return 0;

}




void SpiralStream(int x, int y, int r, byte dimm) {

	for (int d = r; d >= 0; d--) { // from the outside to the inside
		for (int i = x - d; i <= x + d; i++) {
			leds[_XY(i, y - d)] += leds[_XY(i + 1, y - d)]; // lowest row to the right
			leds[_XY(i, y - d)].nscale8(dimm);
		}
		for (int i = y - d; i <= y + d; i++) {
			leds[_XY(x + d, i)] += leds[_XY(x + d, i + 1)]; // right colum up
			leds[_XY(x + d, i)].nscale8(dimm);
		}
		for (int i = x + d; i >= x - d; i--) {
			leds[_XY(i, y + d)] += leds[_XY(i - 1, y + d)]; // upper row to the left
			leds[_XY(i, y + d)].nscale8(dimm);
		}
		for (int i = y + d; i >= y - d; i--) {
			leds[_XY(x - d, i)] += leds[_XY(x - d, i - 1)]; // left colum down
			leds[_XY(x - d, i)].nscale8(dimm);
		}
	}
}

void MoveX(byte delta) {
	CRGB leds2[SCREEN_HEIGHT * SCREEN_WIDTH];
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH - delta; x++) {
			leds2[_XY(x, y)] = leds[_XY(x + delta, y)];
		}
		for (int x = SCREEN_WIDTH - delta; x < SCREEN_WIDTH; x++) {
			leds2[_XY(x, y)] = leds[_XY(x + delta - SCREEN_WIDTH, y)];
		}
	}

	// write back to leds
	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
			leds[_XY(x, y)] = leds2[_XY(x, y)];
		}
	}
}

void MoveY(byte delta) {
	CRGB leds2[SCREEN_HEIGHT * SCREEN_WIDTH];
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT - delta; y++) {
			leds2[_XY(x, y)] = leds[_XY(x, y + delta)];
		}
		for (int y = SCREEN_HEIGHT - delta; y < SCREEN_HEIGHT; y++) {
			leds2[_XY(x, y)] = leds[_XY(x, y + delta - SCREEN_HEIGHT)];
		}
	}

	// write back to leds
	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
		for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
			leds[_XY(x, y)] = leds2[_XY(x, y)];
		}
	}
}

void MoveFractionalNoiseX(byte amt = 16) {
	// move delta pixelwise
	CRGB leds2[SCREEN_HEIGHT * SCREEN_WIDTH];
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		uint16_t amount = noise.At(0,y) * amt;
		byte delta = 31 - (amount / 256);

		for (int x = 0; x < SCREEN_WIDTH - delta; x++) {
			leds2[_XY(x, y)] = leds[_XY(x + delta, y)];
		}
		for (int x = SCREEN_WIDTH - delta; x < SCREEN_WIDTH; x++) {
			leds2[_XY(x, y)] = leds[_XY(x + delta - SCREEN_WIDTH, y)];
		}
	}

	//move fractions
	CRGB PixelA;
	CRGB PixelB;

	for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
		uint16_t amount = noise.At(0, y) * amt;
		byte delta = 31 - (amount / 256);
		byte fractions = amount - (delta * 256);

		for (uint8_t x = 1; x < SCREEN_WIDTH; x++) {
			PixelA = leds2[_XY(x, y)];
			PixelB = leds2[_XY(x - 1, y)];

			PixelA %= 255 - fractions;
			PixelB %= fractions;

			leds[_XY(x, y)] = PixelA + PixelB;
		}

		PixelA = leds2[_XY(0, y)];
		PixelB = leds2[_XY(SCREEN_WIDTH - 1, y)];

		PixelA %= 255 - fractions;
		PixelB %= fractions;

		leds[_XY(0, y)] = PixelA + PixelB;
	}
}

void MoveFractionalNoiseY(byte amt = 16) {
	// move delta pixelwise
	CRGB leds2[SCREEN_HEIGHT * SCREEN_WIDTH];
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		uint16_t amount = noise.At(x,0) * amt;
		byte delta = 31 - (amount / 256);

		for (int y = 0; y < SCREEN_WIDTH - delta; y++) {
			leds2[_XY(x, y)] = leds[_XY(x, y + delta)];
		}
		for (int y = SCREEN_WIDTH - delta; y < SCREEN_WIDTH; y++) {
			leds2[_XY(x, y)] = leds[_XY(x, y + delta - SCREEN_WIDTH)];
		}
	}

	//move fractions
	CRGB PixelA;
	CRGB PixelB;

	for (uint8_t x = 0; x < SCREEN_HEIGHT; x++) {
		uint16_t amount = noise.At(x, 0) * amt;
		byte delta = 31 - (amount / 256);
		byte fractions = amount - (delta * 256);

		for (uint8_t y = 1; y < SCREEN_WIDTH; y++) {
			PixelA = leds2[_XY(x, y)];
			PixelB = leds2[_XY(x, y - 1)];

			PixelA %= 255 - fractions;
			PixelB %= fractions;

			leds[_XY(x, y)] = PixelA + PixelB;
		}

		PixelA = leds2[_XY(x, 0)];
		PixelB = leds2[_XY(x, SCREEN_WIDTH - 1)];

		PixelA %= 255 - fractions;
		PixelB %= fractions;

		leds[_XY(x, 0)] = PixelA + PixelB;
	}
}

void standardNoiseSmearing() {
	noise.xOffset += 1000;
	noise.yOffset += 1000;
	noise.xScale = 4000;
	noise.yScale = 4000;
	noise.fillNoise();

	MoveX(1);
	MoveFractionalNoiseY(2);

	MoveY(1);
	MoveFractionalNoiseX(2);
}


//flow field
//https://web.archive.org/web/20160418004455/http://freespace.virgin.net/hugo.elias/graphics/feedback.cpp


const int OFFSET_X = 6;
const int OFFSET_Y = 4;
struct PARTICLE
{
	float x = 0, y = 0, xv = 0, yv = 0, xint = 0, yint = 0, xacc = 0, yacc = 0, c =0 ;
}offset[OFFSET_X][OFFSET_Y];



inline void TextureBlock(int xo, int yo)
{

	float VLDx, VRDx, HDx;
	float VLDy, VRDy, HDy;

	int  TX1, TY1, TX2, TY2, tx, ty;
	int x, y, xi = (xo << 4), yi = (yo << 4);
	//float a, b, c, d;
	CRGB *Tptr;

	Tptr = &(leds[xi + (yi * SCREEN_WIDTH)]);

	VLDx = (offset[xo][yo + 1].xint - offset[xo][yo].xint) / 16;
	VRDx = (offset[xo + 1][yo + 1].xint - offset[xo + 1][yo].xint) / 16;
	VLDy = (offset[xo][yo + 1].yint - offset[xo][yo].yint) / 16;
	VRDy = (offset[xo + 1][yo + 1].yint - offset[xo + 1][yo].yint) / 16;

	TX1 = int(offset[xo][yo].xint);
	TY1 = int(offset[xo][yo].yint);
	TX2 = int(offset[xo + 1][yo].xint);
	TY2 = int(offset[xo + 1][yo].yint);

	for (y = yi; y < (yi + 16); y++)
	{
		HDx = (TX2 - TX1) >> 4;
		HDy = ((TY2 - TY1) >> 4);
		tx = TX1;
		ty = TY1;

		for (x = xi; x < (xi + 16); x++)
		{
			*Tptr++ = leds[int(tx) + SCREEN_WIDTH * int(ty)];
			tx += HDx;
			ty += HDy;
		}

		Tptr += (SCREEN_HEIGHT - 16);

		TX1 += VLDx;
		TY1 += VLDy;
		TX2 += VRDx;
		TY2 += VRDy;
	}
}


void flowMap() {


	int x, y, xi, yi, xh, xl, yh, yl;
	float Xspring, Yspring, length, norm, scaler;
	//float xv, yv, ext;

	// firstly calculate the forces on all points
	for (y = 1; y < OFFSET_Y - 1; y++)
	{
		yh = y + 2;
		yl = y - 2;
		if (yl < 0)	yl = 0;
		if (yh > OFFSET_Y - 1)	yh = OFFSET_Y - 1;

		for (x = 1; x < OFFSET_X; x++)
		{
			xh = x + 2;
			xl = x - 2;
			if (xl < 0)	xl = 0;
			if (xh > OFFSET_X - 1)	xh = OFFSET_X - 1;

			for (yi = yl; yi <= yh; yi++)
			{
				for (xi = xl; xi <= xh; xi++)
				{
					if ((xi != x) || (yi != y))
					{
						Xspring = offset[xi][yi].x - offset[x][y].x;
						Yspring = offset[xi][yi].y - offset[x][y].y;

						norm = sqrt(abs(xi - x)*abs(xi - x) + abs(yi - y)*abs(yi - y));
						length = sqrt(Xspring*Xspring + Yspring * Yspring);
						scaler = (norm - length);// *0.00002;

						Xspring *= scaler;
						Yspring *= scaler;

						offset[xi][yi].xv += Xspring;
						offset[xi][yi].yv += Yspring;
					}
				}
			}
		}
	}

	// now adjust the positions and velocities of the points
	for (y = 1; y < OFFSET_Y - 1; y++)
		for (x = 1; x < OFFSET_X - 1; x++)
		{
			if (!((x == 0) || (x == OFFSET_X - 1) || (y == 0) || (y == OFFSET_Y - 1)))
			{
				offset[x][y].x += offset[x][y].xv;
				offset[x][y].y += offset[x][y].yv;
				offset[x][y].xv *= 0.9999;
				offset[x][y].yv *= 0.9999;
			}

			offset[x][y].xint = trunc(offset[x][y].x);
			offset[x][y].yint = trunc(offset[x][y].y);


			// this bit allows it to handle non-integer values of points on the
			// grid
			//offset[x][y].xacc += fracof2(offset[x][y].x);
			//if (offset[x][y].xacc > (1))
			//{
			//	offset[x][y].xacc -= (1);
			//	offset[x][y].xint += (1);
			//}
			//if (offset[x][y].xacc < (0))
			//{
			//	offset[x][y].xacc += (1);
			//	offset[x][y].xint -= (1);
			//}

			//offset[x][y].yacc += fracof2(offset[x][y].y);
			//if (offset[x][y].yacc > (1))
			//{
			//	offset[x][y].yacc -= (1);
			//	offset[x][y].yint += (1);
			//}
			//if (offset[x][y].yacc < (0))
			//{
			//	offset[x][y].yacc += (1);
			//	offset[x][y].yint -= (1);
			//}

		}


	// Make sure points on the grid never stray outside the screen, causing a crash
	for (x = 0; x <= OFFSET_X - 1; x++)
	{
		if (offset[x][1].yint < (0))
			offset[x][1].yint = (0);
		if (offset[x][11].yint > (SCREEN_WIDTH))
			offset[x][11].yint = (SCREEN_WIDTH);
	}
	for (y = 0; y <= OFFSET_Y - 1; y++)
	{
		if (offset[1][y].xint < (0))
			offset[1][y].xint = (0);
		if (offset[19][y].xint > (SCREEN_HEIGHT))
			offset[19][y].xint = (SCREEN_HEIGHT);
	}

	float yf;

	yf = (0);
	for (y = 0; y <= OFFSET_Y - 1; y++)
	{
		for (x = 0; x <= OFFSET_X - 1; x++)
			TextureBlock(x, y);
	}

}


void rotateCW(float angle) {

}