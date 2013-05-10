#pragma once
#include "Container.h"
#include "FloatColorPicker.h"
#include "Instantiator.h"
#include "Slider.h"
#include "Toggle.h"
#include "SegmentedControl.h"
#include "List.h"
#include "PushButton.h"
#include "ofGuiEventDispatcher.h"
#include "controls/Drawable.h"
#include "Panner.h"
#include "IntSlider.h"
#include "Meter.h"
#include "Slider2D.h"
#include "TextField.h"
#include "IntField.h"
#include "HorizontalRule.h"
#include "RangeSlider.h"
#include "FloatField.h"
#include "Title.h"
#include "HexColorPicker.h"


//#include "SliderBank.h"


//#define SIMPLE_GUI_WIDTH 150

namespace xmlgui {

	
	class SimpleGui: public xmlgui::Container, public xmlgui::Listener {
	public:

		SimpleGui();



		void setEnabled(bool enabled);
		// implement this method if you want to receive
		// control changed stuff in your superclass as
		// you can't implement an xmlgui::Listener
		virtual void ctrlChanged(xmlgui::Event *e) {

		}
		
		void controlChanged(xmlgui::Event *e);
		Title 		 	*addTitle(string title);
		RangeSlider		*addRangeSlider(string name, float *value, float min = 0, float max = 1);
		Drawable		*addDrawable(string name, ofBaseDraws &baseDraws);
		IntSlider		*addSlider(string name, int &value, int min = 0, int max = 128);
		Slider			*addSlider(string name, float &value, float min = 0, float max = 1);
		Slider2D		*addSlider2D(string name, float *value, float minX=-1, float maxX=1, float minY=-1, float maxY=1);
		Slider2D		*addSlider2D(string name, ofVec2f &pos, float minX=-1, float maxX=1, float minY=-1, float maxY=1);
		HexColorPicker	*addHexColorPicker(string name, int &value);
		FloatColorPicker*addColorPicker(string name, ofFloatColor &value);
		HorizontalRule	*addHR();

		Meter			*addMeter(string name, float &value, float min = 0, float max = 1);
		Panner			*addPanner(string name, float &value, float min=-1, float max=1);
		Toggle			*addToggle(string name, bool &value);
		PushButton		*addPushButton(string name);
		SegmentedControl *addSegmented(string name, int &value, string options);
		SegmentedControl *addSegmented(string name, int &value, vector<string> options);
		List			*addList(string name, int &value, vector<string> options);
		List			*addList(string name, int &value, string opts);
		IntField		*addIntField(string name, int &value);
		FloatField		*addFloatField(string name, float &value);
		TextField		*addTextField(string name, string &value);
		Control			*addControl(Control *c);
		SimpleGui			*addSection(string name);
		

		void addColumn();
		xmlgui::ofGuiEventDispatcher events;



		void toggle() {
			setEnabled(!enabled);
		}

		void setColumnWidth(float width) {
			setWidth(width);
		}

		// this must be called before adding controls!
		void setWidth(float width) {
			SIMPLE_GUI_WIDTH = width;
		}
		float getColumnWidth() {
			return SIMPLE_GUI_WIDTH;
		}
		bool isEnabled() { return enabled; }

		void enableInteraction();
		void disableInteraction();
		void setAutoSave(bool autosave) { this->autosave = autosave; }
		
		void windowResized(ofResizeEventArgs &e);
		void setAutoLayout(bool autoLayout) { this->autoLayout = autoLayout; }
		void setCollapse(bool collapsed);
		vector<xmlgui::Control*> collapsedItems;
        void redoLayout();
		
    protected:
		xmlgui::Container *gui;
		
	private:
		
		bool autoLayout;
		bool autosave;
		bool enabled;
		bool isSetup;
		float SIMPLE_GUI_WIDTH;
	};
};
