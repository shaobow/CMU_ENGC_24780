// ps03springcure_shaobow.cpp
// code written by Shaobo Wang from F21-24780B
// final version on 09/18/2021

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include "ps03springcurve_shaobow.h" 
#include "fssimplewindow.h"
#include "ysglfontdata.h"

bool createCurve(const string& inputFile, SpringCurve& aCurve) {
	ifstream inFile;
	bool firstTimeInput = true;

	double sumCoefficient[] = { 0,0,0,0,0 };
	double anValue[] = { 0,0 };
	EuclideanTwoDimension coordInput = { 0,0 };
	EuclideanTwoDimension maxCoord = { 0,0 };

	inFile.open(inputFile);
	if (inFile.is_open()) {
		while (!inFile.eof()) {
			inFile >> coordInput.xCoord >> coordInput.yCoord;
			aCurve.curveCoordPairs.push_back(coordInput);
			if (firstTimeInput) {
				maxCoord = coordInput;
				firstTimeInput = false;
			}
			else {
				maxCoord = findCoordMax(maxCoord, coordInput);
			}
			calculateSum(sumCoefficient, coordInput);
		}
	}
	else {
		cout << "Error: File name invalid!" << endl;
		return false;
	}

	solveAn(sumCoefficient, anValue);
	aCurve.anValue[0] = anValue[0];
	aCurve.anValue[1] = anValue[1];

	aCurve.maxCoordPairs = maxCoord;
	return true;
}

EuclideanTwoDimension findCoordMax(EuclideanTwoDimension xyMax, EuclideanTwoDimension xyInput) {
	double x_in, y_in;
	x_in = xyInput.xCoord;
	if (x_in >= xyMax.xCoord) {
		xyMax.xCoord = x_in;
	}
	y_in = xyInput.yCoord;
	if (y_in >= xyMax.yCoord) {
		xyMax.yCoord = y_in;
	}
	return xyMax;
}

void calculateSum(double sum[5], EuclideanTwoDimension xyInput) {
	double x_in, y_in;
	x_in = xyInput.xCoord;
	y_in = xyInput.yCoord;
	sum[0] += x_in; // sumX
	sum[1] += pow(x_in, 2); // sumX2
	sum[2] += pow(x_in, 3); // sumX3
	sum[3] += y_in; // sumY
	sum[4] += x_in * y_in; // sumXY
}

void solveAn(double sum[5], double a_n[2]) {
	a_n[0] = (sum[3] * sum[2] - sum[4] * sum[1]) / (sum[0] * sum[2] - sum[1] * sum[1]);
	a_n[1] = (sum[3] * sum[1] - sum[4] * sum[0]) / (sum[1] * sum[1] - sum[0] * sum[2]);
}

void printCurve(const SpringCurve& aCurve,ostream& output = cout) {
	output << "	MaxX = " << aCurve.maxCoordPairs.xCoord << endl;
	output << "	MaxY = " << aCurve.maxCoordPairs.yCoord << endl;

	output << "	Curve param a1 = " << aCurve.anValue[0] << endl;
	output << "	Curve param a2 = " << aCurve.anValue[1] << endl;

	switch (aCurve.curveColor) {
	case RED:
		output << "	Color = " << "RED	" << endl;
		break;
	case ORANGE:
		output << "	Color = " << "ORANGE " << endl;
		break;
	case GREEN:
		output << "	Color = " << "GREEN " << endl;
		break;
	case CYAN:
		output << "	Color = " << "CYAN " << endl;
		break;
	case BLUE:
		output << "	Color = " << "BLUE " << endl;
		break;
	default:
		break;
	}
	output << endl;

	for (int i = 0; i < aCurve.curveCoordPairs.size(); i++) {
		output << aCurve.curveCoordPairs[i].xCoord << "\t" << aCurve.curveCoordPairs[i].yCoord << endl;
	}
	output << endl;
}

RGBRange getColorRGB(Color curveColor) {
	RGBRange colorRangeinRGB = { 255,0,0 };
	switch (curveColor) {
	case RED:
		colorRangeinRGB.rRange = 255;
		colorRangeinRGB.gRange = 0;
		colorRangeinRGB.bRange = 0;		
		break;
	case ORANGE:
		colorRangeinRGB.rRange = 255;
		colorRangeinRGB.gRange = 255;
		colorRangeinRGB.bRange = 0;
		break;
	case GREEN:
		colorRangeinRGB.rRange = 0;
		colorRangeinRGB.gRange = 255;
		colorRangeinRGB.bRange = 0;
		break;
	case CYAN:
		colorRangeinRGB.rRange = 0;
		colorRangeinRGB.gRange = 255;
		colorRangeinRGB.bRange = 255;
		break;
	case BLUE:
		colorRangeinRGB.rRange = 0;
		colorRangeinRGB.gRange = 0;
		colorRangeinRGB.bRange = 255;
		break;
	default:
		break;
	}
	return colorRangeinRGB;
}

void drawCurve(SpringCurve& aCurve) {
	int windowWidth = 800, windowLength = 600, sideBar = 10;
	drawDataPoints(aCurve, windowWidth, windowLength, sideBar);
	drawFittedCurve(aCurve, windowWidth, windowLength, sideBar);
}

void drawDataPoints(SpringCurve& aCurve, int width, int length, int bar) {
	GLfloat pointSize = 6;
	glColor3d(aCurve.curveColor_rgb.gRange, aCurve.curveColor_rgb.bRange, aCurve.curveColor_rgb.rRange);
	glPointSize(pointSize);

	double realWidth = (double)width - bar;
	double realLength = (double)length - bar;

	EuclideanTwoDimension plotCoord;

	glBegin(GL_POINTS);
	for (int i = 0; i < aCurve.curveCoordPairs.size(); i++) {
		plotCoord.xCoord = aCurve.curveCoordPairs.at(i).xCoord / aCurve.maxCoordPairs.xCoord * realWidth;
		plotCoord.yCoord = length - aCurve.curveCoordPairs.at(i).yCoord / aCurve.maxCoordPairs.yCoord * realLength;
		glVertex2d(plotCoord.xCoord, plotCoord.yCoord);
	}
	glEnd();
}

void drawFittedCurve(SpringCurve& aCurve, int width, int length, int bar) {
	GLfloat lineWidth = 3;
	glColor3d(aCurve.curveColor_rgb.rRange, aCurve.curveColor_rgb.gRange, aCurve.curveColor_rgb.bRange);
	glLineWidth(lineWidth);

	double realWidth = (double)width - bar;
	double realLength = (double)length - bar;

	double coordStep = 0.5;

	EuclideanTwoDimension plotCoord1, plotCoord2;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glBegin(GL_LINE_STRIP);
	plotCoord1.xCoord = 0 * coordStep / aCurve.maxCoordPairs.xCoord * realWidth;
	plotCoord1.yCoord = length - (0 * coordStep * aCurve.anValue[0] + pow(0 * coordStep, 2) * aCurve.anValue[1]) / aCurve.maxCoordPairs.yCoord * realLength;
	glVertex2d(plotCoord1.xCoord, plotCoord1.yCoord);
	for (int j = 0; j <= (int)(aCurve.maxCoordPairs.xCoord / coordStep + 1); j++) {
		plotCoord2.xCoord = (j)*coordStep / aCurve.maxCoordPairs.xCoord * realWidth;
		plotCoord2.yCoord = length - ((j)*coordStep * aCurve.anValue[0] + pow((j)*coordStep, 2) * aCurve.anValue[1]) / aCurve.maxCoordPairs.yCoord * realLength;
		glVertex2d(plotCoord2.xCoord, plotCoord2.yCoord);
	}
	glEnd();
}

void printConsoleMenu() {
	cout << "A -> Add a curve and display it" << endl << "C -> cycle through possible colors" << endl << "D -> delete the curve currently displayed from the collection" << endl << "L -> list all the data for the currently displayed curve on the console window" << endl << "Arrow Left/Right -> cycle through curves in the collection" << endl << "X -> eXit the program" << endl;
}

int addCurve(vector<SpringCurve> &curveContainer) {
	string fileName;
	bool createdFlag;
	cout << "Type in the data file's name:";
	cin >> fileName; // SpringData01.txt

	SpringCurve springCurveInput;

	if (createdFlag=createCurve(fileName, springCurveInput)) {
		curveContainer.push_back(springCurveInput);
		cout << endl << "New Curve Added!" << endl;
	}
	printConsoleMenu();

	int currentIndex = curveContainer.size() - 1;
	return currentIndex;
}

void listData(vector<SpringCurve>& curveContainer, int currentIndex) {
	cout << endl << "The data for the currently displayed curve" << endl;
	printCurve(curveContainer.at(currentIndex),cout);
	printConsoleMenu();
}

int deleteCurve(vector<SpringCurve>& curveContainer,int currentIndex) {
	curveContainer.erase(curveContainer.begin() + currentIndex);
	cout << endl << "Selected curve deleted!" << endl;
	printConsoleMenu();
	if (currentIndex - 1 < 0) {
		currentIndex = 0;
	}
	else {
		currentIndex = (currentIndex - 1); // avoid negative index
	}
	return currentIndex;
}

void changeColor(vector<SpringCurve>& curveContainer, int currentIndex) {
	Color colorName = curveContainer.at(currentIndex).curveColor;
	Color colorTank[5] = { RED,ORANGE,GREEN,CYAN,BLUE };
	int colorIndex = colorName;
	colorName = colorTank[(colorIndex + 1) % 5];
	curveContainer.at(currentIndex).curveColor_rgb = getColorRGB(colorName);
	curveContainer.at(currentIndex).curveColor = colorName;
}

void userInteraction() {
	bool terminateFlag = false;
	vector<SpringCurve> curveCollection;
	int plotCurveIndex = 0, key;

	while (!terminateFlag)
	{
		FsPollDevice();
		key = FsInkey();

		if (!curveCollection.size()) {
			if (FsCheckWindowOpen()) {
				glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
				FsSwapBuffers();
				FsSleep(20);
			}
			cout << endl << "Please create new curve first!" << endl;
			plotCurveIndex = addCurve(curveCollection);
		}
		else {
			if (!FsCheckWindowOpen()) {
				FsOpenWindow(160, 160, 800, 600, 1);
			}

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			drawCurve(curveCollection.at(plotCurveIndex));

			FsSwapBuffers();
			FsSleep(20);

			switch (key) {
			case FSKEY_X:
				terminateFlag = true;
				break;
			case FSKEY_A:
				plotCurveIndex = addCurve(curveCollection);
				break;
			case FSKEY_C:
				changeColor(curveCollection, plotCurveIndex);
				break;
			case FSKEY_D:
				plotCurveIndex = deleteCurve(curveCollection, plotCurveIndex);
				break;
			case FSKEY_L:
				listData(curveCollection, plotCurveIndex);
				break;
			case FSKEY_LEFT:
				if (plotCurveIndex - 1 < 0) {
					plotCurveIndex = curveCollection.size() - 1;
				}
				else {
					plotCurveIndex = plotCurveIndex - 1;
				}
				break;
			case FSKEY_RIGHT:
				plotCurveIndex = (plotCurveIndex + 1) % curveCollection.size();
				break;
			default:
				break;
			}
		}
	}
}

void inputRequiredMessage()
{
	// display message on graphics window directing user to check console

	string message = "Input required on console . . .";
	// black color
	glColor3f(0, 0, 0);

	//center horizontally and vertically for 20-pixel-wide by 28-pixel-height letters
	int wid, hei;
	FsGetWindowSize(wid, hei);
	glRasterPos2d(wid / 2 - message.length() * 20 / 2, hei / 2 + 28 / 2);

	YsGlDrawFontBitmap20x28(message.c_str()); // function requires a C-language string
	FsSwapBuffers();

}