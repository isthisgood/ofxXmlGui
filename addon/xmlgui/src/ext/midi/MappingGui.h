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
 *  MappingGui.h, created by Marek Bereza on 25/11/2012.
 */

#pragma once
#include "SimpleGui.h"
#include "Map.h"
#include "ofxMidi.h"
namespace xmlgui {
	namespace midi {
		




		class MappingGui: public xmlgui::Listener, public ofxMidiListener {
		public:
			MappingGui();
			virtual ~MappingGui();
			
			xmlgui::SimpleGui g;
			
			void setup(xmlgui::Container *gui);
			void newMidiMessage(ofxMidiEventArgs &e);
			void controlChanged(xmlgui::Event *e);
			void setEnabled(bool enabled) { g.setEnabled(enabled); }
			void toggle() { g.toggle(); }
			bool isEnabled() { return g.isEnabled(); }
			void setControlValue(xmlgui::Control *s, float value);
			void windowResized(ofResizeEventArgs &e);
			void update(ofEventArgs &e);
			void save(string mappingsPath, string settingsPath);
			void load(string mappingsPath, string settingsPath);
			
			void setSettingsDir(string settingsDir) { this->settingsDir = settingsDir; }
			
			
			
		private:
			
			string settingsDir;
			void createMidiMapping(int ccNum, string ctrlId);
			void selectMapping(Map *map);
			void deleteMapping(int index);
			map<int,MidiMap*> midiMappings;
			vector<Map*> allMaps;
			int selectedIndex;
			xmlgui::Container *gui;
			xmlgui::Control *destControl;
			List *mappingList;
			MidiMap dummyMap;
			PushButton *createMappingButton;
			PushButton *saveToMidiNoteButton;
			void refreshMappingList();
			bool creatingMapping;
			bool savingToMidiNote;
		};

	}
}