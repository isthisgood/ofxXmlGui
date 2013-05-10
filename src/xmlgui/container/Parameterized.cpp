/**
 *  Parameterized.cpp
 *
 *  Created by Marek Bereza on 22/11/2012.
 */

#include "xmlgui/container/Parameterized.h"
#include "xmlgui/container/ParameterizedGui.h"



void xmlgui::Parameterized::save() {
	bool mustDelete = false;
	if(parent==NULL) {
		parent = new ParameterizedGui();
		mustDelete = true;
	}
	parent->save(this);
	if(mustDelete) {
		delete parent;
	}
}
void xmlgui::Parameterized::saveToFile(string path) {
	ParameterizedGui *p = new ParameterizedGui();

	parameterize(p);
	p->saveSettings(path);

	delete p;
}


string xmlgui::Parameterized::getPath() {
	return "settings/"+name+".xml";
}


void xmlgui::Parameterized::loadFromFile(string path) {

	if(path=="") {
		path = "settings/"+name+".xml";
	}

	ParameterizedGui *oldParent = parent;
//	printf("trying to save here\n");
		parent = new ParameterizedGui();


	parameterize(parent);
	parent->loadSettings(path);
		delete parent;

	parent = oldParent;

}



