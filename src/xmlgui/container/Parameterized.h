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
 *  Description:  To create parameterized objects, subclass
 *
 *  Parameterized.h, created by Marek Bereza on 13/11/2012.
 */

#pragma once

#include "xmlgui/container/SimpleGui.h"


namespace xmlgui {

	// forward decl for callback
	class ParameterizedGui;

	class Parameterized {
	public:
		// buildGui changed its name to parameterize()
		virtual void parameterize(SimpleGui *gui) {}
		

		void save();
		string name;

		void loadFromFile(string path = "");
		void saveToFile(string path);

		Parameterized() {
			parent = NULL;
		}
		string getPath();


	private:
		ParameterizedGui *parent;
	};
}
