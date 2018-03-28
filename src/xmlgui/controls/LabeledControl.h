/**
 * LabeledControl.h
 * PdGui
 *
 * Created by Marek Bereza on 06/07/2011.
 *
 */

#pragma once

#include "xmlgui/framework/Control.h"
namespace xmlgui {
	class LabeledControl: public xmlgui::Control {
	public:
        static int DEFAULT_CONTROL_HEIGHT;
		static int lastDrawnFrame;
		LabeledControl();
		static void drawAllLabels();
		static vector<pair<ofVec2f, string> > labels;
		
		void drawLabel(float x = -1, float y = -1);

		void drawCustomLabel(string label, float x, float y);
		void drawLabelEitherSide(string left, string right, float dx = 0, float dy = 0);
		void getParameterInfo(vector<ParameterInfo> &params);
		
		bool drawingLabel;
		int labelColor;
	};

}
