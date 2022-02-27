#pragma once
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

template <class T, const int NR, const int NC>
class Matrix2D {
private:

	// T content[NR][NC];  // not what I want
	T* content;

public:
	// Class constructor and destructor
	Matrix2D() {
		// allocate space for array
		content = new T[NR * NC];
	};
	~Matrix2D() {
		delete[] content;
	};

	// Sets value at given location, rows and columns are numbered 1 to whatever
	// Use the following if row or col are out-of-range
	// ->> throw std::out_of_range("Bad matrix operation");
	void set(int row, int col, const T& incoming);

	// Gets value at given location, throwing error similar to set()
	const T& value(int row, int col) const;

	// Loads matrix data from the file given. Returns false if unable to read.
	bool readFile(const std::string& fileName);

	// Prints the whole array to output, defaulting to cout.
	// If positive limit is given, only the first printLimit rows and columns
	// are printed rather than whole matrix (useful for checking big matrices).
	void print(std::ostream& output = std::cout, int printLimit = -1) const;

	// copy constructor
	Matrix2D(const Matrix2D<T, NR, NC>& original);

	// assignment operator
	Matrix2D<T, NR, NC>& operator=(const Matrix2D<T, NR, NC>& original);

	// Transposes the matrix in place.
	void transpose();

	// multiply the matrix by a scalar
	void scalarMultiply(const T& factor);

	// Multiplies the matrix with otherMatrix and stores the result in the
	// resultMatrix. Note that otherMatrix is the right-hand operand (since matrix
	// multiplication is NOT commutative)
	void matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix,
		Matrix2D <T, NR, NC>& resultMatrix);
};

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::set(int row, int col, const T& incoming)
{
	// matrices go from 1 to whatever
	// arrays go from zero to whatever
	if (1 <= row && row <= NR && 1 <= col && col <= NC)
		content[(row - 1) * NC + col - 1] = incoming;
	else
		throw std::out_of_range("Bad matrix operation on write.");
}

template<class T, int NR, int NC>
inline const T& Matrix2D<T, NR, NC>::value(int row, int col) const
{
	if (1 <= row && row <= NR && 1 <= col && col <= NC)
		return content[(row - 1) * NC + col - 1];
	else
		throw std::out_of_range("Bad matrix operation on read.");
}

template<class T, int NR, int NC>
inline bool Matrix2D<T, NR, NC>::readFile(const std::string& fileName)
{
	using namespace std;

	ifstream inFile(fileName);
	if (inFile.is_open()) {
		for (int i = 0; i < NR; i++)
			for (int j = 0; j < NC; j++)
				inFile >> content[i * NC + j];
		inFile.close();
		return true;
	}
	else
		return false;


}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::print(std::ostream& output, int printLimit) const
{
	using namespace std;

	int rowLimit = NR, colLimit = NC;
	if (printLimit > 0) {
		rowLimit = min(NR, printLimit);
		colLimit = min(NC, printLimit);
	}

	for (int i = 0; i < rowLimit; i++) {
		for (int j = 0; j < colLimit; j++) {
			output << setw(10) << setprecision(3) << content[i * NC + j] << " ";
		}
		output << endl;
	}
}

template<class T, int NR, int NC>
inline Matrix2D<T, NR, NC>::Matrix2D(const Matrix2D<T, NR, NC>& original)
{
	// copy all the member variables

	 // create new dynamic stuff
	content = new T[NR * NC];

	// copy into dynamic stuff using loop
	for (int i = 0; i < NR * NC; i++)
		content[i] = original.content[i];

	//for (int i = 1; i <= NR; i++) {
	//	for (int j = 1; j < +NC; j++) {
	//		set(i, j, original.get(i, j));
	//	}
	//}

}

template<class T, int NR, int NC>
inline Matrix2D<T, NR, NC>& Matrix2D<T, NR, NC>::operator=(const Matrix2D<T, NR, NC>& original) {
	// copy into dynamic stuff using loop
	for (int i = 0; i < NR * NC; i++)
		content[i] = original.content[i];

	return *this;  // so I could do matA = matB = matC;
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::transpose()
{
	if (NR != NC)
		throw std::out_of_range("Cannot transponse a non-square matrix");
	else {
		T temp;
		for (int i = 2; i <= NR; i++)
			for (int j = 1; j < i; j++) { // note upper bound of index j is NOT NC
				//temp = value(i, j);
				//set(i, j, value(j, i));
				//set(j, i, temp);
				std::swap(content[(i - 1) * NC + j - 1], content[(j - 1) * NC + i - 1]);
			}
	}
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::scalarMultiply(const T& factor)
{
	for (int i = 1; i <= NR; ++i)
		for (int j = 1; j <= NC; ++j)
			set(i, j, value(i, j) * factor);
			// content[(i-1)*NC+j-1] *= factor;

	// alternate method
	//for (int i = 0; i < NR * NC; i++)
	//	content[i] *= factor;
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D<T, NR, NC>& resultMatrix)
{
	T tempSum;
	for (int i = 1; i <= NR; i++) {
		for (int j = 1; j <= NC; j++) {
			tempSum = 0;
			for (int k = 1; k <= NC; k++)
				tempSum += value(i, k) * otherMatrix.value(k, j);

			resultMatrix.set(i, j, tempSum);
		}
	}
}

