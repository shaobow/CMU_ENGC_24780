// ps03springcure_shaobow.h
// code written by Shaobo Wang from F21-24780B
// final version on 09/18/2021

#include <vector>
#include <string>

using namespace std;

enum Color {
	RED = 0,
	ORANGE = 1,
	GREEN = 2,
	CYAN = 3,
	BLUE = 4,
}; // 5 kinds of color

struct EuclideanTwoDimension {
	double xCoord;
	double yCoord;
}; // 2 coordinates in R^2 euclidean space

struct RGBRange {
	int rRange;
	int gRange;
	int bRange;
}; // RGB range for colors

struct SpringCurve {
	vector<EuclideanTwoDimension> curveCoordPairs; // vector that stores the curve data 
	double anValue[2] = { 0,0 }; // a1,a2 parameters as fitting result
	EuclideanTwoDimension maxCoordPairs = { 0,0 }; // maximum x and y values
	Color curveColor = RED; // color of the curve plot
	RGBRange curveColor_rgb = { 255,0,0 };
}; // structure that stores info of one spring cure

bool createCurve(const string& inputFile, SpringCurve& aCurve); // create one spring curve from data file

EuclideanTwoDimension findCoordMax(EuclideanTwoDimension xyMax, EuclideanTwoDimension xyInput); // find the current maximum coord

void calculateSum(double sum[5], EuclideanTwoDimension xyInput); // calculate coefficients in summation form

void solveAn(double sum[5], double a_n[2]); // solve an with pre-defined solutions

void printCurve(const SpringCurve& aCurve); // print out the data stored in structure

void drawCurve(SpringCurve& aCurve); // provide graphical display of curve structure

void drawDataPoints(SpringCurve& aCurve, int width, int length, int bar); // draw discrete data point

void drawFittedCurve(SpringCurve& aCurve, int width, int length, int bar); // draw fitted curve

RGBRange getColorRGB(Color curveColor); // generate rgb range from color name

void printConsoleMenu(); // print control menu on console

int addCurve(vector<SpringCurve>& curveContainer); // add new curve to collection

void listData(vector<SpringCurve>& curveContainer, int currentIndex); // print out curve information

int deleteCurve(vector<SpringCurve>& curveContainer, int currentIndex); // delete curve from collection

void changeColor(vector<SpringCurve>& curveContainer, int currentIndex); // change curve color to next in selection

void userInteraction(); // general user interaction function