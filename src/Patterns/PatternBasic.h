#pragma once

#include "Pattern.h"


class PatternBlank : public _Pattern {
public:
	PatternBlank() : _Pattern("Blank") {}

	void start() {
		gfx.clear();
		//gfx.fillRect(5, 5, 10, 10, CRGB::White);
	}

	uint8_t drawFrame() {

		//gfx.fillRect(5, 5, 10, 10, CRGB::White);
		//shiftX(1);

		return returnVal;
	}
};

class PatternSolid : public _Pattern {
public:
	PatternSolid() : _Pattern("Solid") {}

	uint8_t drawFrame() {
		_Pattern::drawFrame();

		uint8_t val = myMap(*beat, 0, 255, 196, 255);

		gfx.fill(gfx.getColour().nscale8(val));



		return returnVal;
	}

};

class PatternRowThenCol : public _Pattern {
public:
	PatternRowThenCol() : _Pattern("Rows then Colums") {}

	uint8_t drawFrame() {
		_Pattern::drawFrame();


		//      unsigned long now = millis();

		// call once every 50msec
		static uint16_t index = 0;
		static uint8_t offset = 0;

		uint16_t nextIndex = 0;
		nextIndex = myMap(beat8(beat.getBPM() >> 10), 0, 255, 0, 128);


		uint8_t colourOffset = 64;
		//  static uint8_t hue = 0;
		CRGB colour = gfx.getColour();
		//  adjustGamma(colour);
		while (index <= nextIndex) {

			if (!_Pattern::useCustomEffect) {
				gfx.fade(40);
			}

			if (index < SCREEN_HEIGHT) { // 0 to 32
				uint8_t row = index;
				colour = gfx.getColour(row);
				//gfx.drawLine(0, index, SCREEN_WIDTH, index, colour);
				gfx.drawLine(0, index, SCREEN_WIDTH / 2, index, row, row + colourOffset);
				gfx.drawLine(SCREEN_WIDTH / 2, index, SCREEN_WIDTH, index, row + colourOffset, row);
				//ledControl.setRow(index, colour);
				//index++;
			}
			else if (index < (SCREEN_HEIGHT * 2)) { // 32 to 64
				uint8_t row = SCREEN_HEIGHT - 1 - (index - SCREEN_HEIGHT);
				colour = gfx.getColour(row);
				//gfx.drawLine(0, row, SCREEN_WIDTH, row, colour);
				gfx.drawLine(0, row, SCREEN_WIDTH / 2 - 1, row, colourOffset / 2 + index % 32, colourOffset / 2 + index % 32 + colourOffset);
				gfx.drawLine(SCREEN_WIDTH / 2, row, SCREEN_WIDTH - 1, row, colourOffset / 2 + index % 32 + colourOffset, colourOffset / 2 + index % 32);
				//ledControl.setRow(SCREEN_HEIGHT - 1 - (index - SCREEN_HEIGHT), colour);
				//index++;
			}
			else if (index < ((SCREEN_HEIGHT * 2) + SCREEN_WIDTH / 2)) { // 64 to 96
				uint8_t col = (index - (SCREEN_HEIGHT * 2) + offset) % SCREEN_WIDTH / 2;
				colour = gfx.getColour(col);
				gfx.drawLine(col, 0, col, SCREEN_HEIGHT - 1, col, col + colourOffset / 2);
				colour = gfx.getColour(col + 64);
				gfx.drawLine(col + 32, 0, col + 32, SCREEN_HEIGHT - 1, col + 64, col + 64 - colourOffset / 2);
				//ledControl.setCol(index - (SCREEN_HEIGHT * 2), colour);
				//index++;
			}
			else if (index < ((SCREEN_HEIGHT * 2) + SCREEN_WIDTH)) { // 96 to 128
				uint8_t col = (SCREEN_WIDTH / 2 - 1) - (index - SCREEN_HEIGHT * 2 - SCREEN_WIDTH / 2);
				//uint8_t col = (16 * 4 - 1) - (index - (SCREEN_HEIGHT * 2) - 8 * 4);
				col = (col + offset) % SCREEN_WIDTH;
				colour = gfx.getColour(col);
				gfx.drawLine(col, 0, col, SCREEN_HEIGHT - 1, col, col + colourOffset / 2);
				colour = gfx.getColour(col + 64);
				gfx.drawLine(col + 32, 0, col + 32, SCREEN_HEIGHT - 1, col + 64, col + 64 - colourOffset / 2);
				//ledControl.setCol((16 * 4 - 1) - (index - (SCREEN_HEIGHT * 2) - 16 * 4), colour);

				//index++;
			}
			else {
				index = 0;
				nextIndex = 0;
				offset = offset + random8(SCREEN_WIDTH / 4) - SCREEN_WIDTH / 8;
				offset %= SCREEN_WIDTH;
				break;
			}

			index++;
		}
		if (index == SCREEN_WIDTH + SCREEN_HEIGHT * 2) index = 0;
		if (index > nextIndex + 20) index = nextIndex;


		//      uint16_t offset = millis() - now;
		return returnVal;
	}
};


class PatternSparks : public _Pattern {
public:
	PatternSparks() : _Pattern("Sparks") {
		numVars = 2;
		vars[0] = &chanceRef;
		vars[1] = &fadeRef;
	}

	//variables
	uint8_t chance = 128;
	uint8_t fade = 10;

	VariableReference chanceRef{ "Chance", &chance };
	VariableReference fadeRef{ "Fade", &fade, 10, 5, 20 };

	uint8_t drawFrame() {
		_Pattern::drawFrame();

		//uint8_t chance = beatsin8(90); //128 = 50%

		chance = *beat;

		if (!_Pattern::useCustomEffect) {
			uint8_t fade = 10;
			if (chance > GuiVars1 * 128) //128 gui1
				fade = myMap(chance, GuiVars1 * 128, 255, 10, GuiVars3 * 20); //gui3 23
			gfx.fade(fade);
		}

		uint8_t maxI = 1;
		if (chance > GuiVars2 * 128) { //49 gui2
			maxI = myMap(chance, GuiVars2 * 128, 255, 0, GuiVars4 * 5); //gui4 7 or 8
		}
		for (int i = 0; i < maxI; i++) {
			//if (i == 0 or chance > 200)
			gfx.putPixel(random8(SCREEN_WIDTH), random8(SCREEN_HEIGHT), gfx.getColour(random8(64)));
		}



		return returnVal;
	}

};


class PatternVWaves : public _Pattern {
public:
	PatternVWaves() : _Pattern("Vertical Wave") {}

	virtual void start() {
		beat.halfBPM = true;
	}
	virtual void stop() {
		beat.halfBPM = false;
	}

	uint8_t drawFrame() {
		_Pattern::drawFrame();

		if (!_Pattern::useCustomEffect) {

			gfx.fade(64);
			//gfx.blur(177);
		}

		uint8_t scale = 256 / SCREEN_WIDTH;
		static uint16_t index = 0;
		const uint8_t microSteps = 16;

		CRGB colour;// = gfx.getColour();
		//CRGB colour2 = gfx.getColour(64);

		uint8_t x = 0;
		static uint16_t offset = 0;
		uint8_t width = 0;
		//uint8_t maxWidth = myMap(beatsin8(60), 0, 255, 16, 1);
		uint8_t maxWidth = myMap(*beat, 0, 255, 16, 1);

		//        for (int8_t i = 8; i > 0; i--) {

		for (uint8_t y = 0; y < SCREEN_HEIGHT; y++) {
			//    uint8_t x = quadwave8(y * 2 - (Data::index/microSteps)%256) / scale;
			x = quadwave8(y * 2 - (index / microSteps) % 256) / scale;
			//    x = (y * 2 - (Data::index/microSteps)%256) / scale;
			//    if ((Data::index/microSteps) %256 > 128)
			//      x = COLS-x;
			//    x = (x + ((Data::index / 128) % COLS)) % COLS;
			colour = gfx.getColour(y);
			width = myMap(sin8(beat8(30) + y * 4), 0, 256, 0, 16);
			width = min(width, maxWidth);
			for (int8_t w = width; w >= 0; w--) {
				colour.fadeToBlackBy((width - w) * (160 / (width + 1)) + y * 2);
				gfx.blendPixel((x + w + (offset)) % SCREEN_WIDTH, y, colour);
			}
			//                colour.fadeToBlackBy((8-i) * 20 + y * 2);
			//                gfx.blendPixel((x + i+ (offset)) % SCREEN_WIDTH, y, colour);
		}
		for (int8_t y = SCREEN_HEIGHT - 1; y >= 0; y--) {
			x = quadwave8((y + SCREEN_HEIGHT / 2) * 2 + 128 - (index / microSteps) % 256) / scale;
			//    x = (x + ((Data::index / 128) % COLS)) % COLS;
			colour = gfx.getColour(64 + y);
			width = myMap(sin8(beat8(30) + y * 4), 0, 256, 0, 16);
			width = min(width, maxWidth);
			for (int8_t w = width; w >= 0; w--) {
				colour.fadeToBlackBy((width - w) * (160 / (width + 1)) + y * 2);
				gfx.blendPixel((x + w + (offset)) % SCREEN_WIDTH, y, colour);
			}
			//                colour.fadeToBlackBy((8 - i) * 20 + y *2);
			//                gfx.blendPixel((x + i + (offset)) % SCREEN_WIDTH, y, colour);
		}
		//colour.fadeToBlackBy(60);
		//colour.fadeToBlackBy(60);

//        }

		//offset += 1;
		offset %= SCREEN_WIDTH;
		index += microSteps;

		return returnVal;
	}


};



enum {
	LV_IMG_CF_UNKNOWN = 0,

	LV_IMG_CF_RAW,              /**< Contains the file as it is. Needs custom decoder function*/
	LV_IMG_RGB888,
	LV_IMG_RGB565,


	LV_IMG_CF_INDEXED_1BIT, /**< Can have 2 different colors in a palette (always chroma keyed)*/
	LV_IMG_CF_INDEXED_2BIT, /**< Can have 4 different colors in a palette (always chroma keyed)*/
	LV_IMG_CF_INDEXED_4BIT, /**< Can have 16 different colors in a palette (always chroma keyed)*/
	LV_IMG_CF_INDEXED_8BIT, /**< Can have 256 different colors in a palette (always chroma keyed)*/

	LV_IMG_CF_ALPHA_1BIT, /**< Can have one color and it can be drawn or not*/
	LV_IMG_CF_ALPHA_2BIT, /**< Can have one color but 4 different alpha value*/
	LV_IMG_CF_ALPHA_4BIT, /**< Can have one color but 16 different alpha value*/
	LV_IMG_CF_ALPHA_8BIT, /**< Can have one color but 256 different alpha value*/

};
typedef uint8_t lv_img_cf_t;

typedef struct
{

	/* The first 8 bit is very important to distinguish the different source types.
	 * For more info see `lv_img_get_src_type()` in lv_img.c */
	uint32_t cf : 5;          /* Color format: See `lv_img_color_format_t`*/
	uint32_t always_zero : 3; /*It the upper bits of the first byte. Always zero to look like a
								 non-printable character*/
	uint32_t reserved : 2; /*Reserved to be used later*/

	uint32_t w : 11; /*Width of the image map*/
	uint32_t h : 11; /*Height of the image map*/
} lv_img_header_t;

typedef struct
{
	lv_img_header_t header;
	uint32_t data_size;
	const uint8_t * data;
} lv_img_dsc_t;


// https://littlevgl.com/image-to-c-array

const uint8_t fish_map[] = {
  0x04, 0x02, 0x04, 0xff, 	/*Color of index 0*/
  0x3c, 0x7e, 0xfc, 0xff, 	/*Color of index 1*/
  0x34, 0x2e, 0xbc, 0xff, 	/*Color of index 2*/
  0x3c, 0xbe, 0xfc, 0xff, 	/*Color of index 3*/

  0x00, 0xf8, 0x00, 0x00,
  0x00, 0x2e, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00,
  0x80, 0x0a, 0x60, 0x00,
  0xe0, 0x3a, 0x9c, 0x00,
  0x28, 0xfe, 0x97, 0x00,
  0x3b, 0xfa, 0x9b, 0xc0,
  0x3b, 0xfa, 0x97, 0xc0,
  0x28, 0xfe, 0x97, 0x00,
  0xe0, 0x3a, 0x9c, 0x00,
  0x80, 0x0a, 0x60, 0x00,
  0x00, 0x0f, 0x80, 0x00,
  0x00, 0x2e, 0x00, 0x00,
  0x00, 0xf8, 0x00, 0x00,
};






CRGB getPixelColor(lv_img_dsc_t *dsc, uint16_t x, uint16_t y, uint8_t *palette = NULL) {

	CRGB p_color = CRGB::Black;

	uint8_t * buf_u8 = (uint8_t *)dsc->data;

	if (dsc->header.cf == LV_IMG_RGB888) {
		uint8_t px_size = 3;
		uint32_t px = dsc->header.w * y * px_size + x * px_size;
		p_color.r = buf_u8[px];
		p_color.g = buf_u8[px + 1];
		p_color.b = buf_u8[px + 2];
		//memcpy(&p_color.raw, &buf_u8[px],3);
	}
	else if (dsc->header.cf == LV_IMG_RGB565) {
		uint8_t px_size = 2;
		uint32_t px = dsc->header.w * y * px_size + x * px_size;
		uint16_t pxData = buf_u8[px] << 8 | buf_u8[px + 1];
		p_color = CRGB(pgm_read_byte(&gamma5[pxData >> 11]),
			pgm_read_byte(&gamma6[(pxData >> 5) & 0x3F]),
			pgm_read_byte(&gamma5[pxData & 0x1F]));
	}
	else {
		if (dsc->header.cf == LV_IMG_CF_INDEXED_1BIT) {
			buf_u8 += 4 * 2;
			uint8_t bit = x & 0x7;
			x = x >> 3;

			/* Get the current pixel.
			 * dsc->header.w + 7 means rounding up to 8 because the lines are byte aligned
			 * so the possible real width are 8, 16, 24 ...*/
			uint32_t px = ((dsc->header.w + 7) >> 3) * y + x;
			uint8_t i = (buf_u8[px] & (1 << (7 - bit))) >> (7 - bit);
			if (palette) {
				p_color = CRGB{ palette[0 + i * 3], palette[1 + i * 3], palette[2 + i * 3] };
			}
			else p_color = CRGB{ dsc->data[2 + i * 4], dsc->data[1 + i * 4], dsc->data[0 + i * 4] };
		}
		else if (dsc->header.cf == LV_IMG_CF_INDEXED_2BIT) {
			buf_u8 += 4 * 4;
			uint8_t bit = (x & 0x3) * 2;
			x = x >> 2;

			/* Get the current pixel.
			 * dsc->header.w + 3 means rounding up to 4 because the lines are byte aligned
			 * so the possible real width are 4, 8, 12 ...*/
			uint32_t px = ((dsc->header.w + 3) >> 2) * y + x;
			//CRGB c[4];
			//for (uint8_t i = 0; i < 4; i++) {
			//	c[0] = CRGB{ dsc->data[2 + i*4], dsc->data[1 + i * 4], dsc->data[0 + i * 4] };
			//}
			uint8_t i = (buf_u8[px] & (3 << (6 - bit))) >> (6 - bit); // should be 0 to 3
			if (palette) {
				p_color = CRGB{ palette[0 + i * 3], palette[1 + i * 3], palette[2 + i * 3] };
			}
			else p_color = CRGB{ dsc->data[2 + i * 4], dsc->data[1 + i * 4], dsc->data[0 + i * 4] };
		}
		else if (dsc->header.cf == LV_IMG_CF_INDEXED_4BIT) {
			buf_u8 += 4 * 16;
			uint8_t bit = (x & 0x1) * 4;
			x = x >> 1;

			/* Get the current pixel.
			 * dsc->header.w + 1 means rounding up to 2 because the lines are byte aligned
			 * so the possible real width are 2, 4, 6 ...*/
			uint32_t px = ((dsc->header.w + 1) >> 1) * y + x;
			uint8_t i = (buf_u8[px] & (0xF << (4 - bit))) >> (4 - bit);
			if (palette) {
				p_color = CRGB{ palette[0 + i * 3], palette[1 + i * 3], palette[2 + i * 3] };
			}
			else p_color = CRGB{ dsc->data[2 + i * 4], dsc->data[1 + i * 4], dsc->data[0 + i * 4] };
		}
		else if (dsc->header.cf == LV_IMG_CF_INDEXED_8BIT) {
			buf_u8 += 4 * 256;
			uint32_t px = dsc->header.w * y + x;
			uint8_t i = buf_u8[px];
			p_color = CRGB{ dsc->data[2 + i * 4], dsc->data[1 + i * 4], dsc->data[0 + i * 4] };
		}
	}

	return p_color;
}

void drawBitmap(lv_img_dsc_t *dsc, uint16_t x, int16_t y, bool flip, uint8_t *palette = NULL) {

	const uint16_t w = dsc->header.w;
	const uint16_t h = dsc->header.h;
	CRGB c;

	for (uint8_t j = 0; j < h; j++) {
		for (uint8_t i = 0; i < w; i++) {
			uint8_t ii = i;
			if (flip)
				ii = w - i;
			c = getPixelColor(dsc, ii, j, palette);
			if (c.r < 5 and c.g < 5 and c.b < 5) {}
			else
			{
				if (y + j > 0)
					gfx.putPixel((x + i) % SCREEN_WIDTH, y + j, c);
			}
		}
	}

}

//CRGB getPixelColor(img_dsc_t *dsc, uint8_t x, uint8_t y) {
//	uint8_t * buf_u8 = (uint8_t *)dsc->data;
//
//	if (dsc->header.cf == LV_IMG_CF_INDEXED_1BIT) {
//		buf_u8 += 4 * 2;
//		uint8_t bit = x & 0x7;
//		x = x >> 3;
//
//		/* Get the current pixel.
//		 * dsc->header.w + 7 means rounding up to 8 because the lines are byte aligned
//		 * so the possible real width are 8, 16, 24 ...*/
//		uint32_t px = ((dsc->header.w + 7) >> 3) * y + x;
//		p_color.full = (buf_u8[px] & (1 << (7 - bit))) >> (7 - bit);
//	}
//	else if (dsc->header.cf == LV_IMG_CF_INDEXED_2BIT) {
//		buf_u8 += 4 * 4;
//		uint8_t bit = (x & 0x3) * 2;
//		x = x >> 2;
//
//		/* Get the current pixel.
//		 * dsc->header.w + 3 means rounding up to 4 because the lines are byte aligned
//		 * so the possible real width are 4, 8, 12 ...*/
//		uint32_t px = ((dsc->header.w + 3) >> 2) * y + x;
//		p_color.full = (buf_u8[px] & (3 << (6 - bit))) >> (6 - bit);
//	}
//	else if (dsc->header.cf == LV_IMG_CF_INDEXED_4BIT) {
//		buf_u8 += 4 * 16;
//		uint8_t bit = (x & 0x1) * 4;
//		x = x >> 1;
//
//		/* Get the current pixel.
//		 * dsc->header.w + 1 means rounding up to 2 because the lines are byte aligned
//		 * so the possible real width are 2, 4, 6 ...*/
//		uint32_t px = ((dsc->header.w + 1) >> 1) * y + x;
//		p_color.full = (buf_u8[px] & (0xF << (4 - bit))) >> (4 - bit);
//	}
//	else if (dsc->header.cf == LV_IMG_CF_INDEXED_8BIT) {
//		buf_u8 += 4 * 256;
//		uint32_t px = dsc->header.w * y + x;
//		p_color.full = buf_u8[px];
//}


//https://littlevgl.com/image-to-c-array



class PatternFish : public _Pattern {
public:
	PatternFish() : _Pattern("Fishy") {


		fish.header.always_zero = 0;
		fish.header.w = 13;
		fish.header.h = 14;
		fish.data_size = 72;
		fish.header.cf = LV_IMG_CF_INDEXED_2BIT;
		fish.data = fish_map;

		for (uint8_t i = 0; i < numFish; i++) {
			fishX[i] = random8(SCREEN_WIDTH);
			fishY[i] = random8(0, fishYMax);
			fishCol[i] = i % 4;
			fishDir[i] = (i % 2);

			fish_p[i].pos = Vec3{ (float)fishX[i], (float)fishY[i], 0.0f };
			fish_p[i].vel = Vec3{ 0,0,0 };
			fish_p[i].acc = Vec3{ 0,0,0 };
		}
	}



	lv_img_dsc_t fish;

	static const uint8_t numFish = 8;
	const int8_t fishYMax = 32 - 14 + 6;
	const int8_t fishYMin = -6;
	uint8_t fishX[numFish];
	int8_t fishY[numFish];
	uint8_t fishCol[numFish];
	bool fishDir[numFish];

	Particle fish_p[numFish];

	uint8_t drawFrame() {
		if (!_Pattern::useCustomEffect) {
			gfx.fade(64);
			//gfx.blur(177);
		}
		for (uint8_t i = 0; i < numFish; i++) {
			moveFish(i);
			seperateFish();
			drawFish(i);
		}

		return returnVal;
	}

	void seperateFish() {
		float radius = 20.0f;// GuiVars1 * 20.0f;
		float maxSpeed = 0.084f; // GuiVars2 * 0.2f;
		float maxForce = 0.00033f;// GuiVars3 * 0.003f;

		for (Particle& p : fish_p) {
			Vec3 cohesion = Vec3{ 0,0,0 },
				alignment = Vec3{ 0,0,0 },
				separation = Vec3{ 0,0,0 };
			uint8_t count = 0;
			for (Particle& other : fish_p) {
				if (&other == &p) continue;
				float d = Particle::distance(p, other);
				if (d < radius) {
					//cohesion
					cohesion += other.pos;
					alignment += other.vel;
					Vec3 pos = p.pos - other.pos;
					pos.Normalize();
					pos *= (radius - d) / radius;
					separation += pos;
					count++;
				}
			}
			if (count) {
				separation /= count;
				separation.Normalize();
				separation *= maxSpeed;
				separation -= p.vel;
				separation.Limit(maxForce);

				alignment /= count;
				alignment.Normalize();
				alignment *= maxSpeed;
				alignment -= p.vel;
				alignment.Limit(maxForce);

				cohesion /= count;
				cohesion -= p.pos;
				cohesion.Normalize();
				cohesion *= maxSpeed;
				cohesion -= p.vel;
				cohesion.Limit(maxForce);

				//apply forces & weight
				cohesion *= 0.64f;
				alignment *= 1.05f;
				separation *= 1.4f;
				p.applyForce(cohesion);
				p.applyForce(alignment);
				p.applyForce(separation);
			}
		}


	}

	void moveFish(uint8_t i) {
		if (random8(20) == 0) {
			//fishX[i] = (SCREEN_WIDTH + fishX[i] + (fishDir[i] ? -1 : 1)) % SCREEN_WIDTH;
			fish_p[i].pos.x += (fishDir[i] ? -1 : 1);
			if (fish_p[i].pos.x > SCREEN_WIDTH)
				fish_p[i].pos.x -= SCREEN_WIDTH;
			if (fish_p[i].pos.x < 0)
				fish_p[i].pos.x += SCREEN_WIDTH;
		}
		if (random8(255) == 0)
			fishDir[i] = !fishDir[i];

		if (random8(40) == 0) {
			if (random8(2)) {
				//if (fishY[i] < fishYMax)
				//	fishY[i]++;
				if (fish_p[i].pos.y < fishYMax) {
					fish_p[i].pos.y++;
				}
			}
			else {
				//if (fishY[i] > fishYMin) {
				//	fishY[i]--;
				//}
				if (fish_p[i].pos.y > fishYMin) {
					fish_p[i].pos.y--;
				}
			}

		}
		fish_p[i].vel *= 0.9;
		//if (fish_p[i].acc.Len() > 0.1f) {
		//	fish_p[i].acc.Normalize();
		//	fish_p[i].acc *= 0.1f;
		//}
		if (fish_p[i].pos.y < 9)
			fish_p[i].acc += 0.0001f;
		if (fish_p[i].pos.y > 23)
			fish_p[i].acc -= 0.0001f;
		fish_p[i].vel += fish_p[i].acc;// *0.05f;
		//if (fish_p[i].vel.Len() > 0.1f) {
		//	fish_p[i].vel.Normalize();
		//	fish_p[i].vel *= 0.1f;
		//}
		fish_p[i].pos += fish_p[i].vel;
		if (fish_p[i].pos.y < fishYMin) {
			fish_p[i].pos.y = fishYMin;
		}

		if (fish_p[i].pos.y > fishYMax) {
			fish_p[i].pos.y = fishYMax;
		}

		fish_p[i].acc *= 0;

	}

	void drawFish(uint8_t i) {
		//drawFish(fishX[i], fishY[i], fishCol[i], fishDir[i]);
		bool f = true;
		if (fish_p[i].vel.x > 0)
			f = false;
		drawFish(fish_p[i].pos.x, fish_p[i].pos.y, fishCol[i], fishDir[i]);
	}

	void drawFish(uint8_t x, int8_t y, uint8_t c, bool flip = false) {
		CRGBPalette16 p = { CRGB(0,0,0), CRGB(252,126,60), CRGB(188, 46, 52), CRGB(252, 190, 62), // orange, red, yellow, or 
			CRGB(0,0,0), CRGB(19,190,74), CRGB(62, 66, 255), CRGB(150, 225, 155),
			CRGB(0,0,0), CRGB(237,253,115), CRGB(120, 227, 100), CRGB(53, 156, 86),
			CRGB(0,0,0), CRGB(123,161,255), CRGB(163, 60, 170), CRGB(137, 129, 247)
		};
		c = constrain(c, 0, 3);
		drawBitmap(&fish, x, y, flip, &p.entries[c * 4].raw[0]);
		//gfx.drawRGBBitmap(x, y, &fish.pixel_data[0], fish.width, fish.height);
	}


};