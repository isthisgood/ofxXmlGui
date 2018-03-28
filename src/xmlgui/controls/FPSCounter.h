/*
 *  GuiFPSCounter.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */

#pragma once

#include "xmlgui/framework/Control.h"
namespace xmlgui {
	class FPSCounter: public xmlgui::Control {
	public:
		
		float fps;
		float counterWidth;
		const static int HISTORY_SIZE = 100;
		float maxFps;
		FPSCounter(): Control() {
			counterWidth = 30;
			fps = 0;
			maxFps = 0;
			height = 25;
			width = counterWidth + HISTORY_SIZE;
		}
		deque<float> history;

		void draw() {
			
			if(fps==0) fps = ofGetFrameRate();
			else fps = ofGetFrameRate() * 0.05 + fps * 0.95;
			history.push_front(fps);
			if(fps>maxFps) {
				maxFps = fps;
			}
			while(history.size()>HISTORY_SIZE) history.pop_back();		
			ofSetColor(0, 100, 0);
			ofDrawRectangle(x, y, width, height);
			ofSetColor(255, 255, 255);
			xmlgui::Resources::drawString(this, ofToString(fps, 1), x+3, y+14);
			ofNoFill();
			ofBeginShape();
			//glBegin(GL_LINE_STRIP);
			int i = 0;
			glColor4f(1,1,1,1);
			deque<float>::iterator it = history.begin();
			for(; it != history.end(); it++) {
				ofVertex(x+counterWidth+i, y + height - height * ((*it)/maxFps));		
				i++;		
			}		
			//glEnd();
			ofEndShape();
			ofFill();
		}
	};

}

