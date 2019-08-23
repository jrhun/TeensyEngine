#if defined(ARDUINO)
#else

#include "ofApp.h"


#include "Patterns/PatternController.h"
#include "../data.h"



//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowShape(SCREEN_WIDTH * 8 + 200, SCREEN_HEIGHT * 8);
	ofSetWindowPosition(94, 30);
	ofBackground(0, 0, 0);

	ofSetRectMode(OF_RECTMODE_CORNER);

	//ofSetFrameRate(60);

	//engine.init();
	//Pattern::setGfx(&gfx_t);

	// GUI
	gui.setup();
	gui.setPosition(SCREEN_WIDTH * 8, 50);
	gui.add(floatSlider1.setup("X1", 1.6, 0.1, 2.0));
	gui.add(floatSlider2.setup("Y1", 0.6, 0.1, 2.0));
	gui.add(floatSlider3.setup("Z1", 1.25, 0.1, 2.0));
	gui.add(floatSlider4.setup("X2", 0.87, 0.1, 2.0));
	gui.add(floatSlider5.setup("Y2", 1.25, 0.1, 2.0));
	gui.add(floatSlider6.setup("Z2", 1.15, 0.1, 2.0));
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	//ofBackground(ofColor::black);
	//engine.fade(128);
	GuiVars1 = floatSlider1;
	GuiVars2 = floatSlider2;
	GuiVars3 = floatSlider3;
	GuiVars4 = floatSlider4;
	GuiVars5 = floatSlider5;
	GuiVars6 = floatSlider6;


	patterns.run();

	ofSetColor(ofColor(5,5,8));
	ofDrawRectRounded(SCREEN_WIDTH * 8, 0, 200, SCREEN_HEIGHT * 8, 10);
	//engine.updateModel();
	//engine.composeFrame();
	//engine.show();
	gui.draw();
	ofSetColor(ofColor::slateGrey);
	ofDrawBitmapString(patterns.getCurrentPatternName(), SCREEN_WIDTH * 8, 30);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_LEFT)
		patterns.dec();
	if (key == OF_KEY_RIGHT)
		patterns.inc();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}


#endif