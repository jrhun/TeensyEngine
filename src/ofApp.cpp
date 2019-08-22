#if defined(ARDUINO)
#else

#include "ofApp.h"


#include "Patterns/PatternController.h"


PatternController patterns;

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowShape(SCREEN_WIDTH * 8, SCREEN_HEIGHT * 8);
	ofSetWindowPosition(100, 100);
	ofBackground(0, 0, 0);

	ofSetRectMode(OF_RECTMODE_CORNER);

	ofSetFrameRate(60);

	//engine.init();
	//Pattern::setGfx(&gfx_t);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	//ofBackground(ofColor::black);
	//engine.fade(128);
	patterns.run();
	//engine.updateModel();
	//engine.composeFrame();
	//engine.show();
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