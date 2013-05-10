/*
 *  GuiFPSCounter.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */

#pragma once

#include "xmlgui/framework/Control.h"

class FPSCounter: public xmlgui::Control {
public:
	
	float fps;
	FPSCounter(): Control() {
		fps = 10;
		height = 25;
		width = 60;
	}
	
	void draw() {
		fps = ofGetFrameRate() * 0.05 + fps * 0.95;
		ofSetColor(0, 100, 0);
		ofRect(x, y, width, height);
		ofSetColor(255, 255, 255);
		xmlgui::Resources::drawString(ofToString(fps, 1), x+3, y+14);
	}
};