/**
 * Container.h
 * emptyExample
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */
#pragma once
#include "xmlgui/framework/Control.h"
#include "xmlgui/framework/Listener.h"
#define AUTO_LAYOUT_PADDING 15
#define XMLGUI_STICKY
namespace xmlgui {

	enum LayoutType {

		// do nothing
		LayoutType_absolute,
		// stack the elements on top of eachother
		LayoutType_vertical,
        LayoutType_horizontal
	};

	class Container: public Control {
	public:
		Container();
		~Container();
		virtual void draw() ;
		virtual void clear();
		void addChild(Control *child);
		bool removeChild(Control *child);
		virtual void touchOver(int x, int y, int id);
		virtual bool touchDown(int x, int y, int id);
		virtual bool touchMoved(int x, int y, int id);
		virtual bool touchUp(int x, int y, int id);

		virtual bool keyPressed(int key);
		virtual bool keyReleased(int key);
		virtual Control *hitTest(float x, float y);
		void saveToXmlObject(ofxXmlSettings &xml);
		virtual void saveToXml(string file = "");

		void loadFromXmlObject(TiXmlElement *xml);
		virtual bool isContainer() { return true; };

		void addListener(Listener *listener);
		void removeListener(Listener *listener);
		virtual Control *getControlById(string id);
		
		// case insensitive version
		virtual Control *getControlByIdi(string id);
		
		
		void sendToBack(Control *child);
		void sendToFront(Control *child);
		
		void setLayoutType(LayoutType layoutType);
		void pointToValue(string controlName, void *pointer);


		float getFloat(string controlName);
		int getInt(string controlName);
		bool getBool(string controlName);
		string getString(string controlName);

        virtual void settingsLoaded();
		// if you want a control to have keyboard focus, you call
		// this and it receives keyboard events directly from the
		// root container
		void setKeyboardFocus(Control *keyboardFocusedControl);
		bool isKeyboardFocusedOn(Control *ctrl);
		int getNumChildren();
		Control *getChild(int index);

		vector<Listener*> listeners;

		void notifyChange(Event *e);
		bool isOpaque();
		void setOpaque(bool opaque);
		void print(int indent = 0);
		virtual void saveSettings(string file = "");
		virtual void loadSettings(string file);

		// recursive settings load/save function
		void saveSettings(ofxXmlSettings &xml);
		void loadSettings(ofxXmlSettings &xml);
		
		void setContained(bool contained);
		
		
	protected:
		bool contained;
		bool opaque;

		string bgImageUrl;
		ofImage *bgImage;
		// the control currently receiving keyboard events
		Control *keyboardFocusedControl;
		Control *focusedControl;
		LayoutType layoutType;
		void drawChildren() ;

		deque<Control*> children;
		string settingsFile;
		
		
	};

};
