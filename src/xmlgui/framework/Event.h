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
 *  Event.h, created by Marek Bereza on 16/11/2012.
 */

#pragma once
#include "xmlgui/framework/Control.h"


namespace xmlgui {
	struct Event {
		enum EventType {
			TOUCH_MOVED,
			TOUCH_UP,
			TOUCH_DOWN,
			KEY_PRESSED,
			KEY_RELEASED
		};
		xmlgui::Control *control;
		
		EventType type;
		
		Event(Control *control, EventType type) {
			this->control = control;
			this->type = type;
		}
		void print() {
			printf("%s\n", this->toString().c_str());
		}
		string toString() {
			string s = "Event (";
			if(type==TOUCH_MOVED) {
				s += "TOUCH_MOVED";
			} else if(type==TOUCH_UP) {
				s += "TOUCH_UP";
			} else if(type==TOUCH_DOWN) {
				s += "TOUCH_DOWN";
			}
			s += ") id = '"+control->id+"'\n";
			return  s;
		}
		
	};
}