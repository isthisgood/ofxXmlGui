/**
 *  Mapper.cpp
 *
 *  Created by Marek Bereza on 27/11/2012.
 */

#include "Mapper.h"

ofMutex Mapper::mutex;

void Mapper::setup(xmlgui::Container *targetGui, xmlgui::ofGuiEventDispatcher *events, xmlgui::SimpleGui *sourceGui) {
	this->sourceGui = sourceGui;
	this->targetGui = targetGui;
	this->targetEvents = events;


	//this->targetGui->addListener(this);
	ofAddListener(ofEvents().mousePressed, this, &Mapper::mousePressed);
	learning = false;



	vector<string> sss;
	learningGui.setColumnWidth(250);
	mappingList = learningGui.addList("Mapping", mapping, sss);
	mappingList->height = 400;




	//learningGui.addMeter("input", *dummyMap.input, 0, 1);

	learningGui.addSlider("input min", dummyMap.inMin, 0, 1);

	learningGui.addSlider("input max", dummyMap.inMax, 0, 1);

	learningGui.addSegmented("curve", dummyMap.curve, "LIN|SQR|SQRT");
	learningGui.addSlider("smoothing", dummyMap.smoothing, 0, 1);
	learningGui.addSlider("output min", dummyMap.outMin, 0, 1);
	learningGui.addSlider("output max", dummyMap.outMax, 0, 1);
	learningGui.addPushButton("[x] delete");
//	learningGui.setEnabled(true);
	learningGui.x = ofGetWidth()-260;
	learningGui.addListener(this);

	midiIn.openPort();
midiIn.addListener(this);
}

void Mapper::reconnectMidi() {
    midiIn.removeListener(this);

    midiIn.closePort();

    midiIn.openPort();

}


void Mapper::load(string path) {


	for(int i = 0; i < mappings.size(); i++) {
		delete mappings[i];
		mappings[i] = NULL;
	}
	mappings.clear();
	mappingList->clearItems();


	ofxXmlSettings xml;
	xml.loadFile(path);

	xml.pushTag("mappings");
	int numTags = xml.getNumTags("mapping");
	for(int i = 0; i < numTags; i++) {
		if(xml.getAttribute("mapping", "type", "", i)=="midi") {
		    if(targetGui->getControlById(xml.getAttribute("mapping", "to", "", i))) {
                mappings.push_back(new MidiMap(xml, i, targetGui));
		    }
		} else {
			xmlgui::Control *c = sourceGui->getControlById(xml.getAttribute("mapping", "from", "", i));
			if(c!=NULL) {
			mappings.push_back(new FloatMap(xml, i, sourceGui, targetGui));
			} else {
				ofLogError() << "Error broken mapping " <<  xml.getAttribute("mapping", "from", "", i);
			}
		}
	}

	updateMappingList();
	pointGuiAtMapping(&dummyMap);
}

void Mapper::save(string path) {

	ofxXmlSettings xml;
	xml.addTag("mappings");
	xml.pushTag("mappings");
	for(int i = 0; i < mappings.size(); i++) {
		mappings[i]->save(xml, i);
	}

	xml.saveFile(path);
}



void Mapper::controlChanged(xmlgui::Event *e) {
	if(e->control->id=="[x] delete") {

		if(mapping>=0 && mapping<mappings.size()) {
			delete mappings[mapping];
			mappings.erase(mappings.begin() + mapping);
			mapping = -1;
			pointGuiAtMapping(&dummyMap);
			updateMappingList();
		}
	} else if(e->control->id=="Mapping") {
		if(mapping>=0 && mapping<mappings.size()) {
			pointGuiAtMapping(mappings[mapping]);
		}
	}
}



void Mapper::newMidiMessage(ofxMidiMessage& eventArgs) {
	if(eventArgs.status==MIDI_CONTROL_CHANGE) {

        mutex.lock();
		int ccNum = eventArgs.control;
		int ccVal = eventArgs.value;
		if(learning) { // learn the control
			if(learnStep==LEARN_STEP_SRC) {

				addMapping(new MidiMap(ccNum, destControl));
				showMaps();
				learning = false;
				ok();
			}
		}

		if(MidiMap::midiMappings.find(ccNum)!=MidiMap::midiMappings.end()) {
			for(int i = 0; i < MidiMap::midiMappings[ccNum].size(); i++) {
				MidiMap::midiMappings[ccNum][i]->updateValue(ccVal);
			}
		}
		mutex.unlock();
	}
}


void Mapper::learn() {
targetEvents->setEnabled(true);
	targetEvents->disableInteraction();
	learning = true;
	// show the learning gui
	sourceGui->setEnabled(false);
	learnStep = LEARN_STEP_DST;
}

void Mapper::showMaps() {
	targetEvents->setEnabled(true);
	targetEvents->disableInteraction();
	sourceGui->setEnabled(true);
	sourceGui->enableInteraction();
//	learningGui.setEnabled(true);
}

void Mapper::ok() {
	learning = false;
	targetEvents->setEnabled(true);
	targetEvents->enableInteraction();
	sourceGui->setEnabled(true);
	sourceGui->enableInteraction();

	//learningGui.setEnabled(false);
}


void Mapper::mousePressed(ofMouseEventArgs &e) {
	if(learning) {
		if(learnStep==LEARN_STEP_DST) {
			xmlgui::Control *c = targetGui->hitTest(e.x, e.y);
			if(c!=NULL) {
				destControl = c;
				learnStep = LEARN_STEP_SRC;
				sourceGui->setEnabled(true);

			}
		} else if(learnStep==LEARN_STEP_SRC) {
			xmlgui::Control *c = sourceGui->hitTest(e.x, e.y);
			if(c!=NULL) {
				addMapping(new FloatMap(c, destControl));
				showMaps();
				ok();
				learning = false;
			}
		}
	}
}

void Mapper::update() {

	for(int i = 0; i < mappings.size(); i++) {
		mappings[i]->update();
	}
}

void Mapper::addMapping(Map *m) {
	// add to the list
	mappings.push_back(m);
	mapping = mappings.size()-1;
	updateMappingList();

	pointGuiAtMapping(m);
	ok();

}

void Mapper::pointGuiAtMapping(Map *m) {


//	Meter *meter = (Meter*)learningGui.getControlById("input");
	//meter->pointToValue(m->input);
//	meter->min = m->inRangeMin;
//	meter->max = m->inRangeMax;

	Slider *s = (Slider*)learningGui.getControlById("input min");
	s->pointToValue(&m->inMin);
	s->min = m->inRangeMin;
	s->max = m->inRangeMax;


	s = (Slider*)learningGui.getControlById("input max");
	s->pointToValue(&m->inMax);
	s->min = m->inRangeMin;
	s->max = m->inRangeMax;


	learningGui.getControlById("curve")->pointToValue(&m->curve);

	s = (Slider*)learningGui.getControlById("output min");
	s->pointToValue(&m->outMin);
	s->min = m->outRangeMin;
	s->max = m->outRangeMax;

	s = (Slider*)learningGui.getControlById("output max");
	s->pointToValue(&m->outMax);
	s->min = m->outRangeMin;
	s->max = m->outRangeMax;

	s = (Slider*)learningGui.getControlById("smoothing");
	s->pointToValue(&m->smoothing);

	if(m==&dummyMap) mapping = -1;
}


void Mapper::updateMappingList() {
	mappingList->clearItems();
	for(int i = 0; i < mappings.size(); i++) {
		mappingList->addItem(mappings[i]->getName());
	}
}
void Mapper::draw() {
	if(learning) {
		ofBackgroundHex(0x335577);
		string instruction = "";
		if(learnStep==LEARN_STEP_SRC) {
			instruction = "EITHER ROTATE THE MIDI CONTROL YOU WANT TO USE OR CLICK A SLIDER ON THE LEFT";
		} else if(learnStep==LEARN_STEP_DST) {
			instruction = "CLICK ON A SLIDER FROM THE DAZZLE PARAMETERS";
		}
		ofSetHexColor(0xFFFFFF);
		ofDrawBitmapString(instruction, 20, ofGetHeight() - 20);
	} else {

	}
}
