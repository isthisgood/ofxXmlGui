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
	class Resources {
	public:
		static void drawString(string str, int x, int y);

		static ofImage *getImage(string path);
		static void bindFont();
		static void unbindFont();
		static ofTrueTypeFont *getFont();
		static int getFontCharWidth();
	private:
		static ofTrueTypeFont *font;
		static bool customFontNotAvailable;
		static map<string,ofImage*> images;
		static void checkFontLoaded();
	};
};
