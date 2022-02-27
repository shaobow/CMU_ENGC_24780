/*
Nestor Gomez
Carnegie Mellon University
Eng. Computation, 24-780-B
Prob Set 6
Due Mon. Oct. 26, 2020
*/
#pragma once

#include <vector>

#include "Node.h"
class ViewManager;

class Shape {
protected:
		std::vector<Node> theGuides;
		std::vector<Node> theSpline;   

		int splineSubdivisions = 20;
		float splineTension = 0.0;

		bool shapeFilled = false;

		float minX, minY, maxX, maxY;  // useful for finding default origin and scale
		float tolerance = 1e-8;

		// FOR PS06: TO BE WRITTEN BY YOU!!!
		float area;                    // area of shape
		float momX0, momY0;            // moments of inertia about centroid
		float centroidX, centroidY;    // location of centroid of shape

public:
	// clears old data and reads points from a file stream
	void readFile(std::ifstream& inFile);

	// gets the distance along the spline for a given coord.
	// figures out nearest point to the spline
	float getDistanceFromXY(float x, float y);

	// gets coordinates given a distance along the spline
	void getXYfromDistance(float distance, float& xCoord, float& yCoord);

	// gets the slope of the spline at the given distance along the spline
	float getSlopeAngle(float distance);

	// get the index of the guide point that is just before the distance given
	int getIndexFromDistance(float distance);

	// new node becomes the index-th node (i.e., index = 0 inserts at head)
	void insertNode(Node& newNode, int index);

	// given model coordinates x,y , function returns the first guide point (or spline point)
	// that is within given distance
	// returns nullptr if no node meets the criteria
	Node* findNode(float x, float y, float distance, bool searchSpline = false);

	// draws the shape using given parameters
	void draw(int shapeColor,
		int lineWidth, int nodeColor, bool showNodes = false, bool showAxes = false);

	// returns the bounding box of the shape
	void getBounds(float& xMin, float& xMax, float& yMin, float& yMax) {
		xMin = minX; xMax = maxX; yMin = minY; yMax = maxY;
	}

	// writes all data in the model to the given stream 
	void writeFile(std::ostream& output);

	void toggleFilled() { shapeFilled = !shapeFilled; }

	void clearAll();

	float getSplineLength();

	// deletes the node whose memory address matches the given node pointer
	bool deleteNode(Node* badNode);

	// creates a smooth spline that goes through the guide points
	// and stores the points in a vector, including distance from start
	// of curve. Also resets bounds
	void recalcSpline();

	void setSplineParams(int divisions, float tension) {
		splineSubdivisions = divisions;
		splineTension = tension;
		recalcSpline();
	}

	// determines if given test point is contained within the area of the shape
	// uses ray casting algorithm
	// previously defined by Fall 2020 students
	bool isContained(float testX, float testY, bool isInclusive = true);

	// FOR PS06: TO BE WRITTEN BY YOU!!!
	// calculates and stores the shape properties of area, moments of inertia, and centroid
	void calcProperties();

protected:
	// develops cubic spline points (used only for inserting points)
	void segmentPoints(int segIndex);

	// get the index of the spline point that is just before the distance given
	int getIndexFromDistanceSpline(float distance);

};