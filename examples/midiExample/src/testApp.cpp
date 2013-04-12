#include "testApp.h"
#include "TabbedGui.h"
#include "MidiMapper.h"

xmlgui::TabbedGui gui;
xmlgui::MidiMapper midi;
float a = 1, b = 0, c = 0;
float d = 1, e = 0, f = 0;

//--------------------------------------------------------------
void testApp::setup() {

	gui.addTab("one");
	gui.addSlider("a", a);
	gui.addSlider("b", b);
	gui.addSlider("c", c);
	gui.addTab("two");
	gui.addSlider("d", d);
	gui.addSlider("e", e);
	gui.addSlider("f", f);
	
	
	// pass in the gui to point to and the name
	// of the midi device you want to use.
	// If unspecified, it will use the first one
	// (usuall IAC driver on mac)
	midi.setup(gui, "LPD8");
	
	// Optionally specify a subdirectory of your
	// data folder to put all the settings files
	// (there are quite a lot!)
	midi.setSettingsDir("settings");
	gui.setEnabled(true);

	
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
		midi.toggle();
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