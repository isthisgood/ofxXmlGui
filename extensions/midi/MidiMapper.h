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
 *  MidiMapper.h, created by Marek Bereza on 01/04/2013.
 */

#pragma once
#include "Container.h"
#include "SimpleGui.h"
#include "ofxMidi.h"
#include "Map.h"
#include "MappingGui.h"


namespace xmlgui {

	class MidiMapper: public SimpleGui, public ofxMidiListener {
	public:
		MidiMapper(): SimpleGui() {
			mustDeleteMidi = false;
			midi = NULL;
            rightX = -1;
		}
		
		
		~MidiMapper() {
			if(mustDeleteMidi && midi) {
				delete midi;
			}
		}
		vector<xmlgui::midi::Map*> mappings;

		void setSettingsDir(string settingsDir) {
			mappingGui.setSettingsDir(settingsDir);
		}
		
		/**
		 * Create a midi mapper with default or specified midi port
		 */
		void setup(xmlgui::Container &gui, int midiPort = 0) {
			ofxMidiIn *inp = new ofxMidiIn();
			inp->openPort(midiPort);
			inp->setVerbose(false);
			mustDeleteMidi = true;
			setup(gui, inp);
		}
		
		ofxMidiIn *getMidiInPtr() { return midi; }
		  
		
		/**
		 * If you pass in a nameSearchString, the program will try
		 * to open the first midi device that matches against that name.
		 * If it can't it'll just open the first one.
		 */
		void setup(xmlgui::Container &gui, string nameSearchString) {
			nameSearchString = ofToLower(nameSearchString);
			vector<string> names = getMidiPortNames();
			for(int i = 0; i < names.size(); i++) {
				int f = ofToLower(names[i]).find(nameSearchString);
				if(f>-1) {
					ofLogNotice() << "Found midi port " << nameSearchString;
					setup(gui, i);
					return;
				}
			}
			// if couldn't find favourite midi port.
			if(names.size()>0) {
				ofLogError() << "Can't find midi port '"<<nameSearchString << "'. Opening default ("<<names[0]<<").";
				setup(gui);
			} else {
				ofLogError() << "Can't set up - no midi port available.";
			}
		}
		
		xmlgui::Container *gui;
		/**
		 * Create a midi mapper with an existing midi port.
		 */
		void setup(xmlgui::Container &gui, ofxMidiIn *inp) {
			this->midi = inp;
			this->gui = &gui;
			this->midi->addListener(this);
			this->midi->addListener(&mappingGui);
			ofAddListener(ofEvents().windowResized, this, &MidiMapper::windowResized);
			ofAddListener(ofEvents().update, this, &MidiMapper::update);
			buildGui();
			setEnabled(true);
		}
		int mapping;

		bool showingMidiMapping;
		Control *showButton;
		void triggerResize() {
			ofResizeEventArgs e;
			e.width = ofGetWidth();
			e.height = ofGetHeight();
			windowResized(e);
		}
		
		void windowResized(ofResizeEventArgs &e) {
			showButton->width = 140;
			if (rightX == -1) showButton->position(e.width - showButton->width, 0);
			else showButton->position(rightX - showButton->width, 0);
			mappingGui.windowResized(e);
		}
		void ctrlChanged(xmlgui::Event *e) {
			if(e->control==showButton && e->type==xmlgui::Event::TOUCH_UP) {
				toggleMappingGui();
			}
		}
		
		void update(ofEventArgs &e) {
			mappingGui.update(e);
		}
		void toggleMappingGui() {
			
			mappingGui.toggle();
			if(!mappingGui.isEnabled()) {
				showButton->name = "show midi mappings   v";
			} else {
				showButton->name = "hide midi mappings   x";
			}
		}
		
		
		
		
		
		void buildGui() {
			showingMidiMapping = false;
//			mappingList = addList();
			x = 0;
			y = 0;
			
			setAutoLayout(false);
			showButton = addPushButton("show midi mappings   v");
			
			triggerResize();
			
			mappingGui.setup(gui);
			mappingGui.setEnabled(false);
		}
		
		
		
		
		List *mappingList;
		static void listMidiPorts() {
			ofxMidiIn inp;
			inp.listPorts();
		}
		
		static vector<string> getMidiPortNames() {
			ofxMidiIn inp;
			inp.listPorts();
			return inp.getPortList();
		}
		
		void newMidiMessage(ofxMidiMessage& e) {
		}
        
        void saveMappings(const string& mappingsPath)
        {
            mappingGui.saveMappings(mappingsPath);
        }
        
        void loadMappings(const string& mappingsPath)
        {
            mappingGui.loadMappings(mappingsPath);
        }
        
        void setRightX(float rightX) { this->rightX = rightX; mappingGui.setRightX(rightX); }
        
	private:
        float rightX;
		ofxMidiIn *midi;
		bool mustDeleteMidi;
		xmlgui::midi::MappingGui mappingGui;
	};
}
