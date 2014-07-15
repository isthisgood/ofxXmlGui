#pragma once
#include "xmlgui/framework/Container.h"
#include "xmlgui/controls/FloatColorPicker.h"
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
#include "xmlgui/controls/Title.h"
#include "xmlgui/controls/HexColorPicker.h"
#include "xmlgui/controls/Graph.h"
#include "xmlgui/controls/Knob.h"
#include "xmlgui/controls/FileField.h"
#include "xmlgui/controls/FloatMapper.h"
#include "xmlgui/controls/Multiball.h"

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

		virtual void controlChanged(xmlgui::Event *e);
		Title 		 	*addTitle(string title);
		RangeSlider		*addRangeSlider(string name, float *value, float min = 0, float max = 1);
		Drawable		*addDrawable(string name, ofBaseDraws &baseDraws);
		IntSlider		*addSlider(string name, int &value, int min = 0, int max = 128, bool logarithmic = false);
		Slider			*addSlider(string name, float &value, float min = 0, float max = 1, bool logarithmic = false);
		Slider2D		*addSlider2D(string name, float *value, float minX=-1, float maxX=1, float minY=-1, float maxY=1);
		Slider2D		*addSlider2D(string name, ofVec2f &pos, float minX=-1, float maxX=1, float minY=-1, float maxY=1);
		HexColorPicker	*addHexColorPicker(string name, int &value);
		FloatColorPicker*addColorPicker(string name, ofFloatColor &value, float scale = 1);
		FloatColorPicker*addColorPicker(string name, ofVec4f &value, float scale = 1);
		HorizontalRule	*addHR();
        Graph           *addGraph(string name, float &value, float min = 0, float max = 1, int updatePeriod = 1);
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
		Knob			*addKnob(string name, float &value, float min = 0, float max = 1);
		Knob			*addAngleKnob(string name, float &value, bool radians = false);
		Control			*addControl(Control *c);
		SimpleGui		*addSection(string name);
		FileField		*addFileField(string name, string &path);
		FloatMapper		*addFloatMapper(string name, float &value, float controlMin = 0, float controlMax = 1);
		Multiball		*addMultiball(string name, xmlgui::MultiballListener *listener);
		void addColumn();
		xmlgui::ofGuiEventDispatcher events;
		
		void loadFromUrl(const string &url);

		void toggle() {
			setEnabled(!enabled);
		}

		int getDrawPriority() {
			return events.getDrawPriority();
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
		void setCollapsed(bool collapsed);
		vector<xmlgui::Control*> collapsedItems;
        void redoLayout();

		void setManualDraw(bool manualDraw) { events.setManualDraw(manualDraw); }
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
