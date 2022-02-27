/*
Nestor Gomez
Carnegie Mellon University
24-780-B Engineering Computation
PS 09: Matrices

Due: Tues, Nov. 23, 2021

*/

#include <iostream>
#include <chrono>
#include "ps09_matrix2D_shaobow.h"


int main() {
	using namespace std;

	cout << " MATRIX TESTER " << endl;

	//Matrix2D<float, 4, 4> mat4, mat4a;
	Matrix2D<float, 4, 4> mat4;
	mat4.readFile("matrix4x4.txt");

	cout << "Matrix 4x4 read:" << endl;
	mat4.print();

	Matrix2D<float, 4, 4> mat4Other(mat4);

	auto begin = std::chrono::high_resolution_clock::now();
	
	mat4.transpose();

	double timeElapsed = chrono::duration_cast<chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - begin).count() / 1e9;

	cout << "Matrix 4x4 transposed:" << endl;
	mat4.print();
	cout << "Transpose of 4x4: elapsed " << timeElapsed << " sec." << endl;

	// deep copy test
	Matrix2D<float, 4, 4> mat4Transp;
	mat4Transp = mat4; // copy the transposed matrix
	mat4.transpose();  // set it back to how it was

	cout << "COPY of Matrix 4x4 transposed:" << endl;
	mat4Transp.print();

	
	// multiplying
	Matrix2D<float, 4, 4> result4;
	begin = std::chrono::high_resolution_clock::now();

	mat4.matrixMultiply(mat4, result4);

	timeElapsed = chrono::duration_cast<chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - begin).count() / 1e9;

	cout << "Matrix multiply result (squaring):" << endl;
	result4.print();
	cout << "Squaring of 4x4: elapsed " << timeElapsed << " sec." << endl;

	
	// ==================================
	Matrix2D<double, 500, 500> mat500;
	mat500.readFile("matrix500x500.txt");

	// multiplying
	Matrix2D<double, 500, 500> result500;
	begin = std::chrono::high_resolution_clock::now();

	mat500.matrixMultiply(mat500, result500);

	timeElapsed = chrono::duration_cast<chrono::nanoseconds>
		(std::chrono::high_resolution_clock::now() - begin).count() / 1e9;

	cout << "Matrix multiply result (squaring):" << endl;
	result500.print(cout, 10);
	cout << "Squaring of 500x500: elapsed " << timeElapsed << " sec." << endl;
	
}