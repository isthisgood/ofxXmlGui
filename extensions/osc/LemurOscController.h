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
 * Description: Converts Lemur OSC to ofxXmlGui control.
 * 
 *				Your osc messages should be in the format /gui/[tab_name]/[control_name]
 *				Where everything is lowercase and probably underscores too.
 *
 *				The lemur should send normalized values for sliders, panners and other 1-dimensional
 *              things. For multiball you should send the x, y, and z on different channels
 *				e.g.
 *							/gui/default/multiball/x
 *							/gui/default/multiball/y
 *                          /gui/default/multiball/z
 *
 *				 
 *  LemurOscController.h, created by Marek Bereza on 23/12/2013.
 */

#pragma once
#include "ofMain.h"
#include "ofxXmlGui.h"
#include "ofxOsc.h"

class LemurOscController {
public:
	ofxXmlGui *gui;
	void setup(ofxXmlGui &gui) {
		this->gui = &gui;
	}
	
	
	void oscReceived(ofxOscMessage &m);
	
	string oscify(string inp );
	
};
