#include <fstream>
#include <sstream>
#include <algorithm>
#include "GearSystem.h"
#include "fssimplewindow.h"
#include "ysglfontdata.h"

Gear* newGearPtr;

void GearSystem::draw(bool highlightOther) {
	// draw the gears
	glColor3ub(0, 0, 0);
	glLineWidth(1);
	for (Gear* currGear : allGears)
		currGear->draw();

	//highlight both Gears
	if (highlightOther) {
		glColor3ub(0, 0, 255);
		glLineWidth(3);
		if (otherGearIndex >= 0 && otherGearIndex < allGears.size()) // extra protection
			allGears[otherGearIndex]->draw();
	}
	glColor3ub(255, 0, 0);
	glLineWidth(3);
	if (currGearIndex >= 0 && currGearIndex < allGears.size()) // extra protection
		allGears[currGearIndex]->draw();
}

void GearSystem::showMenu(){
	std::cout << "\n\n";
	std::cout << "Use these keys on the graphics window:" << std::endl << std::endl;
	std::cout << "\tClick the left mouse key to select current gear" << std::endl;
	//std::cout << "\tHold the left mouse key to rotate current gear" << std::endl;
	//std::cout << "\tHold the right mouse kry to move current gear" << std::endl;
	std::cout << std::endl;
	std::cout << "\tA : Add a gear from input file" << std::endl;
	std::cout << "\tD : Delete the current selected gear from the collection" << std::endl;
	std::cout << "\tL : List all the data for the currently selected gear on the console window" << std::endl;
	std::cout << "\tE : Edit the current gear" << std::endl;
	std::cout << "\tT : Attach the current gear to another selected gear" << std::endl;
	std::cout << "\tM : Mesh the current gear into another selected gear" << std::endl;
	std::cout << "\tS : Save all gears to a single file" << std::endl;
	std::cout << std::endl;
	std::cout << "\tUp/Down arrows: select current gear in the collection" << std::endl;
	std::cout << "\tShift + Up/Down arrows: select the other gear in the collection" << std::endl;
	std::cout << "\tLeft/Right arrows: rotate the currently selected gear" << std::endl;
	std::cout << std::endl;
	std::cout << "\tX/esc : eXit the program" << std::endl;
}

void GearSystem::inputRequiredMessage(){
	// display message on graphics window directing user to check console

	std::string message = "Input required on console . . .";
	// black color
	glColor3f(0, 0, 0);

	//center horizontally and vertically for 20-pixel-wide by 28-pixel-height letters
	int wid, hei;
	FsGetWindowSize(wid, hei);
	glRasterPos2d(wid / 2 - message.length() * 20 / 2, hei / 2 + 28 / 2);

	YsGlDrawFontBitmap20x28(message.c_str()); // function requires a C-language string
	FsSwapBuffers();
}

void GearSystem::rotateCurrent(float deg){
	if (allGears.size() > 0) 
		allGears[currGearIndex]->rotate(deg);
}

void GearSystem::deleteCurrent(){
	if (currGearIndex >= 0) {
		char yesNo;
		// redirect to console
		inputRequiredMessage();

		// ask for verification
		std::cout << "\n\nAre you sure you want to delete this gear (" << allGears[currGearIndex]->getID() << ")? (Y/N) >> ";
		std::cin >> yesNo;

		// delete/erase the current curve and make the previous curve current
		if (yesNo == 'Y' || yesNo == 'y') {
			Gear* oldGear = allGears[currGearIndex];
			allGears.erase(allGears.begin() + currGearIndex);
			currGearIndex--;
			delete oldGear;
			if (currGearIndex < 0)
				currGearIndex = allGears.size() - 1;
		}

		while (getchar() != '\n'); // this will clear the newline associated with last input
		showMenu();
	}
}

void GearSystem::saveAll() {
	std::string savefilename;
	std::ofstream outFile;

	std::cout << "\n\nPlease enter the name of the file to save > ";
	std::cin >> savefilename;
	if (savefilename.find(".gear") == std::string::npos)
		savefilename += ".gear";     // allows user to omit extension

	outFile.open(savefilename);
	if (outFile.is_open()) {
		for (Gear*& currGear : allGears) {
			currGear->print(outFile);
		}
		outFile.close();
		std::cout << "\nFile saved." << std::endl;
	}
	else
		std::cout << "\nError saving file." << std::endl;

	// redisplay console menu
	showMenu();
}

void GearSystem::editCurrent(){
	if (currGearIndex >= 0) {
		char yesNo;
		// redirect to console
		inputRequiredMessage();

		// ask for verification
		std::cout << "\n\nAre you sure you want to edit this gear (" << allGears[currGearIndex]->getID() << ")? (Y/N) >> ";
		std::cin >> yesNo;

		// edit the current curve
		if (yesNo == 'Y' || yesNo == 'y') 
			allGears[currGearIndex]->edit();

		while (getchar() != '\n'); // this will clear the newline associated with last input
		showMenu();
	}
}

void GearSystem::printCurrent(){
	std::cout << "\n\n";
	if (currGearIndex >= 0)
		allGears[currGearIndex]->print();
	showMenu();
}

void GearSystem::changeCurrent(int change){
	if (allGears.size() > 0) {
		currGearIndex += change;
		currGearIndex = (currGearIndex + allGears.size()) % allGears.size();
		if (currGearIndex == otherGearIndex && allGears.size() > 1)
			changeOther(change);
	}
}

void GearSystem::changeOther(int change){
	if (allGears.size() > 0) {
		otherGearIndex += change;
		otherGearIndex = (otherGearIndex + allGears.size()) % allGears.size();
		if (otherGearIndex == currGearIndex && allGears.size() > 1)
			changeOther(change);
	}
}

void GearSystem::addGear(){
	std::string datafilename;
	std::ifstream inFile;

	std::string wholeLineString;
	std::stringstream wholeLineStream;
	bool continueReading = true;

	// redirect to console
	inputRequiredMessage();

	//ask for filename
	std::cout << "\n\nPlease enter the name of the file to read > ";
	std::cin >> datafilename;
	if (datafilename.find(".gear") == std::string::npos)
		datafilename += ".gear";     // allows user to omit extension
	inFile.open(datafilename);

	if (inFile.is_open()) { // may not have found file
		while (!inFile.eof() && continueReading) {
			getline(inFile, wholeLineString);
			if (wholeLineString.find("Gear:") != std::string::npos) {
				newGearPtr = new Gear;
				newGearPtr->load(inFile);

				allGears.push_back(newGearPtr);
				currGearIndex = allGears.size() - 1;
				otherGearIndex = 0;
			}
			wholeLineStream.clear(); // get ready for next line
		}
		inFile.close();
	}
	else
		std::cout << "\nError reading file. Please check data and try again." << std::endl;

	// redisplay console menu
	showMenu();
}

Gear* GearSystem::getGear(const std::string& gearID){
	for (Gear* &currGear : allGears) {
		currGear->getID() == gearID;
		return currGear;
	}
	std::cout << "required gear ID not found" << std::endl;
	return nullptr;
}

Gear* GearSystem::getGear(int gearIndex)
{
	if (gearIndex < allGears.size()) {
		return allGears.at(gearIndex);
	}
	std::cout << "required gear index not found" << std::endl;
	return nullptr;
}

void GearSystem::getOriginAndScale(float& originX, float& originY, float& scale){
	int hei, wid;
	FsGetWindowSize(wid, hei);

	Point2D currMax, maxBound = { 0,0 };
	Point2D currMin, minBound = { wid,hei };
	for (Gear* &currGear : allGears) {
		currMax.x = currGear->getTopRad() + currGear->getGearOrigin().x;
		currMax.y = currGear->getTopRad() + currGear->getGearOrigin().y;
		currMin.x = -currGear->getTopRad() + currGear->getGearOrigin().x;
		currMin.y = -currGear->getTopRad() + currGear->getGearOrigin().y;
		if (currMax.x >= maxBound.x) { maxBound.x = currMax.x; }
		if (currMax.y >= maxBound.y) { maxBound.y = currMax.y; }
		if (currMin.x <= minBound.x) { minBound.x = currMin.x; }
		if (currMin.y <= minBound.y) { minBound.y = currMin.y; }
	}

	scale = std::min((float)wid / (maxBound.x - minBound.x), (float)hei / (maxBound.y - minBound.y));
	originX = -(minBound.x) * scale;
	originY = (minBound.y) * scale;
}

bool GearSystem::attachGears() { return allGears[currGearIndex]->attachToGear(allGears[otherGearIndex], true); }

bool GearSystem::meshGears() { return allGears[currGearIndex]->meshInto(allGears[otherGearIndex], true); }

void GearSystem::mouseClick(mouseInfo& mymouse, int& mouseEvent, bool shiftIsOn) {
	int locX = mymouse.locX;
	int locY = mymouse.locY;
	int height = mymouse.height;
	float originX = mymouse.originX;
	float originY = mymouse.originY;
	float scalar = mymouse.scalar;

	// use mouse to choose current gear
	if (mouseEvent == FSMOUSEEVENT_LBUTTONDOWN) {
		for (int k = 0; k < allGears.size(); k++) {
			Point2D possGearO = allGears[k]->getGearOrigin();
			float possGearR = allGears[k]->getTopRad();
			float dsX = pow(locX - (originX + possGearO.x * scalar), 2); // squared difference of X in pixels
			float dsY = pow(locY - (height + originY + possGearO.y * -scalar), 2); // squared difference of Y in pixels
			if (sqrt(dsX + dsY) <= possGearR * scalar) { 
				if (shiftIsOn)
					otherGearIndex = k;
				else
					currGearIndex = k; 
			}
		}
	}
}

//void GearSystem::mouseRotateAndDrag(mouseInfo& mymouse, int& mouseEvent){
//	int locX = mymouse.locX;
//	int locY = mymouse.locY;
//	int height = mymouse.height;
//	float originX = mymouse.originX;
//	float originY = mymouse.originY;
//	float scalar = mymouse.scalar;
//	int preLocX = mymouse.preLocX;
//	int preLocY = mymouse.preLocY;
//
//	bool isMouseRDown = false;
//	bool isMouseLDown = false;
//
//	Point2D mouseRInc = { 0,0 };
//	Point2D mouseLInc = { 0,0 };
//
//	float mouseAngInc = 0;
//
//	if (allGears.size() != 0) {
//		Point2D currGearO = allGears[currGearIndex]->getGearOrigin();
//		float currGearR = allGears[currGearIndex]->getTopRad();
//		float pixGearOx = originX + currGearO.x * scalar;
//		float pixGearOy = height + originY + currGearO.y * -scalar;
//		float currDsX = pow(locX - pixGearOx, 2);
//		float currDsY = pow(locY - pixGearOy, 2);
//		if (sqrt(currDsX + currDsY) <= currGearR * scalar) {// mouse inside the outter circle
//			switch (mouseEvent) {
//			case FSMOUSEEVENT_RBUTTONDOWN:
//				isMouseRDown = true;
//				break;
//			case  FSMOUSEEVENT_RBUTTONUP:
//				isMouseRDown = false;
//				break;
//			case FSMOUSEEVENT_LBUTTONDOWN:
//				isMouseLDown = true;
//				break;
//			case FSMOUSEEVENT_LBUTTONUP:
//				isMouseLDown = false;
//				break;
//			default:
//				break;
//			}
//		}
//		//else {// turndown flag when out of object
//		//	isMouseRDown = false;
//		//	isMouseLDown = false;
//		//}
//
//		// drag with Right key down to move gear with mouse
//		if (isMouseRDown) {
//			if (preLocX != locX || preLocY != locY) {
//				mouseRInc = { (float)locX - preLocX , (float)locY - preLocY };
//				allGears[currGearIndex]->dragGear(mouseRInc.x / scalar, (mouseRInc.y) / -scalar);
//			}
//		}
//
//		// drag with Left key down to rotate gear with mouse
//		if (isMouseLDown) {
//			if (preLocX != locX || preLocY != locY) {
//				mouseLInc = { (float)locX - preLocX , (float)locY - preLocY };
//				int mouseAngDir = -((preLocX - pixGearOx) * mouseLInc.y - (preLocY - pixGearOy) * mouseLInc.x) / abs((preLocX - pixGearOx) * mouseLInc.y - (preLocY - pixGearOy) * mouseLInc.x);
//				mouseAngInc = mouseAngDir * 12;
//				allGears[currGearIndex]->rotate(mouseAngInc / 10);
//				std::cout << "inLeft" << std::endl;
//			}
//		}
//	}
//}

GearSystem::~GearSystem(){
	delete newGearPtr;
}



