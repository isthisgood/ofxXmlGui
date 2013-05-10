/*
 *  GuiColourPicker.h
 *  Gui
 *
 *  Created by Marek Bereza on 12/04/2010.
 *
 */

#pragma once

#include "xmlgui/controls/LabeledControl.h"
class HexColorPicker: public LabeledControl {
public:

	HexColorPicker(): LabeledControl() {

		height = 55;
		width = 70;
		value = new int[1];
		ival(value) = 0;
	}

	int getBrightness() {
		float r = ((ival(value)&0xFF0000)>>16);
		float g = ((ival(value)&0xFF00)>>8);
		float b = ival(value)&0xFF;
		float a = 255 - ((ival(value)&0xFF000000)>>24);
		return (r + g + b + a) / 4;
	}

	void draw() {



		setRGBA(ival(value));
		ofRect(x, y, width, height);

		float h4 = height/4;
		if(getBrightness()<128) {
			ofSetColor(255, 255, 255);
		} else {
			ofSetHexColor(0);
		}
		ofNoFill();
		ofRect(x, y, width, height);
		ofLine(x, y + h4,  x+width, y + h4);
		ofLine(x, y + h4*2,  x+width, y + h4*2);
		ofLine(x, y + h4*3,  x+width, y + h4*3);


		float c = ((ival(value)&0xFF0000)>>16);
		c /= 255.f;
		ofLine(x + width*c, y, x + width*c, y+h4);


		c = ((ival(value)&0xFF00)>>8);
		c /= 255.f;
		ofLine(x + width*c, y+h4, x + width*c, y+h4*2);


		c = ival(value)&0xFF;
		c /= 255.f;
		ofLine(x + width*c, y+h4*2, x + width*c, y+h4*3);


		c = 255 - ((ival(value)&0xFF000000)>>24);
		c /= 255.f;
		ofLine(x + width*c, y+h4*3, x + width*c, y+height);
		ofFill();

		xmlgui::Resources::bindFont();
		xmlgui::Resources::drawString("red", x+3, y+h4-5);
		xmlgui::Resources::drawString("green", x+3, y+h4*2 - 5);
		xmlgui::Resources::drawString("blue", x+3, y+h4*3 - 5);
		xmlgui::Resources::drawString("alpha", x+3, y+height - 5);
		drawLabel(x+3, y-2);
		xmlgui::Resources::unbindFont();


	}


	bool touchDown(int _x, int _y, int touchId) {


		float val = (255.f*(_x-x)/width);
		val = ofClamp(val, 0, 255);
		int v = val;
		if(_y-y<height/4) { // r value
			ival(value) &= 0x0000FFFF;
			ival(value) += v << 16;
		} else if(_y-y<(height*2)/4) { // g value
			ival(value) &= 0x00FF00FF;
			ival(value) += v << 8;

		} else if(_y-y<(height*3)/4) { // b value
			ival(value) &= 0x00FFFF00;
			ival(value) += v;

		} else { // a value
			ival(value) &= 0x00FFFFFF;
			ival(value) += (255-v) << 24;

		}
		return true;

	}



	bool touchMoved(int _x, int _y, int touchId) {
		touchDown(_x, _y, touchId);
		return true;
	}



	void getParameterInfo(vector<ParameterInfo> &params) {
		LabeledControl::getParameterInfo(params);
		params.push_back(ParameterInfo("Value","value", "intfield", value));
	}

	string valueToString() { return ofToString(ival(value)); }
	void valueFromString(string inp) { ival(value) = atoi(inp.c_str()); }
};
