/**
 *  FloatMapper.cpp
 *
 *  Created by Marek Bereza on 27/12/2013.
 */
#include "xmlgui/controls/FloatMapper.h"
#include "xmlgui/controls/Slider.h"
#include "xmlgui/controls/Panner.h"
#include "xmlgui/controls/IntSlider.h"
#include "xmlgui/framework/Container.h"

bool xmlgui::FloatMapper::updateHookEnabled = false;

vector<xmlgui::FloatMapper*> xmlgui::FloatMapper::mappers;


bool xmlgui::FloatMapper::update(ofEventArgs &args) {
	for(int i = 0; i < mappers.size(); i++) {
		mappers[i]->updateValue();
	}
	return false;
}

void xmlgui::FloatMapper::reset() {
    // clear mapping if it's already set.
    target = NULL;
    inputRange[0] = 0;
    inputRange[1] = 1;
    outputRange[0] = 0;
    outputRange[1] = 1;
    sval(value) = "";
}

void xmlgui::FloatMapper::settingsLoaded() {
    //printf("settings loaded on %s\n", name.c_str());
	if(sval(value)!="") {
		vector<string> s = ofSplitString(sval(value), "|");
		if(s.size()==5) {

			target = parent->getControlById(s[0]);
			if(target==NULL) {
				ofLogError() << "No control to map to called '"<<s[0]<<"'";
				return;
			}
			
			inputRange[0] = ofToFloat(s[1]);
			inputRange[1] = ofToFloat(s[2]);
			outputRange[0] = ofToFloat(s[3]);
			outputRange[1] = ofToFloat(s[4]);
			
		} else {
			ofLogError() << "FloatMapper settings string is incompatible (must have 4 '|' characters) - '"<<sval(value)<<"'";
		}
	} else {
        reset();
        
    }
}
void xmlgui::FloatMapper::updateValue() {
	if(target!=NULL) {
		float targetMin;
		float targetMax;
		
		if(target->type=="slider") {
			Slider *s = (Slider*)target;
			targetMin = s->min;
			targetMax = s->max;
			
		} else if(target->type=="panner") {
			Panner *s = (Panner*)target;
			targetMin = s->min;
			targetMax = s->max;
			
		} else if(target->type=="intslider") {
			IntSlider *s = (IntSlider*)target;
			targetMin = s->min;
			targetMax = s->max;
			
		}

		float v = ofMap(*controlValue, controlMin, controlMax, 0, 1, true);
		
		
		//v = ofClamp(v, inputRange[0], inputRange[1]);
		//v -= inputRange[0];
		//v /= inputRange[1] - inputRange[0];
		
		float tMin = ofMap(outputRange[0], 0, 1, targetMin, targetMax);
		float tMax = ofMap(outputRange[1], 0, 1, targetMin, targetMax);
		v = ofMap(v, inputRange[0], inputRange[1], tMin, tMax, true);
		
		
		if(target->type=="intslider") {
			target->setValue((int)v);
			
		} else {
			target->setValue(v);
		}
	}
}



xmlgui::FloatMapper::FloatMapper(): LabeledControl() {
	if(!updateHookEnabled) {
		ofAddListener(ofEvents().update, this, &FloatMapper::update);
		updateHookEnabled = true;
	}
	mappers.push_back(this);
	
	
	currentlyMapping = false;
	controlMin = 0;
	controlMax = 1;
	
	outputRange[0] = 0;
	outputRange[1] = 1;
	
	// the value of this i
	value = new string;
	height = 80;
	target = NULL;
	
	inRange.drawingLabel = false;
	inRange.showValue = false;
	inRange.pointToValue(inputRange);
	
	outRange.drawingLabel = false;
	outRange.showValue = false;
	outRange.pointToValue(outputRange);
	

	
}

void xmlgui::FloatMapper::draw() {
	
	mapButton.set(x, y, width/2, height/4);
	controlPreview.set(x, y+mapButton.height +2, width, (height/4) - 2);
	inRange.set(x, controlPreview.y+controlPreview.height +2, width, (height/4));
	outRange.set(x, inRange.y+inRange.height +2, width, (height/4));
	
	ofSetColor(50);
	ofDrawRectangle(mapButton);
	ofDrawRectangle(controlPreview);
	
	
	ofSetColor(200, 200, 0);
	
	float val = ofMap(*controlValue, controlMin, controlMax, 0, 1, true);
	ofDrawRectangle(controlPreview.x, controlPreview.y, controlPreview.width * val, controlPreview.height);
	
	
	ofSetColor(255);
	string msg = "Map control";
	
	if(currentlyMapping) {
		msg = "cancel";
	} else if(target!=NULL) {
		msg = "remove";
	}
	
	xmlgui::Resources::drawString(this, msg, mapButton.x+3, mapButton.y + mapButton.height - 3);
	
	msg = "[ no target ]";
	if(target!=NULL) {
		msg = target->id;
	}
	
	xmlgui::Resources::drawString(this, msg, mapButton.x + mapButton.width + 5, mapButton.y + mapButton.height - 3);
	inRange.draw();
	ofDrawBitmapString("in", inRange.x + 3, inRange.y + 15);
	outRange.draw();
	ofDrawBitmapString("out", outRange.x + 3, outRange.y + 15);
	drawLabel();
}
void xmlgui::FloatMapper::setVal() {
	if(target==NULL) {
		sval(value) = "";
		return;
	}
	sval(value) = target->id + "|"+ofToString(inputRange[0]) + "|"+ofToString(inputRange[1]) + "|"
	+ofToString(outputRange[0]) + "|"
	+ofToString(outputRange[1]);
	string s = sval(value);
	//printf(">>%s<<%f %f %f %f\n", s.c_str(), inputRange[0], inputRange[1], outputRange[0], outputRange[1]);
}
void xmlgui::FloatMapper::controlChanged(xmlgui::Event *evt) {
	if(evt->control->type=="slider" || evt->control->type=="panner" || evt->control->type=="intslider") {
		target = evt->control;
		setVal();

		stopMapping();
	}
}

bool xmlgui::FloatMapper::touchDown(int _x, int _y, int button) {
	if(mapButton.inside(_x, _y)) {
		if(!currentlyMapping) {
			if(target!=NULL) {
				target = NULL;
				sval(value) = "";
				setVal();
			} else {
				startMapping();
			}
		} else {
			stopMapping();
		}
		return true;
	} else {
		bool ret = false;
		
		if(outRange.inside(_x, _y)) {
			ret = outRange.touchDown(_x, _y, button);
		} else if(inRange.inside(_x, _y)) {
			ret = inRange.touchDown(_x, _y, button);
		}
		
		setVal();
		return ret;
	}
	return false;
}

bool xmlgui::FloatMapper::touchMoved(int _x, int _y, int button) {
	bool ret = false;
	
	if(outRange.inside(_x, _y)) {
		ret = outRange.touchMoved(_x, _y, button);
	} else if(inRange.inside(_x, _y)) {
		ret = inRange.touchMoved(_x, _y, button);
	}
			setVal();
	return ret;
}


void xmlgui::FloatMapper::stopMapping() {
	currentlyMapping = false;
	this->parent->removeListener(this);
}

void xmlgui::FloatMapper::startMapping() {
	currentlyMapping = true;
	this->parent->addListener(this);
}


