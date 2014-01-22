/**     ___           ___           ___                         ___           ___     
 *     /__/\         /  /\         /  /\         _____         /  /\         /__/|    
 *    |  |::\       /  /::\       /  /::|       /  /::\       /  /::\       |  |:|    
 *    |  |:|:\     /  /:/\:\     /  /:/:|      /  /:/\:\     /  /:/\:\      |  |:|    
 *  __|__|:|\:\   /  /:/~/::\   /  /:/|:|__   /  /:/~/::\   /  /:/  \:\   __|__|:|    
 * /__/::::| \:\ /__/:/ /:/\:\ /__/:/ |:| /\ /__/:/ /:/\:| /__/:/ \__\:\ /__/::::\____
 * \  \:\~~\__\/ \  \:\/:/__\/ \__\/  |:|/:/ \  \:\/:/~/:/ \  \:\ /  /:/    ~\~~\::::/
 *  \  \:\        \  \::/          |  |:/:/   \  \::/ /:/   \  \:\  /:/      |~~|:|~~ 
 *   \  \:\        \  \:\          |  |::/     \  \:\/:/     \  \:\/:/       |  |:|   
 *    \  \:\        \  \:\         |  |:/       \  \::/       \  \::/        |  |:|   
 *     \__\/         \__\/         |__|/         \__\/         \__\/         |__|/   
 *
 *  Description: 
 *				 
 *  FloatMapper.h, created by Marek Bereza on 27/12/2013.
 */

#pragma once

#include "xmlgui/controls/LabeledControl.h"
#include "xmlgui/controls/RangeSlider.h"
#include "xmlgui/framework/Listener.h"

namespace xmlgui {

	class FloatMapper: public LabeledControl, public xmlgui::Listener {
	public:
		float *controlValue;
		float controlMin;
		float controlMax;
		xmlgui::RangeSlider inRange;
		xmlgui::RangeSlider outRange;
		
		float outputRange[2];
		float inputRange[2];
		
		ofRectangle mapButton;
		ofRectangle controlPreview;
		
		xmlgui::Control *target;
		bool currentlyMapping;
		
		FloatMapper();
        void reset();
		void draw();
		void settingsLoaded();
		
		void controlChanged(xmlgui::Event *evt);
		
		bool touchDown(int _x, int _y, int button);
		
		
		bool touchMoved(int _x, int _y, int button);
		
		void updateValue();
		void stopMapping();
		
		void startMapping();
		static bool updateHookEnabled;
		bool update(ofEventArgs &args);
		string valueToString() { return sval(value); }
		void valueFromString(string inp) { sval(value) = inp; }
		static vector<FloatMapper*> mappers;
		void setVal();
	};
}