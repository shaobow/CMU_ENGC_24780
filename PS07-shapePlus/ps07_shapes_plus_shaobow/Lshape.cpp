#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Lshape.h"

const float PI = 3.1415927;

void Lshape::readFile(std::ifstream& inFile) {
	std::string wholeLineString;
	std::stringstream wholeLineStream;
	bool continueReading = true;
	int colonLocation;

	// go through file
	while (!inFile.eof() && continueReading) {
		// read the whole line
		getline(inFile, wholeLineString);

		// find the colon and prepare to read from stringstream after the colon
		if ((colonLocation = wholeLineString.find(":")) != std::string::npos)
			wholeLineStream.str(wholeLineString.substr(colonLocation + 1));

		// when the find() function doesn't find it, string::npos is returned

		if (wholeLineString.find("locX") != std::string::npos) {
			wholeLineStream >> locX;
		}
		else if (wholeLineString.find("locY") != std::string::npos) {
			wholeLineStream >> locY;
		}
		else if (wholeLineString.find("height") != std::string::npos) {
			wholeLineStream >> height;
		}
		else if (wholeLineString.find("width") != std::string::npos) {
			wholeLineStream >> width;
		}
		else if (wholeLineString.find("thickness") != std::string::npos) {
			wholeLineStream >> thickness;
		}
		else if (wholeLineString.find("theta") != std::string::npos) {
			wholeLineStream >> theta;
			continueReading = false;
		}

		wholeLineStream.clear(); // get ready for next line
	}
	
	recalcSpline();
}

void Lshape::writeFile(std::ostream& output){
	output << "\t locX: " << locX << std::endl;
	output << "\t locY: " << locY << std::endl;
	output << "\t height: " << height << std::endl;
	output << "\t width: " << width << std::endl;
	output << "\t thickness: " << thickness << std::endl;
	output << "\t theta: " << theta << std::endl;
}

void Lshape::recalcSpline(){
	theGuides.clear();

	float tempX, tempY;
	tempX = locX; tempY = locY;
	theGuides.push_back(Node(tempX,tempY));
	tempX += width * cosf(PI/180.0f*theta); tempY += width * sinf(PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += thickness * cosf(PI / 180.0f * theta + PI / 2.0f); tempY += thickness * sinf(PI / 180.0f * theta + PI / 2.0f);
	theGuides.push_back(Node(tempX, tempY));
	tempX += (width-thickness) * cosf(PI+PI / 180.0f * theta); tempY += (width - thickness) * sinf(PI+PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += (height - thickness) * cosf(PI / 180.0f * theta + PI / 2.0f); tempY += (height - thickness) * sinf(PI / 180.0f * theta + PI / 2.0f);
	theGuides.push_back(Node(tempX, tempY));
	tempX += thickness * cosf(PI+ PI / 180.0f * theta); tempY += thickness * sinf(PI+ PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += height * cosf(1.50f*PI + PI / 180.0f * theta); tempY += height * sinf(1.50f * PI + PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));


	Shape::recalcSpline();
}