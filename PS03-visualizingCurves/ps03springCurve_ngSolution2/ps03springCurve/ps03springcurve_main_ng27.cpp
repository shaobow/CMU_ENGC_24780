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
#include <string.h>
#include <vector>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "ps03springcurve_ng27.h"

using namespace std;

void showMenu();

void inputRequiredMessage();

int main(void)
{
	bool terminate = false;
	char yesNo;
	int currCurveIndex = -1;
	SpringCurve newCurve;
	vector<SpringCurve> allCurves;
	string datafilename;
	Color currColor;
	MarkerType currMarker;
	int key;

	FsOpenWindow(16, 16, 1200, 600, 1, "Spring Curve Viewer (NG2021)"); // use double buffer

	showMenu();

	// for UI testing, just load 3 curves, commment/uncomment as needed
	createCurve("SpringData01.txt", newCurve);
	allCurves.push_back(newCurve);
	createCurve("SpringData02.txt", newCurve);
	newCurve.theColor = GREEN;
	allCurves.push_back(newCurve);
	createCurve("SpringData03.txt", newCurve);
	newCurve.theColor = BLUE;
	allCurves.push_back(newCurve);
	currCurveIndex = 0;

	while (!terminate) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		FsPollDevice();
		key = FsInkey();

		switch (key) {
		case FSKEY_ESC:
		case FSKEY_X:   // exit program
			terminate = true;
			break;
		case FSKEY_LEFT: // prev curve
			currCurveIndex--;
			if (currCurveIndex < 0)
				currCurveIndex = allCurves.size() - 1;
			break;
		case FSKEY_RIGHT: // next curve
			currCurveIndex++;
			if (currCurveIndex >= allCurves.size())
				currCurveIndex = 0;
			break;
		case FSKEY_L: // list all data and redisplay menu
			if (currCurveIndex >= 0)
				printCurve(allCurves.at(currCurveIndex));
			showMenu();
			break;
		case FSKEY_C: // change color
			if (currCurveIndex >= 0) {
				currColor = allCurves.at(currCurveIndex).theColor;
				if (currColor == BLUE) // if now last color, make it first color
					currColor = RED;
				else                   // make it next color
					// a bit weird, but I am casting currColor to integer, then incrementing,
					// then casting back to Color
					currColor = (Color)(((int)currColor) + 1);
				allCurves.at(currCurveIndex).theColor = currColor;
			}
			break;
		case FSKEY_M: // change marker
			if (currCurveIndex >= 0) {
				currMarker = allCurves.at(currCurveIndex).theMarker;
				if (currMarker == TRIANGLE_FILL) // if now last type, make it first type
					currMarker = PLUS;
				else                   // make it next marker type
					currMarker = (MarkerType)(((int)currMarker) + 1);
				allCurves.at(currCurveIndex).theMarker = currMarker;
			}
			break;
		case FSKEY_A: // add a curve
			// redirect to console
			inputRequiredMessage();

			//ask for filename
			cout << "\n\nPlease enter the name of the file to read > ";
			cin >> datafilename;
			createCurve(datafilename, newCurve);
			
			// if curve actually loaded, add it to collection and make ir the current curve
			if (newCurve.a1 > -INFINITY) {
				allCurves.push_back(newCurve);
				currCurveIndex = allCurves.size() - 1;
			}
			else
				cout << "\nError reading file. Please check data and try again." << endl;

			// redisplay console menu
			showMenu();
			break;
		case FSKEY_D: // delete a curve, if there are any in the collection
			if (currCurveIndex >= 0) {
				// redirect to console
				inputRequiredMessage();

				// ask for verification
				cout << "\n\nAre you sure you want to delete this spring curve (Curve " 
					<< currCurveIndex+1 << ")? (Y/N) >> ";
				cin >> yesNo;

				// delete/erase the current curve and make the previous curve current
				if (yesNo == 'Y' || yesNo == 'y') {
					allCurves.erase(allCurves.begin() + currCurveIndex);
					currCurveIndex--;
					if (currCurveIndex < 0)
						currCurveIndex = allCurves.size() - 1;
				}

				while (getchar() != '\n'); // this will clear the newline associated with last input
				showMenu();
			}
			break;
		}

		// draw the curve
		if (currCurveIndex >= 0 && currCurveIndex < allCurves.size()) // extra protection
			drawCurve(allCurves.at(currCurveIndex));

		FsSwapBuffers();  // use this instead of glFlush() when using double buffer
		FsSleep(20);

	}

}

void showMenu()
{
	cout << "\n\n";
	cout << "Use these keys on the graphics window:" << endl << endl;
	cout << "\tA : Add a curve from input file" << endl;
	cout << "\tD : Delete the curve currently displayed from the collection" << endl;
	cout << "\tC : cycle through possible Colors" << endl;
	cout << "\tM : cycle through possible Marker styles" << endl;
	cout << "\tL : List all the data for the currently displayed curve on the console window" << endl;
	cout << endl;
	cout << "\tLeft/Right arrows: cycle through curves in the collection" << endl;
	cout << endl;
	cout << "\tX/esc : eXit the program" << endl;
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
	glRasterPos2d(wid / 2 - message.length() * 20 / 2, hei/2 + 28/2);

	YsGlDrawFontBitmap20x28(message.c_str()); // function requires a C-language string
	FsSwapBuffers();

}
