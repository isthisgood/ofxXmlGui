/*
 *  GuiColorPicker.h
 *  Gui
 *
 *  Created by Marek Bereza on 12/04/2010.
 *
 */

#pragma once

#include "xmlgui/controls/LabeledControl.h"
namespace xmlgui {
	class FloatColorPicker: public LabeledControl {
	public:

		float scale;

		FloatColorPicker(): LabeledControl() {

			height = 55;
			width = 70;
			scale = 1;
			value = new float[4];
			faval(value, 0) = 0;
			faval(value, 1) = 0;
			faval(value, 2) = 0;
			faval(value, 3) = 0;
		}

		float getBrightness() {
			return (faval(value,0)+faval(value,1)+faval(value,2))/3.f;
		}


		void draw() {



			//setRGBA(ival(value));
			glColor4f(faval(value,0)/scale, faval(value,1)/scale, faval(value,2)/scale, faval(value,3)/scale);
			ofRect(x, y, width, height);

			float h4 = height/4;

			if(getBrightness()>scale/2) {
				ofSetHexColor(0);
			} else {
				ofSetColor(255, 255, 255);
			}

			ofNoFill();
			ofRect(x, y, width, height);
			ofLine(x, y + h4,  x+width, y + h4);
			ofLine(x, y + h4*2,  x+width, y + h4*2);
			ofLine(x, y + h4*3,  x+width, y + h4*3);


			float c = faval(value, 0)/scale;

			ofLine(x + width*c, y, x + width*c, y+h4);

			c = faval(value, 1)/scale;
			ofLine(x + width*c, y+h4, x + width*c, y+h4*2);


			c = faval(value, 2)/scale;
			ofLine(x + width*c, y+h4*2, x + width*c, y+h4*3);


			c = faval(value, 3)/scale;
			ofLine(x + width*c, y+h4*3, x + width*c, y+height);
			ofFill();
			xmlgui::Resources::bindFont();
			xmlgui::Resources::drawString(this, "R", x+3, y+h4-3);
			xmlgui::Resources::drawString(this, "G", x+3, y+h4*2 - 3);
			xmlgui::Resources::drawString(this, "B", x+3, y+h4*3 - 3);
			xmlgui::Resources::drawString(this, "A", x+3, y+height - 3);
			drawLabel(x+3, y-2);
			xmlgui::Resources::unbindFont();


		}


		bool touchDown(int _x, int _y, int touchId) {


			float v = scale * (_x-x)/width;
			v = ofClamp(v, 0, scale);

			if(_y-y<height/4) { // r value

				faval(value,0) = v;

			} else if(_y-y<(height*2)/4) { // g value
				faval(value,1) = v;

			} else if(_y-y<(height*3)/4) { // b value
				faval(value,2) = v;

			} else { // a value
				faval(value,3) = v;

			}
			return true;

		}



		bool touchMoved(int _x, int _y, int touchId) {
			touchDown(_x, _y, touchId);
			return true;
		}



		void getParameterInfo(vector<ParameterInfo> &params) {
			LabeledControl::getParameterInfo(params);
			params.push_back(ParameterInfo("Red","red", "slider", value));
			params.push_back(ParameterInfo("Green","green", "slider", ((float*)value)+1));
			params.push_back(ParameterInfo("Blue","blue", "slider", ((float*)value)+2));
			params.push_back(ParameterInfo("Alpha","alpha", "slider", ((float*)value)+3));
			params.push_back(ParameterInfo("Scale","scale", "slider", &scale));
		}

		string valueToString()
		{
			ostringstream oss;
			oss <<  ofToString(faval(value, 0), 9) << "," <<
					ofToString(faval(value, 1), 9) << "," <<
					ofToString(faval(value, 2), 9) << "," <<
					ofToString(faval(value, 3), 9);
			return oss.str();
			/*return ofToString(faval(value, 0), 9)
			+ "," + ofToString(faval(value, 1), 9);
			+ "," + ofToString(faval(value, 2), 9);
			+ "," + ofToString(faval(value, 3), 9);*/
		}

		void valueFromString(string inp) {
			vector<string> vals = ofSplitString(inp, ",");
			if(vals.size()>=3) {
				faval(value, 0) = atof(vals[0].c_str());
				faval(value, 1) = atof(vals[1].c_str());
				faval(value, 2) = atof(vals[2].c_str());
				if(vals.size()>=4) {
					faval(value, 3) = atof(vals[3].c_str());
				}
			}
		}
	};
}

