/**
 * ofGuiEventDispatcher.cpp
 * emptyExample 
 *
 * Created by Marek Bereza on 16/06/2011.
 *
 */

#include "xmlgui/framework/ofGuiEventDispatcher.h"
#include "xmlgui/framework/Container.h"
xmlgui::ofGuiEventDispatcher::ofGuiEventDispatcher() {
	this->enabled = false;
	this->manualDraw = false;
}

void xmlgui::ofGuiEventDispatcher::setup(Container *root) {
	this->root = root;
}
void xmlgui::ofGuiEventDispatcher::draw(ofEventArgs &e) {
	if(manualDraw) return;
	root->draw();
}


bool xmlgui::ofGuiEventDispatcher::mousePressed(ofMouseEventArgs &e) {

	return root->touchDown(e.x, e.y, e.button);
}

void xmlgui::ofGuiEventDispatcher::mouseMoved(ofMouseEventArgs &e) {
	root->touchOver(e.x, e.y, e.button);
}

bool xmlgui::ofGuiEventDispatcher::mouseDragged(ofMouseEventArgs &e) {
	return root->touchMoved(e.x, e.y, e.button);
}

bool xmlgui::ofGuiEventDispatcher::mouseReleased(ofMouseEventArgs &e) {
	return root->touchUp(e.x, e.y, e.button);
}

bool xmlgui::ofGuiEventDispatcher::keyPressed(ofKeyEventArgs &e) {
	return root->keyPressed(e.key);
}

bool xmlgui::ofGuiEventDispatcher::keyReleased(ofKeyEventArgs &e) {
	return root->keyReleased(e.key);
}


bool xmlgui::ofGuiEventDispatcher::touchDown(ofTouchEventArgs &e) {
	return root->touchDown(e.x, e.y, e.id);
}

bool xmlgui::ofGuiEventDispatcher::touchMoved(ofTouchEventArgs &e) {
	return root->touchMoved(e.x, e.y, e.id);
}

bool xmlgui::ofGuiEventDispatcher::touchUp(ofTouchEventArgs &e) {
	return root->touchUp(e.x, e.y, e.id);
}

void xmlgui::ofGuiEventDispatcher::enableInteraction() {
	ofAddListener(ofEvents().mousePressed, this, &xmlgui::ofGuiEventDispatcher::mousePressed);
	ofAddListener(ofEvents().mouseMoved, this, &xmlgui::ofGuiEventDispatcher::mouseMoved);
	ofAddListener(ofEvents().mouseDragged, this, &xmlgui::ofGuiEventDispatcher::mouseDragged);
	
	ofAddListener(ofEvents().touchDown, this, &xmlgui::ofGuiEventDispatcher::touchDown);
	ofAddListener(ofEvents().touchUp, this, &xmlgui::ofGuiEventDispatcher::touchUp);
	ofAddListener(ofEvents().touchMoved, this, &xmlgui::ofGuiEventDispatcher::touchMoved);
	
	ofAddListener(ofEvents().mouseReleased, this, &xmlgui::ofGuiEventDispatcher::mouseReleased);
	ofAddListener(ofEvents().keyPressed, this, &xmlgui::ofGuiEventDispatcher::keyPressed);
	ofAddListener(ofEvents().keyReleased, this, &xmlgui::ofGuiEventDispatcher::keyReleased);
}


void xmlgui::ofGuiEventDispatcher::enableEvents() {

	enableInteraction();
	
	ofAddListener(ofEvents().draw, this, &xmlgui::ofGuiEventDispatcher::draw);
	

	
}

void xmlgui::ofGuiEventDispatcher::disableInteraction() {
	ofRemoveListener(ofEvents().mousePressed, this, &xmlgui::ofGuiEventDispatcher::mousePressed);
	ofRemoveListener(ofEvents().mouseMoved, this, &xmlgui::ofGuiEventDispatcher::mouseMoved);
	ofRemoveListener(ofEvents().mouseDragged, this, &xmlgui::ofGuiEventDispatcher::mouseDragged);
	
	ofRemoveListener(ofEvents().touchDown, this, &xmlgui::ofGuiEventDispatcher::touchDown);
	ofRemoveListener(ofEvents().touchUp, this, &xmlgui::ofGuiEventDispatcher::touchUp);
	ofRemoveListener(ofEvents().touchMoved, this, &xmlgui::ofGuiEventDispatcher::touchMoved);
	
	ofRemoveListener(ofEvents().mouseReleased, this, &xmlgui::ofGuiEventDispatcher::mouseReleased);
	ofRemoveListener(ofEvents().keyPressed, this, &xmlgui::ofGuiEventDispatcher::keyPressed);
	ofRemoveListener(ofEvents().keyReleased, this, &xmlgui::ofGuiEventDispatcher::keyReleased);
}

void xmlgui::ofGuiEventDispatcher::disableEvents() {
	disableInteraction();
	ofRemoveListener(ofEvents().draw, this, &xmlgui::ofGuiEventDispatcher::draw);
	

}

bool xmlgui::ofGuiEventDispatcher::isEnabled() {
	return enabled;
}
void xmlgui::ofGuiEventDispatcher::setEnabled(bool enabled) {

	if(enabled!=this->enabled) {
		if(enabled) {
			enableEvents();
		} else {
			disableEvents();
		}
	}
	this->enabled = enabled;
}

void xmlgui::ofGuiEventDispatcher::setManualDraw(bool manualDraw) {
	this->manualDraw = manualDraw;
}