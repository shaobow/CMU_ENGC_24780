#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Ishape.h"

const float PI = 3.1415927;

void Ishape::readFile(std::ifstream& inFile) {
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
		else if (wholeLineString.find("flangeT") != std::string::npos) {
			wholeLineStream >> flangeT;
		}
		else if (wholeLineString.find("webT") != std::string::npos) {
			wholeLineStream >> webT;
		}
		else if (wholeLineString.find("theta") != std::string::npos) {
			wholeLineStream >> theta;
			continueReading = false;
		}

		wholeLineStream.clear(); // get ready for next line
	}

	recalcSpline();
}

void Ishape::writeFile(std::ostream& output) {
	output << "\t locX: " << locX << std::endl;
	output << "\t locY: " << locY << std::endl;
	output << "\t height: " << height << std::endl;
	output << "\t width: " << width << std::endl;
	output << "\t flangeT: " << flangeT << std::endl;
	output << "\t webT: " << webT << std::endl;
	output << "\t theta: " << theta << std::endl;
}

void Ishape::recalcSpline() {
	theGuides.clear();

	float tempX, tempY;
	tempX = locX + webT/2.0f * cosf(PI / 180.0f * theta); 
	tempY = locY + webT/2.0f * sinf(PI / 180.0f * theta);
	tempX += (height - 2.0f * flangeT) / 2 * cosf(PI / 180.0f * theta + PI / 2.0f);
	tempY += (height - 2.0f * flangeT) / 2 * sinf(PI / 180.0f * theta + PI / 2.0f);
	theGuides.push_back(Node(tempX, tempY));
	tempX += (width - webT) / 2 * cosf(PI / 180.0f * theta);
	tempY += (width - webT) / 2 * sinf(PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += flangeT * cosf(PI / 180.0f * theta + PI / 2.0f);
	tempY += flangeT * sinf(PI / 180.0f * theta + PI / 2.0f);
	theGuides.push_back(Node(tempX, tempY));
	tempX += width * cosf(PI + PI / 180.0f * theta); 
	tempY += width * sinf(PI + PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += flangeT * cosf(1.50f * PI + PI / 180.0f * theta);
	tempY += flangeT * sinf(1.50f * PI + PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += (width - webT) / 2 * cosf(PI / 180.0f * theta);
	tempY += (width - webT) / 2 * sinf(PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += (height - 2.0f * flangeT) * cosf(1.50f * PI + PI / 180.0f * theta);
	tempY += (height - 2.0f * flangeT) * sinf(1.50f * PI + PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += (width - webT) / 2 * cosf(PI + PI / 180.0f * theta);
	tempY += (width - webT) / 2 * sinf(PI + PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += flangeT * cosf(1.50f * PI + PI / 180.0f * theta);
	tempY += flangeT * sinf(1.50f * PI + PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += width * cosf(PI / 180.0f * theta);
	tempY += width * sinf(PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += flangeT * cosf(PI / 180.0f * theta + PI / 2.0f);
	tempY += flangeT * sinf(PI / 180.0f * theta + PI / 2.0f);
	theGuides.push_back(Node(tempX, tempY));
	tempX += (width - webT) / 2 * cosf(PI + PI / 180.0f * theta);
	tempY += (width - webT) / 2 * sinf(PI + PI / 180.0f * theta);
	theGuides.push_back(Node(tempX, tempY));
	tempX += (height - 2.0f * flangeT) * cosf(PI / 180.0f * theta + PI / 2.0f);
	tempY += (height - 2.0f * flangeT) * sinf(PI / 180.0f * theta + PI / 2.0f);
	theGuides.push_back(Node(tempX, tempY));
	Shape::recalcSpline();
}