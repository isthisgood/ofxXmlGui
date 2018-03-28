/**
 *  Multiball.cpp
 *
 *  Created by Marek Bereza on 07/01/2014.
 */

#include "Multiball.h"


xmlgui::Multiball::Multiball(): LabeledControl() {
	listener = NULL;
	

	height = 100;
	width = 100;
	

	value = new int[1];
	ival(value) = 0;


	bgColor = 0x505050;
	borderColor = 0xFFFFFF;

}

void xmlgui::Multiball::draw() {

	setRGBA(bgColor);
	ofRect(*this);
	

	//map<int,MultiballTouch>::iterator it;
	for(auto it = touches.begin(); it != touches.end(); it++) {

		ofVec3f t = (*it).second;
		ofSetColor(255, t.z * 255.f);
		ofCircle(x + t.x*width, y + t.y*height, 5*t.z);

	}
	
	ofNoFill();
	setRGBA(borderColor);
	ofRect(*this);
	ofFill();
	drawLabel();
}

void xmlgui::Multiball::sendTouchDown (ofVec3f t, int id) {
	MultiballTouch touch(t, id);
	touches[id] = touch;
	listener->multiballTouchDown(touches[id]);
}

void xmlgui::Multiball::sendTouchMoved(ofVec3f t, int id) {
	if(touches.find(id)==touches.end()) {
		ofLogError() << "Received a touch moved event for a touch that didn't exist";
		sendTouchDown(t, id);
		return;
	}
	touches[id].update(t);
	listener->multiballTouchMoved(touches[id]);

}

void xmlgui::Multiball::sendTouchUp   (ofVec3f t, int id) {

	MultiballTouch touch(t, id);
	
	if(touches.find(id)==touches.end()) {
		ofLogError() << "Received a touch up for a non existant touch";
		return;
	}
	touches[id].update(t);
	
	listener->multiballTouchUp(touches[id]);
	touches.erase(id);
}




bool xmlgui::Multiball::touchDown(int _x, int _y, int touchId) {
	sendTouchDown(ofVec3f(_x, _y), touchId);
	return true;
}

bool xmlgui::Multiball::touchUp(int x, int y, int id) {
	sendTouchUp(ofVec3f(x, y), id);
	return true;
}

bool xmlgui::Multiball::touchMoved(int _x, int _y, int touchId) {
	sendTouchMoved(ofVec3f(_x, _y), touchId);
	return true;
}
