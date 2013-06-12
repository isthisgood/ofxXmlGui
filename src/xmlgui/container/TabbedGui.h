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
		int tabIndex;
		TabbedGui(): SimpleGui() {
			tabIndex = 0;
			tabber = (SegmentedControl*)INSTANTIATE_WITH_ID("segmented", "tabber");
			tabber->name = ""; // we need to mute the name otherwise it'll be drawn
			tabber->pointToValue(&tabIndex);
			//tabber->width = SIMPLE_GUI_WIDTH;
			//tabber->opts = options;
			addChild(tabber);
			fps = (FPSCounter*)INSTANTIATE_WITH_ID("fps", "fps");
			
			addChild(fps);
//			fps->setPosition(ofGetWidth() - fps->width - AUTO_LAYOUT_PADDING, 0);
			fps->setPosition(500, 0);
			addTab("default");
			
		}
		void addTab(string name) {
			bool alreadyAdded = false;
			
			// if we're adding the first tab, we want to just rename
			// the first tab and not re-add it to the container
			if(tabs.size()==1 && tabs[0].first=="default") {
				tabs[0].first = name;
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
	
		static const int guiYOffset = 30;
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
		void ctrlChanged(xmlgui::Event *e) {
			if(e->control->id=="tabber") {
				setTab(tabIndex);
			}
		}
		
		void updateTabber() {
			tabber->options.clear();
			for(int i = 0; i < tabs.size(); i++) {

				tabber->addOption(tabs[i].first);
			}
			tabber->width = tabs.size() * 60;
		}
	};
};

