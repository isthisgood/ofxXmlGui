/**
 * ParameterInfo.h
 * emptyExample
 *
 * Created by Marek Bereza on 17/06/2011.
 *
 */

#pragma once
#include <string>
using namespace std;
namespace xmlgui {
	class ParameterInfo {
	public:
		string name;
		string xmlName;
		void *value;
		string type;
		ParameterInfo(string _name, string _xmlName, string _type, void *_value) {
			name = _name;
			type = _type;
			value = _value;
			xmlName = _xmlName;
		}
	};
}