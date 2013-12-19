/**
 * Resources.h
 * emptyExample
 *
 * Created by Marek Bereza on 29/06/2011.
 *
 */
#pragma once
#include "ofMain.h"


#include <map>

namespace xmlgui {
	class Control;
	class Resources {
	public:
		// when you draw a string, it doesn't draw it immediately
		// unless you call "drawStringImmediate", or you're using bitmap font.
		// it draws them at the end of the draw loop all together.
		// you need to pass in the object calling this, so we can calculate
		// the absolute position on the screen.
		static void drawString(xmlgui::Control *caller, string str, int x, int y, ofFloatColor color = ofFloatColor(1.f,1.f,1.f,1.f));
		
		static void drawStringImmediate(string str, int x, int y);

		static ofImage *getImage(string path);
		static void bindFont();
		static void unbindFont();
		

		static void drawAllDeferredStrings();
		static ofTrueTypeFont *getFont();
		static int getFontCharWidth();
	private:
		static ofTrueTypeFont *font;
		static bool customFontNotAvailable;
		static map<string,ofImage*> images;
		static void checkFontLoaded();
	};
};
