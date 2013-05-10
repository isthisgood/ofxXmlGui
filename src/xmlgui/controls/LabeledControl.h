/**
 * LabeledControl.h
 * PdGui
 *
 * Created by Marek Bereza on 06/07/2011.
 *
 */

#pragma once

#include "xmlgui/framework/Control.h"

class LabeledControl: public xmlgui::Control {
public:
	static int lastDrawnFrame;
	LabeledControl(): Control() {
		lastDrawnFrame = 0;
		labelColor = 0xFFFFFF;
		drawingLabel = true;

	}
	
	static void drawAllLabels() {
		xmlgui::Resources::bindFont();
		setRGBA(0xFFFFFF);
		for(int i = 0; i < labels.size(); i++) {
			xmlgui::Resources::drawString(labels[i].second,labels[i].first.x, labels[i].first.y);
		}
		xmlgui::Resources::unbindFont();
		labels.clear();
	}
	
	static vector<pair<ofVec2f, string> > labels;
	
	/*void drawLabelsIfNeeded() {
		if(lastDrawnFrame != ofGetFrameNum()) {
			ofVec2f p = parent->getAbsolutePosition();

			drawAllLabels(-p);
			lastDrawnFrame = ofGetFrameNum();
			labels.clear();
		}
	}*/
	void drawLabel(float x = -1, float y = -1) {
		//drawLabelsIfNeeded();
		
		
		if(drawingLabel) {
			if(x==-1) {
				x = this->x;
				y = this->y - 3;
			}
			setRGBA(labelColor);
			xmlgui::Resources::drawString(name, x, y);
//			ofVec2f p = parent->getAbsolutePosition();
//			labels.push_back(make_pair(ofVec2f(x,y)+p, name));
		}
		
	}

	void drawCustomLabel(string label, float x, float y) {
<<<<<<< HEAD:src/xmlgui/controls/LabeledControl.h
=======
		//drawLabelsIfNeeded();
>>>>>>> origin/dev:addon/xmlgui/src/controls/LabeledControl.h
		if(drawingLabel) {
			if(parent->isOpaque()) {
				ofSetColor(0, 0, 0, 150);
				ofRect( x, y-10, width, 14);
			}
<<<<<<< HEAD:src/xmlgui/controls/LabeledControl.h
			setRGBA(labelColor);
			xmlgui::Resources::drawString(label, x, y);

=======
////			setRGBA(labelColor);
			xmlgui::Resources::drawString(label, x, y);
//			ofVec2f p = parent->getAbsolutePosition();
//			labels.push_back(make_pair(ofVec2f(x,y)+p, label));
>>>>>>> origin/dev:addon/xmlgui/src/controls/LabeledControl.h
		}
	}


	void getParameterInfo(vector<ParameterInfo> &params) {
		params.push_back(ParameterInfo("Draw Label", "drawlabel", "toggle", &drawingLabel));
		params.push_back(ParameterInfo("Label Color", "labelColor", "colourpicker", &labelColor));
	}
	
	bool drawingLabel;
	int labelColor;
};
<<<<<<< HEAD:src/xmlgui/controls/LabeledControl.h
=======

>>>>>>> origin/dev:addon/xmlgui/src/controls/LabeledControl.h
