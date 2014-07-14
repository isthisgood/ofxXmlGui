/*
 *  GuiPanner.h
 *  Snowboard
 *
 *  Created by Marek Bereza on 09/12/2010.
 *

 *  TODO: Doesn't work for graphics files, just plain colours I don't think.
 */


#pragma once

#include "xmlgui/controls/Slider.h"
namespace xmlgui {
	class Panner: public Slider {
	public:

		
		void draw() {

			if(sliderBG!=NULL) {
				ofSetHexColor(0xFFFFFF);
				sliderBG->draw(x, y);
			} else {
				setRGBA(bgColor);
				ofRect(x, y, width, height);
			}

			float val = (fval(value)-min)/(max-min);
			
			if(val>1) val = 1;
			else if(val<0) val = 0;
			
			if(sliderFG!=NULL) {
				ofSetHexColor(0xFFFFFF);
				ofPoint abs = getAbsolutePosition();
				if(vertical) pushMask(abs.x, abs.y + height-height*val, width, height*val);
				else pushMask(abs.x, abs.y, width*val, height);
				sliderFG->draw(x, y);
				popMask();
			} else {
				setRGBA(fgColor);

				if(vertical) ofRect(x, y + height - height*val , width, height*val - height/2);
				else ofRect(x+width/2, y, width*val - width/2, height);
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

				if(stepped) {
					drawLabelEitherSide(name, ofToString((int)__round(fval(value))), 0, -3);
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
				drawLabel(x, y-3);
			}
		}
	};

}

