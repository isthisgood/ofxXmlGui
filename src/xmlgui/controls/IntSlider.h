/**     ___           ___           ___                         ___           ___
 *     /__/\         /  /\         /  /\         _____         /  /\         /__/|
 *    |  |::\       /  /::\       /  /::|       /  /::\       /  /::\       |  |:|
 *    |  |:|:\     /  /:/\:\     /  /:/:|      /  /:/\:\     /  /:/\:\      |  |:|
 *  __|__|:|\:\   /  /:/~/::\   /  /:/|:|__   /  /:/~/::\   /  /:/  \:\   __|__|:|
 * /__/::::| \:\ /__/:/ /:/\:\ /__/:/ |:| /\ /__/:/ /:/\:| /__/:/ \__\:\ /__/::::\____
 * \  \:\~~\__\/ \  \:\/:/__\/ \__\/  |:|/:/ \  \:\/:/~/:/ \  \:\ /  /:/    ~\~~\::::/
 *  \  \:\        \  \::/          |  |:/:/   \  \::/ /:/   \  \:\  /:/      |~~|:|~~
 *   \  \:\        \  \:\          |  |::/     \  \:\/:/     \  \:\/:/       |  |:|
 *    \  \:\        \  \:\         |  |:/       \  \::/       \  \::/        |  |:|
 *     \__\/         \__\/         |__|/         \__\/         \__\/         |__|/
 *
 *  Description:
 *
 *  IntSlider.h, created by Marek Bereza on 07/02/2012.
 */



#pragma once

#include "xmlgui/framework/Control.h"
#include "xmlgui/controls/LabeledControl.h"
#include "Container.h"

#include <math.h>


#ifdef TARGET_WIN32
#define GLUT_BUILDING_LIB
#include "glut.h"
#endif
#ifdef TARGET_OSX
#include <OpenGL/OpenGL.h>
#include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
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


namespace xmlgui {
	class IntSlider: public LabeledControl {
	public:

		bool vertical;
		int min;
		int max;
		int bgColor;
		int fgColor;
		int borderColor;
		bool showValue;
		
		bool logarithmic;

		string sliderBGUrl;
		string sliderFGUrl;
		string sliderHandleUrl;
		ofImage *sliderBG, *sliderFG, *sliderHandle;
		IntSlider(): LabeledControl() {
			sliderBG = sliderFG = sliderHandle = NULL;
			vertical = false;
			height = 20;
			width = 100;

			logarithmic = false;
			value = new int[1];
			ival(value) = 0;
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

		void load() {
			if(ival(value)<min) ival(value) = min;
			if(ival(value)>max) ival(value) = max;
			sliderBG = xmlgui::Resources::getImage(sliderBGUrl);
			sliderFG = xmlgui::Resources::getImage(sliderFGUrl);
			sliderHandle = xmlgui::Resources::getImage(sliderHandleUrl);
			if(sliderBG!=NULL) {
				scalable = false;
				width = sliderBG->getWidth();
				height = sliderBG->getHeight();
			}

		}



		void draw() {

			if(sliderBG!=NULL) {
				ofSetHexColor(0xFFFFFF);
				sliderBG->draw(x, y);
			} else {
				if(parent->isKeyboardFocusedOn(this)) {
					setRGBA(0x777777);
				} else {
					setRGBA(bgColor);
				}
				ofRect(x, y, width, height);
			}

			float val = ((float)ival(value)-min)/(float)(max-min);
			
			if(logarithmic) {
				val = sqrt(val);
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
                if (val < 0) // out of range indicators
                {
                    ofSetColor(0, 0, 0);
                    ofRect(x, y, width, height);
                }
                else if (val > 1)
                {
                    ofSetColor(255, 0, 0);
                    ofRect(x, y, width, height);
                } else {
                    if(vertical) ofRect(x, y+height-height*val, width, height*val);
                    else ofRect(x, y, width*val, height);
			
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
			if(showValue) {


//				string lab = name + "  " + ofToString(ival(value), 3);
//				drawCustomLabel(lab, x, y-3);
				drawLabelEitherSide(name, ofToString(ival(value)), 0, -3);
			} else {
				drawLabel(x, y-3);
			}
			if(sliderBG==NULL) {
				setRGBA(borderColor);
				ofNoFill();
				ofRect(*this);
				ofFill();
			}
		}
		// round() not supported in vs2010
		float __round(float number) {
			return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
		}

		bool touchDown(int _x, int _y, int touchId) {
			
			float val = 0;
			
			if(vertical) {
				val = 1 - (float)(_y-y)/height;
			} else {
				val = (float)(_x-x)/width;
			}

			
			val =	ofClamp(val, 0, 1);
			if(logarithmic) {
				val = val * val;
			}
			
			ival(value) = __round((float)val*((float)max-min) + min);
//			ival(value) = __round((float)val*((float)max-min) + min);
			
			return true;
		}

		bool touchMoved(int _x, int _y, int touchId) {
			touchDown(_x, _y, touchId);
			return true;
		}

		void getParameterInfo(vector<ParameterInfo> &params) {
			LabeledControl::getParameterInfo(params);
			params.push_back(ParameterInfo("Vertical", "vertical", "toggle", &vertical));
			params.push_back(ParameterInfo("Min", "min", "intfield", &min));
			params.push_back(ParameterInfo("Max", "max", "intfield", &max));
			params.push_back(ParameterInfo("Value", "value", "intfield", value));
			params.push_back(ParameterInfo("Show Value", "showvalue", "toggle", &showValue));
			params.push_back(ParameterInfo("BG Color", "bgColor", "hexcolorpicker", &bgColor));
			params.push_back(ParameterInfo("Slider Color", "fgColor", "hexcolorpicker", &fgColor));
			params.push_back(ParameterInfo("Border Color", "borderColor", "hexcolorpicker", &borderColor));
			params.push_back(ParameterInfo("Slider BG", "sliderBGUrl", "file", &sliderBGUrl));
			params.push_back(ParameterInfo("Slider FG", "sliderFGUrl", "file", &sliderFGUrl));
			params.push_back(ParameterInfo("Slider Handle", "sliderHandleUrl", "file", &sliderHandleUrl));
		}

		string valueToString() {
			return ofToString(ival(value));
		}

		void valueFromString(string inp) {
			ival(value) = atoi(inp.c_str());
		}
		
		
		void touchOver(int _x, int _y, int id) {
			if(inside(_x, _y)) {
				parent->setKeyboardFocus(this);
			} else {
				if(this->focus) {
					parent->setKeyboardFocus(NULL);
				}
			}
		}
		
		
		bool keyPressed(int key) {
			float increment = 0;
			if(key==OF_KEY_LEFT) {
				increment = -1;
			} else if(key==OF_KEY_RIGHT) {
				increment = 1;
			}
#ifndef TARGET_RASPBERRY_PI			
			if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
				if((max-min)>width) {
					increment *= (max-min)/width;
				} else {
					increment *= 10;
				}
			}
#endif

            if(increment!=0) {
                ival(value) = ofClamp(ival(value)+increment, min, max);
                return true;
            }
            
            return false;
		}
		
	};

}
