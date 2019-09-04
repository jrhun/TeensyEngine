#pragma once


// some ported fastled functions for use on PC emulation 

typedef uint8_t   fract8;   ///< ANSI: unsigned short _Fract
typedef int8_t    sfract7;  ///< ANSI: signed   short _Fract
typedef uint16_t  fract16;  ///< ANSI: unsigned       _Fract
typedef int16_t   sfract15; ///< ANSI: signed         _Fract
typedef uint16_t  accum88;  ///< ANSI: unsigned short _Accum.  8 bits int, 8 bits fraction
typedef int16_t   saccum78; ///< ANSI: signed   short _Accum.  7 bits int, 8 bits fraction
typedef uint32_t  accum1616;///< ANSI: signed         _Accum. 16 bits int, 16 bits fraction
typedef int32_t   saccum1516;///< ANSI: signed         _Accum. 15 bits int, 16 bits fraction
typedef uint16_t  accum124; ///< no direct ANSI counterpart. 12 bits int, 4 bits fraction
typedef int32_t   saccum114;///< no direct ANSI counterpart. 1 bit int, 14 bits fraction
typedef uint8_t	  byte;

/// typedef for IEEE754 "binary32" float type internals
typedef union {
	uint32_t i;
	float    f;
	struct {
		uint32_t mantissa : 23;
		uint32_t exponent : 8;
		uint32_t signbit : 1;
	};
	struct {
		uint32_t mant7 : 7;
		uint32_t mant16 : 16;
		uint32_t exp_ : 8;
		uint32_t sb_ : 1;
	};
	struct {
		uint32_t mant_lo8 : 8;
		uint32_t mant_hi16_exp_lo1 : 16;
		uint32_t sb_exphi7 : 8;
	};
} IEEE754binary32_t;

#define FASTLED_RAND16_2053  ((uint16_t)(2053))
#define FASTLED_RAND16_13849 ((uint16_t)(13849))

uint16_t rand16seed = 1654;

uint8_t random8()
{
	rand16seed = (rand16seed * FASTLED_RAND16_2053) + FASTLED_RAND16_13849;
	// return the sum of the high and low bytes, for better
	//  mixing and non-sequential correlation
	return (uint8_t)(((uint8_t)(rand16seed & 0xFF)) +
		((uint8_t)(rand16seed >> 8)));
}

/// Generate a 16 bit random number
uint16_t random16()
{
	rand16seed = (rand16seed * FASTLED_RAND16_2053) + FASTLED_RAND16_13849;
	return rand16seed;
}

/// Generate an 8-bit random number between 0 and lim
/// @param lim the upper bound for the result
uint8_t random8(uint8_t lim)
{
	uint8_t r = random8();
	r = (r*lim) >> 8;
	return r;
}

/// Generate an 8-bit random number in the given range
/// @param min the lower bound for the random number
/// @param lim the upper bound for the random number
uint8_t random8(uint8_t min, uint8_t lim)
{
	uint8_t delta = lim - min;
	uint8_t r = random8(delta) + min;
	return r;
}

/// Generate an 16-bit random number between 0 and lim
/// @param lim the upper bound for the result
uint16_t random16(uint16_t lim)
{
	uint16_t r = random16();
	uint32_t p = (uint32_t)lim * (uint32_t)r;
	r = p >> 16;
	return r;
}

/// Generate an 16-bit random number in the given range
/// @param min the lower bound for the random number
/// @param lim the upper bound for the random number
uint16_t random16(uint16_t min, uint16_t lim)
{
	uint16_t delta = lim - min;
	uint16_t r = random16(delta) + min;
	return r;
}

/// Set the 16-bit seed used for the random number generator
void random16_set_seed(uint16_t seed)
{
	rand16seed = seed;
}

/// Get the current seed value for the random number generator
uint16_t random16_get_seed()
{
	return rand16seed;
}

/// Add entropy into the random number generator
void random16_add_entropy(uint16_t entropy)
{
	rand16seed += entropy;
}


//SCALE8
uint8_t scale8(uint8_t i, fract8 scale)
{
	return (((uint16_t)i) * (1 + (uint16_t)(scale))) >> 8;
}

uint8_t scale8_video(uint8_t i, fract8 scale)
{
	uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale) ? 1 : 0);
	return j;
}


uint8_t scale8_LEAVING_R1_DIRTY(uint8_t i, fract8 scale)
{
	return (((uint16_t)i) * ((uint16_t)(scale)+1)) >> 8;
}

void nscale8_LEAVING_R1_DIRTY(uint8_t& i, fract8 scale)
{
	i = (((uint16_t)i) * ((uint16_t)(scale)+1)) >> 8;
}

uint8_t scale8_video_LEAVING_R1_DIRTY(uint8_t i, fract8 scale)
{
	uint8_t j = (((int)i * (int)scale) >> 8) + ((i&&scale) ? 1 : 0);
	return j;
}

void nscale8_video_LEAVING_R1_DIRTY(uint8_t & i, fract8 scale)
{
	i = (((int)i * (int)scale) >> 8) + ((i&&scale) ? 1 : 0);
}

void cleanup_R1() {}

void nscale8x3(uint8_t& r, uint8_t& g, uint8_t& b, fract8 scale)
{
	uint16_t scale_fixed = scale + 1;
	r = (((uint16_t)r) * scale_fixed) >> 8;
	g = (((uint16_t)g) * scale_fixed) >> 8;
	b = (((uint16_t)b) * scale_fixed) >> 8;
}

void nscale8x3_video(uint8_t& r, uint8_t& g, uint8_t& b, fract8 scale)
{
	uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
	r = (r == 0) ? 0 : (((int)r * (int)(scale)) >> 8) + nonzeroscale;
	g = (g == 0) ? 0 : (((int)g * (int)(scale)) >> 8) + nonzeroscale;
	b = (b == 0) ? 0 : (((int)b * (int)(scale)) >> 8) + nonzeroscale;
}

void nscale8x2(uint8_t& i, uint8_t& j, fract8 scale)
{
	uint16_t scale_fixed = scale + 1;
	i = (((uint16_t)i) * scale_fixed) >> 8;
	j = (((uint16_t)j) * scale_fixed) >> 8;
}

void nscale8x2_video(uint8_t& i, uint8_t& j, fract8 scale)
{
	uint8_t nonzeroscale = (scale != 0) ? 1 : 0;
	i = (i == 0) ? 0 : (((int)i * (int)(scale)) >> 8) + nonzeroscale;
	j = (j == 0) ? 0 : (((int)j * (int)(scale)) >> 8) + nonzeroscale;
}

uint16_t scale16by8(uint16_t i, fract8 scale)
{
	return uint16_t((i * (1 + ((uint16_t)scale))) >> 8);
}

uint16_t scale16(uint16_t i, fract16 scale)
{
	uint16_t result;
	result = ((uint32_t)(i) * (1 + (uint32_t)(scale))) / 65536;
	return result;
}

uint8_t dim8_raw(uint8_t x)
{
	return scale8(x, x);
}

/// Adjust a scaling value for dimming for video (value will never go below 1)
uint8_t dim8_video(uint8_t x)
{
	return scale8_video(x, x);
}

/// Linear version of the dimming function that halves for values < 128
uint8_t dim8_lin(uint8_t x)
{
	if (x & 0x80) {
		x = scale8(x, x);
	}
	else {
		x += 1;
		x /= 2;
	}
	return x;
}

/// inverse of the dimming function, brighten a value
uint8_t brighten8_raw(uint8_t x)
{
	uint8_t ix = 255 - x;
	return 255 - scale8(ix, ix);
}

/// inverse of the dimming function, brighten a value
uint8_t brighten8_video(uint8_t x)
{
	uint8_t ix = 255 - x;
	return 255 - scale8_video(ix, ix);
}

/// inverse of the dimming function, brighten a value
uint8_t brighten8_lin(uint8_t x)
{
	uint8_t ix = 255 - x;
	if (ix & 0x80) {
		ix = scale8(ix, ix);
	}
	else {
		ix += 1;
		ix /= 2;
	}
	return 255 - ix;
}


// MATH8
uint8_t qadd8(uint8_t i, uint8_t j)
{
	unsigned int t = i + j;
	if (t > 255) t = 255;
	return t;
}

int8_t qadd7(int8_t i, int8_t j)
{
	int16_t t = i + j;
	if (t > 127) t = 127;
	return t;
}
uint8_t qsub8(uint8_t i, uint8_t j)
{
	int t = i - j;
	if (t < 0) t = 0;
	return t;
}
uint8_t add8(uint8_t i, uint8_t j)
{
	int t = i + j;
	return t;
}
uint16_t add8to16(uint8_t i, uint16_t j)
{
	uint16_t t = i + j;
	return t;
}

uint8_t sub8(uint8_t i, uint8_t j)
{
	int t = i - j;
	return t;
}

uint8_t avg8(uint8_t i, uint8_t j)
{
	return (i + j) >> 1;
}

uint16_t avg16(uint16_t i, uint16_t j)
{
	return (uint32_t)((uint32_t)(i)+(uint32_t)(j)) >> 1;
}

int8_t avg7(int8_t i, int8_t j)
{
	return ((i + j) >> 1) + (i & 0x1);
}

int16_t avg15(int16_t i, int16_t j)
{
	return ((int32_t)((int32_t)(i)+(int32_t)(j)) >> 1) + (i & 0x1);
}

uint8_t mod8(uint8_t a, uint8_t m)
{
	while (a >= m) a -= m;
	return a;
}

uint8_t addmod8(uint8_t a, uint8_t b, uint8_t m)
{
	a += b;
	while (a >= m) a -= m;
	return a;
}

uint8_t submod8(uint8_t a, uint8_t b, uint8_t m)
{
	a -= b;
	while (a >= m) a -= m;
	return a;
}

uint8_t mul8(uint8_t i, uint8_t j)
{
	return ((int)i * (int)(j)) & 0xFF;
}

uint8_t qmul8(uint8_t i, uint8_t j)
{
	int p = ((int)i * (int)(j));
	if (p > 255) p = 255;
	return p;
}

int8_t abs8(int8_t i)
{
	if (i < 0) i = -i;
	return i;
}

uint8_t sqrt16(uint16_t x)
{
	if (x <= 1) {
		return x;
	}
	uint8_t low = 1; // lower bound
	uint8_t hi, mid;
	if (x > 7904) {
		hi = 255;
	}
	else {
		hi = (x >> 5) + 8; // initial estimate for upper bound
	}
	do {
		mid = (low + hi) >> 1;
		if ((uint16_t)(mid * mid) > x) {
			hi = mid - 1;
		}
		else {
			if (mid == 255) {
				return 255;
			}
			low = mid + 1;
		}
	} while (hi >= low);

	return low - 1;
}

uint8_t blend8(uint8_t a, uint8_t b, uint8_t amountOfB)
{
	uint16_t partial;
	uint8_t result;

	uint8_t amountOfA = 255 - amountOfB;

	partial = (a * amountOfA);
	partial += a;
	//partial = add8to16( a, partial);

	partial += (b * amountOfB);
	partial += b;
	//partial = add8to16( b, partial);

	result = partial >> 8;

	return result;

}

int16_t sin16(uint16_t theta)
{
	static const uint16_t base[] =
	{ 0, 6393, 12539, 18204, 23170, 27245, 30273, 32137 };
	static const uint8_t slope[] =
	{ 49, 48, 44, 38, 31, 23, 14, 4 };

	uint16_t offset = (theta & 0x3FFF) >> 3; // 0..2047
	if (theta & 0x4000) offset = 2047 - offset;

	uint8_t section = offset / 256; // 0..7
	uint16_t b = base[section];
	uint8_t  m = slope[section];

	uint8_t secoffset8 = (uint8_t)(offset) / 2;

	uint16_t mx = m * secoffset8;
	int16_t  y = mx + b;

	if (theta & 0x8000) y = -y;

	return y;
}

int16_t cos16(uint16_t theta)
{
	return sin16(theta + 16384);
}

const uint8_t b_m16_interleave[] = { 0, 49, 49, 41, 90, 27, 117, 10 };

uint8_t sin8(uint8_t theta)
{
	uint8_t offset = theta;
	if (theta & 0x40) {
		offset = (uint8_t)255 - offset;
	}
	offset &= 0x3F; // 0..63

	uint8_t secoffset = offset & 0x0F; // 0..15
	if (theta & 0x40) secoffset++;

	uint8_t section = offset >> 4; // 0..3
	uint8_t s2 = section * 2;
	const uint8_t* p = b_m16_interleave;
	p += s2;
	uint8_t b = *p;
	p++;
	uint8_t m16 = *p;

	uint8_t mx = (m16 * secoffset) >> 4;

	int8_t y = mx + b;
	if (theta & 0x80) y = -y;

	y += 128;

	return y;
}

uint8_t cos8(uint8_t theta)
{
	return sin8(theta + 64);
}


float sfract15ToFloat(sfract15 y)
{
	return y / 32768.0;
}

/// conversion from IEEE754 float in the range (-1,1)
///                  to 16-bit fixed point.  Note that the extremes of
///                  one and negative one are NOT representable.  The
///                  representable range is basically
sfract15 floatToSfract15(float f)
{
	return f * 32768.0;
}

uint8_t lerp8by8(uint8_t a, uint8_t b, fract8 frac)
{
	uint8_t result;
	if (b > a) {
		uint8_t delta = b - a;
		uint8_t scaled = scale8(delta, frac);
		result = a + scaled;
	}
	else {
		uint8_t delta = a - b;
		uint8_t scaled = scale8(delta, frac);
		result = a - scaled;
	}
	return result;
}

/// linear interpolation between two unsigned 16-bit values,
/// with 16-bit fraction
uint16_t lerp16by16(uint16_t a, uint16_t b, fract16 frac)
{
	uint16_t result;
	if (b > a) {
		uint16_t delta = b - a;
		uint16_t scaled = scale16(delta, frac);
		result = a + scaled;
	}
	else {
		uint16_t delta = a - b;
		uint16_t scaled = scale16(delta, frac);
		result = a - scaled;
	}
	return result;
}

/// linear interpolation between two unsigned 16-bit values,
/// with 8-bit fraction
uint16_t lerp16by8(uint16_t a, uint16_t b, fract8 frac)
{
	uint16_t result;
	if (b > a) {
		uint16_t delta = b - a;
		uint16_t scaled = scale16by8(delta, frac);
		result = a + scaled;
	}
	else {
		uint16_t delta = a - b;
		uint16_t scaled = scale16by8(delta, frac);
		result = a - scaled;
	}
	return result;
}

/// linear interpolation between two signed 15-bit values,
/// with 8-bit fraction
int16_t lerp15by8(int16_t a, int16_t b, fract8 frac)
{
	int16_t result;
	if (b > a) {
		uint16_t delta = b - a;
		uint16_t scaled = scale16by8(delta, frac);
		result = a + scaled;
	}
	else {
		uint16_t delta = a - b;
		uint16_t scaled = scale16by8(delta, frac);
		result = a - scaled;
	}
	return result;
}

/// linear interpolation between two signed 15-bit values,
/// with 8-bit fraction
int16_t lerp15by16(int16_t a, int16_t b, fract16 frac)
{
	int16_t result;
	if (b > a) {
		uint16_t delta = b - a;
		uint16_t scaled = scale16(delta, frac);
		result = a + scaled;
	}
	else {
		uint16_t delta = a - b;
		uint16_t scaled = scale16(delta, frac);
		result = a - scaled;
	}
	return result;
}



/// ease16InOutQuad: 16-bit quadratic ease-in / ease-out function
// C implementation at this point
uint16_t ease16InOutQuad(uint16_t i)
{
	uint16_t j = i;
	if (j & 0x8000) {
		j = 65535 - j;
	}
	uint16_t jj = scale16(j, j);
	uint16_t jj2 = jj << 1;
	if (i & 0x8000) {
		jj2 = 65535 - jj2;
	}
	return jj2;
}


/// ease8InOutCubic: 8-bit cubic ease-in / ease-out function
///                 Takes around 18 cycles on AVR
fract8 ease8InOutCubic(fract8 i)
{
	uint8_t ii = scale8_LEAVING_R1_DIRTY(i, i);
	uint8_t iii = scale8_LEAVING_R1_DIRTY(ii, i);

	uint16_t r1 = (3 * (uint16_t)(ii)) - (2 * (uint16_t)(iii));

	/* the code generated for the above *'s automatically
	   cleans up R1, so there's no need to explicitily call
	   cleanup_R1(); */

	uint8_t result = r1;

	// if we got "256", return 255:
	if (r1 & 0x100) {
		result = 255;
	}
	return result;
}


uint8_t triwave8(uint8_t in)
{
	if (in & 0x80) {
		in = 255 - in;
	}
	uint8_t out = in << 1;
	return out;
}

uint8_t ease8InOutQuad(uint8_t i)
{
	uint8_t j = i;
	if (j & 0x80) {
		j = 255 - j;
	}
	uint8_t jj = scale8(j, j);
	uint8_t jj2 = jj << 1;
	if (i & 0x80) {
		jj2 = 255 - jj2;
	}
	return jj2;
}

uint8_t quadwave8(uint8_t in)
{
	return ease8InOutQuad(triwave8(in));
}

/// cubicwave8: cubic waveform generator.  Spends visibly more time
///             at the limits than 'sine' does.
uint8_t cubicwave8(uint8_t in)
{
	return ease8InOutCubic(triwave8(in));
}

uint8_t squarewave8(uint8_t in, uint8_t pulsewidth = 128)
{
	if (in < pulsewidth || (pulsewidth == 255)) {
		return 255;
	}
	else {
		return 0;
	}
}





template<class T, int F, int I> class q {
	T i : I;
	T f : F;
public:
	q(float fx) { i = fx; f = (fx - i) * (1 << F); }
	q(uint8_t _i, uint8_t _f) { i = _i; f = _f; }
	uint32_t operator*(uint32_t v) { return (v*i) + ((v*f) >> F); }
	uint16_t operator*(uint16_t v) { return (v*i) + ((v*f) >> F); }
	int32_t operator*(int32_t v) { return (v*i) + ((v*f) >> F); }
	int16_t operator*(int16_t v) { return (v*i) + ((v*f) >> F); }
#ifdef FASTLED_ARM
	int operator*(int v) { return (v*i) + ((v*f) >> F); }
#endif
};

template<class T, int F, int I> static uint32_t operator*(uint32_t v, q<T, F, I> & q) { return q * v; }
template<class T, int F, int I> static uint16_t operator*(uint16_t v, q<T, F, I> & q) { return q * v; }
template<class T, int F, int I> static int32_t operator*(int32_t v, q<T, F, I> & q) { return q * v; }
template<class T, int F, int I> static int16_t operator*(int16_t v, q<T, F, I> & q) { return q * v; }
#ifdef FASTLED_ARM
template<class T, int F, int I> static int operator*(int v, q<T, F, I> & q) { return q * v; }
#endif

/// A 4.4 integer (4 bits integer, 4 bits fraction)
typedef q<uint8_t, 4, 4> q44;
/// A 6.2 integer (6 bits integer, 2 bits fraction)
typedef q<uint8_t, 6, 2> q62;
/// A 8.8 integer (8 bits integer, 8 bits fraction)
typedef q<uint16_t, 8, 8> q88;
/// A 12.4 integer (12 bits integer, 4 bits fraction)
typedef q<uint16_t, 12, 4> q124;


#include "ofMain.h"

#define GET_MILLIS ofGetSystemTimeMillis


// beat16 generates a 16-bit 'sawtooth' wave at a given BPM,
///        with BPM specified in Q8.8 fixed-point format; e.g.
///        for this function, 120 BPM MUST BE specified as
///        120*256 = 30720.
///        If you just want to specify "120", use beat16 or beat8.
uint16_t beat88(accum88 beats_per_minute_88, uint32_t timebase = 0)
{
	// BPM is 'beats per minute', or 'beats per 60000ms'.
	// To avoid using the (slower) division operator, we
	// want to convert 'beats per 60000ms' to 'beats per 65536ms',
	// and then use a simple, fast bit-shift to divide by 65536.
	//
	// The ratio 65536:60000 is 279.620266667:256; we'll call it 280:256.
	// The conversion is accurate to about 0.05%, more or less,
	// e.g. if you ask for "120 BPM", you'll get about "119.93".
	return (((GET_MILLIS()) - timebase) * beats_per_minute_88 * 280) >> 16;
}

/// beat16 generates a 16-bit 'sawtooth' wave at a given BPM
uint16_t beat16(accum88 beats_per_minute, uint32_t timebase = 0)
{
	// Convert simple 8-bit BPM's to full Q8.8 accum88's if needed
	if (beats_per_minute < 256) beats_per_minute <<= 8;
	return beat88(beats_per_minute, timebase);
}

/// beat8 generates an 8-bit 'sawtooth' wave at a given BPM
uint8_t beat8(accum88 beats_per_minute, uint32_t timebase = 0)
{
	return beat16(beats_per_minute, timebase) >> 8;
}

/// beatsin88 generates a 16-bit sine wave at a given BPM,
///           that oscillates within a given range.
///           For this function, BPM MUST BE SPECIFIED as
///           a Q8.8 fixed-point value; e.g. 120BPM must be
///           specified as 120*256 = 30720.
///           If you just want to specify "120", use beatsin16 or beatsin8.
uint16_t beatsin88(accum88 beats_per_minute_88, uint16_t lowest = 0, uint16_t highest = 65535,
	uint32_t timebase = 0, uint16_t phase_offset = 0)
{
	uint16_t beat = beat88(beats_per_minute_88, timebase);
	uint16_t beatsin = (sin16(beat + phase_offset) + 32768);
	uint16_t rangewidth = highest - lowest;
	uint16_t scaledbeat = scale16(beatsin, rangewidth);
	uint16_t result = lowest + scaledbeat;
	return result;
}

/// beatsin16 generates a 16-bit sine wave at a given BPM,
///           that oscillates within a given range.
uint16_t beatsin16(accum88 beats_per_minute, uint16_t lowest = 0, uint16_t highest = 65535,
	uint32_t timebase = 0, uint16_t phase_offset = 0)
{
	uint16_t beat = beat16(beats_per_minute, timebase);
	uint16_t beatsin = (sin16(beat + phase_offset) + 32768);
	uint16_t rangewidth = highest - lowest;
	uint16_t scaledbeat = scale16(beatsin, rangewidth);
	uint16_t result = lowest + scaledbeat;
	return result;
}

/// beatsin8 generates an 8-bit sine wave at a given BPM,
///           that oscillates within a given range.
uint8_t beatsin8(accum88 beats_per_minute, uint8_t lowest = 0, uint8_t highest = 255,
	uint32_t timebase = 0, uint8_t phase_offset = 0)
{
	uint8_t beat = beat8(beats_per_minute, timebase);
	uint8_t beatsin = sin8(beat + phase_offset);
	uint8_t rangewidth = highest - lowest;
	uint8_t scaledbeat = scale8(beatsin, rangewidth);
	uint8_t result = lowest + scaledbeat;
	return result;
}


/// Return the current seconds since boot in a 16-bit value.  Used as part of the
/// "every N time-periods" mechanism
uint16_t seconds16()
{
	uint32_t ms = GET_MILLIS();
	uint16_t s16;
	s16 = ms / 1000;
	return s16;
}

/// Return the current minutes since boot in a 16-bit value.  Used as part of the
/// "every N time-periods" mechanism
uint16_t minutes16()
{
	uint32_t ms = GET_MILLIS();
	uint16_t m16;
	m16 = (ms / (60000L)) & 0xFFFF;
	return m16;
}

/// Return the current hours since boot in an 8-bit value.  Used as part of the
/// "every N time-periods" mechanism
uint8_t hours8()
{
	uint32_t ms = GET_MILLIS();
	uint8_t h8;
	h8 = (ms / (3600000L)) & 0xFF;
	return h8;
}


// FASTLED Noise
#define P(x) pData[x]
#define EASE8(x)		(ease8InOutQuad(x) )
#define EASE16(x)		(ease16InOutQuad(x))
#define FADE(x)			scale16(x,x)
#define LERP(a,b,u)		lerp15by16(a,b,u)
#define AVG15(U,V)		(((U)+(V)) >> 1)

uint8_t const pData[] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,151
};

int16_t grad16(uint8_t hash, int16_t x, int16_t y, int16_t z) {
	hash = hash & 15;
	int16_t u = hash < 8 ? x : y;
	int16_t v = hash < 4 ? y : hash == 12 || hash == 14 ? x : z;
	if (hash & 1) { u = -u; }
	if (hash & 2) { v = -v; }

	return AVG15(u, v);
}

int16_t grad16(uint8_t hash, int16_t x, int16_t y) {
	hash = hash & 7;
	int16_t u, v;
	if (hash < 4) { u = x; v = y; }
	else { u = y; v = x; }
	if (hash & 1) { u = -u; }
	if (hash & 2) { v = -v; }

	return AVG15(u, v);
}

int16_t grad16(uint8_t hash, int16_t x) {
	hash = hash & 15;
	int16_t u, v;
	if (hash > 8) { u = x; v = x; }
	else if (hash < 4) { u = x; v = 1; }
	else { u = 1; v = x; }
	if (hash & 1) { u = -u; }
	if (hash & 2) { v = -v; }

	return AVG15(u, v);
}

int16_t lerp15by12(int16_t a, int16_t b, fract16 frac)
{
	//if(1) return (lerp(frac,a,b));
	int16_t result;
	if (b > a) {
		uint16_t delta = b - a;
		uint16_t scaled = scale16(delta, frac << 4);
		result = a + scaled;
	}
	else {
		uint16_t delta = a - b;
		uint16_t scaled = scale16(delta, frac << 4);
		result = a - scaled;
	}
	return result;
}

int8_t lerp7by8(int8_t a, int8_t b, fract8 frac)
{
	// int8_t delta = b - a;
	// int16_t prod = (uint16_t)delta * (uint16_t)frac;
	// int8_t scaled = prod >> 8;
	// int8_t result = a + scaled;
	// return result;
	int8_t result;
	if (b > a) {
		uint8_t delta = b - a;
		uint8_t scaled = scale8(delta, frac);
		result = a + scaled;
	}
	else {
		uint8_t delta = a - b;
		uint8_t scaled = scale8(delta, frac);
		result = a - scaled;
	}
	return result;
}

int8_t selectBasedOnHashBit(uint8_t hash, uint8_t bitnumber, int8_t a, int8_t b) {
	int8_t result;
	result = (hash & (1 << bitnumber)) ? a : b;
	return result;
}

int8_t grad8(uint8_t hash, int8_t x, int8_t y, int8_t z) {
	hash &= 0xF;

	int8_t u, v;
	//u = (hash&8)?y:x;
	u = selectBasedOnHashBit(hash, 3, y, x);

	v = hash < 4 ? y : hash == 12 || hash == 14 ? x : z;


	if (hash & 1) { u = -u; }
	if (hash & 2) { v = -v; }

	return avg7(u, v);
}

int8_t grad8(uint8_t hash, int8_t x, int8_t y)
{
	// since the tests below can be done bit-wise on the bottom
	// three bits, there's no need to mask off the higher bits
	//  hash = hash & 7;

	int8_t u, v;
	if (hash & 4) {
		u = y; v = x;
	}
	else {
		u = x; v = y;
	}

	if (hash & 1) { u = -u; }
	if (hash & 2) { v = -v; }

	return avg7(u, v);
}

int8_t grad8(uint8_t hash, int8_t x)
{
	// since the tests below can be done bit-wise on the bottom
	// four bits, there's no need to mask off the higher bits
	//	hash = hash & 15;

	int8_t u, v;
	if (hash & 8) {
		u = x; v = x;
	}
	else {
		if (hash & 4) {
			u = 1; v = x;
		}
		else {
			u = x; v = 1;
		}
	}

	if (hash & 1) { u = -u; }
	if (hash & 2) { v = -v; }

	return avg7(u, v);
}



// struct q44 {
//   uint8_t i:4;
//   uint8_t f:4;
//   q44(uint8_t _i, uint8_t _f) {i=_i; f=_f; }
// };

// uint32_t mul44(uint32_t v, q44 mulby44) {
//     return (v *mulby44.i)  + ((v * mulby44.f) >> 4);
// }
//
// uint16_t mul44_16(uint16_t v, q44 mulby44) {
//     return (v *mulby44.i)  + ((v * mulby44.f) >> 4);
// }

