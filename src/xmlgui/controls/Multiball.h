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
 *  Multiball.h, created by Marek Bereza on 07/01/2014.
 */

#pragma once

#include "LabeledControl.h"

namespace xmlgui {
	
	class MultiballTouch: public ofVec3f {
	public:
		int id;
		MultiballTouch(float x = 0, float y = 0, float z = 0, int id = 0) {
			set(x, y, z);
			this->id = id;
		}
		MultiballTouch(ofVec3f t, int id) {
			set(t);
			this->id = id;
		}
		
		void update(ofVec3f pos) {
			set(pos);
		}
		
	};
		
	
	
	
	
	class MultiballListener {
	public:
		virtual void multiballTouchDown(MultiballTouch &touch) {}
		virtual void multiballTouchMoved(MultiballTouch &touch) {}
		virtual void multiballTouchUp(MultiballTouch &touch) {}
	};
	
	
	
	class Multiball: public xmlgui::LabeledControl {
	public:
		MultiballListener *listener;
		Multiball();
		void draw();
		
		
		map<int,MultiballTouch> touches;
		
		int bgColor;
		int borderColor;
		
		
		
		
		bool touchDown(int _x, int _y, int touchId);
		
		bool touchUp(int x, int y, int id);
		
		bool touchMoved(int _x, int _y, int touchId);
			
			
			
			
		
		void sendTouchDown (ofVec3f t, int id);
		void sendTouchMoved(ofVec3f t, int id);
		void sendTouchUp   (ofVec3f t, int id);
		
	};
}