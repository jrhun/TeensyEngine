#if defined(ARDUINO)
#else

#include "ofApp.h"


#include "Patterns/PatternController.h"
#include "../data.h"

#include "Menu.h"



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
	gui.add(floatSlider1.setup("X1", 1.6, 0.0, 2.0));
	gui.add(floatSlider2.setup("Y1", 0.6, 0.0, 2.0));
	gui.add(floatSlider3.setup("Z1", 1.25, 0.0, 2.0));
	gui.add(floatSlider4.setup("X2", 0.87, 0.0, 2.0));
	gui.add(floatSlider5.setup("Y2", 1.25, 0.0, 2.0));
	gui.add(floatSlider6.setup("Z2", 1.15, 0.0, 2.0));
	gui.add(intSliderFade.setup("Fade", 128, 0, 255));
	gui.add(intSliderBlur.setup("Blur", 0, 0, 255));
	gui.add(intSliderSpiral.setup("Spiral", 0, 0, 255));
	gui.add(intSliderNoiseSmear.setup("Noise smear", 0, 0, 255));

	menu.display();
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
	FxFade	 = intSliderFade;
	FxBlur   = intSliderBlur;
	FxSpiral = intSliderSpiral;
	FxNoiseSmear = intSliderNoiseSmear;

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
	if (key == ' ') {
		//spacebar

	}
	if (key == 'a') { //left
		menu.left();
	}
	if (key == 'd') { //right
		menu.right();
	}
	if (key == 'w') { //up
		menu.up();
	}
	if (key == 's') {
		menu.down();
	}
	if (key == 'q') {
		menu.dec();
	}
	if (key == 'e') {
		menu.inc();
	}
	if (key == 'f') {
		menu.press();
	}
	menu.display();
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