#if defined(ARDUINO)
#else

#include "ofApp.h"

#include "Engine.h"

GraphicsPC gfx_t;
Engine engine(gfx_t);

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(SCREEN_WIDTH *8, SCREEN_HEIGHT*8);
	ofSetWindowPosition(100, 100);
	ofBackground(0, 0, 0);

	ofSetRectMode(OF_RECTMODE_CORNER);

	ofSetFrameRate(60);

	engine.init();
}

//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(ofColor::black);
	engine.updateModel();
	engine.composeFrame();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


#endif