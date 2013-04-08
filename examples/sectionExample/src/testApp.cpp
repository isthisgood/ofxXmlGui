#include "testApp.h"
#include "SimpleGui.h"

#define SQUARE 0
#define ELLIPSE 1

xmlgui::SimpleGui gui;

float width = 30;
float height = 50;

int hexColor = 0xFF0000;
ofFloatColor floatColor;
ofVec2f pos;
bool stroke = false;

int shape = SQUARE;
int listVal = 0;

List *myList = NULL;
float range[2] = {0, 0.5};
string text = "t";
//--------------------------------------------------------------
void testApp::setup(){
	gui.setEnabled(true);
	gui.addSlider("width", width,  20, 200);
	gui.addSlider("height", height, 20, 200);
	xmlgui::SimpleGui *section = gui.addSection("bumballs");
	{
		section->addSlider2D("Pos", &pos.x, -100, 100, -100, 100);
		section->addToggle("Stroke", stroke);
		section->addHexColorPicker("Rect Colour", hexColor);
	}
	
	section = gui.addSection("bumballs 2");
	{
		section->addColorPicker("BG Colour", floatColor);
		section->addRangeSlider("Range", range, 0, 1);
		section->addTextField("Textfield", text);
	}
	gui.addSegmented("shape", shape, "Square|Ellipse");
	myList = gui.addList("List", listVal, "One|Two|Three|Four|Five|Six|Seven");
	gui.loadSettings("settings.xml");
	ofEnableAlphaBlending();
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSeedRandom(0);
	
	ofBackground(0,0,0);
	

	glColor4f(floatColor.r, floatColor.g, floatColor.b, floatColor.a);
	

	ofRect(200, 0, ofGetWidth(), ofGetHeight());
	
	ofSetHexColor(0);
	for(int i = 0; i < 50; i++) {
		ofDrawBitmapString(myList->getSelectedString(), ofRandomWidth(), ofRandomHeight());
	}

	ofPushStyle();
	ofSetHexColor(hexColor);
	if(stroke) {
		ofNoFill();
		ofSetLineWidth(4);
	}
	if(shape==SQUARE) {
		ofRect(ofGetWidth()/2 - width/2 + pos.x, ofGetHeight()/2 - height/2 + pos.y, width, height);
	} else {
		ofEllipse(ofGetWidth()/2 + pos.x, ofGetHeight()/2 + pos.y, width, height);
	}
	ofPopStyle();
	
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