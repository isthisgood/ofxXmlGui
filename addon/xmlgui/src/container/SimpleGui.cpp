/**
 *  SimpleGui.cpp
 *
 *  Created by Marek Bereza on 13/11/2012.
 */

#pragma once

#include "SimpleGui.h"
#include "Container.h"
#include "Instantiator.h"
#include "Slider.h"
#include "Toggle.h"
#include "SegmentedControl.h"
#include "List.h"
#include "PushButton.h"
#include "ofGuiEventDispatcher.h"
#include "Drawable.h"
#include "Panner.h"
#include "IntSlider.h"
#include "Meter.h"
#include "Slider2D.h"
#include "TextField.h"
#include "IntField.h"
#include "HorizontalRule.h"
#include "RangeSlider.h"
#include "FloatField.h"
#include "FloatColorPicker.h"
//#include "SliderBank.h"


//#define SIMPLE_GUI_WIDTH 150


namespace xmlgui {


	SimpleGui::SimpleGui(): xmlgui::Container() {
		gui = this;
		addListener(this);
		
		setLayoutType(xmlgui::LayoutType_vertical);
		x = 10;
		y = 20;
		isSetup = false;
		SIMPLE_GUI_WIDTH = 150;
		autosave = true;
		autoLayout = true;
		type = "simplegui";
		
	}

	void SimpleGui::setEnabled(bool enabled) {
		if(!isSetup) {
			events.setup(this);
			isSetup = true;
			this->enabled = false;
			setEnabled(true);
			setEnabled(false);
			ofAddListener(ofEvents().windowResized, this, &xmlgui::SimpleGui::windowResized);
			redoLayout();

		}
		if(this->enabled!=enabled) {
			events.setEnabled(enabled);
			this->enabled = enabled;
		}
	}

	void SimpleGui::windowResized(ofResizeEventArgs &e) {
		redoLayout();
	}
	
	
	void SimpleGui::setCollapse(bool collapsed) {
		if(!collapsed) {
			//printf("Close\n");
            ofLogVerbose() << "Collapse close";
            while (getNumChildren() > 1)
            {
                collapsedItems.push_back(getChild(1));
				removeChild(getChild(1));
            }
			/*for(int i = 1; i < getNumChildren(); i++) {
				collapsedItems.push_back(getChild(i));
				removeChild(getChild(i));
			}*/
		} else {
			//printf("Open\n");
			ofLogVerbose() << "Collapse open";
            for(int i = 0; i < collapsedItems.size(); i++) {
				addChild(collapsedItems[i]);
			}
			collapsedItems.clear();
		}
	}

	void SimpleGui::controlChanged(xmlgui::Event *e) {
		this->ctrlChanged(e);

		if(settingsFile!="" && autosave) {
			saveSettings();
		}
		if(e->type==xmlgui::Event::TOUCH_DOWN && e->control->id.find("_sectiontoggle")!=-1) {
			
			SimpleGui *s = (SimpleGui*) e->control->parent;
			s->setCollapse(e->control->getBool());
			redoLayout();
			
		}
	}

	Title *SimpleGui::addTitle(string title) {
		Title *t = (Title*) INSTANTIATE_WITH_ID("title", title);
		t->width = SIMPLE_GUI_WIDTH;
		gui->addChild(t);
		return t;
	}

	RangeSlider *SimpleGui::addRangeSlider(string name, float *value, float min, float max) {
		RangeSlider *r = (RangeSlider*)INSTANTIATE_WITH_ID("rangeslider", name);
		r->pointToValue(value);
		r->width = SIMPLE_GUI_WIDTH;
		r->min = min;
		r->max = max;
		gui->addChild(r);
		return r;
	}


	Drawable *SimpleGui::addDrawable(string name, ofBaseDraws &baseDraws) {
		Drawable *drawable = (Drawable*)INSTANTIATE_WITH_ID("drawable", name);
		drawable->drawable = &baseDraws;
		drawable->width = SIMPLE_GUI_WIDTH;
		drawable->height = baseDraws.getHeight()*SIMPLE_GUI_WIDTH/baseDraws.getWidth();
		gui->addChild(drawable);
		return drawable;

	}
	
	HexColorPicker	*SimpleGui::addHexColorPicker(string name, int &value) {
		HexColorPicker *cp = (HexColorPicker*)INSTANTIATE_WITH_ID("hexcolorpicker", name);
		cp->pointToValue(&value);
		cp->width = SIMPLE_GUI_WIDTH;
		
		gui->addChild(cp);
		return cp;
	}
	
	FloatColorPicker	*SimpleGui::addColorPicker(string name, ofFloatColor &value) {
		FloatColorPicker *cp = (FloatColorPicker*)INSTANTIATE_WITH_ID("floatcolorpicker", name);
		cp->pointToValue(&value);
		cp->width = SIMPLE_GUI_WIDTH;
		
		gui->addChild(cp);
		return cp;
	}
	SimpleGui *SimpleGui::addSection(string name) {
		SimpleGui *sg = new SimpleGui();
		sg->name = name;
        
		xmlgui::Control *st = INSTANTIATE_WITH_ID("sectiontoggle", name+"_sectiontoggle");
		st->name = name;
		st->width = SIMPLE_GUI_WIDTH;
		sg->addChild(st);
		
		gui->addChild(sg);
        
		return sg;
	}
	
	IntSlider *SimpleGui::addSlider(string name, int &value, int min, int max) {
		IntSlider *slider = (IntSlider*)INSTANTIATE_WITH_ID("intslider", name);
		slider->pointToValue(&value);
		if(value<min) value = min;
		if(value>max) value = max;
		slider->min = min;
		slider->max = max;
		slider->width = SIMPLE_GUI_WIDTH;
		slider->showValue = true;
		gui->addChild(slider);
		return slider;

	}
	Slider *SimpleGui::addSlider(string name, float &value, float min, float max) {
		Slider *slider = (Slider*)INSTANTIATE_WITH_ID("slider", name);
		if(value<min) value = min;
		if(value>max) value = max;
		
		slider->pointToValue(&value);
		slider->min = min;
		slider->max = max;
		slider->width = SIMPLE_GUI_WIDTH;
		slider->showValue = true;
		gui->addChild(slider);
		return slider;

	}


	Slider2D *SimpleGui::addSlider2D(string name, float *value, float minX, float maxX, float minY, float maxY) {
		Slider2D *s2d = (Slider2D*) INSTANTIATE_WITH_ID("slider2d", name);
		s2d->pointToValue(value);
		s2d->minX = minX;
		s2d->minY = minY;
		s2d->maxX = maxX;
		s2d->maxY = maxY;
		
		s2d->width = SIMPLE_GUI_WIDTH;
		s2d->height = SIMPLE_GUI_WIDTH;
		s2d->showValue = true;
		gui->addChild(s2d);
		return s2d;
		
	}
	
	Slider2D *SimpleGui::addSlider2D(string name, ofVec2f &pos, float minX, float maxX, float minY, float maxY) {
		return addSlider2D(name, &pos.x, minX, maxX, minY, maxY);
	}



	HorizontalRule *SimpleGui::addHR() {
		string dummy = ofToString(ofRandomuf());
		HorizontalRule *r = (HorizontalRule*)INSTANTIATE_WITH_ID("horizontalrule", dummy);
		r->width = SIMPLE_GUI_WIDTH;
		gui->addChild(r);
		return r;
	}

	
	Meter *SimpleGui::addMeter(string name, float &value, float min, float max) {
		Meter *slider = (Meter*)INSTANTIATE_WITH_ID("meter", name);
		slider->pointToValue(&value);
		slider->min = min;
		slider->max = max;
		slider->width = SIMPLE_GUI_WIDTH;
		gui->addChild(slider);
		return slider;

	}

	Panner *SimpleGui::addPanner(string name, float &value, float min, float max) {
		Panner *slider = (Panner*)INSTANTIATE_WITH_ID("panner", name);
		slider->min = min;
		slider->max = max;
		slider->width = SIMPLE_GUI_WIDTH;
		slider->pointToValue(&value);
		slider->showValue = true;
		gui->addChild(slider);
		return slider;
	}

	Toggle *SimpleGui::addToggle(string name, bool &value) {
		Toggle *tog = (Toggle*)INSTANTIATE_WITH_ID("toggle", name);
		tog->pointToValue(&value);
		tog->width = tog->height; // make it square
		gui->addChild(tog);
		return tog;
	}

	PushButton *SimpleGui::addPushButton(string name) {
		PushButton *tog = (PushButton*)INSTANTIATE_WITH_ID("pushbutton", name);
		tog->width = 80;
		tog->height = 20;
		gui->addChild(tog);
		return tog;

	}
	SegmentedControl *SimpleGui::addSegmented(string name, int &value, string options) {
		return addSegmented(name, value, ofSplitString(options, "|"));
	}

	SegmentedControl *SimpleGui::addSegmented(string name, int &value, vector<string> options) {

		SegmentedControl *seg = (SegmentedControl*)INSTANTIATE_WITH_ID("segmented", name);
		seg->pointToValue(&value);
		seg->width = SIMPLE_GUI_WIDTH;
		seg->opts = options;
		gui->addChild(seg);
		return seg;
	}

	List *SimpleGui::addList(string name, int &value, string options) {
		vector<string> opts = ofSplitString(options, "|");
		if(options=="") opts.clear();
		
		return addList(name, value, opts);
	}

	List *SimpleGui::addList(string name, int &value, vector<string> options) {
		List *list = (List*)INSTANTIATE_WITH_ID("list", name);
		list->pointToValue(&value);
		list->items = options;
		list->width = SIMPLE_GUI_WIDTH;
		gui->addChild(list);
		return list;
	}

	IntField *SimpleGui::addIntField(string name, int &value) {
		IntField *field = (IntField*)INSTANTIATE_WITH_ID("intfield", name);
		field->pointToValue(&value);
		field->width = SIMPLE_GUI_WIDTH;
		gui->addChild(field);
		return field;
	}


	FloatField *SimpleGui::addFloatField(string name, float &value) {
		FloatField *field = (FloatField*)INSTANTIATE_WITH_ID("floatfield", name);
		field->pointToValue(&value);
		field->width = SIMPLE_GUI_WIDTH;

		gui->addChild(field);
		return field;
	}

	void SimpleGui::redoLayout() {

		if(!autoLayout) return;
		ofVec2f startingPos(0,0);
		float winHeight = ofGetHeight();
		float guiY = gui->getAbsolutePosition().y;
		ofRectangle r;
		for(int i = 0; i < gui->getNumChildren(); i++) {
			Control *c = gui->getChild(i);
			
			if(c->type=="column") {
				startingPos.y = 0;
				startingPos.x += SIMPLE_GUI_WIDTH+AUTO_LAYOUT_PADDING;
			} else {
				if(c->type=="simplegui") {
					SimpleGui *s = (SimpleGui*)c;
					s->redoLayout();
				}
				c->position(startingPos.x, startingPos.y);
				if(guiY+c->y+c->height>winHeight) {
					startingPos.y = 0;
					startingPos.x += SIMPLE_GUI_WIDTH+AUTO_LAYOUT_PADDING;
					c->position(startingPos.x, startingPos.y);
				}
				
				startingPos.y += c->height + AUTO_LAYOUT_PADDING;
				r.growToInclude(*c);
			}
		}
		width = r.width;
		height = r.height;
	}


	TextField *SimpleGui::addTextField(string name, string &value) {
		TextField *field = (TextField*)INSTANTIATE_WITH_ID("textfield", name);
		field->pointToValue(&value);
		field->width = SIMPLE_GUI_WIDTH;
		gui->addChild(field);
		return field;
	}


	Control			*SimpleGui::addControl(Control *c) {
		c->width = SIMPLE_GUI_WIDTH;
		gui->addChild(c);
	}
	



	void SimpleGui::enableInteraction() {
		events.enableInteraction();
	}


	void SimpleGui::disableInteraction() {
		events.disableInteraction();
	}
	void SimpleGui::addColumn() {
		
		// adds an invisible "column" control (doesn't actually do anything except identify
		// itself as of type "column"
		Control *c = new Control();
		c->type = "column";
		c->id = ofToString(ofRandomuf());
		c->name = id;
		c->set(0, 0, 0, 0);
		gui->addChild(c);
	}
}


