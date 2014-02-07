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
 *  PresetManager.h, created by Marek Bereza on 18/12/2013.
 */

#pragma once
#include "ofMain.h"
#include "xmlgui/container/TabbedGui.h"
#include "xmlgui/framework/Event.h"
#include "ofxOsc.h"

namespace xmlgui {
	class PresetManager: public xmlgui::Listener {
	public:
		
		void setup(xmlgui::TabbedGui *gui, string tabName);

		
		void windowResized(ofResizeEventArgs &args);
		void draw(ofEventArgs &args);
		void toggleShowing();
		void toggle();
		void setShowing(bool showing);
		void setVisible(bool visible);
        
		void controlChanged(xmlgui::Event *evt);
		void savePreset(string presetName);
		void loadPreset(int presetId);
		
		void oscReceived(ofxOscMessage &m);
		bool keyPressed(ofKeyEventArgs &k);
		bool mouseMoved(ofMouseEventArgs &m);
	private:
		xmlgui::Container *gui;
		ofRectangle guiRect;
		void recalculateGuiRect();
		void layoutGui();
		void createPreset();
		void reloadPresets();
		void deletePreset(int presetId);
		void loadCurrentPreset();
		bool isInt(const string &inp);
		string tryToReselect; // set this to the preset name you want to reselect after reloadPresets is called.
		const static int TOTAL_GUI_HEIGHT = 300;
		const static int MINI_BUTTON_SIZE = 24;
		
		bool showing;
		
		xmlgui::SimpleGui interface;
		xmlgui::PushButton *guiToggler;
		xmlgui::TextField *currPresetTxt;
		xmlgui::List *presetList;
		string currPresetName;
		int currPresetId;
		string oscify(string inp );

	};
}