//
//  DropDown.h
//
//  Created by Marek Bereza
//

#pragma once

#include "xmlgui/controls/LabeledControl.h"
namespace xmlgui {
	
	class DropDown: public xmlgui::LabeledControl {
	public:
		int bgColor;
		int fgColor;
		int textColor;
		float cornerRadius = 3;
		
		bool out = false;
		vector<string> options;
		
		int itemHeight = 20;
		int initialWidth = 200;
		
		
		
		string ellipsize(string s, float w) {
			
			if(xmlgui::Resources::stringWidth(s) < w) {
				return s;
			}
			
			for(int i = s.size()-1; i > 0; i--) {
				string a = s.substr(0, i) + "...";
				if(xmlgui::Resources::stringWidth(a) < w) {
					return a;
				}
			}
			return s;
			
		}
		
		
		DropDown(): LabeledControl() {
			
			width = 200;
			height = 20;
			bgColor = 0x505050;
			fgColor = 0xCCCCCC;
			textColor = fgColor;
			value = new int[1];
			ival(value) = 0;

		}
		
		
		void touchOver(int x, int y, int id) {
			if(out && inside(x, y)) {
				itemOver = (y - this->y) / itemHeight;
			}
		}

		int downedOn = -1;
		int itemOver = -1;
		
		bool touchDown(int x, int y, int id) {
			if(!out) {
				itemHeight = height;
				initialWidth = width;
			}
			
			down = true;
			if(out) {
				downedOn = (y - this->y) / itemHeight;
			}
			return true;
		}
		
		bool touchMoved(int x, int y, int id) {
			if(!inside(x, y)) {
				down = false;

			}
			return true;
		}
		
		
		float widthOfWidestString() {
			float maxWidth = initialWidth;
			for(int i = 0; i < options.size(); i++) {
				float w = xmlgui::Resources::stringWidth(options[i]);
				if(w>maxWidth) {
					maxWidth = w;
				}
			}
			return maxWidth + 12;
		}
		
		
		void toggleOut() {
			out ^= true;
			if(out) {
				itemOver = 0;
				height = itemHeight * options.size();
				width = widthOfWidestString();
				sendToFront();
				
			} else {
				height = itemHeight;
				width = initialWidth;
			}
			//printf("%f\n", height);
		}
		
		
		
		bool touchUp(int x, int y, int id) {
			
			if(inside(x, y)) {
				if(out) {
					if(ival(value)!=downedOn) {
						ival(value) = downedOn;
						downedOn = -1;
//						if(valueChanged) {
//							valueChanged(selectedIndex);
//						}
					}
				}
				toggleOut();
			}
			
			down = false;
			return true;
		}
		
		
		
		
		void draw() {
			
			ofFill();
			setRGBA(bgColor);
			ofDrawRectRounded(*this, cornerRadius);
			
			
			
			
			
			
			if(!out) {
				string itemToShow = "--";
				if(options.size()>0) {
					itemToShow = options[ival(value)];
				}
				setRGBA(textColor);
				xmlgui::Resources::drawString(ellipsize(itemToShow, width - 20), x+8, y+15);
				drawDownTriangle();
				
			} else {
				for(int i = 0; i < options.size(); i++) {
					if(itemOver==i || (down && downedOn==i)) {
						if(down && downedOn==i) {
							ofSetColor(150, 150, 255);
						} else {
							ofSetColor(220, 220, 255);
						}
						ofDrawRectangle(x, y + i * itemHeight, width, itemHeight);
					}
					setRGBA(textColor);
					xmlgui::Resources::drawString(options[i], x+8, i * itemHeight + y+15);
				}
			}
			setRGBA(fgColor);
			ofNoFill();
			ofDrawRectRounded(*this, cornerRadius);
			ofFill();
			
			drawLabel(x+3, y);
		}
		
		void drawDownTriangle() {
			setRGBA(fgColor);
			
			float w = 9;
			float h = 7;
			float _x = x + width - w * 1.5;
			float _y = y + h;
			ofBeginShape();
			ofVertex(_x, _y);
			ofVertex(_x+w, _y);
			ofVertex(_x+w/2, _y+h);
			ofEndShape();
		}
	};
	

};