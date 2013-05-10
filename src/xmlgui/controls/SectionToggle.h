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

<<<<<<< HEAD:src/xmlgui/controls/SectionToggle.h
#include "xmlgui/framework/Control.h"
=======
#include "LabeledControl.h"
>>>>>>> origin/dev:addon/xmlgui/src/controls/SectionToggle.h


class SectionToggle: public LabeledControl {
public:
	
	
	SectionToggle(): LabeledControl() {
		height = 12;
		width = 110;
		value = new bool[1];
		bval(value) = false;
	}
	
	
	
	void draw() {

		ofSetHexColor(0x333333);
		ofRect(*this);
		
		ofSetHexColor(0xFFFFFF);
		
		string action = "<";
		if(bval(value)) action = "v";
		drawLabel(x+3,y+9);
		drawCustomLabel(action, x+width-6, y + 9);
/*		xmlgui::Resources::bindFont();
		xmlgui::Resources::drawString(name, x+3, y + 9);
		xmlgui::Resources::drawString(action, x+width-6, y + 9);
		xmlgui::Resources::unbindFont();
 */


		
	//	ofLine(x, y+height, x+width, y+height);

	}
	
	bool touchDown(int _x, int _y, int button) {
		bval(value) ^= true;
		return true;
	}
	string valueToString() { return bval(value)?"true":"false"; }
};