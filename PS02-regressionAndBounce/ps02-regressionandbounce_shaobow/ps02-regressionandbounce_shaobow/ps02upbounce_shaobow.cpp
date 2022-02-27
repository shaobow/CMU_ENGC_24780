// ps02upbounce_shaobow.cpp : This script randomly creates a unique kinetics problem and asks for input, which will be checked with correct answer
// code written by Shaobo Wang from F21-24780B
// final version on 09/11/2021

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cmath>

using namespace std;

double getRandNumInRange(double lowerBound, double upperBound, double roundUnit) {
	int intLowerBound = round(lowerBound / roundUnit);
	int intUpperBound = round(upperBound / roundUnit);
	srand(time(NULL));
	int intRandNumInRange = (rand() % (intUpperBound - intLowerBound + 1)) + intLowerBound;
	double randNumInRange = (double)intRandNumInRange * roundUnit;
	return randNumInRange;
} // function that get rand number between given range and round unit e.g. one decimal->unit=0.1

double calculateCorrectAnswer(double m_B, double L, double s_0, double k_stif, double e_rest) {
	double v_B1 = sqrt(2 * (-m_B * 9.81 * (L + s_0) + .5 * k_stif * pow(s_0, 2)) / m_B);
	double v_B2 = e_rest * (-v_B1);
	double s_3 = (m_B * 9.81 + sqrt(pow(m_B * 9.81, 2) + 2 * k_stif * (m_B * 9.81 * L + .5 * m_B * pow(v_B2, 2)))) / k_stif;
	return s_3;
} // calculate the correct answer with random generated parameters

int main() {
	// parameter labels
	double mass, length, pullDistance, stiffness, coefRestitution, maxStretch;
	mass = getRandNumInRange(1., 2.5, .1); // m_B
	length = getRandNumInRange(.8, 1.5, .1); // L
	pullDistance = getRandNumInRange(.15 * length, .4 * length, .01); // s_0
	stiffness = getRandNumInRange(500, 1000, 50); // k
	coefRestitution = getRandNumInRange(.6, .9, .01); // e
	maxStretch = calculateCorrectAnswer(mass, length, pullDistance, stiffness, coefRestitution);
	//maxStretch = calculateCorrectAnswer(1.5, 1, 0.25, 800, 0.8);

	// create unique kinematic problem
	cout << "The ball B has a mass of " << mass << " kg and is suspended from the ceiling by a " << length << "-m-long elastic cord. If the cord is stretched downward " << pullDistance << " m and the ball is released from rest, determine how far the cord streches after the ball rebounds from the ceiling. The stiffness of the cord is k = " << stiffness << " N/m, and the coefficient of restitution between the ball and ceiling is e =" << coefRestitution << ". The ball makes a central impact with the ceiling. " << endl << "Please enter the maximum strerch of the cord after impact in meters: ";

	// check answer with error
	double userAnswer;
	cin >> userAnswer;
	double errorRate = abs((userAnswer - maxStretch) / maxStretch);
	if (errorRate <.05) {
		cout << "Your answer is correct! The error rate is " << errorRate * 100 << " %." << endl;
	}
	else {
		cout << "Your answer is wrong! Please check again." << endl;
	}

	return 0; 
}