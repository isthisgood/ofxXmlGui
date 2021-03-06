/*
 *  GuiDropDown.h
 *  Gui
 *
 *  Created by Marek Bereza on 05/05/2010.
 *
 */

#pragma once

#define SCROLL_NONE 0
#define SCROLL_UP 4
#define SCROLL_DOWN -4

#include "xmlgui/controls/LabeledControl.h"

#define hexValR(A)  ((A >> 16) & 0xff)
#define hexValG(A) ((A >> 8) & 0xff)
#define hexValB(A) ((A >> 0) & 0xff)

namespace xmlgui {
	class List: public LabeledControl {
	public:
		function<void(int)> valueChanged;
		int bgColor;
		int fgColor;

		int scrollOffset;
		int itemHeight;
		int scrollerWidth;
		int scrolling;
		bool dragging;

		string options;
		List(): LabeledControl() {
			bgColor = 0x333333;
			fgColor = 0xFFFFFF;
			dragging = false;
			scrolling = SCROLL_NONE;
            itemHeight = LabeledControl::DEFAULT_CONTROL_HEIGHT - 4;
			scrollerWidth = 22;
			scrollOffset = 0;
			value = new int[1];
			ival(value) = -1;
			width = 100;
			height = 100;
			x = y = 0;
		}

		void load() {
			items = ofSplitString(options, "|");
			// must have zero items if blank
			if(options=="") items.clear();

		}

		void addItem(string tem) {
			items.push_back(tem);
		}
		void clearItems() {
			items.clear();
		}

		string getSelectedString() {
			if(ival(value)==-1) return "";
			else return items[ival(value)];
		}

		void setSelection(string itemName) {
			for(int i = 0; i < items.size(); i++) {
				if(items[i]==itemName) {
					ival(value) = i;
					return;
				}
			}
			ival(value) = -1;
		}
		void draw() {

			scrollOffset += scrolling;
			if(dragging) {
				scrolling = SCROLL_NONE;
			}
			if(scrollOffset>0) scrollOffset = 0;

			if(needsScrollbar() && items.size()*itemHeight+scrollOffset<height) {
				scrollOffset = height - items.size()*itemHeight;
			}

			setRGBA(bgColor);
			ofDrawRectangle(x, y, width, height);

			//glEnable(GL_SCISSOR_TEST);

			ofNoFill();
			setRGBA(fgColor);
			ofDrawRectangle(x, y, width, height);

			
			
			ofPoint abs = getAbsolutePosition();
			pushMask(abs.x, abs.y, width - scrollerWidth, height);



			ofFill();
			ofSetHexColor(0x999999);
			
			for(int i =0; i < items.size(); i++) {
				float yy = scrollOffset + y + i*itemHeight;
				if(i==ival(value)) {
					ofSetHexColor(0x007700);
					ofDrawRectangle(x, yy, width-scrollerWidth, itemHeight);
				}
				
				ofSetHexColor(0x999999);
				ofLine(x, yy+itemHeight, x+width-scrollerWidth, yy+itemHeight);

			}
			
			
			
			ofNoFill();
			
			setRGBA(fgColor);
			
			// do all font writing in one fell swoop
			xmlgui::Resources::bindFont();
			for(int i =0; i < items.size(); i++) {
				//if(i==ival(value)) ofSetHexColor(0x00FF00);
				//else
				//	setRGBA(fgColor);
				
				xmlgui::Resources::drawString(items[i], x+5, scrollOffset + y + (i+1)*itemHeight-4);
				
				// only draw as much as we need
				//if((i+1)*itemHeight>height) break;
			}
			xmlgui::Resources::unbindFont();
			
		
			popMask();
			
			
			int c = 0;
			if(needsScrollbar()) c = fgColor;
			else c = blendColor(fgColor, bgColor, 0.2);
			setRGBA(c);
			ofLine(x+width-scrollerWidth, y+height/2, x+width, y+height/2);
			ofLine(x+width-scrollerWidth, y, x+width-scrollerWidth, y+height);
			
			//xmlgui::Resources::bindFont();
			ofSetColor(hexToFloatColor(c));
			xmlgui::Resources::drawString("UP", 4+x+width-scrollerWidth, y+height/2 - 5);//, hexToFloatColor(c));
			xmlgui::Resources::drawString("DN", 4+x+width-scrollerWidth, y+height/2 + 15);//, hexToFloatColor(c));
			//xmlgui::Resources::unbindFont();
			
			
			
			ofFill();
			drawLabel();
		}


		int blendColor(int color1, int color2, float amt) {
			float r = hexValR(color1)*amt + hexValR(color2)*(1.f-amt);
			float g = hexValG(color1)*amt + hexValG(color2)*(1.f-amt);
			float b = hexValB(color1)*amt + hexValB(color2)*(1.f-amt);

			return colorFloat255ToHex(r, g, b);
		}
		ofFloatColor hexToFloatColor(int hex) {
			float r = hexValR(hex);
			float g = hexValG(hex);
			float b = hexValB(hex);
			return ofFloatColor(r/255.f, g/255.f, b/255.f);
		}
		
		int colorFloat255ToHex(float r, float g, float b) {
			int rr = r;
			int gg = g;
			int bb = b;
			return (rr << 16) + (gg << 8) + bb;
		}


		bool needsScrollbar() {
			return items.size()*itemHeight>height;
		}
		bool touchDown(int _x, int _y, int touchId) {
			if(_x>x+width-scrollerWidth && needsScrollbar()) {
				// scrolling with scroll bars
				float yy = _y - y;
				yy /= height;
				if(yy<0.5) {
					scrolling = SCROLL_UP;
				} else {
					scrolling = SCROLL_DOWN;
				}

			} else {
				scrolling = SCROLL_NONE;
				lastMouse = ofPoint(_x, _y);
			}
			return true;
		}

		bool touchMoved(int _x, int _y, int touchId) {
			if(_x>x+width-scrollerWidth) {

			} else if(needsScrollbar()) {
				dragging = true;
				scrolling = _y - lastMouse.y;
				// do some drag scrolling
				lastMouse = ofPoint(_x, _y);
			}
			return true;
		}


		bool touchUp(int _x, int _y, int touchId) {
			if(!dragging && ofVec2f(_x, _y).distance(lastMouse)<3) {
				// click!
				float offset = (float)(_y - y - scrollOffset)/itemHeight;
				int i = floor(offset);
				if(i>=0 && i < items.size()) {
					ival(value) = i;
					if(valueChanged) valueChanged(i);
	//#pragma warning this was uncommented code before I imported the widgets to the new ofxXmlGui
					/*if(parent!=NULL) {
						xmlgui::Event e(this, xmlgui::Event::TOUCH_UP);
						for(int i = 0; i < parent->listeners.size(); i++) {
							parent->listeners[i]->controlChanged(&e);
						}
					}*/
				}
			} else {
				touchMoved(_x, _y, touchId);
			}
			dragging = false;
			scrolling = SCROLL_NONE;
			return true;
		}

		void getParameterInfo(vector<ParameterInfo> &params) {
			LabeledControl::getParameterInfo(params);
			params.push_back(ParameterInfo("BG Color", "bgColor", "hexcolorpicker", &bgColor));
			params.push_back(ParameterInfo("FG Color", "fgColor", "hexcolorpicker", &fgColor));
			params.push_back(ParameterInfo("Options", "options","textfield",&options));
		}



		ofPoint lastMouse;
		vector<string> items;
		string valueToString() { return ofToString(ival(value)); }
		void valueFromString(string inp) { ival(value) = atoi(inp.c_str()); }
	};
}
