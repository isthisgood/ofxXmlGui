/**
 *  MappingGui.cpp
 *
 *  Created by Marek Bereza on 25/11/2012.
 */

#include "MappingGui.h"

using namespace xmlgui::midi;
MappingGui::MappingGui() {
	mappingList = NULL;
	creatingMapping = false;
	savingToMidiNote = false;
}

MappingGui::~MappingGui() {
}

void MappingGui::setup(xmlgui::Container *gui) {
	this->gui = gui;
	
	g.addListener(this);
	
	vector<string> opts;
	selectedIndex = -1;
	
	g.setColumnWidth(200);
	createMappingButton = g.addPushButton("Create Mapping");
	createMappingButton->width = 120;
	
	mappingList = g.addList("Mappings", selectedIndex, "");
	mappingList->height = 250;
	
	
	
	
	//learningGui.addMeter("input", *dummyMap.input, 0, 1);
	
	g.addSlider("input min", dummyMap.inMin, 0, 1);
	
	g.addSlider("input max", dummyMap.inMax, 0, 1);
	
	g.addSegmented("curve", dummyMap.curve, "LIN|SQR|SQRT");
	g.addSlider("smoothing", dummyMap.smoothing, 0, 1);
	g.addSlider("output min", dummyMap.outMin, 0, 1);
	g.addSlider("output max", dummyMap.outMax, 0, 1);
	g.addPushButton("[x] delete");
	saveToMidiNoteButton = g.addPushButton("Save to midi note");
	saveToMidiNoteButton->width = 160;
	
	//	learningGui.setEnabled(true);
	g.y = 40;
	g.x = ofGetWidth()-mappingList->width;
	g.addListener(this);
	gui->addListener(this);
	
	load("mappings.xml", "settings.xml");
	
}

void MappingGui::windowResized(ofResizeEventArgs &e) {
	if(mappingList==NULL) return;
	
	g.x = ofGetWidth()-mappingList->width;
}
void MappingGui::update(ofEventArgs &e) {
	for(int i = 0; i < allMaps.size(); i++) {
		allMaps[i]->update();
	}
}
	
int lastDeleteFrame = -1;
void MappingGui::controlChanged(xmlgui::Event *e) {
	if(e->type!=xmlgui::Event::TOUCH_UP) return;
	if(e->control==createMappingButton && !creatingMapping) {
		
		destControl = NULL;
		createMappingButton->name = "CREATING MAPPING..";
		creatingMapping = true;
	} else if(e->control==saveToMidiNoteButton && !savingToMidiNote) {
		saveToMidiNoteButton->name = "PRESS A MIDI NOTE TO SAVE TO!";
		savingToMidiNote = true;
	} else {
		
		if(e->control->parent==&g) {
			if(e->control->id=="Mappings") {
				if(selectedIndex>-1 && selectedIndex<allMaps.size()) {
					selectMapping(allMaps[selectedIndex]);
				}
			} else if(e->control->id=="[x] delete") {

				if(lastDeleteFrame==ofGetFrameNum()) {
					return;

				}
				
				lastDeleteFrame = ofGetFrameNum();
				
				if(selectedIndex>-1 && selectedIndex<allMaps.size()) {
					deleteMapping(selectedIndex);
					selectMapping(&dummyMap);
					refreshMappingList();
					printf("Parent %x\n", e->control->parent->parent);
				}
			}
		} else {
			if(creatingMapping && (e->control->type=="slider" || e->control->type=="panner")) {
				printf("Setting dest control\n");
				destControl = e->control;
			}
		}
	}
}


void MappingGui::deleteMapping(int index) {
	if(index>-1 && index<allMaps.size()) {
		Map *_map = allMaps[index];
		allMaps.erase(allMaps.begin()+index);
		map<int,MidiMap*>::iterator it =  midiMappings.begin();
		for(; it !=midiMappings.end(); it++) {
			if(_map==(*it).second) {
				midiMappings.erase(it);
				break;
			}
			
		}
		printf("deleteing ampping\n");
		delete _map;
	}
}

void MappingGui::createMidiMapping(int ccNum, string id) {
	if(midiMappings.find(ccNum)!=midiMappings.end()) {
		delete midiMappings[ccNum];
	}
	xmlgui::Control *c = gui->getControlById(id);
	if(c!=NULL) {
		midiMappings[ccNum] = new MidiMap(ccNum, c);
		allMaps.push_back(midiMappings[ccNum]);
	} else {
		printf("Couldn't make the midi mapping\n");
	}
	save("mappings.xml", "settings.xml");
	
}

void MappingGui::save(string mappingsPath, string settingsPath) {
	ofxXmlSettings xml;
	xml.addTag("mappings");
	xml.pushTag("mappings");
	for(int i = 0; i < allMaps.size(); i++) {
		allMaps[i]->save(xml, i);
	}
	xml.saveFile(mappingsPath);
	gui->saveSettings(settingsPath);
}

void MappingGui::load(string mappingsPath, string settingsPath) {
	
	for(int i = 0; i < allMaps.size(); i++) {
		delete allMaps[i];
		allMaps[i] = NULL;
	}
	allMaps.clear();
	midiMappings.clear();
	mappingList->clearItems();
	printf("load\n");
	
	ofxXmlSettings xml;
	if(!xml.loadFile(mappingsPath)) {
		// ignore files that don't exist
		printf("Can't load %s\n", mappingsPath.c_str());
		return;
	}
	
	xml.pushTag("mappings");
	int numTags = xml.getNumTags("mapping");
	for(int i = 0; i < numTags; i++) {
		if(xml.getAttribute("mapping", "type", "", i)=="midi") {
			
		    if(gui->getControlById(xml.getAttribute("mapping", "to", "", i))) {
				MidiMap *mm = new MidiMap(xml, i, gui);
                allMaps.push_back(mm);
				midiMappings[mm->cc] = mm;
		    }
		} else {
			//xmlgui::Control *c = sourceGui->getControlById(xml.getAttribute("mapping", "from", "", i));
			//if(c!=NULL) {
			//	mappings.push_back(new FloatMap(xml, i, sourceGui, targetGui));
			//} else {
			//	printf("Error broken mapping: %s\n", xml.getAttribute("mapping", "from", "", i).c_str());
			//}
		}
	}
	printf("Got here\n");
	gui->loadSettings(settingsPath);
	
	refreshMappingList();
	selectMapping(&dummyMap);
	
	
}

void MappingGui::refreshMappingList() {
	mappingList->items.clear();
	for(int i = 0; i < allMaps.size(); i++) {
		mappingList->addItem(allMaps[i]->getName());

	}
	if(selectedIndex>-1 && selectedIndex < allMaps.size()) {
		selectMapping(allMaps[selectedIndex]);
	}
}

void MappingGui::selectMapping(Map *mapping) {
	Slider *s = (Slider*)g.getControlById("input min");
	s->pointToValue(&mapping->inMin);
	s->min = mapping->inRangeMin;
	s->max = mapping->inRangeMax;
	
	s = (Slider*)g.getControlById("input max");
	s->pointToValue(&mapping->inMax);
	s->min = mapping->inRangeMin;
	s->max = mapping->inRangeMax;
	
	
	g.getControlById("curve")->pointToValue(&mapping->curve);
	g.getControlById("smoothing")->pointToValue(&mapping->smoothing);
	
	s = (Slider*)g.getControlById("output min");
	s->pointToValue(&mapping->outMin);
	s->min = mapping->outRangeMin;
	s->max = mapping->outRangeMax;
	
	s = (Slider*)g.getControlById("output max");
	s->pointToValue(&mapping->outMax);
	s->min = mapping->outRangeMin;
	s->max = mapping->outRangeMax;
}


void MappingGui::newMidiMessage(ofxMidiMessage& e) {
	if(e.status==MIDI_CONTROL_CHANGE) {
		int ccNum = e.control;
	
		if(destControl!=NULL) {
			createMidiMapping(ccNum, destControl->id);
			destControl = NULL;
			creatingMapping = false;
			createMappingButton->name = "Create Mapping";
			selectedIndex = allMaps.size()-1;
			refreshMappingList();
		} else {
			// affect the control
			// look it up first
			if(midiMappings.find(ccNum)!=midiMappings.end()) {
				midiMappings[ccNum]->updateValue(e.value);
			}
		}
	} else if(e.status==MIDI_NOTE_ON) {
		
		if(settingsDir!="" && settingsDir[settingsDir.size()-1]!='/') {
			settingsDir += "/";
		}
		
		if(savingToMidiNote) {
			saveToMidiNoteButton->name = "Save to midi note";
			savingToMidiNote = false;
			save(settingsDir+"mapping-"+ofToString(e.bytes[0])+".xml", settingsDir+"settings-"+ofToString(e.bytes[0])+".xml");
		} else {
			load(settingsDir+"mapping-"+ofToString(e.bytes[0])+".xml", settingsDir+"settings-"+ofToString(e.bytes[0])+".xml");
		}
		
	}
}

