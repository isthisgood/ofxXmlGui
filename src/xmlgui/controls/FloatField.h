/*
 *  GuiFloatField.h
 *  Gui
 *
 *  Created by Marek Bereza on 11/02/2010.
 *
 */


#pragma once

#include "xmlgui/controls/LabeledControl.h"
class FloatField: public LabeledControl {
public:

	int bgColor;
	int fgColor;

	//either 10000, 1000, 100, 10, 1, 0.1, 0.01 etc
    float incrementUnit;

    // true if a number is being dragged
    bool scrolling;

    // how far left from the control boundary the text is drawn
    int textPadding;

    // how wide each character is (font is usuall fixed width)
    int numberSpacing;

    // this is the original number
    // before scrolling started.
    float originalNumber;

    int originalYOffset;

	FloatField(): LabeledControl() {

		height = 20;
		width = 70;
		value = new float[1];
		fval(value) = 0;
		fgColor = 0xFFFFFF;
		bgColor = 0x323232;
		numberSpacing = xmlgui::Resources::getFontCharWidth();
        textPadding = 3;
	}

		// round() not supported in vs2010
	float __round(float number) {
		return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
	}

	void draw() {

		setRGBA(bgColor);
		ofRect(x, y, width, height);
		setRGBA(fgColor);
		ofNoFill();
		ofRect(x, y, width, height);
		ofFill();

		if(focus && ((int)__round(ofGetElapsedTimef()*2.5))%2==0) {
			xmlgui::Resources::drawString(getNumString() + "|", x+textPadding, y+14);
		} else {
			xmlgui::Resources::drawString(getNumString(), x+textPadding, y+14);
		}
		drawLabel(x, y-3);


	}

	string getNumString() {

	    string u = " ";
	    if(fval(value)<0) u = "";
		return u+ofToString(fval(value),3);
	}
    int getPowerOf10() {
        /*0 = 1
        -1 = 0.1
        -2 = 0.01
        -3 = 0.001
        -4 = 0.0001
        */
        for(int i = -5; i < 10; i++) {
            if(fval(value)>=pow(10, i) && fval(value)<=pow(10, i+1)) {
                return i;
            }

        }
        return 0;
    }
    float getUnit(int pos) {
        int p = 3+getPowerOf10() - pos;
        if(p<-1) p+= 1;
        return pow(10, p);
    }

	bool keyPressed(int key) {
		if(key>='0' && key<='9') { // if it's a printable char
			fval(value) *= 10;
			fval(value) += key - '0';

			return true;
		} else if(key==8 || key==127) { // delete
			fval(value) /= 10;
			if(ABS(fval(value))<0.01) fval(value) = 0;
			return true;
		} else if(key=='-') {
            if(fval(value)>0) fval(value) *= -1;
		} else if(key=='+') {
            if(fval(value)<0) fval(value) *= -1;
		} else if(key==10 || key==13) {
			// this is a return
			//parent->setKeyboardFocus(NULL);
			return true;
		}
		return false;
	}

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

	void getParameterInfo(vector<ParameterInfo> &params) {
		LabeledControl::getParameterInfo(params);
		params.push_back(ParameterInfo("Value","value", "floatfield", value));
		params.push_back(ParameterInfo("BG Color", "bgColor", "colourpicker", &bgColor));
		params.push_back(ParameterInfo("FG Color", "fgColor", "colourpicker", &fgColor));
	}

	string valueToString() { return ofToString(fval(value)); }
	void valueFromString(string inp) { fval(value) = atof(inp.c_str()); }
};
