// ps02leastsquares_shaobow.cpp : This script solves coefficients of quadratic regression of spring curve with least squares approach
// code written by Shaobo Wang from F21-24780B
// final version on 09/10/2021

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

void calculateSum(double sum[5], double x_in, double y_in) {
	sum[0] += x_in; // sumX
	sum[1] += pow(x_in,2); // sumX2
	sum[2] += pow(x_in, 3); // sumX3
	sum[3] += y_in; // sumY
	sum[4] += x_in * y_in; // sumXY
} // calculate coefficients in summation form

void solveAn(double sum[5],double a_n[2]) {
	a_n[0] = (sum[3] * sum[2] - sum[4] * sum[1]) / (sum[0] * sum[2] - sum[1] * sum[1]);
	a_n[1] = (sum[3] * sum[1] - sum[4] * sum[0]) / (sum[1] * sum[1] - sum[0] * sum[2]);
}// solve an with pre-defined solutions

int main() {
	ifstream inFile;
	string fileName;
	cout << "Please input the data file's name:";
	cin >> fileName; // SpringData01.txt

	double sumCoefficient[] = { 0,0,0,0,0 };
	double anValue[] = { 0,0 };
	double xInput;
	double yInput;

	inFile.open(fileName);
	if (inFile.is_open()) {
		while(!inFile.eof()) {
			inFile >> xInput >> yInput;
			if (!inFile.eof()) {
				//cout << xInput << "\t" << yInput << endl;
				calculateSum(sumCoefficient, xInput, yInput);
			}
			//for (int i = 0; i < 5; i++) {
			//	cout << sumCoefficient[i] << "\t";
			//}
			//cout << endl;
		}
	}
	else {
		cout << "File name invalid!" << endl;
		return 1;
	}

	solveAn(sumCoefficient, anValue);

	cout << "a_1=" << anValue[0] << "\ta_2=" << anValue[1] << endl;

	return 0;
}

