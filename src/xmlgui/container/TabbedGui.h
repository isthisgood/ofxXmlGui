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
 *  TabbedGui.h, created by Marek Bereza on 20/03/2013.
 */

#pragma once

#include "xmlgui/container/SimpleGui.h"
#include "xmlgui/controls/FPSCounter.h"
namespace xmlgui {
	class TabbedGui: public SimpleGui {
	public:
		
		vector<pair<string,xmlgui::Container*> > tabs;
		SegmentedControl *tabber;
		FPSCounter *fps;
		PushButton *saveAllButton, *saveThisTabButton, *revertThisTabButton;
		int tabIndex;
		string settingsDirectory;
        
        
		TabbedGui(): SimpleGui() {
            settingsDirectory = "settings";
			//setAutoLayout(false);
			tabIndex = 0;
			tabber = (SegmentedControl*)INSTANTIATE_WITH_ID("segmented", "tabber");
			tabber->name = ""; // we need to mute the name otherwise it'll be drawn
			tabber->pointToValue(&tabIndex);
			//tabber->width = SIMPLE_GUI_WIDTH;
			//tabber->opts = options;
			addChild(tabber);
			fps = (FPSCounter*)INSTANTIATE_WITH_ID("fps", "fps");
			saveAllButton = (PushButton*)INSTANTIATE_WITH_ID("pushbutton", "save all");
			saveThisTabButton = (PushButton*)INSTANTIATE_WITH_ID("pushbutton", "save this");
			revertThisTabButton = (PushButton*)INSTANTIATE_WITH_ID("pushbutton", "revert");
			
			//setPosition(100, 100);
			addChild(saveAllButton);
			addChild(saveThisTabButton);
			addChild(revertThisTabButton);
			addChild(fps);
			fps->setPosition(600, 0);
			fps->height = saveAllButton->height;
			
			saveThisTabButton->width = saveAllButton->width = revertThisTabButton->width = 60;
			
			saveAllButton->x = fps->x + fps->width + AUTO_LAYOUT_PADDING;
			saveThisTabButton->x = saveAllButton->x + saveAllButton->width + AUTO_LAYOUT_PADDING;
			revertThisTabButton->x = saveThisTabButton->x + saveThisTabButton->width + AUTO_LAYOUT_PADDING;
			
			saveAllButton->y = saveThisTabButton->y = revertThisTabButton->y = 0;
			
			
			
//			fps->setPosition(ofGetWidth() - fps->width - AUTO_LAYOUT_PADDING, 0);

			addTab("default");
			x = 10;
			y = 10;

		}
        
        void setSettingsDirectory(string settingsDirectory) {
            this->settingsDirectory = settingsDirectory;
        }
        
        
		void addTab(string name) {
			bool alreadyAdded = false;
			
			// if we're adding the first tab, we want to just rename
			// the first tab and not re-add it to the container
			if(tabs.size()==1 && tabs[0].first=="default") {
				tabs[0].first = name;
				tabs[0].second->name = name;
				tabs[0].second->id = name;
				alreadyAdded = true;
			} else {
				tabs.push_back(make_pair(name, new xmlgui::Container()));
				tabs.back().second->setLayoutType(xmlgui::LayoutType_vertical);
				tabs.back().second->name = name;
				tabs.back().second->id = name;
			}
			
			gui = tabs.back().second;
			gui->y = guiYOffset;
			gui->x = 0;
			if(!alreadyAdded) addChild(gui);
			
			updateTabber();
			setTab(tabs.size()-1);
		}
	
		static const int guiYOffset = 60;
		
		xmlgui::Container *getCurrTab() {
			return tabs[tabIndex].second;
		}
		
		
		void setTab(string name) {
			for(int i = 0; i < tabs.size(); i++) {
				if(tabs[i].first==name) {
					setTab(i);
					return;
				}
			}
			ofLogError() << "setTab(string): Could not find tab called '" << name <<"'";
		}
		
		xmlgui::Container *getTab(string name) {
			
			for(int i = 0; i < tabs.size(); i++) {
				if(tabs[i].first==name) {
					return tabs[i].second;
				}
			}
			return NULL;
		}
		
		// case insensitive version of the above
		xmlgui::Container *getTabi(string name) {
			name = ofToLower(name);
			for(int i = 0; i < tabs.size(); i++) {
				if(ofToLower(tabs[i].first)==name) {
					return tabs[i].second;
				}
			}
			return NULL;
		}
		
		void setTab(int index) {
			tabIndex = index;
			for(int i = 0; i < tabs.size(); i++) {
				if(i==index) {
					tabs[i].second->position(0, guiYOffset);
					tabs[i].second->setActive(true);
					gui = tabs[i].second;
					if(ofGetAppPtr()!=NULL)
                        redoLayout();
				} else {
					//tabs[i].second->position(-1000, 0);
                    tabs[i].second->position(-ofGetWidth()*10, 0);
					tabs[i].second->setActive(false);
				}
			}
		}
		
		void nextTab() {
			if(tabIndex<tabs.size()-1) {
				setTab(tabIndex+1);
			} else {
				setTab(0);
			}
		}
		
		
		void ctrlChanged(xmlgui::Event *e) {
			if(e->control->id=="tabber") {
				setTab(tabIndex);
			} else if(e->control->id=="save all") {
				printf("Saving settings\n");
				saveSettings();
			} else if(e->control->id=="save this") {
				if(!ofFile(settingsDirectory).exists()) {
					ofDirectory().createDirectory(settingsDirectory, true, true);
				}
				tabs[tabIndex].second->saveSettings(settingsDirectory + "/" + tabs[tabIndex].second->name + ".xml");
			} else if(e->control->id=="revert") {
				tabs[tabIndex].second->loadSettings(settingsDirectory + "/" + tabs[tabIndex].second->name + ".xml");
			}
		}
		
		
		
		
		
		
		
		
		void controlChanged(xmlgui::Event *e) {
			this->ctrlChanged(e);
			getCurrTab()->saveSettings();
		}
		
		
		
		
		
		void saveSettings() {
			if(!ofFile(settingsDirectory + "/").exists()) {
				ofDirectory().createDirectory(settingsDirectory, true, true);
			}
			for(int i =0 ; i < tabs.size(); i++) {
				tabs[i].second->saveSettings(settingsDirectory + "/" + tabs[i].second->name + ".xml");
			}
		}
		void loadSettings() {
			for(int i =0 ; i < tabs.size(); i++) {
				tabs[i].second->loadSettings(settingsDirectory + "/" + tabs[i].second->name + ".xml");
			}
		}
		
		void updateTabber() {
			tabber->options.clear();
			for(int i = 0; i < tabs.size(); i++) {

				tabber->addOption(tabs[i].first);
			}
			tabber->width = tabs.size() * 60;
		}
		
		void clear() {
			tabber->options.clear();
			gui->clear();
		}
	};
};

