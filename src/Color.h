#pragma once


#if defined(ARDUINO)
#include "FastLED.h"
#else
#include "Patterns/FastLED_PC.h"

struct CRGB;
struct CHSV;

void hsv2rgb_rainbow(const CHSV& hsv, CRGB& rgb);

struct CHSV {
	union {
		struct {
			union {
				uint8_t hue;
				uint8_t h;
			};
			union {
				uint8_t saturation;
				uint8_t sat;
				uint8_t s;
			};
			union {
				uint8_t value;
				uint8_t val;
				uint8_t v;
			};
		};
		uint8_t raw[3];
	};

	/// default values are UNITIALIZED
	inline CHSV()
	{
	}

	/// allow construction from H, S, V
	inline CHSV(uint8_t ih, uint8_t is, uint8_t iv)
		: h(ih), s(is), v(iv)
	{
	}

	/// allow copy construction
	inline CHSV(const CHSV& rhs)
	{
		h = rhs.h;
		s = rhs.s;
		v = rhs.v;
	}

	inline CHSV& operator= (const CHSV& rhs)
	{
		h = rhs.h;
		s = rhs.s;
		v = rhs.v;
		return *this;
	}

	inline CHSV& setHSV(uint8_t ih, uint8_t is, uint8_t iv)
	{
		h = ih;
		s = is;
		v = iv;
		return *this;
	}
};

/// Pre-defined hue values for HSV objects
typedef enum {
	HUE_RED = 0,
	HUE_ORANGE = 32,
	HUE_YELLOW = 64,
	HUE_GREEN = 96,
	HUE_AQUA = 128,
	HUE_BLUE = 160,
	HUE_PURPLE = 192,
	HUE_PINK = 224
} HSVHue;

/// Representation of an RGB pixel (Red, Green, Blue)
struct CRGB {
	union {
		struct {
			union {
				uint8_t r;
				uint8_t red;
			};
			union {
				uint8_t g;
				uint8_t green;
			};
			union {
				uint8_t b;
				uint8_t blue;
			};
		};
		uint8_t raw[3];
	};

	/// Array access operator to index into the crgb object
	inline uint8_t& operator[] (uint8_t x)
	{
		return raw[x];
	}

	/// Array access operator to index into the crgb object
	inline const uint8_t& operator[] (uint8_t x) const
	{
		return raw[x];
	}

	// default values are UNINITIALIZED
	inline CRGB()
	{
	}

	/// allow construction from R, G, B
	inline CRGB(uint8_t ir, uint8_t ig, uint8_t ib)
		: r(ir), g(ig), b(ib)
	{
	}

	/// allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
	inline CRGB(uint32_t colorcode)
		: r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
	{
	}

	/// allow copy construction
	inline CRGB(const CRGB& rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
	}

	/// allow construction from HSV color
	inline CRGB(const CHSV& rhs)
	{
		hsv2rgb_rainbow(rhs, *this);
	}

	/// allow assignment from one RGB struct to another
	inline CRGB& operator= (const CRGB& rhs)
	{
		r = rhs.r;
		g = rhs.g;
		b = rhs.b;
		return *this;
	}

	/// allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
	inline CRGB& operator= (const uint32_t colorcode)
	{
		r = (colorcode >> 16) & 0xFF;
		g = (colorcode >> 8) & 0xFF;
		b = (colorcode >> 0) & 0xFF;
		return *this;
	}

	/// allow assignment from R, G, and B
	inline CRGB& setRGB(uint8_t nr, uint8_t ng, uint8_t nb)
	{
		r = nr;
		g = ng;
		b = nb;
		return *this;
	}

	/// allow assignment from H, S, and V
	inline CRGB& setHSV(uint8_t hue, uint8_t sat, uint8_t val)
	{
		hsv2rgb_rainbow(CHSV(hue, sat, val), *this);
		return *this;
	}

	/// allow assignment from just a Hue, saturation and value automatically at max.
	inline CRGB& setHue(uint8_t hue)
	{
		hsv2rgb_rainbow(CHSV(hue, 255, 255), *this);
		return *this;
	}

	/// allow assignment from HSV color
	inline CRGB& operator= (const CHSV& rhs)
	{
		hsv2rgb_rainbow(rhs, *this);
		return *this;
	}

	/// allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
	inline CRGB& setColorCode(uint32_t colorcode)
	{
		r = (colorcode >> 16) & 0xFF;
		g = (colorcode >> 8) & 0xFF;
		b = (colorcode >> 0) & 0xFF;
		return *this;
	}


	/// add one RGB to another, saturating at 0xFF for each channel
	inline CRGB& operator+= (const CRGB& rhs)
	{
		r = qadd8(r, rhs.r);
		g = qadd8(g, rhs.g);
		b = qadd8(b, rhs.b);
		return *this;
	}

	/// add a contstant to each channel, saturating at 0xFF
	/// this is NOT an operator+= overload because the compiler
	/// can't usefully decide when it's being passed a 32-bit
	/// constant (e.g. CRGB::Red) and an 8-bit one (CRGB::Blue)
	inline CRGB& addToRGB(uint8_t d)
	{
		r = qadd8(r, d);
		g = qadd8(g, d);
		b = qadd8(b, d);
		return *this;
	}

	/// subtract one RGB from another, saturating at 0x00 for each channel
	inline CRGB& operator-= (const CRGB& rhs)
	{
		r = qsub8(r, rhs.r);
		g = qsub8(g, rhs.g);
		b = qsub8(b, rhs.b);
		return *this;
	}

	/// subtract a constant from each channel, saturating at 0x00
	/// this is NOT an operator+= overload because the compiler
	/// can't usefully decide when it's being passed a 32-bit
	/// constant (e.g. CRGB::Red) and an 8-bit one (CRGB::Blue)
	inline CRGB& subtractFromRGB(uint8_t d)
	{
		r = qsub8(r, d);
		g = qsub8(g, d);
		b = qsub8(b, d);
		return *this;
	}

	/// subtract a constant of '1' from each channel, saturating at 0x00
	inline CRGB& operator-- ()
	{
		subtractFromRGB(1);
		return *this;
	}

	/// subtract a constant of '1' from each channel, saturating at 0x00
	inline CRGB operator-- (int)
	{
		CRGB retval(*this);
		--(*this);
		return retval;
	}

	/// add a constant of '1' from each channel, saturating at 0xFF
	inline CRGB& operator++ ()
	{
		addToRGB(1);
		return *this;
	}

	/// add a constant of '1' from each channel, saturating at 0xFF
	inline CRGB operator++ (int)
	{
		CRGB retval(*this);
		++(*this);
		return retval;
	}

	/// divide each of the channels by a constant
	inline CRGB& operator/= (uint8_t d)
	{
		r /= d;
		g /= d;
		b /= d;
		return *this;
	}

	/// right shift each of the channels by a constant
	inline CRGB& operator>>= (uint8_t d)
	{
		r >>= d;
		g >>= d;
		b >>= d;
		return *this;
	}

	/// multiply each of the channels by a constant,
	/// saturating each channel at 0xFF
	inline CRGB& operator*= (uint8_t d)
	{
		r = qmul8(r, d);
		g = qmul8(g, d);
		b = qmul8(b, d);
		return *this;
	}

	/// scale down a RGB to N 256ths of it's current brightness, using
	/// 'video' dimming rules, which means that unless the scale factor is ZERO
	/// each channel is guaranteed NOT to dim down to zero.  If it's already
	/// nonzero, it'll stay nonzero, even if that means the hue shifts a little
	/// at low brightness levels.
	inline CRGB& nscale8_video(uint8_t scaledown)
	{
		nscale8x3_video(r, g, b, scaledown);
		return *this;
	}

	/// %= is a synonym for nscale8_video.  Think of it is scaling down
	/// by "a percentage"
	inline CRGB& operator%= (uint8_t scaledown)
	{
		nscale8x3_video(r, g, b, scaledown);
		return *this;
	}

	/// fadeLightBy is a synonym for nscale8_video( ..., 255-fadefactor)
	inline CRGB& fadeLightBy(uint8_t fadefactor)
	{
		nscale8x3_video(r, g, b, 255 - fadefactor);
		return *this;
	}

	/// scale down a RGB to N 256ths of it's current brightness, using
	/// 'plain math' dimming rules, which means that if the low light levels
	/// may dim all the way to 100% black.
	inline CRGB& nscale8(uint8_t scaledown)
	{
		nscale8x3(r, g, b, scaledown);
		return *this;
	}

	/// scale down a RGB to N 256ths of it's current brightness, using
	/// 'plain math' dimming rules, which means that if the low light levels
	/// may dim all the way to 100% black.
	inline CRGB& nscale8(const CRGB & scaledown)
	{
		r = ::scale8(r, scaledown.r);
		g = ::scale8(g, scaledown.g);
		b = ::scale8(b, scaledown.b);
		return *this;
	}

	/// return a CRGB object that is a scaled down version of this object
	inline CRGB scale8(const CRGB & scaledown) const
	{
		CRGB out;
		out.r = ::scale8(r, scaledown.r);
		out.g = ::scale8(g, scaledown.g);
		out.b = ::scale8(b, scaledown.b);
		return out;
	}

	/// fadeToBlackBy is a synonym for nscale8( ..., 255-fadefactor)
	inline CRGB& fadeToBlackBy(uint8_t fadefactor)
	{
		nscale8x3(r, g, b, 255 - fadefactor);
		return *this;
	}

	/// "or" operator brings each channel up to the higher of the two values
	inline CRGB& operator|= (const CRGB& rhs)
	{
		if (rhs.r > r) r = rhs.r;
		if (rhs.g > g) g = rhs.g;
		if (rhs.b > b) b = rhs.b;
		return *this;
	}

	/// "or" operator brings each channel up to the higher of the two values
	inline CRGB& operator|= (uint8_t d)
	{
		if (d > r) r = d;
		if (d > g) g = d;
		if (d > b) b = d;
		return *this;
	}

	/// "and" operator brings each channel down to the lower of the two values
	inline CRGB& operator&= (const CRGB& rhs)
	{
		if (rhs.r < r) r = rhs.r;
		if (rhs.g < g) g = rhs.g;
		if (rhs.b < b) b = rhs.b;
		return *this;
	}

	/// "and" operator brings each channel down to the lower of the two values
	inline CRGB& operator&= (uint8_t d)
	{
		if (d < r) r = d;
		if (d < g) g = d;
		if (d < b) b = d;
		return *this;
	}

	/// this allows testing a CRGB for zero-ness
	inline operator bool() const
	{
		return r || g || b;
	}

	/// invert each channel
	inline CRGB operator- ()
	{
		CRGB retval;
		retval.r = 255 - r;
		retval.g = 255 - g;
		retval.b = 255 - b;
		return retval;
	}

#if (defined SmartMatrix_h || defined SmartMatrix3_h)
	operator rgb24() const {
		rgb24 ret;
		ret.red = r;
		ret.green = g;
		ret.blue = b;
		return ret;
	}
#endif

	/// Get the 'luma' of a CRGB object - aka roughly how much light the
	/// CRGB pixel is putting out (from 0 to 255).
	inline uint8_t getLuma()  const {
		//Y' = 0.2126 R' + 0.7152 G' + 0.0722 B'
		//     54            183       18 (!)

		uint8_t luma = scale8_LEAVING_R1_DIRTY(r, 54) + \
			scale8_LEAVING_R1_DIRTY(g, 183) + \
			scale8_LEAVING_R1_DIRTY(b, 18);
		cleanup_R1();
		return luma;
	}

	/// Get the average of the R, G, and B values
	inline uint8_t getAverageLight()  const {
#if FASTLED_SCALE8_FIXED == 1
		const uint8_t eightyfive = 85;
#else
		const uint8_t eightyfive = 86;
#endif
		uint8_t avg = scale8_LEAVING_R1_DIRTY(r, eightyfive) + \
			scale8_LEAVING_R1_DIRTY(g, eightyfive) + \
			scale8_LEAVING_R1_DIRTY(b, eightyfive);
		cleanup_R1();
		return avg;
	}

	/// maximize the brightness of this CRGB object
	inline void maximizeBrightness(uint8_t limit = 255) {
		uint8_t max = red;
		if (green > max) max = green;
		if (blue > max) max = blue;

		// stop div/0 when color is black
		if (max > 0) {
			uint16_t factor = ((uint16_t)(limit) * 256) / max;
			red = (red   * factor) / 256;
			green = (green * factor) / 256;
			blue = (blue  * factor) / 256;
		}
	}

	/// return a new CRGB object after performing a linear interpolation between this object and the passed in object
	inline CRGB lerp8(const CRGB& other, fract8 frac) const
	{
		CRGB ret;

		ret.r = lerp8by8(r, other.r, frac);
		ret.g = lerp8by8(g, other.g, frac);
		ret.b = lerp8by8(b, other.b, frac);

		return ret;
	}

	/// return a new CRGB object after performing a linear interpolation between this object and the passed in object
	inline CRGB lerp16(const CRGB& other, fract16 frac) const
	{
		CRGB ret;

		ret.r = lerp16by16(r << 8, other.r << 8, frac) >> 8;
		ret.g = lerp16by16(g << 8, other.g << 8, frac) >> 8;
		ret.b = lerp16by16(b << 8, other.b << 8, frac) >> 8;

		return ret;
	}

	/// getParity returns 0 or 1, depending on the
	/// lowest bit of the sum of the color components.
	inline uint8_t getParity()
	{
		uint8_t sum = r + g + b;
		return (sum & 0x01);
	}

	/// setParity adjusts the color in the smallest
	/// way possible so that the parity of the color
	/// is now the desired value.  This allows you to
	/// 'hide' one bit of information in the color.
	///
	/// Ideally, we find one color channel which already
	/// has data in it, and modify just that channel by one.
	/// We don't want to light up a channel that's black
	/// if we can avoid it, and if the pixel is 'grayscale',
	/// (meaning that R==G==B), we modify all three channels
	/// at once, to preserve the neutral hue.
	///
	/// There's no such thing as a free lunch; in many cases
	/// this 'hidden bit' may actually be visible, but this
	/// code makes reasonable efforts to hide it as much
	/// as is reasonably possible.
	///
	/// Also, an effort is made to have make it such that
	/// repeatedly setting the parity to different values
	/// will not cause the color to 'drift'.  Toggling
	/// the parity twice should generally result in the
	/// original color again.
	///
	inline void setParity(uint8_t parity)
	{
		uint8_t curparity = getParity();

		if (parity == curparity) return;

		if (parity) {
			// going 'up'
			if ((b > 0) && (b < 255)) {
				if (r == g && g == b) {
					r++;
					g++;
				}
				b++;
			}
			else if ((r > 0) && (r < 255)) {
				r++;
			}
			else if ((g > 0) && (g < 255)) {
				g++;
			}
			else {
				if (r == g && g == b) {
					r ^= 0x01;
					g ^= 0x01;
				}
				b ^= 0x01;
			}
		}
		else {
			// going 'down'
			if (b > 1) {
				if (r == g && g == b) {
					r--;
					g--;
				}
				b--;
			}
			else if (g > 1) {
				g--;
			}
			else if (r > 1) {
				r--;
			}
			else {
				if (r == g && g == b) {
					r ^= 0x01;
					g ^= 0x01;
				}
				b ^= 0x01;
			}
		}
	}

	/// Predefined RGB colors
	typedef enum {
		AliceBlue = 0xF0F8FF,
		Amethyst = 0x9966CC,
		AntiqueWhite = 0xFAEBD7,
		Aqua = 0x00FFFF,
		Aquamarine = 0x7FFFD4,
		Azure = 0xF0FFFF,
		Beige = 0xF5F5DC,
		Bisque = 0xFFE4C4,
		Black = 0x000000,
		BlanchedAlmond = 0xFFEBCD,
		Blue = 0x0000FF,
		BlueViolet = 0x8A2BE2,
		Brown = 0xA52A2A,
		BurlyWood = 0xDEB887,
		CadetBlue = 0x5F9EA0,
		Chartreuse = 0x7FFF00,
		Chocolate = 0xD2691E,
		Coral = 0xFF7F50,
		CornflowerBlue = 0x6495ED,
		Cornsilk = 0xFFF8DC,
		Crimson = 0xDC143C,
		Cyan = 0x00FFFF,
		DarkBlue = 0x00008B,
		DarkCyan = 0x008B8B,
		DarkGoldenrod = 0xB8860B,
		DarkGray = 0xA9A9A9,
		DarkGrey = 0xA9A9A9,
		DarkGreen = 0x006400,
		DarkKhaki = 0xBDB76B,
		DarkMagenta = 0x8B008B,
		DarkOliveGreen = 0x556B2F,
		DarkOrange = 0xFF8C00,
		DarkOrchid = 0x9932CC,
		DarkRed = 0x8B0000,
		DarkSalmon = 0xE9967A,
		DarkSeaGreen = 0x8FBC8F,
		DarkSlateBlue = 0x483D8B,
		DarkSlateGray = 0x2F4F4F,
		DarkSlateGrey = 0x2F4F4F,
		DarkTurquoise = 0x00CED1,
		DarkViolet = 0x9400D3,
		DeepPink = 0xFF1493,
		DeepSkyBlue = 0x00BFFF,
		DimGray = 0x696969,
		DimGrey = 0x696969,
		DodgerBlue = 0x1E90FF,
		FireBrick = 0xB22222,
		FloralWhite = 0xFFFAF0,
		ForestGreen = 0x228B22,
		Fuchsia = 0xFF00FF,
		Gainsboro = 0xDCDCDC,
		GhostWhite = 0xF8F8FF,
		Gold = 0xFFD700,
		Goldenrod = 0xDAA520,
		Gray = 0x808080,
		Grey = 0x808080,
		Green = 0x008000,
		GreenYellow = 0xADFF2F,
		Honeydew = 0xF0FFF0,
		HotPink = 0xFF69B4,
		IndianRed = 0xCD5C5C,
		Indigo = 0x4B0082,
		Ivory = 0xFFFFF0,
		Khaki = 0xF0E68C,
		Lavender = 0xE6E6FA,
		LavenderBlush = 0xFFF0F5,
		LawnGreen = 0x7CFC00,
		LemonChiffon = 0xFFFACD,
		LightBlue = 0xADD8E6,
		LightCoral = 0xF08080,
		LightCyan = 0xE0FFFF,
		LightGoldenrodYellow = 0xFAFAD2,
		LightGreen = 0x90EE90,
		LightGrey = 0xD3D3D3,
		LightPink = 0xFFB6C1,
		LightSalmon = 0xFFA07A,
		LightSeaGreen = 0x20B2AA,
		LightSkyBlue = 0x87CEFA,
		LightSlateGray = 0x778899,
		LightSlateGrey = 0x778899,
		LightSteelBlue = 0xB0C4DE,
		LightYellow = 0xFFFFE0,
		Lime = 0x00FF00,
		LimeGreen = 0x32CD32,
		Linen = 0xFAF0E6,
		Magenta = 0xFF00FF,
		Maroon = 0x800000,
		MediumAquamarine = 0x66CDAA,
		MediumBlue = 0x0000CD,
		MediumOrchid = 0xBA55D3,
		MediumPurple = 0x9370DB,
		MediumSeaGreen = 0x3CB371,
		MediumSlateBlue = 0x7B68EE,
		MediumSpringGreen = 0x00FA9A,
		MediumTurquoise = 0x48D1CC,
		MediumVioletRed = 0xC71585,
		MidnightBlue = 0x191970,
		MintCream = 0xF5FFFA,
		MistyRose = 0xFFE4E1,
		Moccasin = 0xFFE4B5,
		NavajoWhite = 0xFFDEAD,
		Navy = 0x000080,
		OldLace = 0xFDF5E6,
		Olive = 0x808000,
		OliveDrab = 0x6B8E23,
		Orange = 0xFFA500,
		OrangeRed = 0xFF4500,
		Orchid = 0xDA70D6,
		PaleGoldenrod = 0xEEE8AA,
		PaleGreen = 0x98FB98,
		PaleTurquoise = 0xAFEEEE,
		PaleVioletRed = 0xDB7093,
		PapayaWhip = 0xFFEFD5,
		PeachPuff = 0xFFDAB9,
		Peru = 0xCD853F,
		Pink = 0xFFC0CB,
		Plaid = 0xCC5533,
		Plum = 0xDDA0DD,
		PowderBlue = 0xB0E0E6,
		Purple = 0x800080,
		Red = 0xFF0000,
		RosyBrown = 0xBC8F8F,
		RoyalBlue = 0x4169E1,
		SaddleBrown = 0x8B4513,
		Salmon = 0xFA8072,
		SandyBrown = 0xF4A460,
		SeaGreen = 0x2E8B57,
		Seashell = 0xFFF5EE,
		Sienna = 0xA0522D,
		Silver = 0xC0C0C0,
		SkyBlue = 0x87CEEB,
		SlateBlue = 0x6A5ACD,
		SlateGray = 0x708090,
		SlateGrey = 0x708090,
		Snow = 0xFFFAFA,
		SpringGreen = 0x00FF7F,
		SteelBlue = 0x4682B4,
		Tan = 0xD2B48C,
		Teal = 0x008080,
		Thistle = 0xD8BFD8,
		Tomato = 0xFF6347,
		Turquoise = 0x40E0D0,
		Violet = 0xEE82EE,
		Wheat = 0xF5DEB3,
		White = 0xFFFFFF,
		WhiteSmoke = 0xF5F5F5,
		Yellow = 0xFFFF00,
		YellowGreen = 0x9ACD32,

		// LED RGB color that roughly approximates
		// the color of incandescent fairy lights,
		// assuming that you're using FastLED
		// color correction on your LEDs (recommended).
		FairyLight = 0xFFE42D,
		// If you are using no color correction, use this
		FairyLightNCC = 0xFF9D2A

	} HTMLColorCode;
};


inline  bool operator== (const CRGB& lhs, const CRGB& rhs)
{
	return (lhs.r == rhs.r) && (lhs.g == rhs.g) && (lhs.b == rhs.b);
}

inline  bool operator!= (const CRGB& lhs, const CRGB& rhs)
{
	return !(lhs == rhs);
}

inline  bool operator< (const CRGB& lhs, const CRGB& rhs)
{
	uint16_t sl, sr;
	sl = lhs.r + lhs.g + lhs.b;
	sr = rhs.r + rhs.g + rhs.b;
	return sl < sr;
}

inline  bool operator> (const CRGB& lhs, const CRGB& rhs)
{
	uint16_t sl, sr;
	sl = lhs.r + lhs.g + lhs.b;
	sr = rhs.r + rhs.g + rhs.b;
	return sl > sr;
}

inline  bool operator>= (const CRGB& lhs, const CRGB& rhs)
{
	uint16_t sl, sr;
	sl = lhs.r + lhs.g + lhs.b;
	sr = rhs.r + rhs.g + rhs.b;
	return sl >= sr;
}

inline  bool operator<= (const CRGB& lhs, const CRGB& rhs)
{
	uint16_t sl, sr;
	sl = lhs.r + lhs.g + lhs.b;
	sr = rhs.r + rhs.g + rhs.b;
	return sl <= sr;
}



inline CRGB operator+(const CRGB& p1, const CRGB& p2)
{
	return CRGB(qadd8(p1.r, p2.r),
		qadd8(p1.g, p2.g),
		qadd8(p1.b, p2.b));
}


inline CRGB operator-(const CRGB& p1, const CRGB& p2)
{
	return CRGB(qsub8(p1.r, p2.r),
		qsub8(p1.g, p2.g),
		qsub8(p1.b, p2.b));
}


inline CRGB operator*(const CRGB& p1, uint8_t d)
{
	return CRGB(qmul8(p1.r, d),
		qmul8(p1.g, d),
		qmul8(p1.b, d));
}


inline CRGB operator/(const CRGB& p1, uint8_t d)
{
	return CRGB(p1.r / d, p1.g / d, p1.b / d);
}



inline CRGB operator&(const CRGB& p1, const CRGB& p2)
{
	return CRGB(p1.r < p2.r ? p1.r : p2.r,
		p1.g < p2.g ? p1.g : p2.g,
		p1.b < p2.b ? p1.b : p2.b);
}


inline CRGB operator|(const CRGB& p1, const CRGB& p2)
{
	return CRGB(p1.r > p2.r ? p1.r : p2.r,
		p1.g > p2.g ? p1.g : p2.g,
		p1.b > p2.b ? p1.b : p2.b);
}


inline CRGB operator%(const CRGB& p1, uint8_t d)
{
	CRGB retval(p1);
	retval.nscale8_video(d);
	return retval;
}


#define APPLY_DIMMING(X) (X)
#define HSV_SECTION_6 (0x20)
#define HSV_SECTION_3 (0x40)


void hsv2rgb_raw(const struct CHSV & hsv, struct CRGB & rgb)
{
	// Convert hue, saturation and brightness ( HSV/HSB ) to RGB
	// "Dimming" is used on saturation and brightness to make
	// the output more visually linear.

	// Apply dimming curves
	uint8_t value = APPLY_DIMMING(hsv.val);
	uint8_t saturation = hsv.sat;

	// The brightness floor is minimum number that all of
	// R, G, and B will be set to.
	uint8_t invsat = APPLY_DIMMING(255 - saturation);
	uint8_t brightness_floor = (value * invsat) / 256;

	// The color amplitude is the maximum amount of R, G, and B
	// that will be added on top of the brightness_floor to
	// create the specific hue desired.
	uint8_t color_amplitude = value - brightness_floor;

	// Figure out which section of the hue wheel we're in,
	// and how far offset we are withing that section
	uint8_t section = hsv.hue / HSV_SECTION_3; // 0..2
	uint8_t offset = hsv.hue % HSV_SECTION_3;  // 0..63

	uint8_t rampup = offset; // 0..63
	uint8_t rampdown = (HSV_SECTION_3 - 1) - offset; // 63..0

	// We now scale rampup and rampdown to a 0-255 range -- at least
	// in theory, but here's where architecture-specific decsions
	// come in to play:
	// To scale them up to 0-255, we'd want to multiply by 4.
	// But in the very next step, we multiply the ramps by other
	// values and then divide the resulting product by 256.
	// So which is faster?
	//   ((ramp * 4) * othervalue) / 256
	// or
	//   ((ramp    ) * othervalue) /  64
	// It depends on your processor architecture.
	// On 8-bit AVR, the "/ 256" is just a one-cycle register move,
	// but the "/ 64" might be a multicycle shift process. So on AVR
	// it's faster do multiply the ramp values by four, and then
	// divide by 256.
	// On ARM, the "/ 256" and "/ 64" are one cycle each, so it's
	// faster to NOT multiply the ramp values by four, and just to
	// divide the resulting product by 64 (instead of 256).
	// Moral of the story: trust your profiler, not your insticts.

	// Since there's an AVR assembly version elsewhere, we'll
	// assume what we're on an architecture where any number of
	// bit shifts has roughly the same cost, and we'll remove the
	// redundant math at the source level:

	//  // scale up to 255 range
	//  //rampup *= 4; // 0..252
	//  //rampdown *= 4; // 0..252

	// compute color-amplitude-scaled-down versions of rampup and rampdown
	uint8_t rampup_amp_adj = (rampup   * color_amplitude) / (256 / 4);
	uint8_t rampdown_amp_adj = (rampdown * color_amplitude) / (256 / 4);

	// add brightness_floor offset to everything
	uint8_t rampup_adj_with_floor = rampup_amp_adj + brightness_floor;
	uint8_t rampdown_adj_with_floor = rampdown_amp_adj + brightness_floor;


	if (section) {
		if (section == 1) {
			// section 1: 0x40..0x7F
			rgb.r = brightness_floor;
			rgb.g = rampdown_adj_with_floor;
			rgb.b = rampup_adj_with_floor;
		}
		else {
			// section 2; 0x80..0xBF
			rgb.r = rampup_adj_with_floor;
			rgb.g = brightness_floor;
			rgb.b = rampdown_adj_with_floor;
		}
	}
	else {
		// section 0: 0x00..0x3F
		rgb.r = rampdown_adj_with_floor;
		rgb.g = rampup_adj_with_floor;
		rgb.b = brightness_floor;
	}
}

void hsv2rgb_spectrum(const CHSV& hsv, CRGB& rgb)
{
	CHSV hsv2(hsv);
	hsv2.hue = scale8(hsv2.hue, 191);
	hsv2rgb_raw(hsv2, rgb);
}

#define K255 255
#define K171 171
#define K170 170
#define K85  85

void hsv2rgb_rainbow(const CHSV& hsv, CRGB& rgb)
{
	// Yellow has a higher inherent brightness than
	// any other color; 'pure' yellow is perceived to
	// be 93% as bright as white.  In order to make
	// yellow appear the correct relative brightness,
	// it has to be rendered brighter than all other
	// colors.
	// Level Y1 is a moderate boost, the default.
	// Level Y2 is a strong boost.
	const uint8_t Y1 = 1;
	const uint8_t Y2 = 0;

	// G2: Whether to divide all greens by two.
	// Depends GREATLY on your particular LEDs
	const uint8_t G2 = 0;

	// Gscale: what to scale green down by.
	// Depends GREATLY on your particular LEDs
	const uint8_t Gscale = 0;


	uint8_t hue = hsv.hue;
	uint8_t sat = hsv.sat;
	uint8_t val = hsv.val;

	uint8_t offset = hue & 0x1F; // 0..31

	// offset8 = offset * 8
	uint8_t offset8 = offset;

	// On ARM and other non-AVR platforms, we just shift 3.
	offset8 <<= 3;

	uint8_t third = scale8(offset8, (256 / 3)); // max = 85

	uint8_t r, g, b;

	if (!(hue & 0x80)) {
		// 0XX
		if (!(hue & 0x40)) {
			// 00X
			//section 0-1
			if (!(hue & 0x20)) {
				// 000
				//case 0: // R -> O
				r = K255 - third;
				g = third;
				b = 0;
			}
			else {
				// 001
				//case 1: // O -> Y
				if (Y1) {
					r = K171;
					g = K85 + third;
					b = 0;
				}
				if (Y2) {
					r = K170 + third;
					//uint8_t twothirds = (third << 1);
					uint8_t twothirds = scale8(offset8, ((256 * 2) / 3)); // max=170
					g = K85 + twothirds;
					b = 0;
				}
			}
		}
		else {
			//01X
			// section 2-3
			if (!(hue & 0x20)) {
				// 010
				//case 2: // Y -> G
				if (Y1) {
					//uint8_t twothirds = (third << 1);
					uint8_t twothirds = scale8(offset8, ((256 * 2) / 3)); // max=170
					r = K171 - twothirds;
					g = K170 + third;
					b = 0;
				}
				if (Y2) {
					r = K255 - offset8;
					g = K255;
					b = 0;
				}
			}
			else {
				// 011
				// case 3: // G -> A
				r = 0;
				g = K255 - third;
				b = third;
			}
		}
	}
	else {
		// section 4-7
		// 1XX
		if (!(hue & 0x40)) {
			// 10X
			if (!(hue & 0x20)) {
				// 100
				//case 4: // A -> B
				r = 0;
				//uint8_t twothirds = (third << 1);
				uint8_t twothirds = scale8(offset8, ((256 * 2) / 3)); // max=170
				g = K171 - twothirds; //K170?
				b = K85 + twothirds;

			}
			else {
				// 101
				//case 5: // B -> P
				r = third;
				g = 0;
				b = K255 - third;

			}
		}
		else {
			if (!(hue & 0x20)) {
				// 110
				//case 6: // P -- K
				r = K85 + third;
				g = 0;
				b = K171 - third;

			}
			else {
				// 111
				//case 7: // K -> R
				r = K170 + third;
				g = 0;
				b = K85 - third;

			}
		}
	}

	// This is one of the good places to scale the green down,
	// although the client can scale green down as well.
	if (G2) g = g >> 1;
	if (Gscale) g = scale8_video_LEAVING_R1_DIRTY(g, Gscale);

	// Scale down colors if we're desaturated at all
	// and add the brightness_floor to r, g, and b.
	if (sat != 255) {
		if (sat == 0) {
			r = 255; b = 255; g = 255;
		}
		else {
			//nscale8x3_video( r, g, b, sat);
			if (r) r = scale8_LEAVING_R1_DIRTY(r, sat);
			if (g) g = scale8_LEAVING_R1_DIRTY(g, sat);
			if (b) b = scale8_LEAVING_R1_DIRTY(b, sat);
			cleanup_R1();

			uint8_t desat = 255 - sat;
			desat = scale8(desat, desat);

			uint8_t brightness_floor = desat;
			r += brightness_floor;
			g += brightness_floor;
			b += brightness_floor;
		}
	}

	// Now scale everything down if we're at value < 255.
	if (val != 255) {
		val = scale8_video_LEAVING_R1_DIRTY(val, val);
		if (val == 0) {
			r = 0; g = 0; b = 0;
		}
		else {
			if (r) r = scale8_LEAVING_R1_DIRTY(r, val);
			if (g) g = scale8_LEAVING_R1_DIRTY(g, val);
			if (b) b = scale8_LEAVING_R1_DIRTY(b, val);
			cleanup_R1();
		}
	}

	// Here we have the old AVR "missing std X+n" problem again
	// It turns out that fixing it winds up costing more than
	// not fixing it.
	// To paraphrase Dr Bronner, profile! profile! profile!
	//asm volatile(  ""  :  :  : "r26", "r27" );
	//asm volatile (" movw r30, r26 \n" : : : "r30", "r31");
	rgb.r = r;
	rgb.g = g;
	rgb.b = b;
}


void hsv2rgb_raw(const struct CHSV * phsv, struct CRGB * prgb, int numLeds) {
	for (int i = 0; i < numLeds; i++) {
		hsv2rgb_raw(phsv[i], prgb[i]);
	}
}

void hsv2rgb_rainbow(const struct CHSV* phsv, struct CRGB * prgb, int numLeds) {
	for (int i = 0; i < numLeds; i++) {
		hsv2rgb_rainbow(phsv[i], prgb[i]);
	}
}

void hsv2rgb_spectrum(const struct CHSV* phsv, struct CRGB * prgb, int numLeds) {
	for (int i = 0; i < numLeds; i++) {
		hsv2rgb_spectrum(phsv[i], prgb[i]);
	}
}





#endif




//class Colour {
//public:
//	Colour(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
//
//	void setHsb(unsigned char h, unsigned char s, unsigned char b) {
//
//	}
//
//	unsigned char r, g, b;
//};