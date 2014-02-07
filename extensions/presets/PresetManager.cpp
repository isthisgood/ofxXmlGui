/**
 *  PresetManager.cpp
 *
 *  Created by Marek Bereza on 18/12/2013.
 */

#include "PresetManager.h"
#include "ScreenTopology.h"


void positionAfter(xmlgui::Control *c, xmlgui::Control *next) {
	next->x = c->x + c->width + 5;
	next->y = c->y;
}

void positionUnder(xmlgui::Control *c, xmlgui::Control *next) {
	next->y = c->y + c->height + 5;
	next->x = c->x;
}
void xmlgui::PresetManager::setup(xmlgui::TabbedGui *gui, string tabName) {
	this->gui = gui->getTab(tabName);
	currPresetId = -1;
	interface.setAutoLayout(false);
	layoutGui();
	
	
	interface.addListener(this);
	showing = false;
	interface.setEnabled(true);
	ofAddListener(ofEvents().draw, this, &PresetManager::draw, interface.getDrawPriority()-1);
	ofAddListener(ofEvents().windowResized, this, &PresetManager::windowResized);
	ofAddListener(ofEvents().keyPressed, this, &PresetManager::keyPressed);




	reloadPresets();

	ofResizeEventArgs a;
	windowResized(a);
	
	this->setShowing(true);
	
    
}
void xmlgui::PresetManager::setVisible(bool visible) {
    interface.setEnabled(visible);
}

bool xmlgui::PresetManager::isInt(const string &inp) {
	for(int i = 0; i < inp.size(); i++) {
		if(inp[i]<'0' || inp[i]>'9') {
			return false;
		}
	}
	return true;
	
}

string xmlgui::PresetManager::oscify(string inp ) {
	inp = ofToLower(inp);
	ofStringReplace(inp, " ", "_");

	return inp;
}

void xmlgui::PresetManager::oscReceived(ofxOscMessage &m) {
	
	
	string address  = m.getAddress();
	if(address.find("/preset")!=0) {
		ofLogError() << "That's not a PresetManager message - address was '"+address+"' - the address should start with /preset/xxx";
		return;
	}
	int preset = m.getArgAsInt32(0);
    loadPreset(preset);
//  string address.substr(string("/preset/").length());
//	if(isInt(preset)) {
//	if(preset) {
//		int presetId = ofToInt(preset);
//		// load preset by number
//		loadPreset(presetId);
//		printf("Tried to load preset number %d\n", presetId);
//	} else {
//		vector<string> parts = ofSplitString(address, "/", true);
//
//		// if there are only 2 parts, that means the format is
//		// /preset/[preset name]
//		if(parts.size()==2) {
//			preset = parts[1];
//			printf("trying to load preset name: '%s'\n", preset.c_str());
//			preset = oscify(preset);
//			for(int i = 0; i < presetList->items.size(); i++) {
//
//				string presetName = extractPresetName(presetList->items[i]);
//				if(oscify(presetName)==preset) {
//					loadPreset(i);
//					return;
//				}
//			}
//			
//			// with 3 parts the format would be
//			// /preset/[look name]/[preset name]
//		} else if(parts.size()==3) {
//			for(int i = 0; i < presetList->items.size(); i++) {
//				
//				// check to see if preset name matches
//				string presetName = extractPresetName(presetList->items[i]);
//				if(oscify(presetName)==parts[2]) {
//					
//					// check to see if look name matches
//					string lookName = extractLookName(presetList->items[i]);
//					if(oscify(lookName)==parts[1]) {
//						loadPreset(i);
//					}
//				}
//				
//			}
//		}
//	}
}

void xmlgui::PresetManager::layoutGui() {
	
	// make a little button for revealing the interface
	guiToggler = interface.addPushButton(" ^ ");
	xmlgui::PushButton *saver = interface.addPushButton(" s ");
	xmlgui::PushButton *revert = interface.addPushButton(" r ");
	
	xmlgui::PushButton *adder = interface.addPushButton(" + ");
	xmlgui::PushButton *deleter = interface.addPushButton(" - ");
	xmlgui::PushButton *prev = interface.addPushButton(" < ");
	xmlgui::PushButton *next = interface.addPushButton(" > ");
	
	currPresetName = "---";
	currPresetTxt = interface.addTextField("Current Preset", currPresetName);
	currPresetTxt->editable = false;
	
	guiToggler->width	=	guiToggler->height	=	MINI_BUTTON_SIZE;
	prev->width			=	prev->height		=	MINI_BUTTON_SIZE;
	next->width			=	next->height		=	MINI_BUTTON_SIZE;
	adder->width		=	adder->height		=	MINI_BUTTON_SIZE;
	deleter->width		=	deleter->height		=	MINI_BUTTON_SIZE;
	saver->width		=	saver->height		=	MINI_BUTTON_SIZE;
	revert->width		=	revert->height		=	MINI_BUTTON_SIZE;
	
	currPresetTxt->x = 0;
	currPresetTxt->y = 0;
	positionAfter(currPresetTxt, prev);
	positionAfter(prev, next);
	positionAfter(next, saver);
	positionAfter(saver, revert);
	positionAfter(revert, adder);
	positionAfter(adder, deleter);
	positionAfter(deleter, guiToggler);
	
	currPresetTxt->y += 2;
	
	
	presetList = interface.addList("Presets", currPresetId, vector<string>());
	presetList->height = 265;
	positionUnder(currPresetTxt, presetList);
	presetList->y += 8;
	presetList->width = guiToggler->x + guiToggler->width;
	presetList->setActive(false);

}



void xmlgui::PresetManager::draw(ofEventArgs &args) {
	if(!interface.isEnabled()) return;
    ofSetColor(0);
	ofRectangle outset = guiRect;
	float PADDING = 5;
	outset.x -= PADDING;
	outset.y -= PADDING*1.5;
	outset.width += PADDING * 2;
	outset.height += PADDING * 3;
	ofRect(outset);
	//interface.draw();
}


void xmlgui::PresetManager::recalculateGuiRect() {
	int m = ofGetWindowMode();
	float w = ofGetWidth();
	float h = ofGetHeight();
	if(m==OF_FULLSCREEN) {
		w = ScreenTopology::getScreenRect(0).width;
		h = ScreenTopology::getScreenRect(0).height;
	}
	float x = w-(guiToggler->x + guiToggler->width + 5);
	float y = h - MINI_BUTTON_SIZE;
	if(showing) {
		y = h - TOTAL_GUI_HEIGHT;
	}
	guiRect.set(x, y, w - x, h - y);
}


void xmlgui::PresetManager::windowResized(ofResizeEventArgs &args) {
	recalculateGuiRect();
	interface.setPosition(guiRect.x, guiRect.y);
}

void xmlgui::PresetManager::createPreset() {
	// create preset
	string presetName = ofSystemTextBoxDialog("Please give your preset a name");
	if(presetName!="") {
		savePreset(presetName);
	}
}

void xmlgui::PresetManager::controlChanged(xmlgui::Event *evt) {
	if(evt->type==xmlgui::Event::TOUCH_UP) {
		if(evt->control==guiToggler) {
		
			toggleShowing();
		} else if(evt->control->id==" + ") {
			createPreset();
		} else if(evt->control->id==" - ") {
			deletePreset(currPresetId);
		} else if(evt->control==presetList) {
			loadPreset(currPresetId);
		} else if(evt->control->id==" > ") {
			currPresetId++;
			if(currPresetId>=presetList->items.size()) {
				currPresetId = 0;
			}
			loadCurrentPreset();
		} else if(evt->control->id==" < ") {
			currPresetId--;
			if(currPresetId<0) {
				currPresetId = presetList->items.size()-1;
			}
			loadCurrentPreset();
		} else if(evt->control->id==" r ") {
			loadCurrentPreset();
		} else if(evt->control->id==" s ") {
			// save current preset.
			if(presetList->items.size()==0 || currPresetId==-1) {
				createPreset();
				return;
			}
			string presetName = presetList->items[currPresetId];
			savePreset(presetName);

		}
	}
}

void xmlgui::PresetManager::loadCurrentPreset() {
	loadPreset(currPresetId);
}

void xmlgui::PresetManager::deletePreset(int presetId) {
	if(!ofFile("presets/trash").exists()) {
		ofDirectory dir;
		dir.createDirectory("presets/trash", true, true);
	}
	string newFileName = presetList->items[presetId] + "-"+ofGetTimestampString() + ".xml";
	
	string oldPath = "presets/"+ presetList->items[presetId] + ".xml";
	string newPath = "presets/trash/" + newFileName;
	
	ofFile f;
	f.moveFromTo(oldPath, newPath);
	
	reloadPresets();
	loadCurrentPreset();
}


void xmlgui::PresetManager::loadPreset(int presetId) {
	if(presetId<0 || presetId>=presetList->items.size()) {
		return;
	}
	currPresetId = presetId;
	string presetString = presetList->items[presetId];
	string presetXml = "presets/" + presetString + ".xml";
	
	currPresetTxt->setTextFieldValue(presetString);

    
    
	gui->loadSettings(presetXml);
}

void xmlgui::PresetManager::savePreset(string name) {
	
	// create the presets folder if it doesn't already exist.
	if(!ofFile("presets").exists()) {
		ofDirectory dir;
		dir.createDirectory("presets", true, true);
	}
	
	
	string presetString = name;
	
	string presetsFile = "presets/"+presetString + ".xml";
	if(ofFile(presetsFile).exists()) {
		if(!ofFile("presets/backup").exists()) {
			ofDirectory dir;
			dir.createDirectory("presets/backup", true, true);
		}
		string presetsBackupFile = "presets/backup/"+presetString + "-"+ofGetTimestampString()+".xml";
		ofFile f;
		f.moveFromTo(presetsFile, presetsBackupFile);
	}
	
	gui->saveSettings(presetsFile);
	tryToReselect = presetString;
	reloadPresets();
}


void xmlgui::PresetManager::reloadPresets() {
	ofDirectory dir;
	dir.allowExt("xml");
	presetList->items.clear();
	int numPresets = dir.listDir("presets");
    
	for(int i = 0; i < numPresets; i++) {
		string fileName = dir.getFile(i).getFileName();
		// lop off the .xml
		string baseName = fileName.substr(0, fileName.size() - 4);
		presetList->items.push_back(baseName);
		if(baseName==tryToReselect) {
			currPresetId = i;
		}
	}
	
	tryToReselect = "";
}

void xmlgui::PresetManager::toggleShowing() {
	setShowing(!showing);
}

void xmlgui::PresetManager::setShowing(bool showing) {
	if(this->showing==showing) return;
	this->showing = showing;

	presetList->setActive(this->showing);
	
	guiToggler->name = showing?" x ":" ^ ";
	ofResizeEventArgs a;
	windowResized(a);

}


void xmlgui::PresetManager::toggle() {
	interface.toggle();
}


bool xmlgui::PresetManager::keyPressed(ofKeyEventArgs &k) {
    if(k.key==OF_KEY_DOWN) {
        

        currPresetId = (currPresetId+1)%presetList->items.size();
        xmlgui::Event e(presetList, xmlgui::Event::TOUCH_UP);
        controlChanged(&e);
        return true;
    } else if(k.key==OF_KEY_UP) {
        
        currPresetId--;
        if(currPresetId<0) currPresetId = presetList->items.size()-1;
        xmlgui::Event e(presetList, xmlgui::Event::TOUCH_UP);
        controlChanged(&e);
        
        return true;
    }
    
    return false;
}

