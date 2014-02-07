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
    ofxOscReceiver *osc;
    LemurOscController() {
        osc = NULL;
        gui = NULL;
    }
    
    
    // if you want to use the internal osc receiver, specify a port
    // else, you need to pass the messages in yourself.
	void setup(ofxXmlGui &gui, int port=-1) {
		this->gui = &gui;
        if(port!=-1) {
            osc = new ofxOscReceiver();
            osc->setup(port);
        }
	}
    
	
    void update() {
        if(osc==NULL) return;
        ofxOscMessage m;
        while(osc->getNextMessage(&m)) {
            oscReceived(m);
        }
    }
    
	
	void oscReceived(ofxOscMessage &m);
	
	string oscify(string inp );
	
};
