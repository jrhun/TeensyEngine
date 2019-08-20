#pragma once

#include "Graphics.h"
#include "ofMain.h"

class GraphicsPC : public Graphics {
public: 
	void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b) {
		ofSetColor(r, g, b);
		ofDrawRectangle(x*scale, y*scale, scale, scale);
	}
	//void putPixel(int x, int y, mRGB c) {
	//	putPixel(x, y, c.r, c.b, c.g);
	//}

	void putPixel(int x, int y, unsigned char h) {
		ofColor c;
		c.setHsb(h, 255, 255);
		ofSetColor(c);
		ofDrawRectangle(x*scale, y*scale, scale, scale);
	}

	

	int scale = 8;
};

