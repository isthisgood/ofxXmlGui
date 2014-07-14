/**
 *  SimpleGui.cpp
 *
 *  Created by Marek Bereza on 13/11/2012.
 */


#include "xmlgui/container/SimpleGui.h"
#include "xmlgui/framework/Container.h"
#include "xmlgui/framework/Instantiator.h"
#include "xmlgui/controls/Slider.h"
#include "xmlgui/controls/Toggle.h"
#include "xmlgui/controls/SegmentedControl.h"
#include "xmlgui/controls/List.h"
#include "xmlgui/controls/PushButton.h"
#include "xmlgui/framework/ofGuiEventDispatcher.h"
#include "xmlgui/controls/Drawable.h"
#include "xmlgui/controls/Panner.h"
#include "xmlgui/controls/IntSlider.h"
#include "xmlgui/controls/Meter.h"
#include "xmlgui/controls/Slider2D.h"
#include "xmlgui/controls/TextField.h"
#include "xmlgui/controls/IntField.h"
#include "xmlgui/controls/HorizontalRule.h"
#include "xmlgui/controls/RangeSlider.h"
#include "xmlgui/controls/FloatField.h"
#include "xmlgui/controls/FloatColorPicker.h"
#include "xmlgui/controls/FloatMapper.h"
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


	void SimpleGui::setCollapsed(bool collapse) {

		if(getNumChildren()>0) getChild(0)->setValue(!collapse);
		for(int i = 1; i < getNumChildren(); i++) {
			getChild(i)->setActive(!collapse);
		}
		redoLayout();
	}

	void SimpleGui::controlChanged(xmlgui::Event *e) {
		this->ctrlChanged(e);
		
		if(autosave) {

			if(settingsFile=="") {
				settingsFile = "default.xml";
				ofLogError() << "Error: No xml file name set for gui even though it's set to autosave - giving a name of default.xml";
			}
			
			
			saveSettings();
			
		}
		if(e->type==xmlgui::Event::TOUCH_DOWN && e->control->id.find("_sectiontoggle")!=-1) {

			SimpleGui *s = (SimpleGui*) e->control->parent;
			s->setCollapsed(!e->control->getBool());


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
		st->setValue(true);
		sg->addChild(st);
		

		gui->addChild(sg);

		return sg;
	}

	

		
	FloatMapper *SimpleGui::addFloatMapper(string name, float &value, float controlMin, float controlMax) {
		FloatMapper *fm = (FloatMapper*)INSTANTIATE_WITH_ID("floatmapper", name);
		fm->controlMin = controlMin;
		fm->controlMax = controlMax;
		fm->controlValue = &value;
		fm->width = SIMPLE_GUI_WIDTH;
		gui->addChild(fm);
		
		return fm;
		
	}
	
	IntSlider *SimpleGui::addSlider(string name, int &value, int min, int max, bool logarithmic) {
		IntSlider *slider = (IntSlider*)INSTANTIATE_WITH_ID("intslider", name);
		slider->pointToValue(&value);
		if(value<min) value = min;
		if(value>max) value = max;
		slider->min = min;
		slider->max = max;
		slider->logarithmic = logarithmic;
		slider->width = SIMPLE_GUI_WIDTH;
		slider->showValue = true;
		gui->addChild(slider);
		return slider;

	}

	Slider *SimpleGui::addSlider(string name, float &value, float min, float max, bool logarithmic) {
		Slider *slider = (Slider*)INSTANTIATE_WITH_ID("slider", name);
		if(value<min) value = min;
		if(value>max) value = max;

		slider->pointToValue(&value);
		slider->min = min;
		slider->max = max;
		slider->logarithmic = logarithmic;
		slider->width = SIMPLE_GUI_WIDTH;
		slider->showValue = true;
		gui->addChild(slider);
		return slider;
	}

	Knob *SimpleGui::addKnob(string name, float &value, float min, float max) {
		Knob *k = (Knob*)INSTANTIATE_WITH_ID("knob", name);
		if(value<min) value = min;
		if(value>max) value = max;
		
		k->pointToValue(&value);
		k->min = min;
		k->max = max;
		k->width = SIMPLE_GUI_WIDTH;
		k->height = SIMPLE_GUI_WIDTH;
		//k->showValue = true;
		gui->addChild(k);
		return k;
	}
	
	
	
	
	
	Knob *SimpleGui::addAngleKnob(string name, float &value, bool radians) {
		
		Knob *k = (Knob*)INSTANTIATE_WITH_ID("knob", name);
		float min = 0;
		float max = radians?PI*2:360;
		k->minAngle = 0;
		k->maxAngle = 360;
		k->looping = true;

		if(value<min) value = min;
		if(value>max) value = max;
		
		k->pointToValue(&value);
		k->min = min;
		k->max = max;
		k->width = SIMPLE_GUI_WIDTH;
		k->height = SIMPLE_GUI_WIDTH;
		//k->showValue = true;
		gui->addChild(k);
		return k;
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


    Graph  *SimpleGui::addGraph(string name, float &value, float min, float max, int updatePeriod) {
		Graph *r = (Graph*)INSTANTIATE_WITH_ID("graph", name);
		r->pointToValue(&value);
        r->minValue = min;
        r->maxValue = max;
        r->width = SIMPLE_GUI_WIDTH;
        r->updatePeriod = updatePeriod;
        gui->addChild(r);
        return r;
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
		slider->vertical = false;
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
		if(xmlgui::Resources::getFont()!=NULL) {
			tog->width = xmlgui::Resources::getFont()->stringWidth(name) + 6;
		}
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
			if(!c->active) continue;
			if(c->type=="column") {
				startingPos.y = 0;
				startingPos.x += SIMPLE_GUI_WIDTH+AUTO_LAYOUT_PADDING;
			} else {
				if(c->type=="simplegui") {
					SimpleGui *s = (SimpleGui*)c;
					s->redoLayout();
					if(startingPos.y>0) startingPos.y -= AUTO_LAYOUT_PADDING/2;
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
	
	FileField *SimpleGui::addFileField(string name, string &value) {
		FileField *field = (FileField*)INSTANTIATE_WITH_ID("file", name);
		field->pointToValue(&value);
		field->width = SIMPLE_GUI_WIDTH;
		gui->addChild(field);
		return field;
	}
	
	Multiball		*SimpleGui::addMultiball(string name, xmlgui::MultiballListener *listener) {
		Multiball *multiball = (Multiball*) INSTANTIATE_WITH_ID("multiball", name);
		multiball->listener = listener;
		multiball->width = SIMPLE_GUI_WIDTH;
		gui->addChild(multiball);
		return multiball;
	}


	Control			*SimpleGui::addControl(Control *c) {
		c->width = SIMPLE_GUI_WIDTH;
		gui->addChild(c);
		return c;
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

	void SimpleGui::loadFromUrl(const string &url) {
		ofHttpResponse response = ofLoadURL(url);
		//printf("%s\n", response.data.getText().c_str());
		ofxXmlSettings xml;
		xml.loadFromBuffer(response.data.getText());
		string str;
		xml.copyXmlToString(str);
		//printf("hello: %s\n", str.c_str());
		loadFromXmlObject(xml.doc.RootElement());

	}
	
}


