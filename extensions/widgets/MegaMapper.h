//
//  MegaMapper.h
//  MegaMapper
//
// This allows you to map any float to any float with a spline curve - this is just
// the mapping code, no UI.
//
//

#pragma once


class Vec2 {
	
public:
	float x, y;
	
	
	Vec2(float x = 0, float y = 0) {
		set(x, y);
	}
	
	void set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	
	Vec2 operator-(Vec2 b) {
		return Vec2(x - b.x, y - b.y);
	}
	
	float length() {
		return sqrt(x*x + y*y);
	}
	float distance(Vec2 p) {
		return (p - *this).length();
	}
};



#include <regex>

using std::regex;
using std::string;
using std::sregex_token_iterator;



class MegaMapper {
public:
	
	float inMin;
	float inMax;
	
	
	float outMin;
	float outMax;

	int logarithmicInput;
	int logarithmicOutput;
	
	float lastInput;
	float lastNormalizedInput;
	float lastNormalizedOutput;
	float lastOutput;
	
	vector<Vec2> points;
	vector<float> lut;
	
	MegaMapper() {
		logarithmicInput = false;
		logarithmicOutput = false;
		inMin = 0;
		inMax = 1;
		outMin = 0;
		outMax = 1;
		lastNormalizedInput = 0;
		lastNormalizedOutput = 0;
		lastOutput = outMin;
		points.push_back(Vec2(0,0));
		points.push_back(Vec2(1,1));
		lut.resize(200);
	}
	
	
	float s2f(const string& floatString) {
		float x = 0;
		istringstream cur(floatString);
		cur >> x;
		return x;
	}
	
	
	void load(string data) {
		// parse data
		vector<string> out;
		csv(data, out);
		
		
		// structure inMin, inMax, outMin, outMax, logInput, logOutput, x1, y1, x2, y2, ...
		
		if(out.size() < 10) {
			printf("Got a bad mapper string: %s\n", data.c_str());
			return;
		}
		
		points.clear();
		
		inMin = s2f(out[0]);
		inMax = s2f(out[1]);
		outMin = s2f(out[2]);
		outMax = s2f(out[3]);
		logarithmicInput = (int)s2f(out[4]);
		logarithmicOutput = (int)s2f(out[5]);
		
		for(int i = 6; i < out.size(); i+=2) {
			Vec2 v(s2f(out[i]), s2f(out[i+1]));
			points.push_back(v);
		}
		//printf("Loaded %d points\n", points.size());
		createLut();
	}
	
	
	string f2s(float value){
		ostringstream out;
		out << value;
		return out.str();
	}
	string save() {
		string s;
		s += f2s(inMin) + ",";
		s += f2s(inMax) + ",";
		s += f2s(outMin) + ",";
		s += f2s(outMax) + ",";
		s += f2s(logarithmicInput) + ",";
		s += f2s(logarithmicOutput) + ",";
		for(int i = 0; i < points.size(); i++) {
			if(i>0) {
				s+= ",";
			}
			
			s += f2s(points[i].x) + "," + f2s(points[i].y);
			
				
		}
		//printf("%d points\n", points.size());
		return s;
		
	}
	
	
	void csv(const string &s, vector<string> &out) {
		
		regex re("[\\s,]+");
		sregex_token_iterator it(s.begin(), s.end(), re, -1);
		sregex_token_iterator reg_end;
		for (; it != reg_end; ++it) {
			out.push_back(it->str());
		}
	}
	

	
	
	float map(const float &in) {
		lastInput = in;
		if(logarithmicInput) {
			lastNormalizedInput = sqrt(normalizeInput(in));
		} else {
			lastNormalizedInput = normalizeInput(in);
		}
		lastNormalizedOutput = mapFunction(lastNormalizedInput);

		if(logarithmicOutput) {
			lastOutput = scaleOutput(lastNormalizedOutput*lastNormalizedOutput);
		} else {
			lastOutput = scaleOutput(lastNormalizedOutput);
		}
		return lastOutput;
	}
	
	float getLastInput() {
		return lastInput;
	}
	
	float getLastNormalizedInput() {
		return lastNormalizedInput;
	}
	
	float getLastNormalizedOutput() {
		return lastNormalizedOutput;
	}
	
	float getLastOutput() {
		return lastOutput;
	}
	
	inline float normalizeInput(float in) {
		
		
		in = (in - inMin) / (inMax - inMin);
		
		if(in < 0) in = 0;
		else if(in > 1) in = 1;
		
		
		return in;
	}
	
	inline float scaleOutput(const float &in) {
		return in * (outMax - outMin) + outMin;
	}
	inline float linMap(const float &inp, const float &_inMin, const float &_inMax, const float &_outMin, const float &_outMax) {
		if(_inMin==_inMax) return _outMin;
		float v = (inp - _inMin) / (_inMax - _inMin);
		return _outMin + (_outMax - _outMin) * v;
	}
	
	float createLut() {
		int lutSize = lut.size();
		for(int i = 0; i < lutSize; i++) {
			float v = i /((float) lutSize-1);
			//printf("%f\n", v);
			lut[i] = mapFunctionCalculated(v);
		}
		for(int k = 0; k < 10; k++)
		for(int i = 1; i < lutSize-1; i++) {
			lut[i] = (lut[i]+lut[i-1]+lut[i+1])/3.f;
		}
	}
	
	
	// t is a value that goes from 0 to 1 to interpolate in a C1 continuous way across uniformly sampled data points.
	// when t is 0, this will return B.  When t is 1, this will return C.
	inline float CubicHermite (float A, float B, float C, float D, float t)
	{
		float a = -A/2.0f + (3.0f*B)/2.0f - (3.0f*C)/2.0f + D/2.0f;
		float b = A - (5.0f*B)/2.0f + 2.0f*C - D / 2.0f;
		float c = -A/2.0f + C/2.0f;
		float d = B;
		
		return a*t*t*t + b*t*t + c*t + d;
	}
	
	
	
	
	float mapFunction(float v) {
		int lutSize = lut.size();
		v *= lutSize;
		if(v >= lutSize) {
			v = lutSize - 1;
		} else if(v < 0) {
			v = 0;
		}
		//return lut[(int)v]; // nearest neighbour
		
		
		int B = v;
		int C = B + 1;
		if(C>=lutSize) C = lutSize - 1;
		
		float t = v - B;
//		if(C>lut[lut.size() - 5]) {
//			printf("[%d] = %f\n", C, lut[C]);
//		}
		return lut[B] * (1.f - t) + lut[C] * t; // linear
		/*
		
		int D = C + 1;
		int A = B - 1;
		if(A<0) A = 0;

		if(D>=lutSize) D = lutSize - 1;
		return CubicHermite(lut[A], lut[B], lut[C], lut[D], t);*/
	}
	
	
	float mapFunctionCalculated(float v) {

		for(int i = 0; i < points.size()-1; i++) {
			if(v >= (points[i].x) && v <= (points[i+1].x)) {
				// find the value
				
				float ww = linMap(v, points[i].x, points[i+1].x, points[i].y, points[i+1].y);
				
				return ww;
			}
		}
		return v;
	}
	
};