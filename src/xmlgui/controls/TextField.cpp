/**
 *  TextField.cpp
 *
 *  Created by Marek Bereza on 14/10/2013.
 */

#include "TextField.h"
#include "xmlgui/framework/Container.h"

map<int, char> TextField::shiftMap;


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

void TextField::beginEditing() {
    if(!isEditing){
        isEditing = true;
        drawCursor = true;
    }
}

void TextField::endEditing() {
    if(isEditing){
        isEditing = false;
        drawCursor = false;
    }
}

string TextField::getTextFieldValue() {
	if(displayString=="") {
		displayString = valueToString();
	}
	
	return displayString;
}

void TextField::setTextFieldValue(string v) {
	displayString = v;
	valueFromString(v);
}


string displayString;



void TextField::getCursorCoords(int pos, int &cursorX) {

	string text = getTextFieldValue();
	
	cursorX = pos;
	
}


void TextField::draw() {
	if(!focus) {
		selecting = false;
		drawCursor = false;
	}
	ofSetColor(30);
	ofRect(*this);
	
	ofNoFill();
	ofSetColor(focus?255:200);
	ofRect(*this);
	ofFill();
	
	ofPushMatrix();
	ofTranslate(x, y);
	
	string text = getTextFieldValue();
	
	if(selecting) {
		
		
		ofPushStyle();
		// argh, splitting all the time.
		
		int beginCursorX;
		int endCursorX;
		getCursorCoords(selectionBegin, beginCursorX);
		getCursorCoords(selectionEnd, endCursorX);
		
		float startX = fontRef->stringWidth(text.substr(0,beginCursorX));
		float endX = fontRef->stringWidth(text.substr(0, endCursorX));
		
		
		ofSetHexColor(0x6988db);
		ofFill();
		

		// single line selection
		ofRect(HORIZONTAL_PADDING + startX, VERTICAL_PADDING,
				   endX - startX, fontRef->getLineHeight());

		ofPopStyle();
		
		
		//draw cursor line
    } else if(drawCursor) {
        ofPushStyle();
		// cursor should only blink when its been idle, and animation
		// should be a clipped sine wave
        float timeFrac = 0.5 * ofClamp(cos(6.0f * (ofGetElapsedTimef()-lastTimeCursorMoved))*4, -1, 1) + 0.5;
        
        ofColor col = ofGetStyle().color;
		ofSetColor(col.r * timeFrac, col.g * timeFrac, col.b * timeFrac);
		
		
		
		
		// calculate this every loop.
		int cursorX;
        getCursorCoords(cursorPosition, cursorX);
		printf("CursorX: %d\n", cursorX);
		//	printf("Pos: %d    X: %d   Y: %d\n", cursorPosition, cursorX, cursorY);
		int cursorPos = HORIZONTAL_PADDING + fontRef->stringWidth(text.substr(0,cursorX));
        
        
		int cursorTop = VERTICAL_PADDING;
		int cursorBottom = cursorTop + fontRef->getLineHeight();
		
		
		
		
		ofSetLineWidth(1.0f);
		//TODO: multiline with fontRef
        ofLine(cursorPos, cursorTop,
			   cursorPos, cursorBottom);
        ofPopStyle();
    }
	
	fontRef->drawString(text, HORIZONTAL_PADDING, fontRef->getLineHeight()+VERTICAL_PADDING);
	

	
	ofPopMatrix();
	drawLabel();
}




int TextField::getCursorPositionFromMouse(int x) {
	int cursorX = 0;
	float pos = 0;


	
	string text = getTextFieldValue();
	
	cursorX = fontRef->getPosition(text, x - (HORIZONTAL_PADDING + this->x));
	return cursorX;
}



bool TextField::touchDown(int x, int y, int id){
	mouseDownInRect = inside(x, y);
	if(mouseDownInRect) {
		cursorPosition = getCursorPositionFromMouse(x);
		lastTimeCursorMoved = ofGetElapsedTimef();
		selecting = false;
		parent->setKeyboardFocus(this);
		return true;
	}
}


bool TextField::touchMoved(int x, int y, int id) {
	if(inside(x, y)) {
		int pos = getCursorPositionFromMouse(x);
		if(pos!=cursorPosition) {
			selecting = true;
			selectionBegin = MIN(pos, cursorPosition);
			selectionEnd = MAX(pos, cursorPosition);
			
		} else {
			selecting = false;
		}
	}
}

bool TextField::touchUp(int x, int y, int id){
	
    if(inside(x, y)) {
        if(!isEditing && mouseDownInRect){
	        beginEditing();
        }
    }
    else if(isEditing){
		endEditing();
	}
}


bool TextField::isKeyAllowed(int key) {
	return true;
}

bool TextField::keyPressed(int key) {

	if(key==330) key = '.';
	if(key>=320 && key <=329) {
		key = '0' + 320 - key;
	}
	//ew: add charachter (non unicode sorry!)
	//jg: made a step closer to this with swappable renderers and ofxFTGL -- but need unicode text input...
	lastTimeCursorMoved = ofGetElapsedTimef();
	
	string text = getTextFieldValue();
    if(key == OF_KEY_SHIFT) {
        isShifted = true;
    }
    
    if(key == 4352) {
        isCommand = true;
    }
	
#ifdef USE_GLFW_CLIPBOARD
    if(key == 'c' && isCommand ) {
        setClipboard(text.substr(selectionBegin, selectionEnd - selectionBegin));
		setTextFieldValue(text);
        return;
    }
	
    if(key == 'v' && isCommand ) {
        text.insert(cursorPosition, getClipboard());
		setTextFieldValue(text);
        return;
    }
#endif
    
	if ((key >=32 && key <=126) || key=='\t' || key==OF_KEY_RETURN) {
		if(selecting) {
			text.erase(text.begin() + selectionBegin,
					   text.begin() + selectionEnd
					   );
			cursorPosition = selectionBegin;
			selecting = false;
		}
	}
	
	
	if (key == OF_KEY_RETURN) {
		endEditing();
		setTextFieldValue(text);
		return;
	}
	
	if ((key >=32 && key <=126) || key=='\t') {
        
        if(isShifted) {
            
            char toInsert;
            if( !(key > 96 && key < 123) && !(key > 65 && key < 90) && shiftMap.find(key) != shiftMap.end() ) {
                toInsert = shiftMap[key];//toInsert = key - 32;
            } else {
                toInsert = key;
            }
            
            text.insert(text.begin()+cursorPosition, toInsert);
        } else {
            text.insert(text.begin()+cursorPosition, key);
        }
		cursorPosition++;
	}
	
	
	if (key==OF_KEY_BACKSPACE) {
		if(selecting) {
			text.erase(text.begin() + selectionBegin,
					   text.begin() + selectionEnd
					   );
			cursorPosition = selectionBegin;
			selecting = false;
		} else {
			if (cursorPosition>0) {
				text.erase(text.begin()+cursorPosition-1);
				--cursorPosition;
			}
		}
	}
	
	if (key==OF_KEY_DEL) {
		if(selecting) {
			text.erase(text.begin() + selectionBegin,
					   text.begin() + selectionEnd
					   );
			cursorPosition = selectionBegin;
			selecting = false;
		} else {
			if (text.size() > cursorPosition) {
				text.erase(text.begin()+cursorPosition);
			}
		}
	}
	
	if (key==OF_KEY_LEFT){
		if(selecting) {
			cursorPosition = selectionBegin;
			selecting = false;
			
		} else {
			if (cursorPosition>0){
				--cursorPosition;
			}
		}
	}
	
	
	
	if (key==OF_KEY_RIGHT){
		if(selecting) {
			cursorPosition = selectionEnd;
			selecting = false;
		} else {
			if (cursorPosition<text.size()){
				++cursorPosition;
			}
		}
	}
	
	
	if (key==OF_KEY_UP){
		if(selecting) {
			cursorPosition = selectionBegin;
			selecting = false;
			
		} else {
			
			cursorPosition = 0;
				
		}
	}
	
	
	
	if (key==OF_KEY_DOWN){
		if(selecting) {
			cursorPosition = selectionEnd;
			selecting = false;
		} else {
			int xx;
			getCursorCoords(cursorPosition, xx);

				
			cursorPosition = xx;
			
		}
	}
	
	setTextFieldValue(text);
	return true;
}

bool TextField::keyReleased(int key)
{
    
    if(key == 4352) {
        isCommand = false;
    }
	
    if(key == OF_KEY_SHIFT) {
        isShifted = false;
    }
	return true;
}