#pragma once

#if defined(ARDUINO)
#else

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxPanel gui;
		ofxFloatSlider floatSlider1;
		ofxFloatSlider floatSlider2;
		ofxFloatSlider floatSlider3;
		ofxFloatSlider floatSlider4;
		ofxFloatSlider floatSlider5;
		ofxFloatSlider floatSlider6;
};

#endif