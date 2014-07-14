/**
 *  LabeledControl.cpp
 *
 *  Created by Marek Bereza on 14/10/2013.
 */

#include "LabeledControl.h"
#include "Container.h"



xmlgui::LabeledControl::LabeledControl(): Control() {
	lastDrawnFrame = 0;
	labelColor = 0xFFFFFF;
	drawingLabel = true;
	
}

// I don't think this is being used at the moment.
void xmlgui::LabeledControl::drawAllLabels() {
	xmlgui::Resources::bindFont();
	setRGBA(0xFFFFFF);
	for(int i = 0; i < labels.size(); i++) {
		//xmlgui::Resources::drawString(this, labels[i].second,labels[i].first.x, labels[i].first.y);
	}
	xmlgui::Resources::unbindFont();
	labels.clear();
}


void xmlgui::LabeledControl::drawLabel(float x, float y) {
	//drawLabelsIfNeeded();
	
	
	if(drawingLabel) {
		if(x==-1) {
			x = this->x;
			y = this->y - 3;
		}
		if(parent->isOpaque() && x==this->x) {
			ofSetColor(0, 0, 0, 150);
			ofRect( x, y-10, width, 14);
		}
		setRGBA(labelColor);
		xmlgui::Resources::drawString(this, name, x, y);
		//			ofVec2f p = parent->getAbsolutePosition();
		//			labels.push_back(make_pair(ofVec2f(x,y)+p, name));
	}
	
}

void xmlgui::LabeledControl::drawCustomLabel(string label, float x, float y) {
	
	if(drawingLabel) {
		if(parent->isOpaque() && x==this->x) {
			ofSetColor(0, 0, 0, 150);
			ofRect( x, y-10, width, 14);
		}
		
		setRGBA(labelColor);
		xmlgui::Resources::drawString(this, label, x, y);
		
		//			ofVec2f p = parent->getAbsolutePosition();
		//			labels.push_back(make_pair(ofVec2f(x,y)+p, label));
		
	}
}


void xmlgui::LabeledControl::getParameterInfo(vector<ParameterInfo> &params) {
	params.push_back(ParameterInfo("Draw Label", "drawlabel", "toggle", &drawingLabel));
	params.push_back(ParameterInfo("Label Color", "labelColor", "hexcolorpicker", &labelColor));
}


void xmlgui::LabeledControl::drawLabelEitherSide(string left, string right, float dx, float dy) {
	string nam = left;
	if(name.size()>20) {
		nam = name.substr(0, 19);
	}
	
	
	string lab = nam;//ofToString(fval(value), 3);
	drawCustomLabel(lab, x, y-3);
	drawCustomLabel(right, dx + x + width - right.size()*xmlgui::Resources::getFontCharWidth() - 5, y+dy);
	
}