#pragma once
#include "Gear.h"

struct mouseInfo {
	int leftButton;
	int middleButton;
	int rightButton;
	int locX;
	int locY;
	int mouseEvent;
	float originX;
	float originY;
	float scalar;
	int height;
	int preLocX = 0;
	int preLocY = 0;
};

class GearSystem {	
private:
	std::vector<Gear*> allGears;
	int currGearIndex;
	int otherGearIndex;

public:
	// draws all gears, highlighting current gear and optionally
	// highlighting otherGear.
	void draw(bool highlightOther = true);

	// displays helpful message on graphics window
	void showMenu();

	// displays helpful message on graphics window
	void inputRequiredMessage();

	// rotates the current gear
	void rotateCurrent(float deg);

	// removes current gear, with confirmation
	void deleteCurrent();

	// edits current gear
	void editCurrent();

	// prints current gear to console
	void printCurrent();

	// changes the current gear, being careful to "loop around " as needed
	void changeCurrent(int change);

	// changes the other gear, being careful to "loop around " as needed
	// but skipping over current gear
	void changeOther(int change);

	// asks for filename and loads a gear or gears,
	// adding it/them to system
	void addGear();

	// returns a pointer to the gear that matches the givenID,
	// returns nullptr if not found
	Gear* getGear(const std::string& gearID);

	// returns a pointer to the gear at given index,
	// returns nullptr if not found
	Gear* getGear(int gearIndex);

	// figures out best origin and scale for display
	void getOriginAndScale(float& originX, float& originY, float& scale);

	// attaches the current gear to the other gear
	bool attachGears();

	// meshes the current gear into the other gear
	bool meshGears();

	// asks for a file name and saves all gears in a single file
	void saveAll();

	void mouseClick(mouseInfo& mymouse, int& mouseEvent, bool shiftIsOn);

	void mouseRotateAndDrag(mouseInfo& mymouse, int& mouseEvent);

	~GearSystem();
};

