//
//  NumberBox.h
//  MegaMapper
//
//  Created by Marek on 2/18/16.
//
//

#pragma once

#include "xmlgui/controls/LabeledControl.h"
#include "xmlgui/textfield/TextFieldFontRenderer.h"


namespace xmlgui {
	class NumberBox: public LabeledControl {
	public:
		

		int bgColor;
		int fgColor;
		int borderColor;

		static xmlgui::ofxTextInput::FontRenderer* fontRef;	
		
		bool dragging;
		NumberBox();
		
		static const int VERTICAL_PADDING = 2;
		static const int HORIZONTAL_PADDING = 4;
		void draw() {
			ofSetColor(30);
			ofRect(*this);
			
			ofNoFill();
			ofSetColor(focus?255:200);
			ofRect(*this);
			ofFill();
			
			ofNoFill();
			ofSetColor(255, 100);
			// draw a rect around every char
//			float charWidth = 6;
//			float charHeight = 12;
//			
//			for(int i = 0; i < 10; i++) {
//				ofRectangle r;
//				r.x = x + HORIZONTAL_PADDING + i * charWidth;;
//				r.y = y + VERTICAL_PADDING;
//				r.width = charWidth;
//				r.height = charHeight;
//				ofDrawRectangle(r);
//			}
			ofFill();
			ofSetColor(255);
//			pushMask(p.x, p.y, width, height);
			string text = ofToString(fval(value));
			if(text.find(".")==-1) {
				text += ".0";
			} else {
				text += "0";
			}
			fontRef->drawString(text, x + HORIZONTAL_PADDING, y + fontRef->getLineHeight()+VERTICAL_PADDING);
//			popMask();

			drawLabel(x, y-3);
		}
		
		
		
		
		
		
		int getPowIncrementFromOffset(int offset) {
			string s = ofToString(fval(value));
			//1 is 0
			//0.1 is -1
			//10 is 1
			
			int dot = s.find(".");
			
			if(dot==-1) {
				s += ".0";
				dot = s.find(".");
			}
			
			if(offset < dot) {
				return dot - offset - 1;
			}

			return dot - offset;

			
		}
		
		
		ofVec2f initialTouch;
		float initialValue;
		int powIncrement;
		
		bool touchDown(int x, int y, int id){


			if(!inside(x, y)) return false;
//			printf("inside\n");
			float charWidth = 6;
			//float charHeight = 12;
		
			int offset = x - this->x - HORIZONTAL_PADDING;
			offset /= charWidth;
			if(offset < 0) return false;
			
			initialTouch.set(x, y);
			initialValue = fval(value);
			powIncrement = getPowIncrementFromOffset(offset);
			
			printf("%d\n", powIncrement);
			
			dragging = true;
			return true;
 
		}
		
		
		bool touchMoved(int x, int y, int id) {
			if(dragging) {
				
				int pixelsPerUnit = 4;
				int mult = (initialTouch.y - y)/pixelsPerUnit;
				if(fval(value)<0) mult *= -1;
				
				float increment = powf(10, powIncrement);
				printf("Increment: %f, pow: %d\n", increment, powIncrement);
				fval(value) = initialValue + increment * mult;

			}
			return true;
		}
		
		bool touchUp(int x, int y, int id){
			dragging = false;
/*
			if(inside(x, y)) {
				lastTimeClicked = ofGetElapsedTimef();
				if(!isEditing && mouseDownInRect){
					beginEditing();
					return true;
				}
			}
			else if(isEditing){
				endEditing();
			}
 */
			return false;
		}

		
		
		
		
		void getParameterInfo(vector<ParameterInfo> &params) {
			LabeledControl::getParameterInfo(params);
			params.push_back(ParameterInfo("Value","value", "floatfield", value));
		}
		
		string valueToString() { return ofToString(fval(value)); }
		void valueFromString(string inp) { fval(value) = atof(inp.c_str()); }
		
		
	};
	
	
	

	
};