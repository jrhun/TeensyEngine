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


typedef enum { FORWARD_HUES, BACKWARD_HUES, SHORTEST_HUES, LONGEST_HUES } TGradientDirectionCode;
#define saccum87 int16_t


template <typename T>
void fill_gradient(T* targetArray,
	uint16_t startpos, CHSV startcolor,
	uint16_t endpos, CHSV endcolor,
	TGradientDirectionCode directionCode = SHORTEST_HUES)
{
	// if the points are in the wrong order, straighten them
	if (endpos < startpos) {
		uint16_t t = endpos;
		CHSV tc = endcolor;
		endcolor = startcolor;
		endpos = startpos;
		startpos = t;
		startcolor = tc;
	}

	// If we're fading toward black (val=0) or white (sat=0),
	// then set the endhue to the starthue.
	// This lets us ramp smoothly to black or white, regardless
	// of what 'hue' was set in the endcolor (since it doesn't matter)
	if (endcolor.value == 0 || endcolor.saturation == 0) {
		endcolor.hue = startcolor.hue;
	}

	// Similarly, if we're fading in from black (val=0) or white (sat=0)
	// then set the starthue to the endhue.
	// This lets us ramp smoothly up from black or white, regardless
	// of what 'hue' was set in the startcolor (since it doesn't matter)
	if (startcolor.value == 0 || startcolor.saturation == 0) {
		startcolor.hue = endcolor.hue;
	}

	saccum87 huedistance87;
	saccum87 satdistance87;
	saccum87 valdistance87;

	satdistance87 = (endcolor.sat - startcolor.sat) << 7;
	valdistance87 = (endcolor.val - startcolor.val) << 7;

	uint8_t huedelta8 = endcolor.hue - startcolor.hue;

	if (directionCode == SHORTEST_HUES) {
		directionCode = FORWARD_HUES;
		if (huedelta8 > 127) {
			directionCode = BACKWARD_HUES;
		}
	}

	if (directionCode == LONGEST_HUES) {
		directionCode = FORWARD_HUES;
		if (huedelta8 < 128) {
			directionCode = BACKWARD_HUES;
		}
	}

	if (directionCode == FORWARD_HUES) {
		huedistance87 = huedelta8 << 7;
	}
	else /* directionCode == BACKWARD_HUES */
	{
		huedistance87 = (uint8_t)(256 - huedelta8) << 7;
		huedistance87 = -huedistance87;
	}

	uint16_t pixeldistance = endpos - startpos;
	int16_t divisor = pixeldistance ? pixeldistance : 1;

	saccum87 huedelta87 = huedistance87 / divisor;
	saccum87 satdelta87 = satdistance87 / divisor;
	saccum87 valdelta87 = valdistance87 / divisor;

	huedelta87 *= 2;
	satdelta87 *= 2;
	valdelta87 *= 2;

	accum88 hue88 = startcolor.hue << 8;
	accum88 sat88 = startcolor.sat << 8;
	accum88 val88 = startcolor.val << 8;
	for (uint16_t i = startpos; i <= endpos; i++) {
		targetArray[i] = CHSV(hue88 >> 8, sat88 >> 8, val88 >> 8);
		hue88 += huedelta87;
		sat88 += satdelta87;
		val88 += valdelta87;
	}
}


// Convenience functions to fill an array of colors with a
// two-color, three-color, or four-color gradient
template <typename T>
void fill_gradient(T* targetArray, uint16_t numLeds, const CHSV& c1, const CHSV& c2,
	TGradientDirectionCode directionCode = SHORTEST_HUES)
{
	uint16_t last = numLeds - 1;
	fill_gradient(targetArray, 0, c1, last, c2, directionCode);
}

template <typename T>
void fill_gradient(T* targetArray, uint16_t numLeds,
	const CHSV& c1, const CHSV& c2, const CHSV& c3,
	TGradientDirectionCode directionCode = SHORTEST_HUES)
{
	uint16_t half = (numLeds / 2);
	uint16_t last = numLeds - 1;
	fill_gradient(targetArray, 0, c1, half, c2, directionCode);
	fill_gradient(targetArray, half, c2, last, c3, directionCode);
}

template <typename T>
void fill_gradient(T* targetArray, uint16_t numLeds,
	const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4,
	TGradientDirectionCode directionCode = SHORTEST_HUES)
{
	uint16_t onethird = (numLeds / 3);
	uint16_t twothirds = ((numLeds * 2) / 3);
	uint16_t last = numLeds - 1;
	fill_gradient(targetArray, 0, c1, onethird, c2, directionCode);
	fill_gradient(targetArray, onethird, c2, twothirds, c3, directionCode);
	fill_gradient(targetArray, twothirds, c3, last, c4, directionCode);
}

void fill_solid(struct CRGB * leds, int numToFill,
	const struct CRGB& color)
{
	for (int i = 0; i < numToFill; i++) {
		leds[i] = color;
	}
}

void fill_solid(struct CHSV * targetArray, int numToFill,
	const struct CHSV& hsvColor)
{
	for (int i = 0; i < numToFill; i++) {
		targetArray[i] = hsvColor;
	}
}

void fill_rainbow(struct CRGB * pFirstLED, int numToFill,
	uint8_t initialhue,
	uint8_t deltahue)
{
	CHSV hsv;
	hsv.hue = initialhue;
	hsv.val = 255;
	hsv.sat = 240;
	for (int i = 0; i < numToFill; i++) {
		pFirstLED[i] = hsv;
		hsv.hue += deltahue;
	}
}

void fill_rainbow(struct CHSV * targetArray, int numToFill,
	uint8_t initialhue,
	uint8_t deltahue)
{
	CHSV hsv;
	hsv.hue = initialhue;
	hsv.val = 255;
	hsv.sat = 240;
	for (int i = 0; i < numToFill; i++) {
		targetArray[i] = hsv;
		hsv.hue += deltahue;
	}
}


void fill_gradient_RGB(CRGB* leds,
	uint16_t startpos, CRGB startcolor,
	uint16_t endpos, CRGB endcolor)
{
	// if the points are in the wrong order, straighten them
	if (endpos < startpos) {
		uint16_t t = endpos;
		CRGB tc = endcolor;
		endcolor = startcolor;
		endpos = startpos;
		startpos = t;
		startcolor = tc;
	}

	saccum87 rdistance87;
	saccum87 gdistance87;
	saccum87 bdistance87;

	rdistance87 = (endcolor.r - startcolor.r) << 7;
	gdistance87 = (endcolor.g - startcolor.g) << 7;
	bdistance87 = (endcolor.b - startcolor.b) << 7;

	uint16_t pixeldistance = endpos - startpos;
	int16_t divisor = pixeldistance ? pixeldistance : 1;

	saccum87 rdelta87 = rdistance87 / divisor;
	saccum87 gdelta87 = gdistance87 / divisor;
	saccum87 bdelta87 = bdistance87 / divisor;

	rdelta87 *= 2;
	gdelta87 *= 2;
	bdelta87 *= 2;

	accum88 r88 = startcolor.r << 8;
	accum88 g88 = startcolor.g << 8;
	accum88 b88 = startcolor.b << 8;
	for (uint16_t i = startpos; i <= endpos; i++) {
		leds[i] = CRGB(r88 >> 8, g88 >> 8, b88 >> 8);
		r88 += rdelta87;
		g88 += gdelta87;
		b88 += bdelta87;
	}
}

void fill_gradient_RGB(CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2)
{
	uint16_t last = numLeds - 1;
	fill_gradient_RGB(leds, 0, c1, last, c2);
}


void fill_gradient_RGB(CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2, const CRGB& c3)
{
	uint16_t half = (numLeds / 2);
	uint16_t last = numLeds - 1;
	fill_gradient_RGB(leds, 0, c1, half, c2);
	fill_gradient_RGB(leds, half, c2, last, c3);
}

void fill_gradient_RGB(CRGB* leds, uint16_t numLeds, const CRGB& c1, const CRGB& c2, const CRGB& c3, const CRGB& c4)
{
	uint16_t onethird = (numLeds / 3);
	uint16_t twothirds = ((numLeds * 2) / 3);
	uint16_t last = numLeds - 1;
	fill_gradient_RGB(leds, 0, c1, onethird, c2);
	fill_gradient_RGB(leds, onethird, c2, twothirds, c3);
	fill_gradient_RGB(leds, twothirds, c3, last, c4);
}


void nscale8(CRGB* leds, uint16_t num_leds, uint8_t scale);

void nscale8_video(CRGB* leds, uint16_t num_leds, uint8_t scale)
{
	for (uint16_t i = 0; i < num_leds; i++) {
		leds[i].nscale8_video(scale);
	}
}

void fade_video(CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
{
	nscale8_video(leds, num_leds, 255 - fadeBy);
}

void fadeLightBy(CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
{
	nscale8_video(leds, num_leds, 255 - fadeBy);
}


void fadeToBlackBy(CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
{
	nscale8(leds, num_leds, 255 - fadeBy);
}

void fade_raw(CRGB* leds, uint16_t num_leds, uint8_t fadeBy)
{
	nscale8(leds, num_leds, 255 - fadeBy);
}

void nscale8_raw(CRGB* leds, uint16_t num_leds, uint8_t scale)
{
	nscale8(leds, num_leds, scale);
}

void nscale8(CRGB* leds, uint16_t num_leds, uint8_t scale)
{
	for (uint16_t i = 0; i < num_leds; i++) {
		leds[i].nscale8(scale);
	}
}

void fadeUsingColor(CRGB* leds, uint16_t numLeds, const CRGB& colormask)
{
	uint8_t fr, fg, fb;
	fr = colormask.r;
	fg = colormask.g;
	fb = colormask.b;

	for (uint16_t i = 0; i < numLeds; i++) {
		leds[i].r = scale8_LEAVING_R1_DIRTY(leds[i].r, fr);
		leds[i].g = scale8_LEAVING_R1_DIRTY(leds[i].g, fg);
		leds[i].b = scale8(leds[i].b, fb);
	}
}


CRGB& nblend(CRGB& existing, const CRGB& overlay, fract8 amountOfOverlay)
{
	if (amountOfOverlay == 0) {
		return existing;
	}

	if (amountOfOverlay == 255) {
		existing = overlay;
		return existing;
	}

#if 0
	// Old blend method which unfortunately had some rounding errors
	fract8 amountOfKeep = 255 - amountOfOverlay;

	existing.red = scale8_LEAVING_R1_DIRTY(existing.red, amountOfKeep)
		+ scale8_LEAVING_R1_DIRTY(overlay.red, amountOfOverlay);
	existing.green = scale8_LEAVING_R1_DIRTY(existing.green, amountOfKeep)
		+ scale8_LEAVING_R1_DIRTY(overlay.green, amountOfOverlay);
	existing.blue = scale8_LEAVING_R1_DIRTY(existing.blue, amountOfKeep)
		+ scale8_LEAVING_R1_DIRTY(overlay.blue, amountOfOverlay);

	cleanup_R1();
#else
	// Corrected blend method, with no loss-of-precision rounding errors
	existing.red = blend8(existing.red, overlay.red, amountOfOverlay);
	existing.green = blend8(existing.green, overlay.green, amountOfOverlay);
	existing.blue = blend8(existing.blue, overlay.blue, amountOfOverlay);
#endif

	return existing;
}



void nblend(CRGB* existing, CRGB* overlay, uint16_t count, fract8 amountOfOverlay)
{
	for (uint16_t i = count; i; i--) {
		nblend(*existing, *overlay, amountOfOverlay);
		existing++;
		overlay++;
	}
}

CRGB blend(const CRGB& p1, const CRGB& p2, fract8 amountOfP2)
{
	CRGB nu(p1);
	nblend(nu, p2, amountOfP2);
	return nu;
}

CRGB* blend(const CRGB* src1, const CRGB* src2, CRGB* dest, uint16_t count, fract8 amountOfsrc2)
{
	for (uint16_t i = 0; i < count; i++) {
		dest[i] = blend(src1[i], src2[i], amountOfsrc2);
	}
	return dest;
}



CHSV& nblend(CHSV& existing, const CHSV& overlay, fract8 amountOfOverlay, TGradientDirectionCode directionCode)
{
	if (amountOfOverlay == 0) {
		return existing;
	}

	if (amountOfOverlay == 255) {
		existing = overlay;
		return existing;
	}

	fract8 amountOfKeep = 255 - amountOfOverlay;

	uint8_t huedelta8 = overlay.hue - existing.hue;

	if (directionCode == SHORTEST_HUES) {
		directionCode = FORWARD_HUES;
		if (huedelta8 > 127) {
			directionCode = BACKWARD_HUES;
		}
	}

	if (directionCode == LONGEST_HUES) {
		directionCode = FORWARD_HUES;
		if (huedelta8 < 128) {
			directionCode = BACKWARD_HUES;
		}
	}

	if (directionCode == FORWARD_HUES) {
		existing.hue = existing.hue + scale8(huedelta8, amountOfOverlay);
	}
	else /* directionCode == BACKWARD_HUES */
	{
		huedelta8 = -huedelta8;
		existing.hue = existing.hue - scale8(huedelta8, amountOfOverlay);
	}

	existing.sat = scale8_LEAVING_R1_DIRTY(existing.sat, amountOfKeep)
		+ scale8_LEAVING_R1_DIRTY(overlay.sat, amountOfOverlay);
	existing.val = scale8_LEAVING_R1_DIRTY(existing.val, amountOfKeep)
		+ scale8_LEAVING_R1_DIRTY(overlay.val, amountOfOverlay);

	cleanup_R1();

	return existing;
}



void nblend(CHSV* existing, CHSV* overlay, uint16_t count, fract8 amountOfOverlay, TGradientDirectionCode directionCode)
{
	if (existing == overlay) return;
	for (uint16_t i = count; i; i--) {
		nblend(*existing, *overlay, amountOfOverlay, directionCode);
		existing++;
		overlay++;
	}
}

CHSV blend(const CHSV& p1, const CHSV& p2, fract8 amountOfP2, TGradientDirectionCode directionCode)
{
	CHSV nu(p1);
	nblend(nu, p2, amountOfP2, directionCode);
	return nu;
}

CHSV* blend(const CHSV* src1, const CHSV* src2, CHSV* dest, uint16_t count, fract8 amountOfsrc2, TGradientDirectionCode directionCode)
{
	for (uint16_t i = 0; i < count; i++) {
		dest[i] = blend(src1[i], src2[i], amountOfsrc2, directionCode);
	}
	return dest;
}

void blur1d(CRGB* leds, uint16_t numLeds, fract8 blur_amount)
{
	uint8_t keep = 255 - blur_amount;
	uint8_t seep = blur_amount >> 1;
	CRGB carryover = CRGB::Black;
	for (uint16_t i = 0; i < numLeds; i++) {
		CRGB cur = leds[i];
		CRGB part = cur;
		part.nscale8(seep);
		cur.nscale8(keep);
		cur += carryover;
		if (i) leds[i - 1] += part;
		leds[i] = cur;
		carryover = part;
	}
}



// blurRows: perform a blur1d on every row of a rectangular matrix
void blurRows(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
	for (uint8_t row = 0; row < height; row++) {
		CRGB* rowbase = leds + (row * width);
		blur1d(rowbase, width, blur_amount);
	}
}

// blurColumns: perform a blur1d on each column of a rectangular matrix
void blurColumns(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
	auto XY = [](uint8_t x, uint8_t y, uint8_t width) {
		return x + y * width;
	};
	// blur columns
	uint8_t keep = 255 - blur_amount;
	uint8_t seep = blur_amount >> 1;
	for (uint8_t col = 0; col < width; col++) {
		CRGB carryover = CRGB::Black;
		for (uint8_t i = 0; i < height; i++) {
			CRGB cur = leds[XY(col, i, width)];
			CRGB part = cur;
			part.nscale8(seep);
			cur.nscale8(keep);
			cur += carryover;
			if (i) leds[XY(col, i - 1, width)] += part;
			leds[XY(col, i, width)] = cur;
			carryover = part;
		}
	}
}

void blur2d(CRGB* leds, uint8_t width, uint8_t height, fract8 blur_amount)
{
	blurRows(leds, width, height, blur_amount);
	blurColumns(leds, width, height, blur_amount);
}


uint8_t applyGamma_video( uint8_t brightness, float gamma);
CRGB    applyGamma_video( const CRGB& orig, float gamma);
CRGB    applyGamma_video( const CRGB& orig, float gammaR, float gammaG, float gammaB);
// The "n" versions below modify their arguments in-place.
CRGB&  napplyGamma_video( CRGB& rgb, float gamma);
CRGB&  napplyGamma_video( CRGB& rgb, float gammaR, float gammaG, float gammaB);
void   napplyGamma_video( CRGB* rgbarray, uint16_t count, float gamma);
void   napplyGamma_video( CRGB* rgbarray, uint16_t count, float gammaR, float gammaG, float gammaB);

uint8_t applyGamma_video( uint8_t brightness, float gamma)
{
    float orig;
    float adj;
    orig = (float)(brightness) / (255.0);
    adj =  pow( orig, gamma)   * (255.0);
    uint8_t result = (uint8_t)(adj);
    if( (brightness > 0) && (result == 0)) {
        result = 1; // never gamma-adjust a positive number down to zero
    }
    return result;
}

CRGB applyGamma_video( const CRGB& orig, float gamma)
{
    CRGB adj;
    adj.r = applyGamma_video( orig.r, gamma);
    adj.g = applyGamma_video( orig.g, gamma);
    adj.b = applyGamma_video( orig.b, gamma);
    return adj;
}

CRGB applyGamma_video( const CRGB& orig, float gammaR, float gammaG, float gammaB)
{
    CRGB adj;
    adj.r = applyGamma_video( orig.r, gammaR);
    adj.g = applyGamma_video( orig.g, gammaG);
    adj.b = applyGamma_video( orig.b, gammaB);
    return adj;
}


CRGB& napplyGamma_video( CRGB& rgb, float gamma)
{
    rgb = applyGamma_video( rgb, gamma);
    return rgb;
}

CRGB& napplyGamma_video( CRGB& rgb, float gammaR, float gammaG, float gammaB)
{
    rgb = applyGamma_video( rgb, gammaR, gammaG, gammaB);
    return rgb;
}

void napplyGamma_video( CRGB* rgbarray, uint16_t count, float gamma)
{
    for( uint16_t i = 0; i < count; i++) {
        rgbarray[i] = applyGamma_video( rgbarray[i], gamma);
    }
}

void napplyGamma_video( CRGB* rgbarray, uint16_t count, float gammaR, float gammaG, float gammaB)
{
    for( uint16_t i = 0; i < count; i++) {
        rgbarray[i] = applyGamma_video( rgbarray[i], gammaR, gammaG, gammaB);
    }
}










// CRGBSet

#ifndef abs
#include <stdlib.h>
#endif

/// Represents a set of CRGB led objects.  Provides the [] array operator, and works like a normal array in that case.
/// This should be kept in sync with the set of functions provided by CRGB as well as functions in colorutils.  Note
/// that a pixel set is a window into another set of led data, it is not its own set of led data.
template<class PIXEL_TYPE>
class CPixelView {
public:
	const int8_t  dir;
	const int   len;
	PIXEL_TYPE * const leds;
	PIXEL_TYPE * const end_pos;

public:

	/// PixelSet copy constructor
	inline CPixelView(const CPixelView & other) : dir(other.dir), len(other.len), leds(other.leds), end_pos(other.end_pos) {}

	/// pixelset constructor for a pixel set starting at the given PIXEL_TYPE* and going for _len leds.  Note that the length
	/// can be backwards, creating a PixelSet that walks backwards over the data
	/// @param leds point to the raw led data
	/// @param len how many leds in this set
	inline CPixelView(PIXEL_TYPE *_leds, int _len) : dir(_len < 0 ? -1 : 1), len(_len), leds(_leds), end_pos(_leds + _len) {}

	/// PixelSet constructor for the given set of leds, with start and end boundaries.  Note that start can be after
	/// end, resulting in a set that will iterate backwards
	/// @param leds point to the raw led data
	/// @param start the start index of the leds for this array
	/// @param end the end index of the leds for this array
	inline CPixelView(PIXEL_TYPE *_leds, int _start, int _end) : dir(((_end - _start) < 0) ? -1 : 1), len((_end - _start) + dir), leds(_leds + _start), end_pos(_leds + _start + len) {}

	/// Get the size of this set
	/// @return the size of the set
	int size() { return abs(len); }

	/// Whether or not this set goes backwards
	/// @return whether or not the set is backwards
	bool reversed() { return len < 0; }

	/// do these sets point to the same thing (note, this is different from the contents of the set being the same)
	bool operator==(const CPixelView & rhs) const { return leds == rhs.leds && len == rhs.len && dir == rhs.dir; }

	/// do these sets point to the different things (note, this is different from the contents of the set being the same)
	bool operator!=(const CPixelView & rhs) const { return leds != rhs.leds || len != rhs.len || dir != rhs.dir; }

	/// access a single element in this set, just like an array operator
	inline PIXEL_TYPE & operator[](int x) const { if (dir & 0x80) { return leds[-x]; } else { return leds[x]; } }

	/// Access an inclusive subset of the leds in this set.  Note that start can be greater than end, which will
	/// result in a reverse ordering for many functions (useful for mirroring)
	/// @param start the first element from this set for the new subset
	/// @param end the last element for the new subset
	inline CPixelView operator()(int start, int end) { return CPixelView(leds, start, end); }

	/// Access an inclusive subset of the leds in this set, starting from the first.
	/// @param end the last element for the new subset
	/// Not sure i want this? inline CPixelView operator()(int end) { return CPixelView(leds, 0, end); }

	/// Return the reverse ordering of this set
	inline CPixelView operator-() { return CPixelView(leds + len - dir, len - dir, 0); }

	/// Return a pointer to the first element in this set
	inline operator PIXEL_TYPE* () const { return leds; }

	/// Assign the passed in color to all elements in this set
	/// @param color the new color for the elements in the set
	inline CPixelView & operator=(const PIXEL_TYPE & color) {
		for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) = color; }
		return *this;
	}


	void dump() const {
		/**
			Serial.print("len: "); Serial.print(len); Serial.print(", dir:"); Serial.print((int)dir);
			Serial.print(", range:"); Serial.print((uint32_t)leds); Serial.print("-"); Serial.print((uint32_t)end_pos);
			Serial.print(", diff:"); Serial.print((int32_t)(end_pos - leds));
			Serial.println("");
		 **/
	}

	/// Copy the contents of the passed in set to our set.  Note if one set is smaller than the other, only the
	/// smallest number of items will be copied over.
	inline CPixelView & operator=(const CPixelView & rhs) {
		for (iterator pixel = begin(), rhspixel = rhs.begin(), _end = end(), rhs_end = rhs.end(); (pixel != _end) && (rhspixel != rhs_end); ++pixel, ++rhspixel) {
			(*pixel) = (*rhspixel);
		}
		return *this;
	}

	/// @name modification/scaling operators
	//@{
	/// Add the passed in value to r,g, b for all the pixels in this set
	inline CPixelView & addToRGB(uint8_t inc) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) += inc; } return *this; }
	/// Add every pixel in the other set to this set
	inline CPixelView & operator+=(CPixelView & rhs) { for (iterator pixel = begin(), rhspixel = rhs.begin(), _end = end(), rhs_end = rhs.end(); (pixel != _end) && (rhspixel != rhs_end); ++pixel, ++rhspixel) { (*pixel) += (*rhspixel); } return *this; }

	/// Subtract the passed in value from r,g,b for all pixels in this set
	inline CPixelView & subFromRGB(uint8_t inc) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) -= inc; } return *this; }
	/// Subtract every pixel in the other set from this set
	inline CPixelView & operator-=(CPixelView & rhs) { for (iterator pixel = begin(), rhspixel = rhs.begin(), _end = end(), rhs_end = rhs.end(); (pixel != _end) && (rhspixel != rhs_end); ++pixel, ++rhspixel) { (*pixel) -= (*rhspixel); } return *this; }

	/// Increment every pixel value in this set
	inline CPixelView & operator++() { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel)++; } return *this; }
	/// Increment every pixel value in this set
	inline CPixelView & operator++(int DUMMY_ARG) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel)++; } return *this; }

	/// Decrement every pixel value in this set
	inline CPixelView & operator--() { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel)--; } return *this; }
	/// Decrement every pixel value in this set
	inline CPixelView & operator--(int DUMMY_ARG) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel)--; } return *this; }

	/// Divide every led by the given value
	inline CPixelView & operator/=(uint8_t d) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) /= d; } return *this; }
	/// Shift every led in this set right by the given number of bits
	inline CPixelView & operator>>=(uint8_t d) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) >>= d; } return *this; }
	/// Multiply every led in this set by the given value
	inline CPixelView & operator*=(uint8_t d) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) *= d; } return *this; }

	/// Scale every led by the given scale
	inline CPixelView & nscale8_video(uint8_t scaledown) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel).nscale8_video(scaledown); } return *this; }
	/// Scale down every led by the given scale
	inline CPixelView & operator%=(uint8_t scaledown) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel).nscale8_video(scaledown); } return *this; }
	/// Fade every led down by the given scale
	inline CPixelView & fadeLightBy(uint8_t fadefactor) { return nscale8_video(255 - fadefactor); }

	/// Scale every led by the given scale
	inline CPixelView & nscale8(uint8_t scaledown) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel).nscale8(scaledown); } return *this; }
	/// Scale every led by the given scale
	inline CPixelView & nscale8(PIXEL_TYPE & scaledown) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel).nscale8(scaledown); } return *this; }
	/// Scale every led in this set by every led in the other set
	inline CPixelView & nscale8(CPixelView & rhs) { for (iterator pixel = begin(), rhspixel = rhs.begin(), _end = end(), rhs_end = rhs.end(); (pixel != _end) && (rhspixel != rhs_end); ++pixel, ++rhspixel) { (*pixel).nscale8((*rhspixel)); } return *this; }

	/// Fade every led down by the given scale
	inline CPixelView & fadeToBlackBy(uint8_t fade) { return nscale8(255 - fade); }

	/// Apply the PIXEL_TYPE |= operator to every pixel in this set with the given PIXEL_TYPE value (bringing each channel to the higher of the two values)
	inline CPixelView & operator|=(const PIXEL_TYPE & rhs) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) |= rhs; } return *this; }
	/// Apply the PIXEL_TYPE |= operator to every pixel in this set with every pixel in the passed in set
	inline CPixelView & operator|=(const CPixelView & rhs) { for (iterator pixel = begin(), rhspixel = rhs.begin(), _end = end(), rhs_end = rhs.end(); (pixel != _end) && (rhspixel != rhs_end); ++pixel, ++rhspixel) { (*pixel) |= (*rhspixel); } return *this; }
	/// Apply the PIXEL_TYPE |= operator to every pixel in this set
	inline CPixelView & operator|=(uint8_t d) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) |= d; } return *this; }

	/// Apply the PIXEL_TYPE &= operator to every pixel in this set with the given PIXEL_TYPE value (bringing each channel down to the lower of the two values)
	inline CPixelView & operator&=(const PIXEL_TYPE & rhs) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) &= rhs; } return *this; }
	/// Apply the PIXEL_TYPE &= operator to every pixel in this set with every pixel in the passed in set
	inline CPixelView & operator&=(const CPixelView & rhs) { for (iterator pixel = begin(), rhspixel = rhs.begin(), _end = end(), rhs_end = rhs.end(); (pixel != _end) && (rhspixel != rhs_end); ++pixel, ++rhspixel) { (*pixel) &= (*rhspixel); } return *this; }
	/// APply the PIXEL_TYPE &= operator to every pixel in this set with the passed in value
	inline CPixelView & operator&=(uint8_t d) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { (*pixel) &= d; } return *this; }
	//@}

	/// Returns whether or not any leds in this set are non-zero
	inline operator bool() { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { if ((*pixel)) return true; } return false; }

	// Color util functions
	inline CPixelView & fill_solid(const PIXEL_TYPE & color) { *this = color; return *this; }
	inline CPixelView & fill_solid(const CHSV & color) { if (dir > 0) { *this = color; return *this; } }

	inline CPixelView & fill_rainbow(uint8_t initialhue, uint8_t deltahue = 5) {
		if (dir >= 0) {
			::fill_rainbow(leds, len, initialhue, deltahue);
		}
		else {
			::fill_rainbow(leds + len + 1, -len, initialhue, deltahue);
		}
		return *this;
	}

	inline CPixelView & fill_gradient(const CHSV & startcolor, const CHSV & endcolor, TGradientDirectionCode directionCode = SHORTEST_HUES) {
		if (dir >= 0) {
			::fill_gradient(leds, len, startcolor, endcolor, directionCode);
		}
		else {
			::fill_gradient(leds + len + 1, (-len), endcolor, startcolor, directionCode);
		}
		return *this;
	}

	inline CPixelView & fill_gradient(const CHSV & c1, const CHSV & c2, const CHSV &  c3, TGradientDirectionCode directionCode = SHORTEST_HUES) {
		if (dir >= 0) {
			::fill_gradient(leds, len, c1, c2, c3, directionCode);
		}
		else {
			::fill_gradient(leds + len + 1, -len, c3, c2, c1, directionCode);
		}
		return *this;
	}

	inline CPixelView & fill_gradient(const CHSV & c1, const CHSV & c2, const CHSV & c3, const CHSV & c4, TGradientDirectionCode directionCode = SHORTEST_HUES) {
		if (dir >= 0) {
			::fill_gradient(leds, len, c1, c2, c3, c4, directionCode);
		}
		else {
			::fill_gradient(leds + len + 1, -len, c4, c3, c2, c1, directionCode);
		}
		return *this;
	}

	inline CPixelView & fill_gradient_RGB(const PIXEL_TYPE & startcolor, const PIXEL_TYPE & endcolor, TGradientDirectionCode directionCode = SHORTEST_HUES) {
		if (dir >= 0) {
			::fill_gradient_RGB(leds, len, startcolor, endcolor);
		}
		else {
			::fill_gradient_RGB(leds + len + 1, (-len), endcolor, startcolor);
		}
		return *this;
	}

	inline CPixelView & fill_gradient_RGB(const PIXEL_TYPE & c1, const PIXEL_TYPE & c2, const PIXEL_TYPE &  c3) {
		if (dir >= 0) {
			::fill_gradient_RGB(leds, len, c1, c2, c3);
		}
		else {
			::fill_gradient_RGB(leds + len + 1, -len, c3, c2, c1);
		}
		return *this;
	}

	inline CPixelView & fill_gradient_RGB(const PIXEL_TYPE & c1, const PIXEL_TYPE & c2, const PIXEL_TYPE & c3, const PIXEL_TYPE & c4) {
		if (dir >= 0) {
			::fill_gradient_RGB(leds, len, c1, c2, c3, c4);
		}
		else {
			::fill_gradient_RGB(leds + len + 1, -len, c4, c3, c2, c1);
		}
		return *this;
	}

	inline CPixelView & nblend(const PIXEL_TYPE & overlay, fract8 amountOfOverlay) { for (iterator pixel = begin(), _end = end(); pixel != _end; ++pixel) { ::nblend((*pixel), overlay, amountOfOverlay); } return *this; }
	inline CPixelView & nblend(const CPixelView & rhs, fract8 amountOfOverlay) { for (iterator pixel = begin(), rhspixel = rhs.begin(), _end = end(), rhs_end = rhs.end(); (pixel != _end) && (rhspixel != rhs_end); ++pixel, ++rhspixel) { ::nblend((*pixel), (*rhspixel), amountOfOverlay); } return *this; }

	// Note: only bringing in a 1d blur, not sure 2d blur makes sense when looking at sub arrays
	inline CPixelView & blur1d(fract8 blur_amount) {
		if (dir >= 0) {
			::blur1d(leds, len, blur_amount);
		}
		else {
			::blur1d(leds + len + 1, -len, blur_amount);
		}
		return *this;
	}

	inline CPixelView & napplyGamma_video(float gamma) {
		if (dir >= 0) {
			::napplyGamma_video(leds, len, gamma);
		}
		else {
			::napplyGamma_video(leds + len + 1, -len, gamma);
		}
		return *this;
	}

	inline CPixelView & napplyGamma_video(float gammaR, float gammaG, float gammaB) {
		if (dir >= 0) {
			::napplyGamma_video(leds, len, gammaR, gammaG, gammaB);
		}
		else {
			::napplyGamma_video(leds + len + 1, -len, gammaR, gammaG, gammaB);
		}
		return *this;
	}

	// TODO: Make this a fully specified/proper iterator
	template <class T>
	class pixelset_iterator_base {
		T * leds;
		const int8_t dir;
	public:
		inline pixelset_iterator_base(const pixelset_iterator_base & rhs) : leds(rhs.leds), dir(rhs.dir) {}
		inline pixelset_iterator_base(T * _leds, const char _dir) : leds(_leds), dir(_dir) {}

		inline pixelset_iterator_base& operator++() { leds += dir; return *this; }
		inline pixelset_iterator_base operator++(int) { pixelset_iterator_base tmp(*this); leds += dir; return tmp; }

		inline bool operator==(pixelset_iterator_base & other) const { return leds == other.leds; } // && set==other.set; }
		inline bool operator!=(pixelset_iterator_base & other) const { return leds != other.leds; } // || set != other.set; }

		inline PIXEL_TYPE& operator*() const { return *leds; }
	};

	typedef pixelset_iterator_base<PIXEL_TYPE> iterator;
	typedef pixelset_iterator_base<const PIXEL_TYPE> const_iterator;

	iterator begin() { return iterator(leds, dir); }
	iterator end() { return iterator(end_pos, dir); }

	iterator begin() const { return iterator(leds, dir); }
	iterator end() const { return iterator(end_pos, dir); }

	const_iterator cbegin() const { return const_iterator(leds, dir); }
	const_iterator cend() const { return const_iterator(end_pos, dir); }
};

typedef CPixelView<CRGB> CRGBSet;

inline CRGB *operator+(const CRGBSet & pixels, int offset) { return (CRGB*)pixels + offset; }

template<int SIZE>
class CRGBArray : public CPixelView<CRGB> {
public:
	CRGB rawleds[SIZE];
	CRGBArray() : CPixelView<CRGB>(rawleds, SIZE) {}
	using CPixelView::operator=;
};







void fill_raw_noise8(uint8_t *pData, uint8_t num_points, uint8_t octaves, uint16_t x, int scale, uint16_t time) {
	uint32_t _xx = x;
	uint32_t scx = scale;
	for (int o = 0; o < octaves; o++) {
		for (int i = 0, xx = _xx; i < num_points; i++, xx += scx) {
			pData[i] = qadd8(pData[i], inoise8(xx, time) >> o);
		}

		_xx <<= 1;
		scx <<= 1;
	}
}

void fill_raw_noise16into8(uint8_t *pData, uint8_t num_points, uint8_t octaves, uint32_t x, int scale, uint32_t time) {
	uint32_t _xx = x;
	uint32_t scx = scale;
	for (int o = 0; o < octaves; o++) {
		for (int i = 0, xx = _xx; i < num_points; i++, xx += scx) {
			uint32_t accum = (inoise16(xx, time)) >> o;
			accum += (pData[i] << 8);
			if (accum > 65535) { accum = 65535; }
			pData[i] = accum >> 8;
		}

		_xx <<= 1;
		scx <<= 1;
	}
}

void fill_raw_2dnoise8(uint8_t *pData, int width, int height, uint8_t octaves, q44 freq44, fract8 amplitude, int skip, uint16_t x, int scalex, uint16_t y, int scaley, uint16_t time) {
	if (octaves > 1) {
		fill_raw_2dnoise8(pData, width, height, octaves - 1, freq44, amplitude, skip + 1, x*freq44, freq44 * scalex, y*freq44, freq44 * scaley, time);
	}
	else {
		// amplitude is always 255 on the lowest level
		amplitude = 255;
	}

	scalex *= skip;
	scaley *= skip;

	fract8 invamp = 255 - amplitude;
	uint16_t xx = x;
	for (int i = 0; i < height; i++, y += scaley) {
		uint8_t *pRow = pData + (i*width);
		xx = x;
		for (int j = 0; j < width; j++, xx += scalex) {
			uint8_t noise_base = inoise8(xx, y, time);
			noise_base = (0x80 & noise_base) ? (noise_base - 127) : (127 - noise_base);
			noise_base = scale8(noise_base << 1, amplitude);
			if (skip == 1) {
				pRow[j] = scale8(pRow[j], invamp) + noise_base;
			}
			else {
				for (int ii = i; ii < (i + skip) && ii < height; ii++) {
					uint8_t *pRow = pData + (ii*width);
					for (int jj = j; jj < (j + skip) && jj < width; jj++) {
						pRow[jj] = scale8(pRow[jj], invamp) + noise_base;
					}
				}
			}
		}
	}
}

void fill_raw_2dnoise8(uint8_t *pData, int width, int height, uint8_t octaves, uint16_t x, int scalex, uint16_t y, int scaley, uint16_t time) {
	fill_raw_2dnoise8(pData, width, height, octaves, q44(2, 0), 128, 1, x, scalex, y, scaley, time);
}

void fill_raw_2dnoise16(uint16_t *pData, int width, int height, uint8_t octaves, q88 freq88, fract16 amplitude, int skip, uint32_t x, int scalex, uint32_t y, int scaley, uint32_t time) {
	if (octaves > 1) {
		fill_raw_2dnoise16(pData, width, height, octaves - 1, freq88, amplitude, skip, x *freq88, scalex *freq88, y * freq88, scaley * freq88, time);
	}
	else {
		// amplitude is always 255 on the lowest level
		amplitude = 65535;
	}

	scalex *= skip;
	scaley *= skip;
	fract16 invamp = 65535 - amplitude;
	for (int i = 0; i < height; i += skip, y += scaley) {
		uint16_t *pRow = pData + (i*width);
		for (int j = 0, xx = x; j < width; j += skip, xx += scalex) {
			uint16_t noise_base = inoise16(xx, y, time);
			noise_base = (0x8000 & noise_base) ? noise_base - (32767) : 32767 - noise_base;
			noise_base = scale16(noise_base << 1, amplitude);
			if (skip == 1) {
				pRow[j] = scale16(pRow[j], invamp) + noise_base;
			}
			else {
				for (int ii = i; ii < (i + skip) && ii < height; ii++) {
					uint16_t *pRow = pData + (ii*width);
					for (int jj = j; jj < (j + skip) && jj < width; jj++) {
						pRow[jj] = scale16(pRow[jj], invamp) + noise_base;
					}
				}
			}
		}
	}
}

int32_t nmin = 11111110;
int32_t nmax = 0;

void fill_raw_2dnoise16into8(uint8_t *pData, const int width, const int height, uint8_t octaves, q44 freq44, fract8 amplitude, int skip, uint32_t x, int scalex, uint32_t y, int scaley, uint32_t time) {
	if (octaves > 1) {
		fill_raw_2dnoise16into8(pData, width, height, octaves - 1, freq44, amplitude, skip + 1, x*freq44, scalex *freq44, y*freq44, scaley * freq44, time);
	}
	else {
		// amplitude is always 255 on the lowest level
		amplitude = 255;
	}

	scalex *= skip;
	scaley *= skip;
	uint32_t xx;
	fract8 invamp = 255 - amplitude;
	for (int i = 0; i < height; i += skip, y += scaley) {
		uint8_t *pRow = pData + (i*width);
		xx = x;
		for (int j = 0; j < width; j += skip, xx += scalex) {
			uint16_t noise_base = inoise16(xx, y, time);
			noise_base = (0x8000 & noise_base) ? noise_base - (32767) : 32767 - noise_base;
			noise_base = scale8(noise_base >> 7, amplitude);
			if (skip == 1) {
				pRow[j] = qadd8(scale8(pRow[j], invamp), noise_base);
			}
			else {
				for (int ii = i; ii < (i + skip) && ii < height; ii++) {
					uint8_t *pRow = pData + (ii*width);
					for (int jj = j; jj < (j + skip) && jj < width; jj++) {
						pRow[jj] = scale8(pRow[jj], invamp) + noise_base;
					}
				}
			}
		}
	}
}

void fill_raw_2dnoise16into8(uint8_t *pData, const int width, const int height, uint8_t octaves, uint32_t x, int scalex, uint32_t y, int scaley, uint32_t time) {
	fill_raw_2dnoise16into8(pData, width, height, octaves, q44(2, 0), 171, 1, x, scalex, y, scaley, time);
}


void fill_2dnoise8(CRGB *leds, int width, int height, bool serpentine,
	uint8_t octaves, uint16_t x, int xscale, uint16_t y, int yscale, uint16_t time,
	uint8_t hue_octaves, uint16_t hue_x, int hue_xscale, uint16_t hue_y, uint16_t hue_yscale, uint16_t hue_time, bool blend) {
	uint8_t *V = new uint8_t[height*width];
	uint8_t *H = new uint8_t[height*width];

	memset(V, 0, height*width);
	memset(H, 0, height*width);

	fill_raw_2dnoise8((uint8_t*)V, width, height, octaves, x, xscale, y, yscale, time);
	fill_raw_2dnoise8((uint8_t*)H, width, height, hue_octaves, hue_x, hue_xscale, hue_y, hue_yscale, hue_time);

	int w1 = width - 1;
	int h1 = height - 1;
	for (int i = 0; i < height; i++) {
		int wb = i * width;
		for (int j = 0; j < width; j++) {
			CRGB led(CHSV(H[(h1 - i) + (w1 - j) * width], 255, V[i+ j * width]));

			int pos = j;
			if (serpentine && (i & 0x1)) {
				pos = w1 - j;
			}

			if (blend) {
				leds[wb + pos] >>= 1; leds[wb + pos] += (led >>= 1);
			}
			else {
				leds[wb + pos] = led;
			}
		}
	}
	delete[] V;
	delete[] H;
}

void fill_2dnoise16(CRGB *leds, const int width, const int height, bool serpentine,
	uint8_t octaves, uint32_t x, int xscale, uint32_t y, int yscale, uint32_t time,
	uint8_t hue_octaves, uint16_t hue_x, int hue_xscale, uint16_t hue_y, uint16_t hue_yscale, uint16_t hue_time, bool blend, uint16_t hue_shift) {
	uint8_t *V = new uint8_t[height*width];
	uint8_t *H = new uint8_t[height*width];

	memset(V, 0, height*width);
	memset(H, 0, height*width);

	fill_raw_2dnoise16into8((uint8_t*)V, width, height, octaves, q44(2, 0), 171, 1, x, xscale, y, yscale, time);
	// fill_raw_2dnoise16into8((uint8_t*)V,width,height,octaves,x,xscale,y,yscale,time);
	// fill_raw_2dnoise8((uint8_t*)V,width,height,hue_octaves,x,xscale,y,yscale,time);
	fill_raw_2dnoise8((uint8_t*)H, width, height, hue_octaves, hue_x, hue_xscale, hue_y, hue_yscale, hue_time);


	int w1 = width - 1;
	int h1 = height - 1;
	hue_shift >>= 8;

	for (int i = 0; i < height; i++) {
		int wb = i * width;
		for (int j = 0; j < width; j++) {
			CRGB led(CHSV(hue_shift + (H[(h1 - i) + (w1 - j) * width]), 196, V[i + j * width]));

			int pos = j;
			if (serpentine && (i & 0x1)) {
				pos = w1 - j;
			}

			if (blend) {
				leds[wb + pos] >>= 1; leds[wb + pos] += (led >>= 1);
			}
			else {
				leds[wb + pos] = led;
			}
		}
	}
	delete[] V;
	delete[] H;
}


inline uint8_t lsrX4(uint8_t dividend)
{
	dividend >>= 4;
	return dividend;
}






//PALETTES

class CRGBPalette16;
class CRGBPalette32;
class CRGBPalette256;
class CHSVPalette16;
class CHSVPalette32;
class CHSVPalette256;
typedef uint32_t TProgmemRGBPalette16[16];
typedef uint32_t TProgmemHSVPalette16[16];
#define TProgmemPalette16 TProgmemRGBPalette16
typedef uint32_t TProgmemRGBPalette32[32];
typedef uint32_t TProgmemHSVPalette32[32];
#define TProgmemPalette32 TProgmemRGBPalette32

typedef const uint8_t TProgmemRGBGradientPalette_byte;
typedef const TProgmemRGBGradientPalette_byte *TProgmemRGBGradientPalette_bytes;
typedef TProgmemRGBGradientPalette_bytes TProgmemRGBGradientPalettePtr;
typedef union {
	struct {
		uint8_t index;
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
	uint32_t dword;
	uint8_t  bytes[4];
} TRGBGradientPaletteEntryUnion;

typedef uint8_t TDynamicRGBGradientPalette_byte;
typedef const TDynamicRGBGradientPalette_byte *TDynamicRGBGradientPalette_bytes;
typedef TDynamicRGBGradientPalette_bytes TDynamicRGBGradientPalettePtr;

#define FL_PROGMEM
#define FL_PGM_READ_BYTE_NEAR(x)  (*((const  uint8_t*)(x)))
#define FL_PGM_READ_WORD_NEAR(x)  (*((const uint16_t*)(x)))
#define FL_PGM_READ_DWORD_NEAR(x) (*((const uint32_t*)(x)))

typedef enum { NOBLEND = 0, LINEARBLEND = 1 } TBlendType;

#define DEFINE_GRADIENT_PALETTE(X) \
  extern const TProgmemRGBGradientPalette_byte X[] =

#define DECLARE_GRADIENT_PALETTE(X) \
  extern const TProgmemRGBGradientPalette_byte X[] 

void UpscalePalette(const class CRGBPalette16& srcpal16, class CRGBPalette256& destpal256);
void UpscalePalette(const class CHSVPalette16& srcpal16, class CHSVPalette256& destpal256);

// Convert a 16-entry palette to a 32-entry palette
void UpscalePalette(const class CRGBPalette16& srcpal16, class CRGBPalette32& destpal32);
void UpscalePalette(const class CHSVPalette16& srcpal16, class CHSVPalette32& destpal32);

// Convert a 32-entry palette to a 256-entry palette
void UpscalePalette(const class CRGBPalette32& srcpal32, class CRGBPalette256& destpal256);
void UpscalePalette(const class CHSVPalette32& srcpal32, class CHSVPalette256& destpal256);

CRGB ColorFromPalette(const CRGBPalette16& pal,
	uint8_t index,
	uint8_t brightness = 255,
	TBlendType blendType = LINEARBLEND);

CRGB ColorFromPalette(const TProgmemRGBPalette16& pal,
	uint8_t index,
	uint8_t brightness = 255,
	TBlendType blendType = LINEARBLEND);

CRGB ColorFromPalette(const CRGBPalette256& pal,
	uint8_t index,
	uint8_t brightness = 255,
	TBlendType blendType = NOBLEND);

CHSV ColorFromPalette(const CHSVPalette16& pal,
	uint8_t index,
	uint8_t brightness = 255,
	TBlendType blendType = LINEARBLEND);

CHSV ColorFromPalette(const CHSVPalette256& pal,
	uint8_t index,
	uint8_t brightness = 255,
	TBlendType blendType = NOBLEND);

CRGB ColorFromPalette(const CRGBPalette32& pal,
	uint8_t index,
	uint8_t brightness = 255,
	TBlendType blendType = LINEARBLEND);

CRGB ColorFromPalette(const TProgmemRGBPalette32& pal,
	uint8_t index,
	uint8_t brightness = 255,
	TBlendType blendType = LINEARBLEND);

CHSV ColorFromPalette(const CHSVPalette32& pal,
	uint8_t index,
	uint8_t brightness = 255,
	TBlendType blendType = LINEARBLEND);

class CHSVPalette16 {
public:
	CHSV entries[16];
	CHSVPalette16() {};
	CHSVPalette16(const CHSV& c00, const CHSV& c01, const CHSV& c02, const CHSV& c03,
		const CHSV& c04, const CHSV& c05, const CHSV& c06, const CHSV& c07,
		const CHSV& c08, const CHSV& c09, const CHSV& c10, const CHSV& c11,
		const CHSV& c12, const CHSV& c13, const CHSV& c14, const CHSV& c15)
	{
		entries[0] = c00; entries[1] = c01; entries[2] = c02; entries[3] = c03;
		entries[4] = c04; entries[5] = c05; entries[6] = c06; entries[7] = c07;
		entries[8] = c08; entries[9] = c09; entries[10] = c10; entries[11] = c11;
		entries[12] = c12; entries[13] = c13; entries[14] = c14; entries[15] = c15;
	};

	CHSVPalette16(const CHSVPalette16& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
	}
	CHSVPalette16& operator=(const CHSVPalette16& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
		return *this;
	}

	CHSVPalette16(const TProgmemHSVPalette16& rhs)
	{
		for (uint8_t i = 0; i < 16; i++) {
			CRGB xyz = FL_PGM_READ_DWORD_NEAR(rhs + i);
			entries[i].hue = xyz.red;
			entries[i].sat = xyz.green;
			entries[i].val = xyz.blue;
		}
	}
	CHSVPalette16& operator=(const TProgmemHSVPalette16& rhs)
	{
		for (uint8_t i = 0; i < 16; i++) {
			CRGB xyz = FL_PGM_READ_DWORD_NEAR(rhs + i);
			entries[i].hue = xyz.red;
			entries[i].sat = xyz.green;
			entries[i].val = xyz.blue;
		}
		return *this;
	}

	inline CHSV& operator[] (uint8_t x) 
	{
		return entries[x];
	}
	inline const CHSV& operator[] (uint8_t x) const 
	{
		return entries[x];
	}

	inline CHSV& operator[] (int x) 
	{
		return entries[(uint8_t)x];
	}
	inline const CHSV& operator[] (int x) const 
	{
		return entries[(uint8_t)x];
	}

	operator CHSV*()
	{
		return &(entries[0]);
	}

	bool operator==(const CHSVPalette16 rhs)
	{
		const uint8_t* p = (const uint8_t*)(&(this->entries[0]));
		const uint8_t* q = (const uint8_t*)(&(rhs.entries[0]));
		if (p == q) return true;
		for (uint8_t i = 0; i < (sizeof(entries)); i++) {
			if (*p != *q) return false;
			p++;
			q++;
		}
		return true;
	}
	bool operator!=(const CHSVPalette16 rhs)
	{
		return !(*this == rhs);
	}

	CHSVPalette16(const CHSV& c1)
	{
		fill_solid(&(entries[0]), 16, c1);
	}
	CHSVPalette16(const CHSV& c1, const CHSV& c2)
	{
		fill_gradient(&(entries[0]), 16, c1, c2);
	}
	CHSVPalette16(const CHSV& c1, const CHSV& c2, const CHSV& c3)
	{
		fill_gradient(&(entries[0]), 16, c1, c2, c3);
	}
	CHSVPalette16(const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4)
	{
		fill_gradient(&(entries[0]), 16, c1, c2, c3, c4);
	}

};

class CRGBPalette16 {
public:
	CRGB entries[16];
	CRGBPalette16() {};
	CRGBPalette16(const CRGB& c00, const CRGB& c01, const CRGB& c02, const CRGB& c03,
		const CRGB& c04, const CRGB& c05, const CRGB& c06, const CRGB& c07,
		const CRGB& c08, const CRGB& c09, const CRGB& c10, const CRGB& c11,
		const CRGB& c12, const CRGB& c13, const CRGB& c14, const CRGB& c15)
	{
		entries[0] = c00; entries[1] = c01; entries[2] = c02; entries[3] = c03;
		entries[4] = c04; entries[5] = c05; entries[6] = c06; entries[7] = c07;
		entries[8] = c08; entries[9] = c09; entries[10] = c10; entries[11] = c11;
		entries[12] = c12; entries[13] = c13; entries[14] = c14; entries[15] = c15;
	};

	CRGBPalette16(const CRGBPalette16& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
	}
	CRGBPalette16(const CRGB rhs[16])
	{
		memmove(&(entries[0]), &(rhs[0]), sizeof(entries));
	}
	CRGBPalette16& operator=(const CRGBPalette16& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
		return *this;
	}
	CRGBPalette16& operator=(const CRGB rhs[16])
	{
		memmove(&(entries[0]), &(rhs[0]), sizeof(entries));
		return *this;
	}

	//CRGBPalette16(const CHSVPalette16& rhs)
	//{
	//	for (uint8_t i = 0; i < 16; i++) {
	//		entries[i] = rhs.entries[i]; // implicit HSV-to-RGB conversion
	//	}
	//}
	CRGBPalette16(const CHSV rhs[16])
	{
		for (uint8_t i = 0; i < 16; i++) {
			entries[i] = rhs[i]; // implicit HSV-to-RGB conversion
		}
	}
	//CRGBPalette16& operator=(const CHSVPalette16& rhs)
	//{
	//	for (uint8_t i = 0; i < 16; i++) {
	//		entries[i] = rhs.entries[i]; // implicit HSV-to-RGB conversion
	//	}
	//	return *this;
	//}
	CRGBPalette16& operator=(const CHSV rhs[16])
	{
		for (uint8_t i = 0; i < 16; i++) {
			entries[i] = rhs[i]; // implicit HSV-to-RGB conversion
		}
		return *this;
	}

	CRGBPalette16(const TProgmemRGBPalette16& rhs)
	{
		for (uint8_t i = 0; i < 16; i++) {
			entries[i] = FL_PGM_READ_DWORD_NEAR(rhs + i);
		}
	}
	CRGBPalette16& operator=(const TProgmemRGBPalette16& rhs)
	{
		for (uint8_t i = 0; i < 16; i++) {
			entries[i] = FL_PGM_READ_DWORD_NEAR(rhs + i);
		}
		return *this;
	}

	bool operator==(const CRGBPalette16 rhs)
	{
		const uint8_t* p = (const uint8_t*)(&(this->entries[0]));
		const uint8_t* q = (const uint8_t*)(&(rhs.entries[0]));
		if (p == q) return true;
		for (uint8_t i = 0; i < (sizeof(entries)); i++) {
			if (*p != *q) return false;
			p++;
			q++;
		}
		return true;
	}
	bool operator!=(const CRGBPalette16 rhs)
	{
		return !(*this == rhs);
	}

	inline CRGB& operator[] (uint8_t x) 
	{
		return entries[x];
	}
	inline const CRGB& operator[] (uint8_t x) const 
	{
		return entries[x];
	}

	inline CRGB& operator[] (int x) 
	{
		return entries[(uint8_t)x];
	}
	inline const CRGB& operator[] (int x) const 
	{
		return entries[(uint8_t)x];
	}

	operator CRGB*()
	{
		return &(entries[0]);
	}

	CRGBPalette16(const CHSV& c1)
	{
		fill_solid(&(entries[0]), 16, c1);
	}
	CRGBPalette16(const CHSV& c1, const CHSV& c2)
	{
		fill_gradient(&(entries[0]), 16, c1, c2);
	}
	CRGBPalette16(const CHSV& c1, const CHSV& c2, const CHSV& c3)
	{
		fill_gradient(&(entries[0]), 16, c1, c2, c3);
	}
	CRGBPalette16(const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4)
	{
		fill_gradient(&(entries[0]), 16, c1, c2, c3, c4);
	}

	CRGBPalette16(const CRGB& c1)
	{
		fill_solid(&(entries[0]), 16, c1);
	}
	CRGBPalette16(const CRGB& c1, const CRGB& c2)
	{
		fill_gradient_RGB(&(entries[0]), 16, c1, c2);
	}
	CRGBPalette16(const CRGB& c1, const CRGB& c2, const CRGB& c3)
	{
		fill_gradient_RGB(&(entries[0]), 16, c1, c2, c3);
	}
	CRGBPalette16(const CRGB& c1, const CRGB& c2, const CRGB& c3, const CRGB& c4)
	{
		fill_gradient_RGB(&(entries[0]), 16, c1, c2, c3, c4);
	}


	// Gradient palettes are loaded into CRGB16Palettes in such a way
	// that, if possible, every color represented in the gradient palette
	// is also represented in the CRGBPalette16.
	// For example, consider a gradient palette that is all black except
	// for a single, one-element-wide (1/256th!) spike of red in the middle:
	//     0,   0,0,0
	//   124,   0,0,0
	//   125, 255,0,0  // one 1/256th-palette-wide red stripe
	//   126,   0,0,0
	//   255,   0,0,0
	// A naive conversion of this 256-element palette to a 16-element palette
	// might accidentally completely eliminate the red spike, rendering the
	// palette completely black.
	// However, the conversions provided here would attempt to include a
	// the red stripe in the output, more-or-less as faithfully as possible.
	// So in this case, the resulting CRGBPalette16 palette would have a red
	// stripe in the middle which was 1/16th of a palette wide -- the
	// narrowest possible in a CRGBPalette16.
	// This means that the relative width of stripes in a CRGBPalette16
	// will be, by definition, different from the widths in the gradient
	// palette.  This code attempts to preserve "all the colors", rather than
	// the exact stripe widths at the expense of dropping some colors.
	CRGBPalette16(TProgmemRGBGradientPalette_bytes progpal)
	{
		*this = progpal;
	}
	CRGBPalette16& operator=(TProgmemRGBGradientPalette_bytes progpal)
	{
		TRGBGradientPaletteEntryUnion* progent = (TRGBGradientPaletteEntryUnion*)(progpal);
		TRGBGradientPaletteEntryUnion u;

		// Count entries
		uint16_t count = 0;
		do {
			u.dword = FL_PGM_READ_DWORD_NEAR(progent + count);
			count++;;
		} while (u.index != 255);

		int8_t lastSlotUsed = -1;

		u.dword = FL_PGM_READ_DWORD_NEAR(progent);
		CRGB rgbstart(u.r, u.g, u.b);

		int indexstart = 0;
		uint8_t istart8 = 0;
		uint8_t iend8 = 0;
		while (indexstart < 255) {
			progent++;
			u.dword = FL_PGM_READ_DWORD_NEAR(progent);
			int indexend = u.index;
			CRGB rgbend(u.r, u.g, u.b);
			istart8 = indexstart / 16;
			iend8 = indexend / 16;
			if (count < 16) {
				if ((istart8 <= lastSlotUsed) && (lastSlotUsed < 15)) {
					istart8 = lastSlotUsed + 1;
					if (iend8 < istart8) {
						iend8 = istart8;
					}
				}
				lastSlotUsed = iend8;
			}
			fill_gradient_RGB(&(entries[0]), istart8, rgbstart, iend8, rgbend);
			indexstart = indexend;
			rgbstart = rgbend;
		}
		return *this;
	}
	CRGBPalette16& loadDynamicGradientPalette(TDynamicRGBGradientPalette_bytes gpal)
	{
		TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(gpal);
		TRGBGradientPaletteEntryUnion u;

		// Count entries
		uint16_t count = 0;
		do {
			u = *(ent + count);
			count++;;
		} while (u.index != 255);

		int8_t lastSlotUsed = -1;


		u = *ent;
		CRGB rgbstart(u.r, u.g, u.b);

		int indexstart = 0;
		uint8_t istart8 = 0;
		uint8_t iend8 = 0;
		while (indexstart < 255) {
			ent++;
			u = *ent;
			int indexend = u.index;
			CRGB rgbend(u.r, u.g, u.b);
			istart8 = indexstart / 16;
			iend8 = indexend / 16;
			if (count < 16) {
				if ((istart8 <= lastSlotUsed) && (lastSlotUsed < 15)) {
					istart8 = lastSlotUsed + 1;
					if (iend8 < istart8) {
						iend8 = istart8;
					}
				}
				lastSlotUsed = iend8;
			}
			fill_gradient_RGB(&(entries[0]), istart8, rgbstart, iend8, rgbend);
			indexstart = indexend;
			rgbstart = rgbend;
		}
		return *this;
	}

};


class CHSVPalette32 {
public:
	CHSV entries[32];
	CHSVPalette32() {};
	CHSVPalette32(const CHSV& c00, const CHSV& c01, const CHSV& c02, const CHSV& c03,
		const CHSV& c04, const CHSV& c05, const CHSV& c06, const CHSV& c07,
		const CHSV& c08, const CHSV& c09, const CHSV& c10, const CHSV& c11,
		const CHSV& c12, const CHSV& c13, const CHSV& c14, const CHSV& c15)
	{
		for (uint8_t i = 0; i < 2; i++) {
			entries[0 + i] = c00; entries[2 + i] = c01; entries[4 + i] = c02; entries[6 + i] = c03;
			entries[8 + i] = c04; entries[10 + i] = c05; entries[12 + i] = c06; entries[14 + i] = c07;
			entries[16 + i] = c08; entries[18 + i] = c09; entries[20 + i] = c10; entries[22 + i] = c11;
			entries[24 + i] = c12; entries[26 + i] = c13; entries[28 + i] = c14; entries[30 + i] = c15;
		}
	};

	CHSVPalette32(const CHSVPalette32& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
	}
	CHSVPalette32& operator=(const CHSVPalette32& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
		return *this;
	}

	CHSVPalette32(const TProgmemHSVPalette32& rhs)
	{
		for (uint8_t i = 0; i < 32; i++) {
			CRGB xyz = FL_PGM_READ_DWORD_NEAR(rhs + i);
			entries[i].hue = xyz.red;
			entries[i].sat = xyz.green;
			entries[i].val = xyz.blue;
		}
	}
	CHSVPalette32& operator=(const TProgmemHSVPalette32& rhs)
	{
		for (uint8_t i = 0; i < 32; i++) {
			CRGB xyz = FL_PGM_READ_DWORD_NEAR(rhs + i);
			entries[i].hue = xyz.red;
			entries[i].sat = xyz.green;
			entries[i].val = xyz.blue;
		}
		return *this;
	}

	inline CHSV& operator[] (uint8_t x) 
	{
		return entries[x];
	}
	inline const CHSV& operator[] (uint8_t x) const 
	{
		return entries[x];
	}

	inline CHSV& operator[] (int x) 
	{
		return entries[(uint8_t)x];
	}
	inline const CHSV& operator[] (int x) const 
	{
		return entries[(uint8_t)x];
	}

	operator CHSV*()
	{
		return &(entries[0]);
	}

	bool operator==(const CHSVPalette32 rhs)
	{
		const uint8_t* p = (const uint8_t*)(&(this->entries[0]));
		const uint8_t* q = (const uint8_t*)(&(rhs.entries[0]));
		if (p == q) return true;
		for (uint8_t i = 0; i < (sizeof(entries)); i++) {
			if (*p != *q) return false;
			p++;
			q++;
		}
		return true;
	}
	bool operator!=(const CHSVPalette32 rhs)
	{
		return !(*this == rhs);
	}

	CHSVPalette32(const CHSV& c1)
	{
		fill_solid(&(entries[0]), 32, c1);
	}
	CHSVPalette32(const CHSV& c1, const CHSV& c2)
	{
		fill_gradient(&(entries[0]), 32, c1, c2);
	}
	CHSVPalette32(const CHSV& c1, const CHSV& c2, const CHSV& c3)
	{
		fill_gradient(&(entries[0]), 32, c1, c2, c3);
	}
	CHSVPalette32(const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4)
	{
		fill_gradient(&(entries[0]), 32, c1, c2, c3, c4);
	}

};

class CRGBPalette32 {
public:
	CRGB entries[32];
	CRGBPalette32() {};
	CRGBPalette32(const CRGB& c00, const CRGB& c01, const CRGB& c02, const CRGB& c03,
		const CRGB& c04, const CRGB& c05, const CRGB& c06, const CRGB& c07,
		const CRGB& c08, const CRGB& c09, const CRGB& c10, const CRGB& c11,
		const CRGB& c12, const CRGB& c13, const CRGB& c14, const CRGB& c15)
	{
		for (uint8_t i = 0; i < 2; i++) {
			entries[0 + i] = c00; entries[2 + i] = c01; entries[4 + i] = c02; entries[6 + i] = c03;
			entries[8 + i] = c04; entries[10 + i] = c05; entries[12 + i] = c06; entries[14 + i] = c07;
			entries[16 + i] = c08; entries[18 + i] = c09; entries[20 + i] = c10; entries[22 + i] = c11;
			entries[24 + i] = c12; entries[26 + i] = c13; entries[28 + i] = c14; entries[30 + i] = c15;
		}
	};

	CRGBPalette32(const CRGBPalette32& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
	}
	CRGBPalette32(const CRGB rhs[32])
	{
		memmove(&(entries[0]), &(rhs[0]), sizeof(entries));
	}
	CRGBPalette32& operator=(const CRGBPalette32& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
		return *this;
	}
	CRGBPalette32& operator=(const CRGB rhs[32])
	{
		memmove(&(entries[0]), &(rhs[0]), sizeof(entries));
		return *this;
	}

	CRGBPalette32(const CHSVPalette32& rhs)
	{
		for (uint8_t i = 0; i < 32; i++) {
			entries[i] = rhs.entries[i]; // implicit HSV-to-RGB conversion
		}
	}
	CRGBPalette32(const CHSV rhs[32])
	{
		for (uint8_t i = 0; i < 32; i++) {
			entries[i] = rhs[i]; // implicit HSV-to-RGB conversion
		}
	}
	CRGBPalette32& operator=(const CHSVPalette32& rhs)
	{
		for (uint8_t i = 0; i < 32; i++) {
			entries[i] = rhs.entries[i]; // implicit HSV-to-RGB conversion
		}
		return *this;
	}
	CRGBPalette32& operator=(const CHSV rhs[32])
	{
		for (uint8_t i = 0; i < 32; i++) {
			entries[i] = rhs[i]; // implicit HSV-to-RGB conversion
		}
		return *this;
	}

	CRGBPalette32(const TProgmemRGBPalette32& rhs)
	{
		for (uint8_t i = 0; i < 32; i++) {
			entries[i] = FL_PGM_READ_DWORD_NEAR(rhs + i);
		}
	}
	CRGBPalette32& operator=(const TProgmemRGBPalette32& rhs)
	{
		for (uint8_t i = 0; i < 32; i++) {
			entries[i] = FL_PGM_READ_DWORD_NEAR(rhs + i);
		}
		return *this;
	}

	bool operator==(const CRGBPalette32 rhs)
	{
		const uint8_t* p = (const uint8_t*)(&(this->entries[0]));
		const uint8_t* q = (const uint8_t*)(&(rhs.entries[0]));
		if (p == q) return true;
		for (uint8_t i = 0; i < (sizeof(entries)); i++) {
			if (*p != *q) return false;
			p++;
			q++;
		}
		return true;
	}
	bool operator!=(const CRGBPalette32 rhs)
	{
		return !(*this == rhs);
	}

	inline CRGB& operator[] (uint8_t x) 
	{
		return entries[x];
	}
	inline const CRGB& operator[] (uint8_t x) const 
	{
		return entries[x];
	}

	inline CRGB& operator[] (int x) 
	{
		return entries[(uint8_t)x];
	}
	inline const CRGB& operator[] (int x) const 
	{
		return entries[(uint8_t)x];
	}

	operator CRGB*()
	{
		return &(entries[0]);
	}

	CRGBPalette32(const CHSV& c1)
	{
		fill_solid(&(entries[0]), 32, c1);
	}
	CRGBPalette32(const CHSV& c1, const CHSV& c2)
	{
		fill_gradient(&(entries[0]), 32, c1, c2);
	}
	CRGBPalette32(const CHSV& c1, const CHSV& c2, const CHSV& c3)
	{
		fill_gradient(&(entries[0]), 32, c1, c2, c3);
	}
	CRGBPalette32(const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4)
	{
		fill_gradient(&(entries[0]), 32, c1, c2, c3, c4);
	}

	CRGBPalette32(const CRGB& c1)
	{
		fill_solid(&(entries[0]), 32, c1);
	}
	CRGBPalette32(const CRGB& c1, const CRGB& c2)
	{
		fill_gradient_RGB(&(entries[0]), 32, c1, c2);
	}
	CRGBPalette32(const CRGB& c1, const CRGB& c2, const CRGB& c3)
	{
		fill_gradient_RGB(&(entries[0]), 32, c1, c2, c3);
	}
	CRGBPalette32(const CRGB& c1, const CRGB& c2, const CRGB& c3, const CRGB& c4)
	{
		fill_gradient_RGB(&(entries[0]), 32, c1, c2, c3, c4);
	}


	CRGBPalette32(const CRGBPalette16& rhs16)
	{
		UpscalePalette(rhs16, *this);
	}
	CRGBPalette32& operator=(const CRGBPalette16& rhs16)
	{
		UpscalePalette(rhs16, *this);
		return *this;
	}

	CRGBPalette32(const TProgmemRGBPalette16& rhs)
	{
		CRGBPalette16 p16(rhs);
		*this = p16;
	}
	CRGBPalette32& operator=(const TProgmemRGBPalette16& rhs)
	{
		CRGBPalette16 p16(rhs);
		*this = p16;
		return *this;
	}


	// Gradient palettes are loaded into CRGB16Palettes in such a way
	// that, if possible, every color represented in the gradient palette
	// is also represented in the CRGBPalette32.
	// For example, consider a gradient palette that is all black except
	// for a single, one-element-wide (1/256th!) spike of red in the middle:
	//     0,   0,0,0
	//   124,   0,0,0
	//   125, 255,0,0  // one 1/256th-palette-wide red stripe
	//   126,   0,0,0
	//   255,   0,0,0
	// A naive conversion of this 256-element palette to a 16-element palette
	// might accidentally completely eliminate the red spike, rendering the
	// palette completely black.
	// However, the conversions provided here would attempt to include a
	// the red stripe in the output, more-or-less as faithfully as possible.
	// So in this case, the resulting CRGBPalette32 palette would have a red
	// stripe in the middle which was 1/16th of a palette wide -- the
	// narrowest possible in a CRGBPalette32.
	// This means that the relative width of stripes in a CRGBPalette32
	// will be, by definition, different from the widths in the gradient
	// palette.  This code attempts to preserve "all the colors", rather than
	// the exact stripe widths at the expense of dropping some colors.
	CRGBPalette32(TProgmemRGBGradientPalette_bytes progpal)
	{
		*this = progpal;
	}
	CRGBPalette32& operator=(TProgmemRGBGradientPalette_bytes progpal)
	{
		TRGBGradientPaletteEntryUnion* progent = (TRGBGradientPaletteEntryUnion*)(progpal);
		TRGBGradientPaletteEntryUnion u;

		// Count entries
		uint16_t count = 0;
		do {
			u.dword = FL_PGM_READ_DWORD_NEAR(progent + count);
			count++;;
		} while (u.index != 255);

		int8_t lastSlotUsed = -1;

		u.dword = FL_PGM_READ_DWORD_NEAR(progent);
		CRGB rgbstart(u.r, u.g, u.b);

		int indexstart = 0;
		uint8_t istart8 = 0;
		uint8_t iend8 = 0;
		while (indexstart < 255) {
			progent++;
			u.dword = FL_PGM_READ_DWORD_NEAR(progent);
			int indexend = u.index;
			CRGB rgbend(u.r, u.g, u.b);
			istart8 = indexstart / 8;
			iend8 = indexend / 8;
			if (count < 16) {
				if ((istart8 <= lastSlotUsed) && (lastSlotUsed < 31)) {
					istart8 = lastSlotUsed + 1;
					if (iend8 < istart8) {
						iend8 = istart8;
					}
				}
				lastSlotUsed = iend8;
			}
			fill_gradient_RGB(&(entries[0]), istart8, rgbstart, iend8, rgbend);
			indexstart = indexend;
			rgbstart = rgbend;
		}
		return *this;
	}
	CRGBPalette32& loadDynamicGradientPalette(TDynamicRGBGradientPalette_bytes gpal)
	{
		TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(gpal);
		TRGBGradientPaletteEntryUnion u;

		// Count entries
		uint16_t count = 0;
		do {
			u = *(ent + count);
			count++;;
		} while (u.index != 255);

		int8_t lastSlotUsed = -1;


		u = *ent;
		CRGB rgbstart(u.r, u.g, u.b);

		int indexstart = 0;
		uint8_t istart8 = 0;
		uint8_t iend8 = 0;
		while (indexstart < 255) {
			ent++;
			u = *ent;
			int indexend = u.index;
			CRGB rgbend(u.r, u.g, u.b);
			istart8 = indexstart / 8;
			iend8 = indexend / 8;
			if (count < 16) {
				if ((istart8 <= lastSlotUsed) && (lastSlotUsed < 31)) {
					istart8 = lastSlotUsed + 1;
					if (iend8 < istart8) {
						iend8 = istart8;
					}
				}
				lastSlotUsed = iend8;
			}
			fill_gradient_RGB(&(entries[0]), istart8, rgbstart, iend8, rgbend);
			indexstart = indexend;
			rgbstart = rgbend;
		}
		return *this;
	}

};

class CHSVPalette256 {
public:
	CHSV entries[256];
	CHSVPalette256() {};
	CHSVPalette256(const CHSV& c00, const CHSV& c01, const CHSV& c02, const CHSV& c03,
		const CHSV& c04, const CHSV& c05, const CHSV& c06, const CHSV& c07,
		const CHSV& c08, const CHSV& c09, const CHSV& c10, const CHSV& c11,
		const CHSV& c12, const CHSV& c13, const CHSV& c14, const CHSV& c15)
	{
		CHSVPalette16 p16(c00, c01, c02, c03, c04, c05, c06, c07,
			c08, c09, c10, c11, c12, c13, c14, c15);
		*this = p16;
	};

	CHSVPalette256(const CHSVPalette256& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
	}
	CHSVPalette256& operator=(const CHSVPalette256& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
		return *this;
	}

	CHSVPalette256(const CHSVPalette16& rhs16)
	{
		UpscalePalette(rhs16, *this);
	}
	CHSVPalette256& operator=(const CHSVPalette16& rhs16)
	{
		UpscalePalette(rhs16, *this);
		return *this;
	}

	CHSVPalette256(const TProgmemRGBPalette16& rhs)
	{
		CHSVPalette16 p16(rhs);
		*this = p16;
	}
	CHSVPalette256& operator=(const TProgmemRGBPalette16& rhs)
	{
		CHSVPalette16 p16(rhs);
		*this = p16;
		return *this;
	}

	inline CHSV& operator[] (uint8_t x) 
	{
		return entries[x];
	}
	inline const CHSV& operator[] (uint8_t x) const 
	{
		return entries[x];
	}

	inline CHSV& operator[] (int x) 
	{
		return entries[(uint8_t)x];
	}
	inline const CHSV& operator[] (int x) const 
	{
		return entries[(uint8_t)x];
	}

	operator CHSV*()
	{
		return &(entries[0]);
	}

	bool operator==(const CHSVPalette256 rhs)
	{
		const uint8_t* p = (const uint8_t*)(&(this->entries[0]));
		const uint8_t* q = (const uint8_t*)(&(rhs.entries[0]));
		if (p == q) return true;
		for (uint16_t i = 0; i < (sizeof(entries)); i++) {
			if (*p != *q) return false;
			p++;
			q++;
		}
		return true;
	}
	bool operator!=(const CHSVPalette256 rhs)
	{
		return !(*this == rhs);
	}

	CHSVPalette256(const CHSV& c1)
	{
		fill_solid(&(entries[0]), 256, c1);
	}
	CHSVPalette256(const CHSV& c1, const CHSV& c2)
	{
		fill_gradient(&(entries[0]), 256, c1, c2);
	}
	CHSVPalette256(const CHSV& c1, const CHSV& c2, const CHSV& c3)
	{
		fill_gradient(&(entries[0]), 256, c1, c2, c3);
	}
	CHSVPalette256(const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4)
	{
		fill_gradient(&(entries[0]), 256, c1, c2, c3, c4);
	}
};


class CRGBPalette256 {
public:
	CRGB entries[256];
	CRGBPalette256() {};
	CRGBPalette256(const CRGB& c00, const CRGB& c01, const CRGB& c02, const CRGB& c03,
		const CRGB& c04, const CRGB& c05, const CRGB& c06, const CRGB& c07,
		const CRGB& c08, const CRGB& c09, const CRGB& c10, const CRGB& c11,
		const CRGB& c12, const CRGB& c13, const CRGB& c14, const CRGB& c15)
	{
		CRGBPalette16 p16(c00, c01, c02, c03, c04, c05, c06, c07,
			c08, c09, c10, c11, c12, c13, c14, c15);
		*this = p16;
	};

	CRGBPalette256(const CRGBPalette256& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
	}
	CRGBPalette256(const CRGB rhs[256])
	{
		memmove(&(entries[0]), &(rhs[0]), sizeof(entries));
	}
	CRGBPalette256& operator=(const CRGBPalette256& rhs)
	{
		memmove(&(entries[0]), &(rhs.entries[0]), sizeof(entries));
		return *this;
	}
	CRGBPalette256& operator=(const CRGB rhs[256])
	{
		memmove(&(entries[0]), &(rhs[0]), sizeof(entries));
		return *this;
	}

	CRGBPalette256(const CHSVPalette256& rhs)
	{
		for (int i = 0; i < 256; i++) {
			entries[i] = rhs.entries[i]; // implicit HSV-to-RGB conversion
		}
	}
	CRGBPalette256(const CHSV rhs[256])
	{
		for (int i = 0; i < 256; i++) {
			entries[i] = rhs[i]; // implicit HSV-to-RGB conversion
		}
	}
	CRGBPalette256& operator=(const CHSVPalette256& rhs)
	{
		for (int i = 0; i < 256; i++) {
			entries[i] = rhs.entries[i]; // implicit HSV-to-RGB conversion
		}
		return *this;
	}
	CRGBPalette256& operator=(const CHSV rhs[256])
	{
		for (int i = 0; i < 256; i++) {
			entries[i] = rhs[i]; // implicit HSV-to-RGB conversion
		}
		return *this;
	}

	CRGBPalette256(const CRGBPalette16& rhs16)
	{
		UpscalePalette(rhs16, *this);
	}
	CRGBPalette256& operator=(const CRGBPalette16& rhs16)
	{
		UpscalePalette(rhs16, *this);
		return *this;
	}

	CRGBPalette256(const TProgmemRGBPalette16& rhs)
	{
		CRGBPalette16 p16(rhs);
		*this = p16;
	}
	CRGBPalette256& operator=(const TProgmemRGBPalette16& rhs)
	{
		CRGBPalette16 p16(rhs);
		*this = p16;
		return *this;
	}

	bool operator==(const CRGBPalette256 rhs)
	{
		const uint8_t* p = (const uint8_t*)(&(this->entries[0]));
		const uint8_t* q = (const uint8_t*)(&(rhs.entries[0]));
		if (p == q) return true;
		for (uint16_t i = 0; i < (sizeof(entries)); i++) {
			if (*p != *q) return false;
			p++;
			q++;
		}
		return true;
	}
	bool operator!=(const CRGBPalette256 rhs)
	{
		return !(*this == rhs);
	}

	inline CRGB& operator[] (uint8_t x) 
	{
		return entries[x];
	}
	inline const CRGB& operator[] (uint8_t x) const 
	{
		return entries[x];
	}

	inline CRGB& operator[] (int x) 
	{
		return entries[(uint8_t)x];
	}
	inline const CRGB& operator[] (int x) const 
	{
		return entries[(uint8_t)x];
	}

	operator CRGB*()
	{
		return &(entries[0]);
	}

	CRGBPalette256(const CHSV& c1)
	{
		fill_solid(&(entries[0]), 256, c1);
	}
	CRGBPalette256(const CHSV& c1, const CHSV& c2)
	{
		fill_gradient(&(entries[0]), 256, c1, c2);
	}
	CRGBPalette256(const CHSV& c1, const CHSV& c2, const CHSV& c3)
	{
		fill_gradient(&(entries[0]), 256, c1, c2, c3);
	}
	CRGBPalette256(const CHSV& c1, const CHSV& c2, const CHSV& c3, const CHSV& c4)
	{
		fill_gradient(&(entries[0]), 256, c1, c2, c3, c4);
	}

	CRGBPalette256(const CRGB& c1)
	{
		fill_solid(&(entries[0]), 256, c1);
	}
	CRGBPalette256(const CRGB& c1, const CRGB& c2)
	{
		fill_gradient_RGB(&(entries[0]), 256, c1, c2);
	}
	CRGBPalette256(const CRGB& c1, const CRGB& c2, const CRGB& c3)
	{
		fill_gradient_RGB(&(entries[0]), 256, c1, c2, c3);
	}
	CRGBPalette256(const CRGB& c1, const CRGB& c2, const CRGB& c3, const CRGB& c4)
	{
		fill_gradient_RGB(&(entries[0]), 256, c1, c2, c3, c4);
	}

	CRGBPalette256(TProgmemRGBGradientPalette_bytes progpal)
	{
		*this = progpal;
	}
	CRGBPalette256& operator=(TProgmemRGBGradientPalette_bytes progpal)
	{
		TRGBGradientPaletteEntryUnion* progent = (TRGBGradientPaletteEntryUnion*)(progpal);
		TRGBGradientPaletteEntryUnion u;
		u.dword = FL_PGM_READ_DWORD_NEAR(progent);
		CRGB rgbstart(u.r, u.g, u.b);

		int indexstart = 0;
		while (indexstart < 255) {
			progent++;
			u.dword = FL_PGM_READ_DWORD_NEAR(progent);
			int indexend = u.index;
			CRGB rgbend(u.r, u.g, u.b);
			fill_gradient_RGB(&(entries[0]), indexstart, rgbstart, indexend, rgbend);
			indexstart = indexend;
			rgbstart = rgbend;
		}
		return *this;
	}
	CRGBPalette256& loadDynamicGradientPalette(TDynamicRGBGradientPalette_bytes gpal)
	{
		TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(gpal);
		TRGBGradientPaletteEntryUnion u;
		u = *ent;
		CRGB rgbstart(u.r, u.g, u.b);

		int indexstart = 0;
		while (indexstart < 255) {
			ent++;
			u = *ent;
			int indexend = u.index;
			CRGB rgbend(u.r, u.g, u.b);
			fill_gradient_RGB(&(entries[0]), indexstart, rgbstart, indexend, rgbend);
			indexstart = indexend;
			rgbstart = rgbend;
		}
		return *this;
	}
};

CRGB ColorFromPalette(const CRGBPalette16& pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
	//      hi4 = index >> 4;
	uint8_t hi4 = lsrX4(index);
	uint8_t lo4 = index & 0x0F;

	// const CRGB* entry = &(pal[0]) + hi4;
	// since hi4 is always 0..15, hi4 * sizeof(CRGB) can be a single-byte value,
	// instead of the two byte 'int' that avr-gcc defaults to.
	// So, we multiply hi4 X sizeof(CRGB), giving hi4XsizeofCRGB;
	uint8_t hi4XsizeofCRGB = hi4 * sizeof(CRGB);
	// We then add that to a base array pointer.
	const CRGB* entry = (CRGB*)((uint8_t*)(&(pal[0])) + hi4XsizeofCRGB);

	uint8_t blend = lo4 && (blendType != NOBLEND);

	uint8_t red1 = entry->red;
	uint8_t green1 = entry->green;
	uint8_t blue1 = entry->blue;


	if (blend) {

		if (hi4 == 15) {
			entry = &(pal[0]);
		}
		else {
			entry++;
		}

		uint8_t f2 = lo4 << 4;
		uint8_t f1 = 255 - f2;

		//    rgb1.nscale8(f1);
		uint8_t red2 = entry->red;
		red1 = scale8_LEAVING_R1_DIRTY(red1, f1);
		red2 = scale8_LEAVING_R1_DIRTY(red2, f2);
		red1 += red2;

		uint8_t green2 = entry->green;
		green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
		green2 = scale8_LEAVING_R1_DIRTY(green2, f2);
		green1 += green2;

		uint8_t blue2 = entry->blue;
		blue1 = scale8_LEAVING_R1_DIRTY(blue1, f1);
		blue2 = scale8_LEAVING_R1_DIRTY(blue2, f2);
		blue1 += blue2;

		cleanup_R1();
	}

	if (brightness != 255) {
		if (brightness) {
			brightness++; // adjust for rounding
			// Now, since brightness is nonzero, we don't need the full scale8_video logic;
			// we can just to scale8 and then add one (unless scale8 fixed) to all nonzero inputs.
			if (red1) {
				red1 = scale8_LEAVING_R1_DIRTY(red1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				red1++;
#endif
			}
			if (green1) {
				green1 = scale8_LEAVING_R1_DIRTY(green1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				green1++;
#endif
			}
			if (blue1) {
				blue1 = scale8_LEAVING_R1_DIRTY(blue1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				blue1++;
#endif
			}
			cleanup_R1();
		}
		else {
			red1 = 0;
			green1 = 0;
			blue1 = 0;
		}
	}

	return CRGB(red1, green1, blue1);
}

CRGB ColorFromPalette(const TProgmemRGBPalette16& pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
	//      hi4 = index >> 4;
	uint8_t hi4 = lsrX4(index);
	uint8_t lo4 = index & 0x0F;

	CRGB entry = FL_PGM_READ_DWORD_NEAR(&(pal[0]) + hi4);


	uint8_t red1 = entry.red;
	uint8_t green1 = entry.green;
	uint8_t blue1 = entry.blue;

	uint8_t blend = lo4 && (blendType != NOBLEND);

	if (blend) {

		if (hi4 == 15) {
			entry = FL_PGM_READ_DWORD_NEAR(&(pal[0]));
		}
		else {
			entry = FL_PGM_READ_DWORD_NEAR(&(pal[1]) + hi4);
		}

		uint8_t f2 = lo4 << 4;
		uint8_t f1 = 255 - f2;

		uint8_t red2 = entry.red;
		red1 = scale8_LEAVING_R1_DIRTY(red1, f1);
		red2 = scale8_LEAVING_R1_DIRTY(red2, f2);
		red1 += red2;

		uint8_t green2 = entry.green;
		green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
		green2 = scale8_LEAVING_R1_DIRTY(green2, f2);
		green1 += green2;

		uint8_t blue2 = entry.blue;
		blue1 = scale8_LEAVING_R1_DIRTY(blue1, f1);
		blue2 = scale8_LEAVING_R1_DIRTY(blue2, f2);
		blue1 += blue2;

		cleanup_R1();
	}

	if (brightness != 255) {
		if (brightness) {
			brightness++; // adjust for rounding
			// Now, since brightness is nonzero, we don't need the full scale8_video logic;
			// we can just to scale8 and then add one (unless scale8 fixed) to all nonzero inputs.
			if (red1) {
				red1 = scale8_LEAVING_R1_DIRTY(red1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				red1++;
#endif
			}
			if (green1) {
				green1 = scale8_LEAVING_R1_DIRTY(green1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				green1++;
#endif
			}
			if (blue1) {
				blue1 = scale8_LEAVING_R1_DIRTY(blue1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				blue1++;
#endif
			}
			cleanup_R1();
		}
		else {
			red1 = 0;
			green1 = 0;
			blue1 = 0;
		}
	}

	return CRGB(red1, green1, blue1);
}


CRGB ColorFromPalette(const CRGBPalette32& pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
	uint8_t hi5 = index;
#if defined(__AVR__)
	hi5 /= 2;
	hi5 /= 2;
	hi5 /= 2;
#else
	hi5 >>= 3;
#endif
	uint8_t lo3 = index & 0x07;

	// const CRGB* entry = &(pal[0]) + hi5;
	// since hi5 is always 0..31, hi4 * sizeof(CRGB) can be a single-byte value,
	// instead of the two byte 'int' that avr-gcc defaults to.
	// So, we multiply hi5 X sizeof(CRGB), giving hi5XsizeofCRGB;
	uint8_t hi5XsizeofCRGB = hi5 * sizeof(CRGB);
	// We then add that to a base array pointer.
	const CRGB* entry = (CRGB*)((uint8_t*)(&(pal[0])) + hi5XsizeofCRGB);

	uint8_t red1 = entry->red;
	uint8_t green1 = entry->green;
	uint8_t blue1 = entry->blue;

	uint8_t blend = lo3 && (blendType != NOBLEND);

	if (blend) {

		if (hi5 == 31) {
			entry = &(pal[0]);
		}
		else {
			entry++;
		}

		uint8_t f2 = lo3 << 5;
		uint8_t f1 = 255 - f2;

		uint8_t red2 = entry->red;
		red1 = scale8_LEAVING_R1_DIRTY(red1, f1);
		red2 = scale8_LEAVING_R1_DIRTY(red2, f2);
		red1 += red2;

		uint8_t green2 = entry->green;
		green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
		green2 = scale8_LEAVING_R1_DIRTY(green2, f2);
		green1 += green2;

		uint8_t blue2 = entry->blue;
		blue1 = scale8_LEAVING_R1_DIRTY(blue1, f1);
		blue2 = scale8_LEAVING_R1_DIRTY(blue2, f2);
		blue1 += blue2;

		cleanup_R1();

	}

	if (brightness != 255) {
		if (brightness) {
			brightness++; // adjust for rounding
			// Now, since brightness is nonzero, we don't need the full scale8_video logic;
			// we can just to scale8 and then add one (unless scale8 fixed) to all nonzero inputs.
			if (red1) {
				red1 = scale8_LEAVING_R1_DIRTY(red1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				red1++;
#endif
			}
			if (green1) {
				green1 = scale8_LEAVING_R1_DIRTY(green1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				green1++;
#endif
			}
			if (blue1) {
				blue1 = scale8_LEAVING_R1_DIRTY(blue1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				blue1++;
#endif
			}
			cleanup_R1();
		}
		else {
			red1 = 0;
			green1 = 0;
			blue1 = 0;
		}
	}

	return CRGB(red1, green1, blue1);
}


CRGB ColorFromPalette(const TProgmemRGBPalette32& pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
	uint8_t hi5 = index;
#if defined(__AVR__)
	hi5 /= 2;
	hi5 /= 2;
	hi5 /= 2;
#else
	hi5 >>= 3;
#endif
	uint8_t lo3 = index & 0x07;

	CRGB entry = FL_PGM_READ_DWORD_NEAR(&(pal[0]) + hi5);

	uint8_t red1 = entry.red;
	uint8_t green1 = entry.green;
	uint8_t blue1 = entry.blue;

	uint8_t blend = lo3 && (blendType != NOBLEND);

	if (blend) {

		if (hi5 == 31) {
			entry = FL_PGM_READ_DWORD_NEAR(&(pal[0]));
		}
		else {
			entry = FL_PGM_READ_DWORD_NEAR(&(pal[1]) + hi5);
		}

		uint8_t f2 = lo3 << 5;
		uint8_t f1 = 255 - f2;

		uint8_t red2 = entry.red;
		red1 = scale8_LEAVING_R1_DIRTY(red1, f1);
		red2 = scale8_LEAVING_R1_DIRTY(red2, f2);
		red1 += red2;

		uint8_t green2 = entry.green;
		green1 = scale8_LEAVING_R1_DIRTY(green1, f1);
		green2 = scale8_LEAVING_R1_DIRTY(green2, f2);
		green1 += green2;

		uint8_t blue2 = entry.blue;
		blue1 = scale8_LEAVING_R1_DIRTY(blue1, f1);
		blue2 = scale8_LEAVING_R1_DIRTY(blue2, f2);
		blue1 += blue2;

		cleanup_R1();
	}

	if (brightness != 255) {
		if (brightness) {
			brightness++; // adjust for rounding
			// Now, since brightness is nonzero, we don't need the full scale8_video logic;
			// we can just to scale8 and then add one (unless scale8 fixed) to all nonzero inputs.
			if (red1) {
				red1 = scale8_LEAVING_R1_DIRTY(red1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				red1++;
#endif
			}
			if (green1) {
				green1 = scale8_LEAVING_R1_DIRTY(green1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				green1++;
#endif
			}
			if (blue1) {
				blue1 = scale8_LEAVING_R1_DIRTY(blue1, brightness);
#if !(FASTLED_SCALE8_FIXED==1)
				blue1++;
#endif
			}
			cleanup_R1();
		}
		else {
			red1 = 0;
			green1 = 0;
			blue1 = 0;
		}
	}

	return CRGB(red1, green1, blue1);
}



CRGB ColorFromPalette(const CRGBPalette256& pal, uint8_t index, uint8_t brightness, TBlendType)
{
	const CRGB* entry = &(pal[0]) + index;

	uint8_t red = entry->red;
	uint8_t green = entry->green;
	uint8_t blue = entry->blue;

	if (brightness != 255) {
		brightness++; // adjust for rounding
		red = scale8_video_LEAVING_R1_DIRTY(red, brightness);
		green = scale8_video_LEAVING_R1_DIRTY(green, brightness);
		blue = scale8_video_LEAVING_R1_DIRTY(blue, brightness);
		cleanup_R1();
	}

	return CRGB(red, green, blue);
}


CHSV ColorFromPalette(const class CHSVPalette16& pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
	//      hi4 = index >> 4;
	uint8_t hi4 = lsrX4(index);
	uint8_t lo4 = index & 0x0F;

	//  CRGB rgb1 = pal[ hi4];
	const CHSV* entry = &(pal[0]) + hi4;

	uint8_t hue1 = entry->hue;
	uint8_t sat1 = entry->sat;
	uint8_t val1 = entry->val;

	uint8_t blend = lo4 && (blendType != NOBLEND);

	if (blend) {

		if (hi4 == 15) {
			entry = &(pal[0]);
		}
		else {
			entry++;
		}

		uint8_t f2 = lo4 << 4;
		uint8_t f1 = 255 - f2;

		uint8_t hue2 = entry->hue;
		uint8_t sat2 = entry->sat;
		uint8_t val2 = entry->val;

		// Now some special casing for blending to or from
		// either black or white.  Black and white don't have
		// proper 'hue' of their own, so when ramping from
		// something else to/from black/white, we set the 'hue'
		// of the black/white color to be the same as the hue
		// of the other color, so that you get the expected
		// brightness or saturation ramp, with hue staying
		// constant:

		// If we are starting from white (sat=0)
		// or black (val=0), adopt the target hue.
		if (sat1 == 0 || val1 == 0) {
			hue1 = hue2;
		}

		// If we are ending at white (sat=0)
		// or black (val=0), adopt the starting hue.
		if (sat2 == 0 || val2 == 0) {
			hue2 = hue1;
		}


		sat1 = scale8_LEAVING_R1_DIRTY(sat1, f1);
		val1 = scale8_LEAVING_R1_DIRTY(val1, f1);

		sat2 = scale8_LEAVING_R1_DIRTY(sat2, f2);
		val2 = scale8_LEAVING_R1_DIRTY(val2, f2);

		//    cleanup_R1();

		// These sums can't overflow, so no qadd8 needed.
		sat1 += sat2;
		val1 += val2;

		uint8_t deltaHue = (uint8_t)(hue2 - hue1);
		if (deltaHue & 0x80) {
			// go backwards
			hue1 -= scale8(256 - deltaHue, f2);
		}
		else {
			// go forwards
			hue1 += scale8(deltaHue, f2);
		}

		cleanup_R1();
	}

	if (brightness != 255) {
		val1 = scale8_video(val1, brightness);
	}

	return CHSV(hue1, sat1, val1);
}


CHSV ColorFromPalette(const class CHSVPalette32& pal, uint8_t index, uint8_t brightness, TBlendType blendType)
{
	uint8_t hi5 = index;
#if defined(__AVR__)
	hi5 /= 2;
	hi5 /= 2;
	hi5 /= 2;
#else
	hi5 >>= 3;
#endif
	uint8_t lo3 = index & 0x07;

	uint8_t hi5XsizeofCHSV = hi5 * sizeof(CHSV);
	const CHSV* entry = (CHSV*)((uint8_t*)(&(pal[0])) + hi5XsizeofCHSV);

	uint8_t hue1 = entry->hue;
	uint8_t sat1 = entry->sat;
	uint8_t val1 = entry->val;

	uint8_t blend = lo3 && (blendType != NOBLEND);

	if (blend) {

		if (hi5 == 31) {
			entry = &(pal[0]);
		}
		else {
			entry++;
		}

		uint8_t f2 = lo3 << 5;
		uint8_t f1 = 255 - f2;

		uint8_t hue2 = entry->hue;
		uint8_t sat2 = entry->sat;
		uint8_t val2 = entry->val;

		// Now some special casing for blending to or from
		// either black or white.  Black and white don't have
		// proper 'hue' of their own, so when ramping from
		// something else to/from black/white, we set the 'hue'
		// of the black/white color to be the same as the hue
		// of the other color, so that you get the expected
		// brightness or saturation ramp, with hue staying
		// constant:

		// If we are starting from white (sat=0)
		// or black (val=0), adopt the target hue.
		if (sat1 == 0 || val1 == 0) {
			hue1 = hue2;
		}

		// If we are ending at white (sat=0)
		// or black (val=0), adopt the starting hue.
		if (sat2 == 0 || val2 == 0) {
			hue2 = hue1;
		}


		sat1 = scale8_LEAVING_R1_DIRTY(sat1, f1);
		val1 = scale8_LEAVING_R1_DIRTY(val1, f1);

		sat2 = scale8_LEAVING_R1_DIRTY(sat2, f2);
		val2 = scale8_LEAVING_R1_DIRTY(val2, f2);

		//    cleanup_R1();

		// These sums can't overflow, so no qadd8 needed.
		sat1 += sat2;
		val1 += val2;

		uint8_t deltaHue = (uint8_t)(hue2 - hue1);
		if (deltaHue & 0x80) {
			// go backwards
			hue1 -= scale8(256 - deltaHue, f2);
		}
		else {
			// go forwards
			hue1 += scale8(deltaHue, f2);
		}

		cleanup_R1();
	}

	if (brightness != 255) {
		val1 = scale8_video(val1, brightness);
	}

	return CHSV(hue1, sat1, val1);
}

CHSV ColorFromPalette(const class CHSVPalette256& pal, uint8_t index, uint8_t brightness, TBlendType)
{
	CHSV hsv = *(&(pal[0]) + index);

	if (brightness != 255) {
		hsv.value = scale8_video(hsv.value, brightness);
	}

	return hsv;
}


void UpscalePalette(const class CRGBPalette16& srcpal16, class CRGBPalette256& destpal256)
{
	for (int i = 0; i < 256; i++) {
		destpal256[(uint8_t)(i)] = ColorFromPalette(srcpal16, i);
	}
}

void UpscalePalette(const class CHSVPalette16& srcpal16, class CHSVPalette256& destpal256)
{
	for (int i = 0; i < 256; i++) {
		destpal256[(uint8_t)(i)] = ColorFromPalette(srcpal16, i);
	}
}


void UpscalePalette(const class CRGBPalette16& srcpal16, class CRGBPalette32& destpal32)
{
	for (uint8_t i = 0; i < 16; i++) {
		uint8_t j = i * 2;
		destpal32[j + 0] = srcpal16[i];
		destpal32[j + 1] = srcpal16[i];
	}
}

void UpscalePalette(const class CHSVPalette16& srcpal16, class CHSVPalette32& destpal32)
{
	for (uint8_t i = 0; i < 16; i++) {
		uint8_t j = i * 2;
		destpal32[j + 0] = srcpal16[i];
		destpal32[j + 1] = srcpal16[i];
	}
}

void UpscalePalette(const class CRGBPalette32& srcpal32, class CRGBPalette256& destpal256)
{
	for (int i = 0; i < 256; i++) {
		destpal256[(uint8_t)(i)] = ColorFromPalette(srcpal32, i);
	}
}

void UpscalePalette(const class CHSVPalette32& srcpal32, class CHSVPalette256& destpal256)
{
	for (int i = 0; i < 256; i++) {
		destpal256[(uint8_t)(i)] = ColorFromPalette(srcpal32, i);
	}
}


Vec3 toVec(CRGB c) {
	return Vec3(c.r, c.g, c.b);
}

CRGB toCRGB(Vec3 v) {
	return CRGB(v.x, v.y, v.z);
}





#endif

CRGB blendrgb(CRGB c1, CRGB c2) {
	Vec3 v1(c1.r, c1.g, c1.b);
	Vec3 v2(c2.r, c2.g, c2.b);

	//v1 /= 255;;
	//v2.Normalize();
	v1 = v1 * 0.5 + v2 * 0.5;

	//v1 *= 255;

	return CRGB(v1.x, v1.y, v1.z);
}

CRGB blendlch(CRGB c1, CRGB c2) {
	Vec3 lch1 = rgb2lch(Vec3(c1.r, c1.g, c1.b));
	Vec3 lch2 = rgb2lch(Vec3(c2.r, c2.g, c2.b));

	lch1.x = lch1.x * 0.5f + lch2.x * 0.5f;
	lch1.y = lch1.y * 0.5f + lch2.y * 0.5f;

	if (lch1.z < lch2.z) {
		float t = lch1.z;
		lch1.z = lch2.z;
		lch2.z = t;
	}

	if (lch1.z - lch2.z > 180)
		lch2.z += 360;

	lch1.z = lch1.z * 0.5f + lch2.z * 0.5f;

	if (lch1.z > 360)
		lch1.z -= 360;
	if (lch1.z < 0)
		lch1.z += 360;

	lch1 = lch2rgb(lch1);

	return CRGB(lch1.x, lch1.y, lch1.z);
}

CRGB blendlab(CRGB c1, CRGB c2) {
	Vec3 lch1 = rgb2lab(Vec3(c1.r, c1.g, c1.b));
	Vec3 lch2 = rgb2lab(Vec3(c2.r, c2.g, c2.b));

	lch1.x = lch1.x * 0.5f + lch2.x * 0.5f;
	lch1.y = lch1.y * 0.5f + lch2.y * 0.5f;
	lch1.z = lch1.z * 0.5f + lch2.z * 0.5f;

	lch1 = lab2rgb(lch1);

	return CRGB(lch1.x, lch1.y, lch1.z);
}


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
