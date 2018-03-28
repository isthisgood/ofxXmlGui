/*
 *  GuiSegmentedControl.h
 *  Gui
 *
 *  Created by Marek Bereza on 13/02/2010.
 *
 */

#pragma once

#include "xmlgui/controls/LabeledControl.h"
#include "xmlgui/controls/PushButton.h"

namespace xmlgui {

	class SegmentedControl: public LabeledControl {
	public:


		function<void(int)> valueChanged;
		string options;
		bool vertical;
		vector<string> opts;
		int fgColor;
		int bgColor;
		int borderColor;

		SegmentedControl(): LabeledControl() {
			value = new int[1];
			ival(value) = 0;
            height = LabeledControl::DEFAULT_CONTROL_HEIGHT;
			vertical = false;
			fgColor = 0xC80000;
			borderColor = 0;
			bgColor = 0x444444;
		}

		void load() {
			opts = ofSplitString(options, "|");
			//height = 20;
			if(value==NULL) {
				value = new int[1];
				ival(value) = 0;
			}

		}
		string getSelectedString() {
			if(ival(value)==-1) return "";
			else return opts[ival(value)];
		}
		
		void addOption(string opt) {
			if(options=="") options = opt;
			else options += string("|") + opt;
			opts = ofSplitString(options, "|");
		}
		void draw() {
			drawLabel(x, y-3);
			int size = opts.size();
			if(size==0) {
				ofSetColor(100, 100, 100);
				ofDrawRectangle(x,y,width,height);
			}
//			printf("%s: %d\n", name.c_str(), ival(value));

			if(vertical) {
				for(int i = 0; i < size; i++) {
					ofRectangle r(x, y+(height/size)*i, width, (height/size)-3);

					ofFill();
					if(i==ival(value)) {
						setRGBA(fgColor);
						ofDrawRectangle(r);
					} else {
						setRGBA(bgColor);
						ofDrawRectangle(r);
					}

					setRGBA(labelColor);
					xmlgui::Resources::drawString(this, opts[i], x+3, y + (height/size)*(i+1)-3);
					setRGBA(borderColor);
					ofNoFill();
					ofDrawRectangle(r);
					ofFill();
				}
			} else {
				for(int i = 0; i < size; i++) {

					ofRectangle r(x+(width/size)*i, y, (width/size)-3, height);
					if(i==ival(value)) {
						setRGBA(fgColor);
						ofDrawRectangle(r);
					} else {
						setRGBA(bgColor);
						ofDrawRectangle(r);
					}

					setRGBA(labelColor);
					xmlgui::Resources::drawString(this, opts[i], x+(width/size)*i+3, y+height-3);
					setRGBA(borderColor);
					ofNoFill();
					ofDrawRectangle(r);
					ofFill();
				}
			}

		}

		void getParameterInfo(vector<ParameterInfo> &params) {
			LabeledControl::getParameterInfo(params);
			ofJoinString(opts, "|");
			params.push_back(ParameterInfo("Options", "options","textfield",&options));
			params.push_back(ParameterInfo("Value", "value","intfield",value));
			params.push_back(ParameterInfo("vertical", "vertical", "toggle", &vertical));
			params.push_back(ParameterInfo("Border Color", "borderColor", "hexcolorpicker", &borderColor));
			params.push_back(ParameterInfo("Selected Color", "fgColor", "hexcolorpicker", &fgColor));
			params.push_back(ParameterInfo("BG Color", "bgColor", "hexcolorpicker", &bgColor));

		}


		bool touchDown(int _x, int _y, int touchId) {
			if(vertical) {
				ival(value) = (int)((float)opts.size()*(_y-y)/height);
				if(valueChanged) valueChanged(ival(value));

			} else {
				ival(value) = (int)((float)opts.size()*(_x-x)/width);
				if(valueChanged) valueChanged(ival(value));

			}
			return true;
		}
		bool touchMoved(int _x, int _y, int touchId) {
			return touchDown(_x, _y, touchId);
		}

		string valueToString() {
			return ofToString(ival(value));
		}

		void valueFromString(string inp) {
			ival(value) = atoi(inp.c_str());
		}
	};

}

