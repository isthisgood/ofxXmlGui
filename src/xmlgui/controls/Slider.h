/*
 *  GuiSlider.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */

#pragma once

#include "xmlgui/controls/LabeledControl.h"
#include <math.h>
#include <stdlib.h>

namespace xmlgui {
	class Slider: public LabeledControl {
	public:

		bool vertical;
		float min;
		float max;
		int bgColor;
		int fgColor;
		int borderColor;
		bool stepped;
		bool showValue;
		float lastTimePressed;
		bool logarithmic;
		
		string sliderBGUrl;
		string sliderFGUrl;
		string sliderHandleUrl;
		ofImage *sliderBG, *sliderFG, *sliderHandle;

		Slider();

		void load();



		void draw();
		
		// round() not supported in vs2010
		float __round(float number);

		bool rangeIncludesZero();
		
		void touchOver(int _x, int _y, int touchId);
		bool touchDown(int _x, int _y, int touchId);

		bool touchMoved(int _x, int _y, int touchId);
		bool touchUp(int _x, int _y, int touchId);
		bool keyPressed(int key);
		bool keyReleased(int key);
		void getParameterInfo(vector<ParameterInfo> &params);
		string valueToString();
		
		void valueFromString(string inp);
		int decimalPlaces;
		
	};
}
