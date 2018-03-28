//
//  NumberBox.cpp
//  MegaMapper
//
//  Created by Marek on 2/18/16.
//
//

#include "NumberBox.h"
xmlgui::ofxTextInput::FontRenderer* xmlgui::NumberBox::fontRef = NULL;
xmlgui::NumberBox::NumberBox(): LabeledControl() {
	
	
    height = LabeledControl::DEFAULT_CONTROL_HEIGHT;
	width = 100;
	
	
	dragging = false;
	value = new float[1];
	fval(value) = 0;
	
	
	bgColor = 0x505050;
	fgColor = 0x960000;
	borderColor = 0xFFFFFF;


	if(fontRef==NULL) {
		ofTrueTypeFont *f = xmlgui::Resources::getFont();
		if(f!=NULL) {
			fontRef = new xmlgui::ofxTextInput::TypedFontRenderer(f);
		} else {
			fontRef = new xmlgui::ofxTextInput::BitmapFontRenderer();
		}
	}
}
