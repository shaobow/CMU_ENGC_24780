# include <sstream>
# include <fstream>
# include <math.h>

# include "fssimplewindow.h"
# include "ysglfontdata.h"
#include "StringPlus.h"
#include "Gear.h"

const float PI = 3.1415926;

void Gear::load(std::ifstream& inFile)
{
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
		if (wholeLineString.find("partID") != std::string::npos) {
			partID = StringPlus::trim(
				wholeLineString.substr(colonLocation + 1));
		}
		else if (wholeLineString.find("pitch") != std::string::npos) {
			wholeLineStream >> pitch;
		}
		else if (wholeLineString.find("numbTeeth") != std::string::npos) {
			wholeLineStream >> numbTeeth;
		}
		else if (wholeLineString.find("startX") != std::string::npos) {
			wholeLineStream >> loc.x;
		}
		else if (wholeLineString.find("startY") != std::string::npos) {
			wholeLineStream >> loc.y;
		}
		else if (wholeLineString.find("startAngle") != std::string::npos) {
			wholeLineStream >> angle;
		}

		else if (wholeLineString.find("Gear End") != std::string::npos) {
			continueReading = false;
		}

		wholeLineStream.clear(); // get ready for next line
	}
}

void Gear::print(std::ostream& output) {
	output << "Gear:" << std::endl;
	output << "\t partID: " << partID << std::endl;
	output << "\t pitch: " << pitch << std::endl;
	output << "\t numbTeeth: " << numbTeeth << std::endl;
	output << "\t startX: " << loc.x << std::endl;
	output << "\t startY: " << loc.y << std::endl;
	output << "\t startAngle: " << angle << std::endl;
	output << "Gear End :" << std::endl;
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

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x1C47);
	glBegin(GL_LINES);
	glVertex2f(0,0);
	glVertex2f(1.2 * p2Draw[1].x, (p2Draw[1].y + p2Draw[2].y) / 2);
	glEnd();  // indicate line
	glDisable(GL_LINE_STIPPLE);

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

void Gear::rotate(float rotAngle, Gear* requestingGear){
	angle += rotAngle;
	angle = fmod(angle, 360.);
	for (Gear* &currGear : attachedGears) {
		if (currGear != requestingGear) 
			currGear->rotate(rotAngle, this);
	}
	for (Gear* &currGear : meshedGears) {
		if (currGear != requestingGear) {
			float meshRotAngle = -rotAngle * numbTeeth / currGear->getNumbTeeth();
			currGear->rotate(meshRotAngle, this);
		}
	}
}

void Gear::edit(){
	std::cout << "\nChange partID into >";
	std::cin >> partID;
	std::cout << "\nChange pitch into >";
	std::cin >> pitch;
	std::cout << "\nChange numberTeeth into >";
	std::cin >> numbTeeth;
	std::cout << "\nChange currentX into >";
	std::cin >> loc.x;
	std::cout << "\nChange currentY into >";
	std::cin >> loc.y;
	std::cout << "\nChange current angle:";
	std::cin >> angle;
	std::cout << "\nGear information edited!" << std::endl;
}

bool Gear::attachToGear(Gear* otherGear, bool biDirectional){
	attachedGears.push_back(otherGear);
	loc = otherGear->getLoc();
	if (biDirectional)
		otherGear->attachToGear(this, false);
	return true;
}

bool Gear::meshInto(Gear* otherGear, bool biDirectional){
	if (otherGear->getPitch() != pitch) 
		return false;
	else {
		int otherNumbTeeth = otherGear->getNumbTeeth();

		float distance = (pitchDiam() + otherGear->pitchDiam()) / 2;
		Point2D otherLoc = otherGear->getLoc();

		float teethAngle = 360. / numbTeeth;
		float originLineAngle = atan2f(loc.y - otherLoc.y, loc.x - otherLoc.x);

		meshedGears.push_back(otherGear);

		if (biDirectional) {
			float originLineDegree = originLineAngle * 180 / PI;
			loc.x = otherLoc.x + distance * cosf(originLineAngle);
			loc.y = otherLoc.y + distance * sinf(originLineAngle);
			angle = 180 + originLineDegree + teethAngle / 2 - (otherGear->getAngle() - originLineDegree) * otherNumbTeeth / numbTeeth;
			otherGear->meshInto(this, false);
		}
		return true;
	}
}

Gear::~Gear(){
	for (Gear*& currGear : meshedGears) {
		auto meshPos = find(currGear->meshedGears.begin(), currGear->meshedGears.end(), this);
		currGear->meshedGears.erase(meshPos);
	}
	for (Gear*& currGear : attachedGears) {
		auto attachPos = find(currGear->attachedGears.begin(), currGear->attachedGears.end(), this);
		currGear->attachedGears.erase(attachPos);
	}
}
