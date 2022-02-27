#include <string>
#include <iostream>
#include <vector>

struct Point2D {
	float x, y;
};

class Gear {
private:
	std::string partID; // used for identification
	float pitch = 0.; // given in teeth per inch
	int numbTeeth = 0;
	Point2D loc = { 0,0 }; // current location of gear {x, y}
	float angle = 0; // current rotation of gear in degrees

	std::vector<Gear*> attachedGears;
	std::vector<Gear*> meshedGears;

public:
	// reads data from a file over-riding any existing data. 
	void load(std::ifstream& inFile);
	// outputs all the data for a gear in format matching attached example
	void print(std::ostream& output = std::cout);
	// calculates and return the pitch diameter
	float pitchDiam() { return numbTeeth / pitch; }
	// return the number of teeth
	int getNumbTeeth() { return numbTeeth; }
	// draws the gear on graphics window using simplified geometry as attached
	void draw();
	// return the pitch
	float getPitch() { return pitch; }
	// return the angle
	float getAngle() { return angle; }
	// return the origin location
	Point2D getLoc() { return loc; }

	// rotate the gear by given amount
	// and possibly cause the rotation of connected gears (attached and meshed)
	// except do not rotate the gear that is requesting the rotation
	void rotate(float rotAngle, Gear* requestingGear = nullptr);

	// goes through each property, showing existing values
	// and asking user to make changes
	void edit();

	std::string getID() { return partID; }
	void setID(const std::string& newID) { partID = newID; }

	// create a bi-directional attach relationship between this gear and
	// otherGear, unless bi-directional is not appropriate
	// returns false if connection cannot be done
	bool attachToGear(Gear* otherGear, bool biDirectional = true);

	// create a bi-directional meshed relationship between this gear and
	// otherGear, unless bi-directional is not appropriate
	// returns false if connection cannot be done (unmatched pitches)
	bool meshInto(Gear* otherGear, bool biDirectional = true);

	// ADDED: return pitch radius for boundary detection
	float getTopRad() { return ((float)numbTeeth / pitch / 2. + 1.25 / pitch); }
	Point2D getGearOrigin() { return loc; }
	//void dragGear(float addX, float addY) { loc.x += addX;  loc.y += addY; }

	~Gear();
};