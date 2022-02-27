/**
ps03springcure_main_shaobow.cpp
code written by Shaobo Wang from F21-24780B
final version on 10/03/2021

This code visualizes gear from input file
project contains "GearViewer.cpp", "Gear.h", "Gear.cpp"

Self-added function based on draft version:
mouse event detection;
click on gear to change current selected;
drag to move gear with Right mouse key;
drag to rotate gear with Left mouse key;
**/

#include <string.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include "fssimplewindow.h"
#include "ysglfontdata.h"
#include "Gear.h"

using namespace std;

void showMenu();

void inputRequiredMessage();

void updateScalarAndOrigin(float& scalar, Point2D& boundCenter, vector<Gear>& allGears);

int main(void)
{
	const int width = 800, height = 600;
	bool terminate = false;
	char yesNo;
	int currGearIndex = -1;
	Gear newGear;
	vector<Gear> allGears;
	string datafilename;
	int key;
	ifstream inFile;

	/*ADDED*/
	int leftButton, middleButton, rightButton, locX, locY, mouseEvent;
	float scalar = 10;
	Point2D boundCorner = { 0,0 };

	Point2D mouseRInc = { 0,0 };
	Point2D mouseLInc = { 0,0 };
	float mouseAngInc = 0;
	int preLocX = 0;
	int preLocY = 0;
	bool isMouseRDown = false;
	bool isMouseLDown = false;

	FsOpenWindow(16, 16, width, height, 1, "Gear Viewer (SW2021)"); // use double buffer

	showMenu();

	//// for UI testing, just load 2 gears, commment/uncomment as needed
	//inFile.open("A1.gear");
	//newGear.load(inFile);
	//inFile.close();
	//allGears.push_back(newGear);
	//inFile.open("A2.gear");
	//newGear.load(inFile);
	//inFile.close();
	//allGears.push_back(newGear);
	//currGearIndex = 0;
	//allGears[0].print();

	while (!terminate) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		FsPollDevice();
		key = FsInkey();

		/*ADDED: mouse event*/
		mouseEvent = FsGetMouseEvent(leftButton, middleButton, rightButton, locX, locY);
		// use mouse to choose current gear
		if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
			for (int k = 0; k < allGears.size(); k++) {
				Point2D possGearO = allGears.at(k).getGearOrigin();
				float possGearR = allGears.at(k).getTopRad();
				float dsX = pow(locX - (boundCorner.x + possGearO.x * scalar), 2); // squared difference of X in pixels
				float dsY = pow(locY - (height + boundCorner.y + possGearO.y * -scalar), 2); // squared difference of Y in pixels
				if (sqrt(dsX + dsY) <= possGearR * scalar) { currGearIndex = k;}
			}
		}

		if (allGears.size() != 0) {
			Point2D currGearO = allGears.at(currGearIndex).getGearOrigin();
			float currGearR = allGears.at(currGearIndex).getTopRad();
			float pixGearOx = boundCorner.x + currGearO.x * scalar;
			float pixGearOy = height + boundCorner.y + currGearO.y * -scalar;
			float currDsX = pow(locX - pixGearOx, 2);
			float currDsY = pow(locY - pixGearOy, 2);
			if (sqrt(currDsX + currDsY) <= currGearR * scalar) {// mouse inside the outter circle
				switch (mouseEvent) {
				case FSMOUSEEVENT_RBUTTONDOWN:
					isMouseRDown = true;
					break;
				case  FSMOUSEEVENT_RBUTTONUP:
					isMouseRDown = false;
					break;
				case FSMOUSEEVENT_LBUTTONDOWN:
					isMouseLDown = true;
					break;
				case FSMOUSEEVENT_LBUTTONUP:
					isMouseLDown = false;
					break;
				default:
					break;
				}
			}
			else {// turndown flag when out of object
				isMouseRDown = false;
				isMouseLDown = false;
			}
			// drag with Right key down to move gear with mouse
			if (isMouseRDown) {
				if (preLocX != locX || preLocY != locY) {
					mouseRInc = { (float)locX - preLocX , (float)locY - preLocY };
					allGears.at(currGearIndex).dragGear(mouseRInc.x / scalar, (mouseRInc.y) / -scalar);
				}
			}

			// drag with Left key down to rotate gear with mouse
			if (isMouseLDown) {
				if (preLocX != locX || preLocY != locY) {
					mouseLInc = { (float)locX - preLocX , (float)locY - preLocY };
					int mouseAngDir = -((preLocX - pixGearOx) * mouseLInc.y - (preLocY - pixGearOy) * mouseLInc.x) / abs((preLocX - pixGearOx) * mouseLInc.y - (preLocY - pixGearOy) * mouseLInc.x);
					mouseAngInc = mouseAngDir * 12;
					allGears.at(currGearIndex).rotate(mouseAngInc / 10);
				}
			}
		}

		switch (key) {
		case FSKEY_ESC:
		case FSKEY_X:   // exit program
			terminate = true;
			break;
		case FSKEY_UP: // prev gear
			currGearIndex--;
			if (currGearIndex < 0)
				currGearIndex = allGears.size() - 1;
			break;
		case FSKEY_DOWN: // next gear
			currGearIndex++;
			if (currGearIndex >= allGears.size())
				currGearIndex = 0;
			break;
		case FSKEY_LEFT: // rotate CCW
			allGears.at(currGearIndex).rotate(10.);
			break;
		case FSKEY_RIGHT: // rotate CW
			allGears.at(currGearIndex).rotate(-10.);
			break;
		case FSKEY_L: // list all data and redisplay menu
			cout << "\n\n";
			if (currGearIndex >= 0)
				allGears.at(currGearIndex).print();
			showMenu();
			break;
		case FSKEY_A: // add a curve
			// redirect to console
			inputRequiredMessage();

			//ask for filename
			cout << "\n\nPlease enter the name of the file to read > ";
			cin >> datafilename;
			if (datafilename.find(".gear") == string::npos)
				datafilename += ".gear";     // allows user to omit extension
			inFile.open(datafilename);

			if (inFile.is_open()) { // may not have found file
				newGear.load(inFile);
				inFile.close();
				allGears.push_back(newGear);
				currGearIndex = allGears.size() - 1;
			}
			else
				cout << "\nError reading file. Please check data and try again." << endl;
			//ADDED:
			updateScalarAndOrigin(scalar, boundCorner, allGears);
			// redisplay console menu
			showMenu();
			break;
		case FSKEY_D: // delete a gear, if there are any in the collection
			if (currGearIndex >= 0) {
				// redirect to console
				inputRequiredMessage();

				// ask for verification
				cout << "\n\nAre you sure you want to delete this gear ("
					<< allGears.at(currGearIndex).getID() << ")? (Y/N) >> ";
				cin >> yesNo;

				// delete/erase the current curve and make the previous curve current
				if (yesNo == 'Y' || yesNo == 'y') {
					allGears.erase(allGears.begin() + currGearIndex);
					currGearIndex--;
					if (currGearIndex < 0)
						currGearIndex = allGears.size() - 1;
				}

				while (getchar() != '\n'); // this will clear the newline associated with last input
				showMenu();
			}
			break;
		//ADDED: press Z to resize the displayed gear
		case FSKEY_Z: 
			updateScalarAndOrigin(scalar, boundCorner, allGears);
			// since I add a move gear function, it will be weird to reshape the gear all the time
			break;
		default:
			break;
		}

		preLocX = locX;
		preLocY = locY;

		//adjust the graphics plane
		glTranslated(0, height, 0); // make bottom-left the origin
		glTranslated(boundCorner.x, boundCorner.y, 0);
		glScaled(scalar, -scalar, 1.);  // scale everything by scalar, with positive y going up

		// draw the gears
		glColor3ub(0, 0, 0);
		glLineWidth(1);
		for (Gear& currGear : allGears)
			currGear.draw();

		//highlight current Gear
		glColor3ub(255, 0, 0);
		glLineWidth(3);
		if (currGearIndex >= 0 && currGearIndex < allGears.size()) // extra protection
			allGears.at(currGearIndex).draw();

		// reset transformation matrix completely
		glLoadIdentity();  

		FsSwapBuffers();  // use this instead of glFlush() when using double buffer
		FsSleep(20);
	}
}

void updateScalarAndOrigin(float& scalar, Point2D& boundCenter, vector<Gear>& allGears) {
	int hei, wid;
	FsGetWindowSize(wid, hei);

	Point2D currMax, maxBound = { 0,0 };
	Point2D currMin, minBound = { wid,hei };
	for (Gear& currGear : allGears) {
		currMax.x = currGear.getTopRad() + currGear.getGearOrigin().x;
		currMax.y = currGear.getTopRad() + currGear.getGearOrigin().y;
		currMin.x = -currGear.getTopRad() + currGear.getGearOrigin().x;
		currMin.y = -currGear.getTopRad() + currGear.getGearOrigin().y;
		if (currMax.x >= maxBound.x) { maxBound.x = currMax.x; }
		if (currMax.y >= maxBound.y) { maxBound.y = currMax.y; }
		if (currMin.x <= minBound.x) { minBound.x = currMin.x; }
		if (currMin.y <= minBound.y) { minBound.y = currMin.y; }
	}

	scalar = min((float)wid / (maxBound.x - minBound.x), (float)hei / (maxBound.y - minBound.y));
	boundCenter.x = -(minBound.x)* scalar;
	boundCenter.y = (minBound.y)* scalar;
}

void showMenu()
{
	cout << "\n\n";
	cout << "Use these keys on the graphics window:" << endl << endl;
	// ADDED:
	cout << "\tClick the left mouse key to select current gear" << endl;
	cout << "\tHold the left mouse key to rotate current gear" << endl;
	cout << "\tHold the right mouse kry to move current gear" << endl;
	cout << endl;
	cout << "\tZ: Reshape current displayed gears to fit the window" << endl;
	cout << "\tA : Add a gear from input file" << endl;
	cout << "\tD : Delete the current selected gear from the collection" << endl;
	cout << "\tL : List all the data for the currently selected gear on the console window" << endl;
	cout << endl;
	cout << "\tUp/Down arrows: cycle through gears in the collection" << endl;
	cout << "\tLeft/Right arrows: rotate the currently selected gear" << endl;
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
	glRasterPos2d(wid / 2 - message.length() * 20 / 2, hei / 2 + 28 / 2);

	YsGlDrawFontBitmap20x28(message.c_str()); // function requires a C-language string
	FsSwapBuffers();

}



