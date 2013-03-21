#include "testApp.h"
#include "TabbedGui.h"

xmlgui::TabbedGui gui;

float a = 1, b = 0, c = 0;
float d = 1, e = 0, f = 0;

//--------------------------------------------------------------
void testApp::setup() {
	
	gui.addTab("one");
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addTab("two");
	gui.addSlider("d", d);
	gui.addSlider("e", e);
	gui.addSlider("f", f);
	
	gui.addTab("three");
	gui.addSlider("d", d);
	gui.addSlider("e", e);
	gui.addSlider("f", f);
	
	gui.setEnabled(true);
	gui.print();
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
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