/*
 *  GuiIntField.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */


#pragma once

#include "xmlgui/controls/TextField.h"
namespace xmlgui {
	class IntField: public TextField {
	public:
		
		IntField() {
			height = 20;
			width = 70;
			value = new int[1];
			ival(value) = 0;
		}
		
		virtual bool isKeyAllowed(int key) {
			printf("%d\n", key);
			
			if(key==OF_KEY_BACKSPACE) return true;
			if(key>='0' && key<='9') return true;
			
			if(key=='-') {
				return true;
			}
			return false;
		}
		
		
		string getNumString() {
			
			string u = " ";
			if(ival(value)<0) u = "";
			return u+ofToString(ival(value));
		}
		
		int getPowerOf10() {
			/*0 = 1
			 -1 = 0.1
			 -2 = 0.01
			 -3 = 0.001
			 -4 = 0.0001
			 */
			for(int i = -5; i < 10; i++) {
				if(fval(value)>=powf(10, i) && fval(value)<=powf(10, i+1)) {
					return i;
				}
				
			}
			return 0;
		}
		
		float getUnit(int pos) {
			int p = 3+getPowerOf10() - pos;
			if(p<-1) p+= 1;
			return powf(10, p);
		}
		
		/*
		 bool touchUp(int x, int y, int id) {
		 scrolling = false;
		 if(inside(x, y)) {
		 parent->setKeyboardFocus(this);
		 return true;
		 } else {
		 return false;
		 }
		 }
		 
		 bool touchDown(int x, int y, int id) {
		 if(inside(x,y)) {
		 
		 scrolling = true;
		 int off = x - textPadding;
		 off /= numberSpacing;
		 incrementUnit = getUnit(off);
		 printf("start scroll %f     - %d\n", incrementUnit, off);
		 originalNumber = fval(value);
		 originalYOffset = y;
		 return true;
		 }
		 return false;
		 }
		 
		 bool touchMoved(int x, int y, int id) {
		 if(scrolling) {
		 // printf("Scrolling %f\n", fval(value));
		 fval(value) = originalNumber - incrementUnit*((y - originalYOffset)/10);
		 }
		 return true;
		 }
		 */
		void getParameterInfo(vector<ParameterInfo> &params) {
			LabeledControl::getParameterInfo(params);
			params.push_back(ParameterInfo("Value","value", "intfield", value));
		}
		
		string valueToString() { return ofToString(ival(value)); }
		void valueFromString(string inp) { ival(value) = atof(inp.c_str()); }
	};
}
