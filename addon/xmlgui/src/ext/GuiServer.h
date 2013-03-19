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
 *  GuiServer.h, created by Marek Bereza on 14/11/2012.
 */

#pragma once
#include "Container.h"
#include "ofxWebServer.h"
#include "ofxOsc.h"

namespace xmlgui {



    // basically, if you want to use the guiserver's osc client for any
    // other osc receiving ontop of the gui, implement this, and it will
    // be sent any messages that aren't to do with the gui.
    class GuiServerExtraListener {
    public:

        virtual void guiServerExtraMessage(ofxOscMessage &m) = 0;
    };



    class GuiServer: public ofxWSRequestHandler {

	public:

		GuiServer();
		~GuiServer();

		void setup();
		void update();
		void addGui(xmlgui::Container *gui);
		void httpGet(string url);
		void httpPost(string url, char *data, int dataLength);

		void setExtraListener(xmlgui::GuiServerExtraListener *extraListener);


	private:

        GuiServerExtraListener *extraListener;
		ofxWebServer ws;
		ofxOscReceiver osc;
		vector<xmlgui::Container*> guis;
		bool started;

		void smoothChange(xmlgui::Control *c, float value);
		map<xmlgui::Control*, float> smoothControls;
		float getControlRange(xmlgui::Control *c);
	};
}
