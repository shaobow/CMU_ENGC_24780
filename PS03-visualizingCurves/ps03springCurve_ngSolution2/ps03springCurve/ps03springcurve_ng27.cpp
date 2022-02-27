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
#include<fstream>
#include <string>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "ps03springcurve_ng27.h"

using namespace std;

std::string colorAsString(Color aColor)
{
	switch (aColor) {
	case RED: return "RED"; break;
	case ORANGE: return "ORANGE"; break;
	case GREEN: return "GREEN"; break;
	case CYAN: return "CYAN"; break;
	case BLUE: return "BLUE"; break;
	}
}

std::string markerAsString(MarkerType atype)
{
	switch (atype) {
	case PLUS: return "PLUS"; break;
	case EXES: return "EXES"; break;
	case CIRCLE: return "CIRCLE"; break;
	case CIRCLE_FILL: return "CIRCLE_FILL"; break;
	case SQUARE: return "SQUARE"; break;
	case SQUARE_FILL: return "SQUARE_FILL"; break;
	case DIAMOND: return "DIAMOND"; break;
	case DIAMOND_FILL: return "DIAMOND_FILL"; break;
	case TRIANGLE: return "TRIANGLE"; break;
	case TRIANGLE_FILL: return "TRIANGLE_FILL"; break;
	}
}

void setGraphicsColor(Color aColor)
{
	switch (aColor) {
	case RED: glColor3ub(255, 0, 0);
		break;
	case ORANGE: glColor3ub(255, 165, 0);
		break;
	case GREEN: glColor3ub(0, 255, 0);
		break;
	case CYAN: glColor3ub(0, 255, 255);
		break;
	case BLUE: glColor3ub(0, 0, 255);
		break;
	default: glColor3ub(0, 0, 0);
		break;
	}
}

void setGraphicsColorLighter(Color aColor)
{
	switch (aColor) {
	case RED: glColor3ub(255, 128, 128);
		break;
	case ORANGE: glColor3ub(255, 213, 128);
		break;
	case GREEN: glColor3ub(128, 255, 128);
		break;
	case CYAN: glColor3ub(128, 255, 255);
		break;
	case BLUE: glColor3ub(128, 128, 255);
		break;
	default: glColor3ub(0, 0, 0);
		break;
	}
}


void createCurve(const std::string& inputFile, SpringCurve& aCurve)
{
	double sumX = 0., sumX2 = 0., sumX3 = 0., sumY = 0., sumXY = 0.;
	double currX, currY, prevX, prevY;
	bool firstLoop = true;

	// be sure to start fresh
	aCurve.rawData.clear();

	// open input file stream
	ifstream inFile(inputFile);

	if (inFile.is_open()) { // may not have found file
		while (!inFile.eof()) {
			inFile >> currX >> currY;

			// check and disqualify duplicate coordinates
			if (firstLoop || fabs(prevX - currX) > TOLERANCE || fabs(prevY - currY) > TOLERANCE) {
				if (firstLoop) {
					firstLoop = false;
					aCurve.maxX = currX;
					aCurve.maxY = currY;
				}
				else {
					if (currX > aCurve.maxX)
						aCurve.maxX = currX;
					if (currY > aCurve.maxY)
						aCurve.maxY = currY;
				}

				sumX += currX;
				sumY += currY;
				sumX2 += currX * currX;
				sumX3 += currX * currX * currX;
				sumXY += currX * currY;

				aCurve.rawData.push_back({ currX, currY });
			}
			prevX = currX; prevY = currY;
		}
		inFile.close();  // being polite
	}

	if (fabs(sumX) < TOLERANCE || fabs(sumY) < TOLERANCE) {  // no data found
		cout << "ERROR: Could not read data from " << inputFile
			<< "  Please check file." << endl;
		aCurve.a1 = -INFINITY; aCurve.a2 = -INFINITY;
	}
	else {
		aCurve.a2 = (sumY * sumX2 - sumXY * sumX) / (sumX2 * sumX2 - sumX3 * sumX);
		aCurve.a1 = (sumY - sumX2 * aCurve.a2) / sumX;
	}

	aCurve.theColor = RED;
	aCurve.theMarker = PLUS;
}

void printCurve(const SpringCurve& aCurve, std::ostream& output)
{
	output << "Max X = " << aCurve.maxX << endl;
	output << "Max Y = " << aCurve.maxY << endl;
	output << "Curve param a1 = " << aCurve.a1 << endl;
	output << "Curve param a2 = " << aCurve.a2 << endl;
	output << "Color = " << colorAsString(aCurve.theColor) << endl;
	output << "Marker = " << markerAsString(aCurve.theMarker) << endl;

	// output all data (alternate form of for-loop)
	for (const Point2D& currPoint : aCurve.rawData)
		output << currPoint.x << "\t" << currPoint.y << endl;

}

void drawCurve(const SpringCurve& aCurve)
{
	int wid, hei;
	FsGetWindowSize(wid, hei); // this function reads the size of the window
	float originX = 50;   // want to leave some space at right
	float originY = hei - 30;  // want to leave some space at bottom
	float scaleX = (wid - originX - 20) / aCurve.maxX;
	float scaleY = (originY - 30) / aCurve.maxY;
	double curveX, curveY;
	string label;
	double stepsize = aCurve.maxX / wid * 4.; // graph every 4 horz pixels

	// the grid (not required, but I want you to see more code examples)
	int lineCount = 10;
	float gridX = round(aCurve.maxX / lineCount * 10. + 1) / 10.;
	float gridY = round(aCurve.maxY / lineCount * 10. + 1) / 10.;

	glLineWidth(1);
	for (int i = 0; i <= lineCount; i++) {
		glColor3ub(80, 80, 80);
		glBegin(GL_LINES);
		// horz line
		glVertex2d(originX, originY - (i * gridY) * scaleY);
		glVertex2d(originX + (lineCount)*gridX * scaleX, originY - (i * gridY) * scaleY);
		// vert line
		glVertex2d(originX + (i * gridX) * scaleX, originY);
		glVertex2d(originX + (i * gridX) * scaleX, originY - (lineCount)*gridY * scaleY);
		glEnd();

		// x-axis label, rounded to one decimal place
		glColor3ub(120, 120, 120);
		label = to_string(round(i * gridX * 100.) / 100.);
		label = label.substr(0, label.find(".") + 2);
		glRasterPos2i(originX + (i * gridX) * scaleX - 8, originY + 15);  // sets position
		YsGlDrawFontBitmap7x10(label.c_str());
		// y-axis label, right-justified for 7-pixel-wide letters
		label = to_string(round(i * gridY * 100.) / 100.);
		label = label.substr(0, label.find(".") + 2);
		glRasterPos2i(originX - 5 - label.length() * 7, originY - (i * gridY) * scaleY + 4);  // sets position
		YsGlDrawFontBitmap7x10(label.c_str());
	}

	//// the curve
	//setGraphicsColorLighter(aCurve.theColor);
	//glLineWidth(4);
	//glBegin(GL_LINE_STRIP);
	//for (curveX = 0; curveX <= aCurve.maxX; curveX += stepsize) {
	//	curveY = aCurve.a1 * curveX + aCurve.a2 * curveX * curveX;
	//	glVertex2d(originX + curveX * scaleX, originY - curveY * scaleY);
	//}
	//glEnd();

	// the curve, using transformations
	glTranslated(50, hei - 30, 0);
	glScaled(scaleX, -scaleY, 1.);
	setGraphicsColorLighter(aCurve.theColor);
	glLineWidth(4);
	glBegin(GL_LINE_STRIP);
	for (curveX = 0; curveX <= aCurve.maxX; curveX += stepsize) {
		curveY = aCurve.a1 * curveX + aCurve.a2 * curveX * curveX;
		glVertex2d(curveX, curveY); // because of transformations, I can use "real" coordinates
	}
	glEnd();
	glLoadIdentity();  // reset transformation matrix completely

	// or undo just the one I did above
	//glScaled(1/scaleX, -1/scaleY, 1.);
	//glTranslated(-50, -(hei - 30), 0);

	// the data points (note I included a changeable marker type that was not required)
	setGraphicsColor(aCurve.theColor);
	glLineWidth(1);
	double pixelX, pixelY;
	// for-loop for vectors is easier without an index
	for (const Point2D& currPoint : aCurve.rawData) {
		drawDot(originX + currPoint.x * scaleX, originY - currPoint.y * scaleY, aCurve.theMarker);
	}
}

void drawDot(double xCoord, double yCoord, MarkerType atype)
{
	int markerSize = 4;  // this is actually the half-size
	double circle45deg = markerSize / sqrt(2.); // used to draw 8-sided circle quickly

	bool filled = (atype == CIRCLE_FILL || atype == SQUARE_FILL
		|| atype == DIAMOND_FILL || atype == TRIANGLE_FILL);

	switch (atype) {
	default:
	case PLUS:
		glBegin(GL_LINES);
		glVertex2d(xCoord - markerSize, yCoord);
		glVertex2d(xCoord + markerSize, yCoord);
		glVertex2d(xCoord, yCoord - markerSize);
		glVertex2d(xCoord, yCoord + markerSize);
		glEnd();
		break;

	case EXES:
		glBegin(GL_LINES);
		glVertex2d(xCoord - markerSize, yCoord - markerSize);
		glVertex2d(xCoord + markerSize, yCoord + markerSize);
		glVertex2d(xCoord + markerSize, yCoord - markerSize);
		glVertex2d(xCoord - markerSize, yCoord + markerSize);
		glEnd();
		break;

	case SQUARE:
	case SQUARE_FILL:
		if (filled)
			glBegin(GL_QUADS);
		else
			glBegin(GL_LINE_LOOP);

		//note reordering of vertices from exes portion)
		glVertex2d(xCoord - markerSize, yCoord - markerSize);
		glVertex2d(xCoord + markerSize, yCoord - markerSize);
		glVertex2d(xCoord + markerSize, yCoord + markerSize);
		glVertex2d(xCoord - markerSize, yCoord + markerSize);

		glEnd();
		break;
	case DIAMOND:
	case DIAMOND_FILL:
		if (filled)
			glBegin(GL_QUADS);
		else
			glBegin(GL_LINE_LOOP);

		// note reordering of vertices from plus portion 
		glVertex2d(xCoord - markerSize, yCoord);
		glVertex2d(xCoord, yCoord - markerSize);
		glVertex2d(xCoord + markerSize, yCoord);
		glVertex2d(xCoord, yCoord + markerSize);
		glEnd();
		break;

	case TRIANGLE:
	case TRIANGLE_FILL:
		if (filled)
			glBegin(GL_TRIANGLES);
		else
			glBegin(GL_LINE_LOOP);

		glVertex2d(xCoord - markerSize, yCoord + markerSize * 2. / 3.);
		glVertex2d(xCoord, yCoord - markerSize * 4. / 3.);
		glVertex2d(xCoord + markerSize, yCoord + markerSize * 2. / 3.);
		glEnd();
		break;

	case CIRCLE:
	case CIRCLE_FILL:
		if (filled)
			glBegin(GL_POLYGON);
		else
			glBegin(GL_LINE_LOOP);

		glVertex2d(xCoord - markerSize, yCoord);                 // from diamond
		glVertex2d(xCoord - circle45deg, yCoord - circle45deg);
		glVertex2d(xCoord, yCoord - markerSize);                 // from diamond
		glVertex2d(xCoord + circle45deg, yCoord - circle45deg);
		glVertex2d(xCoord + markerSize, yCoord);                 // from diamond
		glVertex2d(xCoord + circle45deg, yCoord + circle45deg);
		glVertex2d(xCoord, yCoord + markerSize);                 // from diamond
		glVertex2d(xCoord - circle45deg, yCoord + circle45deg);
		glEnd();
		break;
	}

}

