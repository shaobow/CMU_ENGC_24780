# include <string>
# include <iostream>

using namespace std;

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

public:
	// reads data from a file over-riding any existing data. 
	void load(std::ifstream& inFile);
	// outputs all the data for a gear in format matching attached example
	void print(std::ostream& output = std::cout);
	// calculates and return the pitch diameter
	float pitchDiam() { return numbTeeth / pitch; };
	// draws the gear on graphics window using simplified geometry as attached
	void draw();
	// rotate the gear by given amount
	void rotate(float rotAngle) { angle += rotAngle; };


	std::string getID() { return partID; }
	void setID(const std::string& newID) { partID = newID; }

	// ADDED: return pitch radius for boundary detection
	float getTopRad() { return ((float)numbTeeth / pitch / 2. + 1.25 / pitch); }
	Point2D getGearOrigin() { return loc; }
	void dragGear(float addX, float addY) { loc.x += addX;  loc.y += addY; }
};