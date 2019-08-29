#pragma once


//effects

void SpiralStream(int x, int y, int r, byte dimm) {
	auto XY = [](int x, int y) {
		if (x >= 0 and x < SCREEN_WIDTH and y >= 0 and y < SCREEN_HEIGHT)
			return x + y * SCREEN_WIDTH;
		return 0;
	};

	for (int d = r; d >= 0; d--) { // from the outside to the inside
		for (int i = x - d; i <= x + d; i++) {
			leds[XY(i, y - d)] += leds[XY(i + 1, y - d)]; // lowest row to the right
			leds[XY(i, y - d)].nscale8(dimm);
		}
		for (int i = y - d; i <= y + d; i++) {
			leds[XY(x + d, i)] += leds[XY(x + d, i + 1)]; // right colum up
			leds[XY(x + d, i)].nscale8(dimm);
		}
		for (int i = x + d; i >= x - d; i--) {
			leds[XY(i, y + d)] += leds[XY(i - 1, y + d)]; // upper row to the left
			leds[XY(i, y + d)].nscale8(dimm);
		}
		for (int i = y + d; i >= y - d; i--) {
			leds[XY(x - d, i)] += leds[XY(x - d, i - 1)]; // left colum down
			leds[XY(x - d, i)].nscale8(dimm);
		}
	}
}

