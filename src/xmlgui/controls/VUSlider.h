//
//  VUSlider.h
//
// *  Created by Marek Bereza
//

#pragma once

#include "xmlgui/controls/LabeledControl.h"

namespace xmlgui {

	class VUSlider: public xmlgui::LabeledControl {
	
	public:
		int bgColor;
		int fgColor;

		ofVboMesh mesh;
		
		float minDb = -25;
		float maxDb = 4;
		
		float min = 0;
		float max = 1;
		
		VUSlider(): LabeledControl() {
			
			width = 200;
			height = 20;
			bgColor = 0x505050;
			fgColor = 0xCCCCCC;
			
			auto green = ofFloatColor(14/255.f, 148/255.f, 9/255.f);
			auto yellow = ofFloatColor(244/255.f, 191/255.f, 5/255.f);
			auto red = ofFloatColor(210/255.f, 0, 0);
			
			mesh.addColor(green);
			mesh.addColor(green);
			
			mesh.addVertex(ofVec3f(0, 0, 0));
			mesh.addVertex(ofVec3f(0, 1, 0));
			
			mesh.addColor(yellow);
			mesh.addColor(yellow);
			mesh.addVertex(ofVec3f(0.4, 0, 0));
			mesh.addVertex(ofVec3f(0.4, 1, 0));
			
			mesh.addColor(red);
			mesh.addColor(red);
			mesh.addVertex(ofVec3f(0.8, 0, 0));
			mesh.addVertex(ofVec3f(0.8, 1, 0));
			
			mesh.addColor(red);
			mesh.addColor(red);
			mesh.addVertex(ofVec3f(1, 0, 0));
			mesh.addVertex(ofVec3f(1, 1, 0));
			mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

			value = new float[1];
			fval(value) = 1;
		}
		

		float dB2lin(float in) {
			return pow(10, in / 20.f);
		}
		
		float lin2dB(float in) {
			return 20 * log(in);
		}
		
		string valueToString() {
			return ofToString(fval(value), 9);
		}
		
		void valueFromString(string inp) {
			fval(value) = atof(inp.c_str());
		}

		
		
		float a = 0;
		void follow(float in) {
			//in = ofClamp(sin(a++) * 1000, -1, 1);
			
			//in = 0.5;
			
			static float attackCurve = 0.99;
			static float releaseCurve = 0.999;
			
			
//			
//			float dB = lin2dB(ABS(in));
//			if(dB > meterValue) {
//				meterValue = meterValue * attackCurve + dB * (1 - attackCurve);
//			} else {
//				meterValue = meterValue * releaseCurve + dB * (1 - releaseCurve);
//			}

			
			if(in > a) {
				a = a * attackCurve + in * (1 - attackCurve);
			} else {
				a = a * releaseCurve + in * (1 - releaseCurve);
			}
			
			meterValue = lin2dB(a);
		}
		
		
		

		float meterValue = -50;
		
		float handleWidth = 8;

		void draw() {
			
//			*meterValue = sin(ofGetElapsedTimef()*2) * 0.5 + 0.5;
			if(meterValue>0.01) {
				setRGBA(0xCC9999);
			} else {
				setRGBA(bgColor);
			}
			ofDrawRectangle(*this);
		
			ofRectangle mask = *this;
			
			
			// here's a dB value, a minDb and a maxDb, now graph it

			float min = dB2lin(minDb);
			float max = dB2lin(maxDb);
			float val = dB2lin(meterValue);
			float scale = ofMap(val, min, max, 0, 1, true);
//			float printf("meterValue: %f     value: %f   scale: %f", meterValue);
			//printf("minDb %f    maxDb %f        min %f  max %f\n", minDb, maxDb, min, max);
			mask.width *= scale; // <-- here!!
			
			
			
			
			pushMask(mask);
			ofPushMatrix();
			ofTranslate(x, y);
			ofScale(width, height);
			mesh.draw();
			ofPopMatrix();
			popMask();
			setRGBA(fgColor);
			ofNoFill();
			ofDrawRectangle(*this);
			ofFill();
			
			float zeroDBLineX = ofMap(1, min, max, x, x + width) - 1;
			
			ofDrawLine(
				zeroDBLineX,
				y,
				zeroDBLineX,
				y + height
			);
			// handle
			if(1) {
				float handleVerticalOverhang = 2;
				float handleHeight = height + handleVerticalOverhang * 2;
				float handleX = x + (handleWidth / 2) + (width - handleWidth) * fval(value) - (handleWidth / 2);
				ofRectangle r(handleX, y - handleVerticalOverhang, handleWidth, handleHeight);
				ofFill();
				setRGBA(bgColor);
				ofDrawRectRounded(r, 3);

				ofNoFill();
				setRGBA(fgColor);
				ofDrawRectRounded(r, 3);
			}
			ofFill();
			
			drawLabel(x+3, y);
		}
		
		
		
		
		bool touchDown(int _x, int _y, int touchId) {
			
//			if(ofGetElapsedTimef()-lastTimePressed<0.2 && rangeIncludesZero()) {
//				fval(value) = 0;
//				return true;
//			}
			
			float val = (_x-x)/(width);
			val = ofClamp(val, 0, 1);
			
//			if(logarithmic) {
//				if(val>0) val = val * val;
//			}
			
			fval(value) = ofMap(val, 0, 1, min, max, true);

			
			return true;
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
		
		bool touchMoved(int _x, int _y, int touchId) {
			touchDown(_x, _y, touchId);
			return true;
		}
		
		bool touchUp(int _x, int _y, int touchId) {
			if(inside(_x, _y)) {
//				lastTimePressed = ofGetElapsedTimef();
				return true;
			}
			return false;
		}

		
		
		
		
	};
};