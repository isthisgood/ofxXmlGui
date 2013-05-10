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
 *  Mapper.h, created by Marek Bereza on 27/11/2012.
 */

#pragma once
#include "Map.h"
#include "ofGuiEventDispatcher.h"
#include "ofxMidi.h"

#define LEARN_STEP_SRC 0
#define LEARN_STEP_DST 1
using namespace xmlgui::midi;
class Mapper: public ofxMidiListener, public xmlgui::Listener {
public:

    static ofMutex mutex;
    ~Mapper() {
        midiIn.closePort();

    }

    vector<xmlgui::midi::Map*> mappings;
    void setup(xmlgui::Container *targetGui,
               xmlgui::ofGuiEventDispatcher *events,
               xmlgui::SimpleGui *sourceGui);

    void controlChanged(xmlgui::Event *e);
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void learn();
    void mousePressed(ofMouseEventArgs &e);
    void update();
    void draw();
    void showMaps();

    void load(string path);
    void save(string path);
    ofxMidiIn midiIn;

    xmlgui::SimpleGui learningGui;
    void reconnectMidi();
private:
    void pointGuiAtMapping(xmlgui::midi::Map *m);
    xmlgui::ofGuiEventDispatcher *targetEvents;
    xmlgui::Container *targetGui;
    xmlgui::SimpleGui *sourceGui;
    bool learning;
    int learnStep;

    xmlgui::Control *destControl;

    // dummy values to initialize the gui


    List *mappingList;
    void addMapping(xmlgui::midi::Map *m);
    void ok();
    void updateMappingList();

    // for gui
    int mapping;
    MidiMap dummyMap;
};
