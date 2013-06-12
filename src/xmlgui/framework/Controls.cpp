/**
 * Controls.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 20/06/2011.
 *
 */

#include "xmlgui/framework/Instantiator.h"
#include "xmlgui/framework/Gui.h"
#include "xmlgui/controls/Slider.h"
#include "xmlgui/controls/PushButton.h"
#include "xmlgui/controls/Menu.h"
#include "xmlgui/controls/FloatColorPicker.h"
#include "xmlgui/controls/MenuItem.h"
#include "xmlgui/controls/MenuBar.h"
#include "xmlgui/controls/HexColorPicker.h"
#include "xmlgui/controls/Knob.h"
#include "xmlgui/controls/Toggle.h"
#include "xmlgui/controls/Contour.h"
#include "xmlgui/controls/VU.h"
#include "xmlgui/controls/Meter.h"
#include "xmlgui/controls/Keyboard.h"
#include "xmlgui/controls/SliderBank.h"
#include "xmlgui/controls/Panner.h"
#include "xmlgui/controls/SegmentedControl.h"
#include "xmlgui/controls/Title.h"
#include "xmlgui/controls/Slider2D.h"
#include "xmlgui/controls/Drawable.h"
#include "xmlgui/controls/List.h"
#include "xmlgui/controls/DoubleField.h"
#include "xmlgui/controls/TextField.h"
#include "xmlgui/controls/Text.h"
#include "xmlgui/controls/FloatField.h"
#include "xmlgui/controls/FPSCounter.h"
#include "xmlgui/controls/IntField.h"
#include "xmlgui/controls/Grid.h"
#include "xmlgui/controls/FileField.h"
#include "xmlgui/controls/Pane.h"
#include "xmlgui/controls/HorizontalRule.h"
#include "xmlgui/controls/RangeSlider.h"
#include "xmlgui/controls/IntSlider.h"
#include "xmlgui/controls/SectionToggle.h"
#include "xmlgui/controls/Graph.h"

#define REGISTER_CONTROL(A, B) addControlConstructor(ControlInstantiator<A>, B);




void xmlgui::Instantiator::registerControls() {
	REGISTER_CONTROL(Control,		"control");

	REGISTER_CONTROL(Container,		"container");
	REGISTER_CONTROL(Pane,			"pane");
	REGISTER_CONTROL(IntSlider,		"intslider");
	REGISTER_CONTROL(PushButton,	"pushbutton");
	REGISTER_CONTROL(Slider,		"slider");
	REGISTER_CONTROL(SliderBank,	"sliderbank");
	REGISTER_CONTROL(HexColorPicker,	"hexcolorpicker");
	REGISTER_CONTROL(SectionToggle,	"sectiontoggle");
	REGISTER_CONTROL(FloatColorPicker,	"floatcolorpicker");
	REGISTER_CONTROL(Knob,			"knob");
	REGISTER_CONTROL(Toggle,		"toggle");
	REGISTER_CONTROL(Contour,		"contour");
	REGISTER_CONTROL(Keyboard,		"keyboard");
	REGISTER_CONTROL(RangeSlider,	"rangeslider");
	REGISTER_CONTROL(VU,			"vu");
	REGISTER_CONTROL(Meter,			"meter");
	REGISTER_CONTROL(Panner,		"panner");
	REGISTER_CONTROL(SegmentedControl, "segmented");
	REGISTER_CONTROL(Title,				"title");
	REGISTER_CONTROL(HorizontalRule,	"horizontalrule");
	REGISTER_CONTROL(Slider2D,		"slider2d");
	REGISTER_CONTROL(Drawable,		"drawable");
	REGISTER_CONTROL(List,			"list");

    REGISTER_CONTROL(Graph,         "graph");
	REGISTER_CONTROL(DoubleField,	"doublefield");
	REGISTER_CONTROL(TextField,		"textfield");
	REGISTER_CONTROL(Text,			"text");
	REGISTER_CONTROL(FloatField,	"floatfield");
	REGISTER_CONTROL(FPSCounter,	"fps");
	REGISTER_CONTROL(IntField,		"intfield");

	REGISTER_CONTROL(FileField,		"file");

	REGISTER_CONTROL(Grid,			"grid");

	// weird menu stuff for gui editor
	REGISTER_CONTROL(Menu,			"menu");
	REGISTER_CONTROL(MenuBar,		"menubar");
	REGISTER_CONTROL(MenuItem,		"menuitem");
}

