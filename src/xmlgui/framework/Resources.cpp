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
#include "bitmapTextures.h"
string xmlgui::Resources::fontPath = DEFAULT_FONT;
int xmlgui::Resources::fontSize = DEFAULT_FONT_SIZE;


StripeyRect *diagLines = NULL;
VerticalLinesRect *verticLines = NULL;
vector<pair<ofVec2f, string> > xmlgui::LabeledControl::labels;
int xmlgui::LabeledControl::lastDrawnFrame = 0;
//#define DEFAULT_FONT "OCRAStd.ttf"
//#define DEFAULT_FONT_SIZE 12



ofTrueTypeFont *xmlgui::Resources::font = NULL;
bool xmlgui::Resources::customFontNotAvailable = false;
map<string,ofImage*> xmlgui::Resources::images;

ofMesh stringMeshes;

ofTrueTypeFont *xmlgui::Resources::getFont() {
	checkFontLoaded();
	return font;
}

void xmlgui::Resources::setCustomFont(string path, int size) {
	fontPath = path;
	fontSize = size;
	
}
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

int xmlgui::Resources::stringWidth(const string &s) {
	return s.size() * getFontCharWidth();
}

void xmlgui::Resources::checkFontLoaded() {
    if(font==NULL && !customFontNotAvailable) {
		// try to load font
		ofFile f(fontPath);

		if(f.exists()) {

			font = new ofTrueTypeFont();
            
            if(!font->load(fontPath, fontSize)) {
                ofLogError() << "Error loading font!! - path =\"" << fontPath << "\"";
                customFontNotAvailable = true;
                delete font;
                font = NULL;
            }
            
            
		} else {
			ofLogError() << "Couldn't find font at " << ofToDataPath(fontPath, true);
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

void xmlgui::Resources::drawString(xmlgui::Control *caller, string str, int x, int y, ofFloatColor color) {
	checkFontLoaded();
	if(customFontNotAvailable) {
		ofDrawBitmapString(str, x, y);
	} else {
//		font->drawString(str, x, y);
		ofVec2f pos = caller->parent->getAbsolutePosition();
		ofMesh m =font->getStringMesh(str, pos.x + x, pos.y + y);
		vector<ofFloatColor> colors(m.getNumVertices(), color);
		m.addColors(colors);
		stringMeshes.append(m);
		
	}

}

void xmlgui::Resources::drawAllDeferredStrings() {
	if(font!=NULL) {
		font->getFontTexture().bind();
		stringMeshes.setMode(OF_PRIMITIVE_TRIANGLES);
		stringMeshes.draw();
		stringMeshes.clear();
		font->getFontTexture().unbind();
	}
}

void xmlgui::Resources::bindFont() {
	if(font!=NULL && !customFontNotAvailable) {
		font->getFontTexture().bind();
	}
}
void xmlgui::Resources::unbindFont() {
	if(font!=NULL && !customFontNotAvailable) {
		font->getFontTexture().unbind();
	}
}




void xmlgui::Resources::drawVerticalStripes(const ofRectangle &r) {

	if(verticLines==NULL) {
		verticLines = new VerticalLinesRect(0x66,0x77);
	}
	verticLines->draw(r);
}

void xmlgui::Resources::drawDiagonalStripes(const ofRectangle &r) {

	if(diagLines==NULL) {
		diagLines = new StripeyRect(3, 0xCC, 0xFF);
	}
	diagLines->draw(r);
}
