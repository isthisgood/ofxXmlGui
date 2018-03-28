/**
 *  Slider.cpp
 *
 *  Created by Marek Bereza on 30/12/2013.
 */

#include "Slider.h"
#include "Container.h"


#ifdef TARGET_WIN32
#define GLUT_BUILDING_LIB
#include "glut.h"
#endif
#ifdef TARGET_OSX
#include <OpenGL/OpenGL.h>
//#include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
#include <Carbon/Carbon.h>
#endif
#ifdef TARGET_LINUX
#ifndef TARGET_RASPBERRY_PI
#include <GL/glut.h>
#include "ofIcon.h"
#include "ofImage.h"
#include <X11/Xatom.h>
#endif
#endif



xmlgui::Slider::Slider(): LabeledControl() {
	logarithmic = false;
	lastTimePressed = 0;
	sliderBG = sliderFG = sliderHandle = NULL;
	vertical = false;
    height = LabeledControl::DEFAULT_CONTROL_HEIGHT;
	width = 100;
	stepped = false;
	value = new float[1];
	fval(value) = 0.5;
	min = 0;
	max = 1;
	bgColor = 0x505050;
	fgColor = 0x960000;
	borderColor = 0xFFFFFF;
	sliderBGUrl = "";
	sliderFGUrl = "";
	sliderHandleUrl = "";
	showValue = false;
}

void xmlgui::Slider::load() {
	if(fval(value)<min) fval(value) = min;
	if(fval(value)>max) fval(value) = max;
	sliderBG = xmlgui::Resources::getImage(sliderBGUrl);
	sliderFG = xmlgui::Resources::getImage(sliderFGUrl);
	sliderHandle = xmlgui::Resources::getImage(sliderHandleUrl);
	if(sliderBG!=NULL) {
		scalable = false;
		width = sliderBG->getWidth();
		height = sliderBG->getHeight();
	}
	
}



void xmlgui::Slider::draw() {
	//printf("%f %f %f %f\n", x, y, width, height);
	if(sliderBG!=NULL) {
		ofSetHexColor(0xFFFFFF);
		sliderBG->draw(x, y);
	} else {
		if(parent->isKeyboardFocusedOn(this)) {
			setRGBA(0x777777);
		} else {
			setRGBA(bgColor);
		}
		Resources::drawVerticalStripes(*this);
	}
	
	float val = (fval(value)-min)/(max-min);
	
	if(logarithmic) {
		if(val>0) {
			val = sqrt(val);
		}
		
	}
	
	if(sliderFG!=NULL) {
		ofSetHexColor(0xFFFFFF);
		ofVec2f abs = getAbsolutePosition();
		if(vertical) pushMask(abs.x, abs.y + height-height*val, width, height*val);
		else pushMask(abs.x, abs.y, width*val, height);
		sliderFG->draw(x, y);
		//ofRect(0, 0, ofGetWidth(), ofGetHeight());
		popMask();
	} else {
		setRGBA(fgColor);
		
		if(vertical) Resources::drawDiagonalStripes(ofRectangle(x, y+height-height*val, width, height*val));
		else
		{
			if (val < 0)
			{
				ofSetColor(0, 0, 0);
				Resources::drawDiagonalStripes(ofRectangle(x, y, width, height));
			}
			else if (val > 1)
			{
				ofSetColor(255, 0, 0);
				Resources::drawDiagonalStripes(ofRectangle(x, y, width, height));
			}
			else Resources::drawDiagonalStripes(ofRectangle(x, y, width*val, height));
		}
	}
	
	if(sliderHandle!=NULL) {
		ofSetHexColor(0xFFFFFF);
		if(vertical) {
			
			sliderHandle->draw(x, y + (1.f - val)*(height - sliderHandle->getHeight()));
		} else {
			sliderHandle->draw(x+val*(width-sliderHandle->getWidth()), y);
		}
	}
	ofSetColor(255, 255, 255);
	if(showValue && value) {
		//		    printf("->%s\n",  name.c_str());

		if(stepped) {
			drawLabelEitherSide(name, ofToString((int)fval(value)), 0, -3);
			
			
		} else {
			float range = (max - min);
			if(range>1) decimalPlaces = 3;
			else if(range>0.01) decimalPlaces = 4;
			else if(range>0.001) decimalPlaces = 5;
			else if(range>0.001) decimalPlaces = 6;
			else decimalPlaces = 5;
	
			string ss = ofToString(fval(value), decimalPlaces);
			drawLabelEitherSide(name, ss, 0, -3);
		}
		
	} else {
		drawLabel(x, y);
	}
	if(sliderBG==NULL) {



		setRGBA(borderColor);

		ofNoFill();
		ofRect(*this);
		ofFill();
	}
}
// round() not supported in vs2010
float xmlgui::Slider::__round(float number) {
	return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
}

bool xmlgui::Slider::rangeIncludesZero() {
	return (min>=0 && max<=0) || (min<=0 && max>=0);
}

bool xmlgui::Slider::touchDown(int _x, int _y, int touchId) {

	if(ofGetElapsedTimef()-lastTimePressed<0.2 && rangeIncludesZero()) {
		fval(value) = 0;
		return true;
	}
	
	float val = 0;
	if(vertical) {
		val = 1 - (_y-y)/height;
	} else {
		val = (_x-x)/width;
	}
	if(logarithmic) {
		if(val>0) val = val * val;
	}
	
	fval(value) = ofMap(val, 0, 1, min, max, true);
	if(stepped) {
		fval(value) = __round(fval(value));
	}
	
	return true;
}


void xmlgui::Slider::touchOver(int _x, int _y, int id) {
	if(inside(_x, _y)) {
		parent->setKeyboardFocus(this);
	} else {
		if(this->focus) {
			parent->setKeyboardFocus(NULL);
		}
	}
}

bool xmlgui::Slider::touchMoved(int _x, int _y, int touchId) {
	touchDown(_x, _y, touchId);
	return true;
}
bool xmlgui::Slider::touchUp(int _x, int _y, int touchId) {
	if(inside(_x, _y)) {
		lastTimePressed = ofGetElapsedTimef();
		return true;
	}
	return false;
}

bool xmlgui::Slider::keyPressed(int key) {
	float increment = 0;
	if(key==OF_KEY_LEFT) {
		increment = -(max-min)/(width*10.f);
	} else if(key==OF_KEY_RIGHT) {
		increment = (max-min)/(width*10.f);
	}
#ifndef TARGET_RASPBERRY_PI
#ifndef TARGET_OF_IOS
//	if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) increment *= 10.f;
#endif	
#endif
	fval(value) = ofClamp(fval(value)+increment, min, max);
    printf("Got key event\n");
    if(key==OF_KEY_LEFT || key==OF_KEY_RIGHT) return true;
    return false;
}


bool xmlgui::Slider::keyReleased(int key) {
}

void xmlgui::Slider::getParameterInfo(vector<ParameterInfo> &params) {
	LabeledControl::getParameterInfo(params);
	params.push_back(ParameterInfo("Vertical", "vertical", "toggle", &vertical));
	params.push_back(ParameterInfo("Min", "min", "floatfield", &min));
	params.push_back(ParameterInfo("Max", "max", "floatfield", &max));
	params.push_back(ParameterInfo("Stepped", "stepped", "toggle", &stepped));
	params.push_back(ParameterInfo("Value", "value", "floatfield", value));
	params.push_back(ParameterInfo("Show Value", "showvalue", "toggle", &showValue));
	params.push_back(ParameterInfo("BG Color", "bgColor", "hexcolorpicker", &bgColor));
	params.push_back(ParameterInfo("Slider Color", "fgColor", "hexcolorpicker", &fgColor));
	params.push_back(ParameterInfo("Border Color", "borderColor", "hexcolorpicker", &borderColor));
	params.push_back(ParameterInfo("Slider BG", "sliderBGUrl", "file", &sliderBGUrl));
	params.push_back(ParameterInfo("Slider FG", "sliderFGUrl", "file", &sliderFGUrl));
	params.push_back(ParameterInfo("Slider Handle", "sliderHandleUrl", "file", &sliderHandleUrl));
}

string xmlgui::Slider::valueToString() {
	return ofToString(fval(value), 9);
}

void xmlgui::Slider::valueFromString(string inp) {
	fval(value) = atof(inp.c_str());
	//printf("Got value from string %s: %f\n", controlId.c_str(), fval(value));
}
