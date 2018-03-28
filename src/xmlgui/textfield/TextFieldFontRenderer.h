

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
 *  ofxTextInputFieldFontRenderer.h, created by Marek Bereza on 14/08/2013.
 */
#pragma once
//For lack of a type abstraction, this lets you #define a font renderer so
//(like ofxFTGL or ofxFont)
//to use ofxFTGL use somethinglike this:
//#define OFX_TEXTFIELD_FONT_RENDERER ofxFTGLFont
//#define OFX_TEXTFIELD_FONT_RENDERER "ofxFTGLFont.h"
#ifndef OFX_TEXTFIELD_FONT_RENDERER
#define OFX_TEXTFIELD_FONT_RENDERER ofTrueTypeFont
#endif

#ifdef OFX_TEXTFIELD_FONT_INCLUDE
#include OFX_TEXTFIELD_FONT_INCLUDE
#endif

#define TEXTFIELD_IS_ACTIVE "textfieldIsActive"
#define TEXTFIELD_IS_INACTIVE "textfieldIsInactive"




namespace xmlgui {
	namespace ofxTextInput {
		class FontRenderer {
		public:
			virtual ~FontRenderer() {}
			virtual void drawString(string text, int x, int y) = 0;
			virtual float getLineHeight() = 0;
			virtual float stringWidth(const string &str) = 0;
			
			
			virtual bool isBitmapFont() { return false; }
			
			
			// this gets the cursor position (in characters) based on an x coordinate.
			int getPosition(const string &str, int x) {
				float lastW = 0;
				
				for(int i = 0; i < str.size(); i++) {
					
					float w = this->stringWidth(str.substr(0, i+1));
					
					if(x<lastW + (w-lastW)/2.f) {
						return i;
					}
					lastW = w;
					
				}
				return str.size();
			}
		};
		
		class TypedFontRenderer: public FontRenderer {
		public:
			OFX_TEXTFIELD_FONT_RENDERER *renderer;
			TypedFontRenderer(OFX_TEXTFIELD_FONT_RENDERER *renderer) {
				this->renderer = renderer;
			}
			
			void drawString(string text, int x, int y) {
				renderer->drawString(text, x, y);
			}
			
			float getLineHeight() {
				return renderer->getLineHeight();
			}
			
			
			float spacesWidth(const string &str) {
				if(str.size()>0 && str[str.size()-1]==' ') {
					return renderer->getSpaceSize()*renderer->getSize();
				}
				return 0;
			}
			float stringWidth(const string &str) {
				
				return renderer->stringWidth(str) + spacesWidth(str) + 3;
			}
		};
		
		
		
		class BitmapFontRenderer: public FontRenderer {
		public:
			
			virtual bool isBitmapFont() { return true; }
			void drawString(string text, int x, int y) {
				ofDrawBitmapString(text, x, y);
			}
			float getLineHeight() {
				return 8.f*1.725f;
			}
			
			
			
			
			
			
			
			float stringWidth(const string &str) {
				int w = 0;
				for(int i = 0; i < str.size(); i++) {
					if(str[i]=='\t')
						w += 8 - (w % 8);
					else
						w ++;
				}
				return w*8;
			}
			
		};
	};
};