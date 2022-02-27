#pragma once
/*
  ^                                    Nestor Gomez
  |                     *              Carnegie Mellon University
  |                      *             Eng. Computation, 24-780-B
  |                   *                Prob Set 3
  |                                    Due Tues. Sept. 21, 2021
  |                  *
  |             *                      Use least squares method to determine
  |                                    best-fit quadratic curve for data set
  |           *                        loaded from file and display several
  |    *   *                           curves using OpenGL
  |  *    *
  +------------------------>

*/
#include <iostream>
#include <vector>

#define TOLERANCE 1e-10

enum Color { RED, ORANGE, GREEN, CYAN, BLUE };
enum MarkerType {
	PLUS, EXES, CIRCLE, CIRCLE_FILL, SQUARE, SQUARE_FILL,
	DIAMOND, DIAMOND_FILL, TRIANGLE, TRIANGLE_FILL
};

struct Point2D {
	double x, y;
};

// curve definition
struct SpringCurve {
	double maxX, maxY;
	double a1, a2;
	Color theColor;
	MarkerType theMarker;  // not required
	std::vector<Point2D> rawData; // trying to avoid "using" in .h files
};

// curve operations
void createCurve(const std::string& inputFile, SpringCurve& aCurve);
void printCurve(const SpringCurve& aCurve, std::ostream& output = std::cout);
void drawCurve(const SpringCurve& aCurve);

// some helper functions
std::string colorAsString(Color aColor);
std::string markerAsString(MarkerType atype);
void setGraphicsColor(Color aColor);
void setGraphicsColorLighter(Color aColor);
void drawDot(double xCoord, double yCoord, MarkerType atype = PLUS);


