/**
 *  Map.cpp
 *
 *  Created by Marek Bereza on 27/11/2012.
 */

#include "Map.h"


map<int,vector<xmlgui::midi::MidiMap*> > xmlgui::midi::MidiMap::midiMappings;

void xmlgui::midi::MidiMap::addMidiMap(int cc, xmlgui::midi::MidiMap *midiMap) {
	if(midiMappings.find(cc)==midiMappings.end()) {
		midiMappings[cc] = vector<MidiMap*>();
	}
	midiMappings[cc].push_back(midiMap);
}

void xmlgui::midi::MidiMap::removeMidiMap(xmlgui::midi::MidiMap *midimap) {
	int cc = midimap->cc;
	for(int i = 0; i < midiMappings[cc].size(); i++) {
		if(midiMappings[cc][i]==midimap) {
			midiMappings[cc].erase(midiMappings[cc].begin()+i);
			return;
		}
	}
}