#include "GearSystem.h"
#include "fssimplewindow.h"

int main(void)
{
	const int width = 800, height = 600;
	float scalar = 0., originX = 0., originY = 0.;
	bool terminate = false;

	GearSystem gearSys;

	int key;
	int mouseEvent;
	mouseInfo myMouse;
	myMouse.height = height;

	FsOpenWindow(16, 16, width, height, 1, "Gear System Viewer (SW2021)"); // use double buffer

	gearSys.showMenu();

	while (!terminate) {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		FsPollDevice();

		key = FsInkey();
		bool shiftIsOn = FsGetKeyState(FSKEY_SHIFT);  // shift key combo
		//bool ctrlIsOn = FsGetKeyState(FSKEY_CTRL);  // ctrl key combo

		myMouse.originX = originX;
		myMouse.originY = originY;
		myMouse.scalar = scalar;
		mouseEvent = FsGetMouseEvent(myMouse.leftButton, myMouse.middleButton, myMouse.rightButton, myMouse.locX, myMouse.locY);

		gearSys.mouseClick(myMouse, mouseEvent, shiftIsOn);  // click to select current, shift+click to select other

		//gearSys.mouseRotateAndDrag(myMouse, mouseEvent);

		switch (key) {
		case FSKEY_ESC:
		case FSKEY_X:   // exit program
			terminate = true;
			break;
		case FSKEY_UP: // prev gear
			if (shiftIsOn) 
				gearSys.changeOther(-1);
			else
				gearSys.changeCurrent(-1);
			break;
		case FSKEY_DOWN: // next gear
			if (shiftIsOn)
				gearSys.changeOther(1);
			else
				gearSys.changeCurrent(1);
			break;
		case FSKEY_LEFT: // rotate CCW
			gearSys.rotateCurrent(10);
			break;
		case FSKEY_RIGHT: // rotate CW
			gearSys.rotateCurrent(-10);
			break;
		case FSKEY_L: // list all data and redisplay menu
			gearSys.printCurrent();
			break;
		case FSKEY_A: // add a curve
			gearSys.addGear();
			gearSys.getOriginAndScale(originX, originY, scalar);
			break;
		case FSKEY_D: // delete a gear, if there are any in the collection
			gearSys.deleteCurrent();
			gearSys.getOriginAndScale(originX, originY, scalar);
			break;
		case FSKEY_E:
			gearSys.editCurrent();
			gearSys.getOriginAndScale(originX, originY, scalar);
			break;
		case FSKEY_T:
			gearSys.attachGears();
			gearSys.getOriginAndScale(originX, originY, scalar);
			break;
		case FSKEY_M:
			if (!gearSys.meshGears()) {
				std::cout << std::endl;
				std::cout << "Gears to mesh do not match!" << std::endl;
				gearSys.showMenu();
			}
			gearSys.getOriginAndScale(originX, originY, scalar);
			break;
		case FSKEY_S:
			gearSys.saveAll();
			break;
		default:
			break;
		}
		
		//adjust the graphics plane
		glTranslated(0, height, 0); // make bottom-left the origin
		glTranslated(originX, originY, 0);
		glScaled(scalar, -scalar, 1.);  // scale everything by scalar, with positive y going up

		gearSys.draw();

		// reset transformation matrix completely
		glLoadIdentity();

		FsSwapBuffers();  // use this instead of glFlush() when using double buffer
		FsSleep(20);
	}

	gearSys.~GearSystem();
}