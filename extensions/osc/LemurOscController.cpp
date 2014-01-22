/**
 *  LemurOscController.cpp
 *
 *  Created by Marek Bereza on 23/12/2013.
 */

#include "LemurOscController.h"





class MultiballState: public ofVec3f {
public:
	
	int state;

	const static int NONE = 0;
	const static int TOUCH_DOWN = 1;
	const static int TOUCH_MOVED = 2;
	const static int TOUCH_UP = 3;

	ofVec3f nextPos;
	
	MultiballState() {
		state = NONE;
	}
	
	int update() {
		if(nextPos.z==0 && z>0) {
			// touch up
			state = TOUCH_UP;
		} else if(nextPos.z>0 && z==0) {
			// touch down
			state = TOUCH_DOWN;
		} else if(nextPos.z>0 && z>0) {
			// touch moved
			state = TOUCH_MOVED;
		} else {
			state = NONE;
		}
		
		set(nextPos);
		return state;
	}
};

vector<MultiballState> multiballStates;


string LemurOscController::oscify(string inp ) {
	inp = ofToLower(inp);
	ofStringReplace(inp, " ", "_");
	
	return inp;
}



void LemurOscController::oscReceived(ofxOscMessage &m) {
	string addr = m.getAddress();
	vector<string> parts = ofSplitString(addr, "/", true);
	if(parts[0]!="gui") {
		ofLogError() << "Lemur OSC Controller received a message that was not meant for it (address '"<<addr<<"')";
		return;
	}
	
	// could potentially reset the settings here.
	
	
	xmlgui::Container *c = gui->getTabi(parts[1]);

	
	
	if(c==NULL) {
		ofLogError() << "Lemur OSC Controller received a message for a look called '"<<parts[1]<<"' which doesn't exist or doesn't have a gui";
		return;
	}
	
	string ctrlId = parts[2];
	
	ofStringReplace(ctrlId,"_", " ");
	
	bool doTouchUpAswell = false;
	
	xmlgui::Control *ctrl = c->getControlByIdi(ctrlId);
	if(ctrl==NULL) {
		ofLogError() << "Lemur OSC Controller got a value for a non-existant control ('" << addr << "')";
		return;
	}
	if(ctrl->type=="slider" || ctrl->type=="panner") {
		xmlgui::Slider *s = (xmlgui::Slider*) ctrl;
		
		s->setValue(ofMap(m.getArgAsFloat(0), 0, 1, s->min, s->max, true));
	} else if(ctrl->type=="intslider") {
		xmlgui::IntSlider *is = (xmlgui::IntSlider*) ctrl;

		is->setValue((int)ofMap(m.getArgAsFloat(0), 0, 1, is->min, is->max, true));
	} else if(ctrl->type=="knob") {
		xmlgui::Knob *k = (xmlgui::Knob*) ctrl;
		k->setValue(ofMap(m.getArgAsFloat(0), 0, 1, k->min, k->max, true));
	} else if(ctrl->type=="segmented") {
	
		xmlgui::SegmentedControl *seg = (xmlgui::SegmentedControl*) ctrl;
		printf("Segmented size: %d, num args: %d\n", seg->opts.size(), m.getNumArgs());
		for(int i = 0; i < seg->opts.size() && i < m.getNumArgs(); i++) {
			printf("%d\n", i);
			if(m.getArgAsFloat(i)>0) {
				printf("Setting value To %d\n", i);
				seg->setValue(i);
				
				break;
			} else {
				printf("[%d] = %f <= 0\n", i, m.getArgAsFloat(i));
			}
		}
		for(int i= 0; i < m.getNumArgs(); i++) {
			printf("Val: %d:  - %f\n", i, m.getArgAsFloat(i));
		}
		
	} else if(ctrl->type=="list") {
		
		// untested as of 23/12/13
		xmlgui::List *list = (xmlgui::List*) ctrl;
		for(int i = 0; i < list->items.size() && i < m.getNumArgs(); i++) {
			if(m.getArgAsFloat(i)>0) {
				list->setValue(i);
				break;
			}
		}
	} else if(ctrl->type=="multiball") {
		
		
		
		string t = parts[3]; // this will be x, y, or z
		
		xmlgui::Multiball *mult = (xmlgui::Multiball*) ctrl;

		if(multiballStates.size()<m.getNumArgs()) {
			multiballStates.resize(m.getNumArgs());
		}
		
		for(int i = 0; i < m.getNumArgs(); i++) {
			bool mustUpdate = false;
			if(t=="x") {
				multiballStates[i].nextPos.x = m.getArgAsFloat(i);
			} else if(t=="y") {
				multiballStates[i].nextPos.y = 1-m.getArgAsFloat(i);
				if(multiballStates[i].nextPos.z==1) mustUpdate = true;
			} else if(t=="z") {
				multiballStates[i].nextPos.z = m.getArgAsFloat(i);
				mustUpdate = true;
			}

			if(mustUpdate) {
				int state = multiballStates[i].update();
				if(state==MultiballState::TOUCH_DOWN) {
					mult->sendTouchDown(multiballStates[i], i);
				} else if(state==MultiballState::TOUCH_MOVED) {
					mult->sendTouchMoved(multiballStates[i], i);
				} else if(state==MultiballState::TOUCH_UP) {
					mult->sendTouchUp(multiballStates[i], i);
				}
			}
		}
		
	} else if(ctrl->type=="toggle") {
		xmlgui::Toggle *tog = (xmlgui::Toggle*) ctrl;
		tog->setValue((bool)m.getArgAsFloat(0)>0);
	} else if(ctrl->type=="button") {
		doTouchUpAswell = true;
	} else {
		ofLogError() << "unhandled " << addr;
	}
	
	// notify the gui that the stuff is changed incase
	// it needs to fire any gui listeners. This is mostly
	//important for buttons
	ofxXmlGuiEvent e(ctrl, ofxXmlGuiEvent::TOUCH_DOWN);
	ctrl->parent->notifyChange(&e);

	// some controls respond to touch up events
	// as well, mostly buttons, which will usually
	// listen to either touch up or down.
	if(doTouchUpAswell) {
		e.type = ofxXmlGuiEvent::TOUCH_UP;
		ctrl->parent->notifyChange(&e);
	}

}