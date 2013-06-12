/**
 * Resources.cpp
 * emptyExample
 *
 * Created by Marek Bereza on 29/06/2011.
 *
 */

#include "xmlgui/framework/Resources.h"
#include <sys/stat.h>
#include "Container.h"
#include "LabeledControl.h"
vector<pair<ofVec2f, string> > LabeledControl::labels;
int LabeledControl::lastDrawnFrame = 0;
//#define DEFAULT_FONT "OCRAStd.ttf"
//#define DEFAULT_FONT_SIZE 12
#define DEFAULT_FONT "res/mono0755.ttf"
#define DEFAULT_FONT_SIZE 6


ofTrueTypeFont *xmlgui::Resources::font = NULL;
bool xmlgui::Resources::customFontNotAvailable = false;
map<string,ofImage*> xmlgui::Resources::images;

ofImage *xmlgui::Resources::getImage(string path) {


	if(path=="") return NULL;

	struct stat stFileInfo;

	if(stat(ofToDataPath(path, true).c_str(),&stFileInfo)!=0) {
		ofLogError() << "Can't find the file "<<path;
		return NULL;
	}

	// cache the image if it's not already loaded
	if(images.find(path)==images.end()) {
		ofImage *img = new ofImage();
		img->loadImage(path);
		images[path] = img;
		//printf("Loaded file from %s\n", string(basePath+path).c_str());
	}

	return images[path];
}

int xmlgui::Resources::getFontCharWidth() {
    checkFontLoaded();
    if(customFontNotAvailable) return 8;  // oF bitmap font width
    else return font->stringWidth("A");
}
void xmlgui::Resources::checkFontLoaded() {
    if(font==NULL && !customFontNotAvailable) {
		// try to load font
		ofFile f(DEFAULT_FONT);

		if(f.exists()) {

			font = new ofTrueTypeFont();
			font->loadFont(DEFAULT_FONT, DEFAULT_FONT_SIZE);
		} else {
			ofLogError() << "Couldn't find font at " << ofToDataPath(DEFAULT_FONT, true);
			customFontNotAvailable = true;
		}
	}
}
void xmlgui::Resources::drawString(string str, int x, int y) {
	checkFontLoaded();
	if(customFontNotAvailable) {
		ofDrawBitmapString(str, x, y);
	} else {
		font->drawString(str, x, y);
	}

}

void xmlgui::Resources::bindFont() {
	if(font!=NULL && !customFontNotAvailable) {
		font->bind();
	}
}
void xmlgui::Resources::unbindFont() {
	if(font!=NULL && !customFontNotAvailable) {
		font->unbind();
	}
}



