//
//  MegaMapperGui.h
//  MegaMapper
//
// This controls a MegaMapper
//

#pragma once
#include "ofxXmlGui.h"
#include "MegaMapper.h"

class MegaMapperGui: public xmlgui::Container {
public:
	
	MegaMapper *mapper;
	
	
	class MapperCurveControl: public xmlgui::Control {
	public:
		float touchAreaSize = 10;
		ofRectangle editRect;
		Vec2 *selectedPoint;
		
		MegaMapper *mapper;
		MapperCurveControl() {
			selectedPoint = NULL;
			width = 100;
			height = 100;
			
		}
		
		
		Vec2 unnormalizePoint(Vec2 p) {
			return Vec2(editRect.x + p.x * editRect.width,
						   editRect.y + (1 - p.y) * editRect.height);
		}
		
		Vec2 normalizePoint(Vec2 p) {
			return Vec2(
				(p.x - editRect.x)/editRect.width,
						   1-(p.y - editRect.y)/editRect.height
			);
			

		}
		void draw() {
			editRect = *this;
			editRect.x += touchAreaSize;
			editRect.y += touchAreaSize;
			editRect.width -= touchAreaSize * 2;
			editRect.height -= touchAreaSize * 2;
			
			ofSetColor(30);
			ofDrawRectangle(editRect);
			
			ofNoFill();
			ofSetColor(focus?255:200);
			ofDrawRectangle(editRect);

			
			ofSetColor(255);
			ofBeginShape();
//			for(int i = 0; i < mapper->points.size(); i++) {
//				Vec2 p = unnormalizePoint(mapper->points[i]);
//				ofVertex(p.x, p.y);
//			}
			for(int i = 0; i < editRect.width; i++) {
				float x = i/(float)editRect.width;
				float y = mapper->mapFunction(x);
				Vec2 p = unnormalizePoint(Vec2(x, y));
				ofVertex(p.x, p.y);
			}
			ofEndShape(false);
			
			for(int i = 0; i < mapper->points.size(); i++) {
				if(&mapper->points[i]==selectedPoint) {
					ofSetColor(0, 255, 0);
				} else {
					ofSetColor(255);
				}
				Vec2 n = unnormalizePoint(mapper->points[i]);
				ofDrawCircle(n.x, n.y, 5);
			}
			
		
			ofSetColor(255);
			
			
			ofFill();
			
			ofSetColor(255, 0, 0);
			
			Vec2 n(mapper->getLastNormalizedInput(), mapper->getLastNormalizedOutput());
			n = unnormalizePoint(n);
			ofCircle(n.x, n.y, 5);
			ofDrawLine(editRect.x, n.y, editRect.getRight(), n.y);
			ofSetColor(255);
			
		}
		
		
		/*void sortPoints() {
			std::sort(points.begin(), points.end(), [] (const ofVec2f &a, const ofVec2f &b) {
				return a.x < b.x;
			});
		}*/
		void touchOver(int x, int y, int id) {
		
		}
		
		void dragSelectedPoint(Vec2 n) {
			if(n.x>1) n.x = 1;
			else if(n.x < 0) n.x = 0;
			
			if(n.y>1) n.y = 1;
			else if(n.y<0) n.y = 0;
			
			
			// at the ends, only allow y movement
			if(selectedPoint==&mapper->points[0] || selectedPoint==&mapper->points[mapper->points.size() - 1]) {
				selectedPoint->y = n.y;
			} else {
				selectedPoint->set(n.x, n.y);
			}
		}
		bool touchMoved(int x, int y, int id){
			if(selectedPoint!=NULL) {
//				selectedPoint->set(normalizePoint(ofVec2f(x, y)));
				dragSelectedPoint(normalizePoint(Vec2(x, y)));
				mapper->createLut();
			//	sortPoints();
			}
			return true;
		}
		
		bool adding = true;
		
		bool touchDown(int x, int y, int id){
			parent->setKeyboardFocus(this);
			selectedPoint = NULL;
			Vec2 n = normalizePoint(Vec2(x, y));
			
			for(int i = 0; i < mapper->points.size(); i++) {
				if(unnormalizePoint(mapper->points[i]).distance(Vec2(x, y)) < 8) {
					selectedPoint = &mapper->points[i];

				}
			}
			if(selectedPoint!=NULL) {
//				selectedPoint->set(n);
				dragSelectedPoint(n);
				mapper->createLut();
			} else if(adding) {
				for(int i = 0; i < mapper->points.size() - 1; i++) {
					
					if(n.x > mapper->points[i].x && n.x < mapper->points[i+1].x) {
						mapper->points.insert(mapper->points.begin() + i + 1, n);
						selectedPoint = &mapper->points[i+1];
						break;
					}
				}
			}
			return true;
		}
		
		bool touchUp(int x, int y, int id){
			
			return inside(x, y);
		}

		void tryToDeleteSelectedPoint() {
			if(selectedPoint==NULL) {
				return;
			}
			
			for(int i = 1; i <mapper->points.size()-1; i++) {
				if(selectedPoint==&mapper->points[i]) {
					mapper->points.erase(mapper->points.begin() + i);
					selectedPoint = NULL;
					mapper->createLut();
					break;
				}
			}
		}
		bool keyPressed(int key) {
			if(key==OF_KEY_BACKSPACE) {
				tryToDeleteSelectedPoint();
			} else if(key=='s') {
				printf("%s\n", mapper->save().c_str());
			}
//			printf("%d\n", key);
			return true;
		}
		
	};
	
	
	MapperCurveControl *curveControl;
	xmlgui::NumberBox *inMin, *inMax, *outMin, *outMax;
	xmlgui::NumberBox *inValue;
	xmlgui::NumberBox *outValue;
	xmlgui::PushButton *deleteButton;
	xmlgui::SegmentedControl *logInput, *logOutput;
	
	void draw() {
		inValue->setValue(mapper->getLastInput());
		outValue->setValue(mapper->getLastOutput());
		ofSetColor(30);
		ofDrawRectangle(*this);
		
		ofNoFill();
		ofSetColor(focus?255:200);
		ofDrawRectangle(*this);
		ofFill();
		xmlgui::Container::draw();
	}
	

	MegaMapperGui() {
		curveControl = new MapperCurveControl();

		static float f = 123;
		static float g = 234;
		inMin = addNumberBox("in min", f);
		inMax = addNumberBox("in max", f);
//
		outMin = addNumberBox("out min", f);
		outMax = addNumberBox("out max", f);
		
		inValue = addNumberBox("in value", f);
		outValue = addNumberBox("out value", g);
		
		deleteButton = (xmlgui::PushButton*)INSTANTIATE_WITH_ID("pushbutton", "delete");

		deleteButton->released = bind(&MegaMapperGui::deleteButtonPressed, this);

		
		logOutput = (xmlgui::SegmentedControl*)INSTANTIATE_WITH_ID("segmented", "exp output");
		logInput = (xmlgui::SegmentedControl*)INSTANTIATE_WITH_ID("segmented", "exp input");
		
		vector<string> opts;
		opts.push_back("lin");
		opts.push_back("log");
		string options = ofJoinString(opts, "|");
		logInput->opts = opts;
		logOutput->opts = opts;
		logInput->options = options;
		logOutput->options = options;
		logInput->width = 70;
		logOutput->width = 70;
		
		
		addChild(logInput);
		addChild(logOutput);
		
		
		addChild(deleteButton);
		addChild(curveControl);
		width = 600;
		height = 400;
		layout();
	}
	
	void setSize(float w, float h) {
		width = w;
		height = h;
		layout();
	}
	
	
	void layout() {

		int padding = 20;
		inMin->position(padding, padding);
		inMax->position(padding, inMin->getBottom() + padding);
		
		curveControl->position(inMin->getRight() + padding, padding);
		curveControl->height = height - padding * 2;
		curveControl->width = width - padding * 4 - inMin->width - outMin->width;
		

		
		curveControl->width += curveControl->touchAreaSize * 2;
		curveControl->height += curveControl->touchAreaSize * 2;
		
		curveControl->x -= curveControl->touchAreaSize;
		curveControl->y -= curveControl->touchAreaSize;
		
		outMin->position(curveControl->getRight() + padding, padding);
		outMax->position(outMin->x, outMin->getBottom() + padding);
		
		inValue->position(inMin->x, curveControl->getBottom() - inValue->height);
		outValue->position(outMin->x, curveControl->getBottom() - outValue->height);
		
		
		
		logInput->position(inMax->x, inMax->getBottom() + padding);
		logOutput->position(outMax->x, outMax->getBottom() + padding);
		
		deleteButton->position(logOutput->getLeft(), logOutput->getBottom() + padding*3);
		deleteButton->width = 70;
		
	}
	
	void deleteButtonPressed() {
		curveControl->tryToDeleteSelectedPoint();
	}
	
	void pointToMapper(MegaMapper &mapper) {
		this->mapper = &mapper;
		curveControl->mapper = this->mapper;
		this->mapper->createLut();
		inMin->pointToValue(&mapper.inMin);
		inMax->pointToValue(&mapper.inMax);
		outMin->pointToValue(&mapper.outMin);
		outMax->pointToValue(&mapper.outMax);
		inValue->pointToValue(&mapper.lastInput);
		outValue->pointToValue(&mapper.lastOutput);
		
		logInput->pointToValue(&mapper.logarithmicInput);
		logOutput->pointToValue(&mapper.logarithmicOutput);
	}
	
	
	xmlgui::NumberBox *addNumberBox(string name, float &value) {
		int TEXT_WIDTH = 70;
		xmlgui::NumberBox *field = (xmlgui::NumberBox*)INSTANTIATE_WITH_ID("numberbox", name);
		field->pointToValue(&value);
		field->width = TEXT_WIDTH;

		addChild(field);
		return field;
	}
};