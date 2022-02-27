#pragma once
#include "Shape.h"

class Lshape : public Shape {
protected:
	float locX;
	float locY;
	float height;
	float width;
	float thickness;
	float theta;

public:
	virtual void readFile(std::ifstream& inFile);

	virtual Node* findNode(double x, double y, double distance, bool searchSpline = false){
		return Shape::findNode(x,y,distance,false);
	};

	virtual void writeFile(std::ostream& output);

	virtual void recalcSpline();
};
