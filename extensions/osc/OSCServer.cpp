/**
 *  OSCServer.cpp
 **
 *  Created by Marek Bereza on 14/11/2012.
 */

#include "OSCServer.h"
#include "ofxXmlSettings.h"
#include "Slider.h"
#include "Panner.h"
#include "Resources.h"


xmlgui::OSCServer::OSCServer() {
	started = false;
	extraListener = NULL;
}


void xmlgui::OSCServer::setExtraListener(xmlgui::OSCServerExtraListener *extraListener) {
    this->extraListener = extraListener;
}


xmlgui::OSCServer::~OSCServer() {
	if(started) {
		ws.stop();
	}
}

void xmlgui::OSCServer::setup(int port, int httpPort) {
	xmlgui::Resources::getFont();

	started = true;
	ws.start("./", httpPort);
	ws.addHandler(this, "*");
	osc.setup(port);
	ofAddListener(ofEvents().update, this, &xmlgui::OSCServer::update);
}

void xmlgui::OSCServer::smoothChange(xmlgui::Control *c, float value) {
	smoothControls[c] = value;
}

float xmlgui::OSCServer::getControlRange(xmlgui::Control *c) {
	if(c->type=="slider") {
		Slider *s = (Slider*)c;
		return s->max - s->min;
	} else if(c->type=="panner") {
		Panner *p = (Panner*)c;
		return p->max - p->min;

	} else {
		ofLogError("Asking for the range of something that isn't a slider or a panner in OSCServer!\n");
		return -1;
	}
}
void xmlgui::OSCServer::update(ofEventArgs &e) {
	ofxOscMessage m;
	while(osc.getNextMessage(&m)) {
		if(m.getAddress()=="/gui") {
			string name = m.getArgAsString(0);
			for(int i = 0; i < guis.size(); i++) {
				if(guis[i]->name==name || (guis[i]->name=="default" && name=="")) {
					printf("Received message\n");
					xmlgui::Control *c = guis[i]->getControlById(m.getArgAsString(1));
					if(c!=NULL) {
						if(c->type=="slider" || c->type=="panner") {
							smoothChange(c, ofToFloat(m.getArgAsString(2)));
						} else if(c->type=="multiball") {
							printf("Multiball!!!\n");
						} else {
							c->valueFromString(m.getArgAsString(2));
							xmlgui::Event e(c, xmlgui::Event::TOUCH_UP);
							c->parent->notifyChange(&e);
						}
					} else {
						printf("Can't find control called %s\n", m.getArgAsString(1).c_str());
					}
					break;
				}
			}
		} else if(extraListener!=NULL) {
			extraListener->oscServerExtraMessage(m);
		}
	}

	float filter = 0.3;
	map<xmlgui::Control*, float>::iterator it = smoothControls.begin();
	while(it != smoothControls.end()) {
		xmlgui::Control *c = (*it).first;

		float f = c->getFloat() * filter + (*it).second * (1.f-filter);
		c->setValue(f);
		xmlgui::Event e(c, xmlgui::Event::TOUCH_UP);
		c->parent->notifyChange(&e);
		float diff = ABS(f - (*it).second);

		// we're pretty much there, so set it
		if(diff/getControlRange(c)<0.01) {
			c->setValue((*it).second);
			xmlgui::Event e(c, xmlgui::Event::TOUCH_UP);
			c->parent->notifyChange(&e);
			smoothControls.erase(it++);
		} else {
			it++;
		}

	}
}


void xmlgui::OSCServer::addTabbedGui(xmlgui::TabbedGui *gui) {
	for(int i = 0; i < gui->tabs.size(); i++) {
		addGui(gui->tabs[i].second);
	}
}

void xmlgui::OSCServer::addGui(xmlgui::Container *gui) {
	if(gui->name=="") {
		gui->name = "default";
	}
	guis.push_back(gui);
}

void xmlgui::OSCServer::httpGet(string url) {
	//printf("GET %s\n", url.c_str());
	ofLogNotice() << "GET " << url;
	if(url=="/") {

		ofxXmlSettings xml;
		xml.addTag("guis");
		xml.pushTag("guis");

		for(int i = 0; i < guis.size(); i++) {
		//	printf("%s\n", guis[i]->name.c_str());
			xml.addTag("gui");
			xml.setAttribute("gui", "name", guis[i]->name, i);
			//xml.pushTag("gui", i);
			//guis[i]->saveToXmlObject(xml);
			//xml.popTag();
		}
		string xmlStr = "";
		xml.copyXmlToString(xmlStr);
		httpResponse("text/xml", xmlStr);
		return;
	} else {
		for(int i = 0; i < guis.size(); i++) {
			if(url.find(guis[i]->name)==1) {
				ofxXmlSettings xml;
				guis[i]->saveToXmlObject(xml);

				string xmlStr = "";
				xml.copyXmlToString(xmlStr);
				httpResponse("text/xml", xmlStr);
				return;
			}
		}
		ofxXmlSettings xml;
		xml.addTag("NotFound");
		string xmlStr = "";
		xml.copyXmlToString(xmlStr);
		httpResponse("text/xml", xmlStr);
		return;
	}
}

void xmlgui::OSCServer::httpPost(string url, char *data, int dataLength) {
	ofLogNotice() << "POST " << url;
}
