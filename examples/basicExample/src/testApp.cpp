#include "testApp.h"
#include "SimpleGui.h"
xmlgui::SimpleGui gui;

float width = 30;
float height = 50;
int hexColor = 0xFF0000;
ofFloatColor floatColor;
//--------------------------------------------------------------
void testApp::setup(){
	gui.setEnabled(true);
	gui.addSlider("width", width,  20, 200);
	gui.addSlider("height", height, 20, 200);
	gui.addHexColorPicker("Rect Colour", hexColor);
	
	gui.addColorPicker("BG Colour", floatColor);
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0,0,0);
	
	
	glColor3f(floatColor.r, floatColor.g, floatColor.b);
	
	ofRect(0, 0, ofGetWidth(), ofGetHeight());
	
	ofSetHexColor(hexColor);

	ofRect(ofGetWidth()/2 - width/2, ofGetHeight()/2 - height/2, width, height);

	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' ') {
		gui.toggle();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}