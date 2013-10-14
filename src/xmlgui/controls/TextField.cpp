/**
 *  TextField.cpp
 *
 *  Created by Marek Bereza on 14/10/2013.
 */

#include "TextField.h"


TextField::TextField(): LabeledControl() {
	
	height = 20;
	width = 70;
	value = new string;
	
	if(shiftMap.size()==0) {
		shiftMap[44] = '<';
		shiftMap[45] = '_';
		shiftMap[46] = '>';
		shiftMap[48] = ')';
		shiftMap[49] = '!';
		shiftMap[50] = '@';
		shiftMap[51] = '#';
		shiftMap[52] = '$';
		shiftMap[53] = '%';
		shiftMap[54] = '^';
		shiftMap[55] = '&';
		shiftMap[56] = '*';
		shiftMap[57] = '(';
		shiftMap[61] = '+';
		shiftMap[63] = '/';
		shiftMap[91] = '{';
		shiftMap[92] = '|';
		shiftMap[93] = '}';
		shiftMap[96] = '~';
	}
	

	cursorPosition = 0;
	selectionBegin = 0;
	selectionEnd = 0;
	selecting = false;
	
	
	fontRef = NULL;
	isEnabled = false;
	isEditing = false;

	
	drawCursor = false;

	mouseDownInRect = false;
	
	fontRef = new xmlgui::ofxTextInput::BitmapFontRenderer();

	
	VERTICAL_PADDING = 3;
	HORIZONTAL_PADDING = 3;
	lastTimeCursorMoved = ofGetElapsedTimef();
}