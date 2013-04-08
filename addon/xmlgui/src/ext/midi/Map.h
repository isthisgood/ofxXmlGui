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
 *  Map.h, created by Marek Bereza on 27/11/2012.
 */

#pragma once

#define CURVE_LIN	0
#define CURVE_SQR	1
#define CURVE_SQRT	2

#include "SimpleGui.h"
#include <math.h>
#include "ofxXmlSettings.h"

namespace xmlgui {
	namespace midi {
		
		class Map {
		public:
			xmlgui::Control *to;

			float inMin;
			float inMax;
			float lastValue;
			float *input;

			float value;


			float outMin;
			float outMax;
			int curve;
			float smoothing;
			float inRangeMin;
			float inRangeMax;

			float outRangeMin;
			float outRangeMax;
			virtual ~Map() {
			}

			virtual string getName() = 0;
			bool alreadySet;
			virtual void update() {
				if(alreadySet) {
					updateValue(lastValue);
				  //  printf("%f\n", lastValue);
				}
			};


			void saveGeneric(ofxXmlSettings &xml, int pos) {
				xml.addAttribute("mapping", "inMin", inMin, pos);
				xml.addAttribute("mapping", "inMax", inMax, pos);
				xml.addAttribute("mapping", "outMin", outMin, pos);
				xml.addAttribute("mapping", "outMax", outMax, pos);
				xml.addAttribute("mapping", "curve", curve, pos);
				xml.addAttribute("mapping", "to", to->id, pos);
				xml.addAttribute("mapping", "smoothing", smoothing, pos);
			}


			void loadGeneric(ofxXmlSettings &xml, int pos, xmlgui::Container *targetGui) {
				to = targetGui->getControlById(xml.getAttribute("mapping", "to", "", pos));
				setTo(to);
				inMin = xml.getAttribute("mapping", "inMin", inMin, pos);
				inMax = xml.getAttribute("mapping", "inMax", inMax, pos);
				outMin = xml.getAttribute("mapping", "outMin", outMin, pos);
				outMax = xml.getAttribute("mapping", "outMax", outMax, pos);
				curve = xml.getAttribute("mapping", "curve", curve, pos);
				smoothing = xml.getAttribute("mapping", "smoothing", smoothing, pos);
			}

			void setTo(xmlgui::Control *to) {
				alreadySet = false;
				this->to = to;
				getMinMax(to, outRangeMin, outRangeMax);
				lastValue = to->getFloat();
				// defaults
				outMin = outRangeMin;
				outMax = outRangeMax;
			}


			void updateValue(float val) {
				alreadySet = true;
				lastValue = val;
				// first normalize
				float inVal = (val - inMin)/(inMax-inMin);

				// then clamp
				if(inVal<0) inVal = 0;
				else if(inVal>1) inVal = 1;


				// then curve
				switch(curve) {
					case CURVE_LIN: break;
					case CURVE_SQR: inVal *= inVal; break;
					case CURVE_SQRT: inVal = sqrt(inVal); break;
				}


				// scale
				inVal *= outMax - outMin;
				inVal += outMin;

				if(to->getFloat()!=inVal) {
					float sm = powf(smoothing, 0.1);
					inVal = inVal * (1-sm) + sm * to->getFloat();

					to->setValue(inVal);
					value = inVal;
					// ping the gui to make sure it sends a midi update
					xmlgui::Event e(to, xmlgui::Event::TOUCH_UP);
					to->parent->notifyChange(&e);
				}
			}

			virtual void save(ofxXmlSettings &xml, int pos) = 0;

		
			
			bool getMinMax(xmlgui::Control *c, float &min, float &max) {
				lastValue = min;
				if(c->type=="slider") {
					Slider *s = (Slider*) c;
					min = s->min;
					max = s->max;
				} else if(c->type=="panner") {
					Panner *p = (Panner*) c;
					min = p->min;
					max = p->max;
				} else {
					return false;
				}
				return true;
			}
		};

		class MidiMap: public Map {
		public:
			int cc;
			string getName() {
				return string("MIDI CC ") + ofToString(cc) + " to " + to->id;
			}



			MidiMap() {
				to = NULL;
				value = 0;
				input = &value;
				smoothing = 0;
				cc = 0;
				inRangeMin = 0;
				inRangeMax = 127;
				inMin = inRangeMin;
				inMax = inRangeMax;
				outRangeMin = 0;
				outRangeMax = 1;
				outMin = outRangeMin;
				outMax = outRangeMax;
			}
			MidiMap(int cc, xmlgui::Control *to) {

				value = 0;
				input = &value;
				smoothing = 0;
				setTo(to);
				this->cc = cc;
				inRangeMin = 0;
				inRangeMax = 127;
				inMin = inRangeMin;
				inMax = inRangeMax;
				curve = CURVE_LIN;
				addMidiMap(cc, this);
			}

			MidiMap(ofxXmlSettings &xml, int pos, xmlgui::Container *targetGui) {
				to = NULL;
				value = 0;
				input = &value;
				smoothing = 0;
				loadGeneric(xml, pos, targetGui);

				inRangeMin = 0;
				inRangeMax = 127;

				cc = xml.getAttribute("mapping", "cc", 0, pos);
				addMidiMap(cc, this);
			}


			virtual void save(ofxXmlSettings &xml, int pos) {
				xml.addTag("mapping");
				xml.addAttribute("mapping", "type", "midi", pos);
				xml.addAttribute("mapping", "cc", cc, pos);
				saveGeneric(xml, pos);
			}


			~MidiMap() {
				removeMidiMap(this);
			}

			static map<int,vector<MidiMap*> > midiMappings;

			static void addMidiMap(int cc, MidiMap *MidiMap);
			static void removeMidiMap(MidiMap *midimap);
		};

		class FloatMap: public Map {
		public:

			xmlgui::Control *from;
			float value;

			string getName() {
				return string("CONTROL ") + from->id + " to " + to->id;
			}

			void update() {
				updateValue(from->getFloat());
			}


			FloatMap(ofxXmlSettings &xml, int pos, xmlgui::SimpleGui *sourceGui, xmlgui::Container *targetGui) {
			// need to get parameters from the actual control that comes in
				to = NULL;
				smoothing = 0;
				input = &value;
				from = sourceGui->getControlById(xml.getAttribute("mapping", "from", "", pos));
				setFrom(from);
				loadGeneric(xml, pos, targetGui);
			}



			void save(ofxXmlSettings &xml, int pos) {
				xml.addTag("mapping");
				xml.addAttribute("mapping", "type", "float", pos);
				xml.addAttribute("mapping", "from", from->id, pos);
				saveGeneric(xml, pos);
			}


			FloatMap(xmlgui::Control *from, xmlgui::Control *to) {
				input = &value;
				smoothing = 0;
				setFrom(from);
				setTo(to);
			}
			void setFrom(xmlgui::Control *from) {
				this->from = from;
				getMinMax(from, inRangeMin, inRangeMax);

				// default
				inMin = inRangeMin;
				inMax = inRangeMax;
			}
		};
	}
}