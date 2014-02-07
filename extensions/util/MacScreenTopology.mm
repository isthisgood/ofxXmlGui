/**
 *  MacScreenTopology.cpp
 *
 *  Created by Marek Bereza on 09/07/2013.
 */

#include "ScreenTopology.h"

int ScreenTopology::getNumScreens() {
    
    NSArray *screenArray = [NSScreen screens];
    unsigned screenCount = [screenArray count];
	return screenCount;
}

ofRectangle ScreenTopology::getScreenRect(int index) {
	NSRect screenRect;
	NSArray *screenArray = [NSScreen screens];
	NSScreen *screen = [screenArray objectAtIndex: index];
	screenRect = [screen visibleFrame];
    ofRectangle r(screenRect.origin.x, screenRect.origin.y, screenRect.size.width, screenRect.size.height);
	return r;
}