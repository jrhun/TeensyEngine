#if defined(ARDUINO)
#else

#include "ofApp.h"


#include "Patterns/PatternController.h"
#include "../data.h"

#include "Menu.h"



//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowShape(SCREEN_WIDTH * 16 + 200, SCREEN_HEIGHT * 16);
	ofSetWindowPosition(94, 30);
	ofBackground(0, 0, 0);

	ofSetRectMode(OF_RECTMODE_CORNER);

	//ofSetFrameRate(60);

	//engine.init();
	//_Pattern::setGfx(&gfx_t);

	// GUI
	gui.setup();
	gui.setPosition(SCREEN_WIDTH * 16, 50);
	gui.add(floatSlider1.setup("X1", 1.04, 0.0, 2.0));
	gui.add(floatSlider2.setup("Y1", 0.6, 0.0, 2.0));
	gui.add(floatSlider3.setup("Z1", 0.11, 0.0, 2.0));
	gui.add(floatSlider4.setup("X2", 1.0, 0.0, 2.0));
	gui.add(floatSlider5.setup("Y2", 1.0, 0.0, 2.0));
	gui.add(floatSlider6.setup("Z2", 1.0, 0.0, 2.0));
	gui.add(intSliderFade.setup("Fade", 128, 0, 255));
	gui.add(intSliderBlur.setup("Blur", 0, 0, 255));
	gui.add(intSliderSpiral.setup("Spiral", 0, 0, 255));
	gui.add(intSliderNoiseSmear.setup("Noise smear", 0, 0, 255));

	paletteIndex_t.setCallback([]() {
		gfx.setPalette(*paletteIndex_t);
	});

	menu.display();

	//patterns.beatControl.ramp(120<<8);
	//patterns.beatControl.oscilate(120 * 256);

}

//--------------------------------------------------------------
void ofApp::update() {
	//patterns.beatControl.update();
	float alpha = 0.02;
	Data::ax = 0.0 * alpha + Data::ax * (1.0 - alpha);
	Data::ay = 9.0 * alpha + Data::ay * (1.0 - alpha);
	Data::az = 0.0 * alpha + Data::az * (1.0 - alpha);

	Data::pitch = -(atan2(-Data::az, Data::ay) * 180.0) / PI;
	Data::roll = -(atan2(Data::ax, sqrt(Data::az * Data::az + Data::ay * Data::ay)) * 180.0) / PI;


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
	ofDrawRectRounded(SCREEN_WIDTH * 16, 0, 200, SCREEN_HEIGHT * 16, 10);
	//engine.updateModel();
	//engine.composeFrame();
	//engine.show();
	gui.draw();
	ofSetColor(ofColor::slateGrey);
	ofDrawBitmapString(patterns.getCurrentPatternName(), SCREEN_WIDTH * 16+5, 20);

	//String v = "beat: " + to_string( *_Pattern::beat );
	//ofDrawBitmapString(v, SCREEN_WIDTH * 8+10, 40);
	String v = String(patterns.getCurrentPattern()->getVarName(0)) + patterns.getCurrentPattern()->getVarValue(0);
	ofDrawBitmapString(v, SCREEN_WIDTH * 16 + 90, 40);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_LEFT)
		patterns.dec();
	if (key == OF_KEY_RIGHT)
		patterns.inc();
	if (key == ' ') {
		//spacebar
		patterns.getCurrentPattern()->trigger();
	}
	if (key == 'a') { //left
		menu.left();
	}
	if (key == 'd') { //right
		menu.right();
	}
	if (key == 'w') { //up
		menu.down();
	}
	if (key == 's') {
		menu.up();
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
	if (key == 'i') {
		//impulse 
		Data::ax += myMap(random8(), 0, 255, -10.0, 10.0); 
		Data::ay += myMap(random8(), 0, 255, -10.0, 10.0);
		Data::az += myMap(random8(), 0, 255, -10.0, 10.0);

		Data::pitch = -(atan2(-Data::ay, Data::az) * 180.0) / PI;
		Data::roll = -(atan2(Data::ax, sqrt(Data::ay * Data::ay + Data::az * Data::az)) * 180.0) / PI;
	}
	if (key == '1') _Pattern::beat.setType(0);
	if (key == '2') _Pattern::beat.setType(1);
	if (key == '3') _Pattern::beat.setType(2);
	if (key == '4') _Pattern::beat.setType(3);
	if (key == '5') _Pattern::beat.setType(4);
	if (key == '6') _Pattern::beat.setType(5);
	if (key == '7') _Pattern::beat.setType(6);
	if (key == '8') _Pattern::beat.setType(7);
	if (key == '9') _Pattern::beat.setType(8);
	if (key == '0') _Pattern::beat.setType(9);

	if (key == OF_KEY_HOME) Data::pitch+= 5;
	if (key == OF_KEY_END) Data::pitch -= 5;
	if (key == OF_KEY_DEL) Data::roll+= 5;
	if (key == OF_KEY_PAGE_DOWN) Data::roll-= 5;

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
