# include <sstream>
# include <fstream>
# include <math.h>

# include "fssimplewindow.h"
# include "ysglfontdata.h"
# include "StringPlus.h"

# include "Gear.h"

const float PI = 3.1415926;
using namespace std;

void Gear::load(std::ifstream& inFile)
{
	string wholeLineString;
	stringstream wholeLineStream;
	bool continueReading = true;
	int colonLocation;

	// go through file
	while (!inFile.eof() && continueReading) {
		// read the whole line
		getline(inFile, wholeLineString);

		// find the colon and prepare to read from stringstream after the colon
		if ((colonLocation = wholeLineString.find(":")) != string::npos)
			wholeLineStream.str(wholeLineString.substr(colonLocation + 1));

		// when the find() function doesn't find it, string::npos is returned
		if (wholeLineString.find("partID") != string::npos) {
			partID = StringPlus::trim(
				wholeLineString.substr(colonLocation + 1));
		}
		else if (wholeLineString.find("pitch") != string::npos) {
			wholeLineStream >> pitch;
		}
		else if (wholeLineString.find("numbTeeth") != string::npos) {
			wholeLineStream >> numbTeeth;
		}
		else if (wholeLineString.find("startX") != string::npos) {
			wholeLineStream >> loc.x;
		}
		else if (wholeLineString.find("startY") != string::npos) {
			wholeLineStream >> loc.y;
		}
		else if (wholeLineString.find("startAngle") != string::npos) {
			wholeLineStream >> angle;
		}

		else if (wholeLineString.find("Gear End") != string::npos) {
			continueReading = false;
		}

		wholeLineStream.clear(); // get ready for next line
	}
}

void Gear::print(std::ostream& output) {
	output << "Gear:" << endl;
	output << "\t partID: " << partID << endl;
	output << "\t pitch: " << pitch << endl;
	output << "\t numbTeeth: " << numbTeeth << endl;
	output << "\t startX: " << loc.x << endl;
	output << "\t startY: " << loc.y <<endl;
	output << "\t startAngle: " << angle << endl;
	output << "Gear End :" << endl;
}

void Gear::draw() {
	float module = 1 / pitch;
	float teethAngle = 360. / numbTeeth;
	float dedendum = 1.25 * module;
	float addendum = module;
	float widToothBottom = 1.8 * module;
	float widToothTop = module;
	float pitchRad = numbTeeth / pitch / 2;
	float toothBottomRad = sqrtf(pow(widToothBottom / 2, 2) + pow(pitchRad - dedendum, 2));
	float theta = teethAngle - 180 * atan2f(widToothBottom / 2, pitchRad - dedendum) / PI;

	Point2D p1 = { pitchRad - dedendum , -widToothBottom / 2 };
	Point2D p2 = { pitchRad + addendum , -widToothTop / 2 };
	Point2D p3 = { pitchRad + addendum , widToothTop / 2 };
	Point2D p4 = { pitchRad - dedendum , widToothBottom / 2 };
	Point2D p5 = { toothBottomRad * cosf(theta / 180 * PI) , toothBottomRad * sinf(theta / 180 * PI) };

	Point2D p2Draw[5] = { p1,p2,p3,p4,p5 };

	glTranslatef(loc.x, loc.y, 0); // translate to given origin
	glRotatef(angle, 0, 0, 1); // rotate to current angle in degrees

	for (int i = 1; i <= numbTeeth; i++) { 
		glBegin(GL_LINES);
		for (int j = 0; j < 4; j++) {
			glVertex2f(p2Draw[j].x, p2Draw[j].y);
			glVertex2f(p2Draw[j + 1].x, p2Draw[j + 1].y);
		}
		glEnd();

		glRotatef(teethAngle, 0, 0, 1);
	}
	glRotatef(angle, 0, 0, -1);
	glTranslatef(-loc.x, -loc.y, 0);
}
