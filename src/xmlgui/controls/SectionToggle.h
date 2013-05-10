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
 *  SectionToggle.h, created by Marek Bereza on 08/04/2013.
 */

#include "xmlgui/framework/Control.h"


class SectionToggle: public xmlgui::Control {
public:
	
	
	SectionToggle(): Control() {
		height = 24;
		width = 110;
		value = new bool[1];
		bval(value) = true;
	}
	
	
	
	void draw() {
		ofSetHexColor(0xFFFFFF);
		
		string action = "<";
		if(bval(value)) action = "v";
		xmlgui::Resources::drawString(name, x+3, y+14);
		xmlgui::Resources::drawString(action, x+width-6, y+14);
		
		ofLine(x, y+height-5, x+width, y+height-5);

	}
	
	bool touchDown(int _x, int _y, int button) {
		bval(value) ^= true;
		return true;
	}
	string valueToString() { return bval(value)?"true":"false"; }
};