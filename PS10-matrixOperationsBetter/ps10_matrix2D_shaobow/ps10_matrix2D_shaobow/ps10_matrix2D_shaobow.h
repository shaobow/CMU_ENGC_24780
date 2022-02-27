#pragma once
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <vector>

template <class T, const int NR, const int NC>
class Matrix2D {
private:
	T* content;

public:
	Matrix2D();
	~Matrix2D();

	// Sets value at given location, rows and columns are numbered 1 to whatever
	// Use the following if row or col are out-of-range
	// ->> throw std::out_of_range("Bad matrix operation");
	void set(int row, int col, const T& incoming);

	// Gets value at given location, throwing error similar to set()
	const T& value(int row, int col) const;

	// Loads matrix data from the file given
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
	//void matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D <T, NR, NC>& resultMatrix);
	void matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D <T, NR, NC>& resultMatrix, int startRow = 1, int endRow = NR, int startCol = 1, int endCol = NC);

	// similar to matrixMultiply, but uses multi-threading
	void matrixMultiplyParallel(Matrix2D<T, NC, NR>& otherMatrix,
		Matrix2D <T, NR, NR>& resultMatrix);

	// thread entry static function
	public: static void threadEntry(Matrix2D<T, NR, NC>* thisPtr);

	Matrix2D<T, NR, NC>* rightHandMatrixPtr;
	Matrix2D<T, NR, NC>* resultMatrixPtr;
	int currThread;
};

template<class T, int NR, int NC>
inline Matrix2D<T, NR, NC>::Matrix2D()
{
	// allocate space for array
	content = new T[NR * NC];
}

template<class T, int NR, int NC>
inline Matrix2D<T, NR, NC>::~Matrix2D()
{
	delete[] content;
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::set(int row, int col, const T& incoming)
{
	if (1 <= row && row <= NR && 1 <= col && col <= NC)
		content[(row - 1) * NC + (col - 1)] = incoming;
	else
		throw std::out_of_range("Error: invalid index");
}

template<class T, int NR, int NC>
inline const T& Matrix2D<T, NR, NC>::value(int row, int col) const
{
	if (1 <= row && row <= NR && 1 <= col && col <= NC)
		return content[(row - 1) * NC + (col - 1)];
	else
		throw std::out_of_range("Error: invalid index");
	
}

template<class T, int NR, int NC>
inline bool Matrix2D<T, NR, NC>::readFile(const std::string& fileName)
{
	std::ifstream inFile(fileName);
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
	//int rowLimit = NR, colLimit = NC;
	//if (printLimit > 0) {
	//	rowLimit = std::min(NR, rowLimit);
	//	colLimit = std::min(NC, colLimit);
	//}
	//for (int i = 0; i < rowLimit; i++) {
	//	for (int j = 0; j < colLimit; j++) {
	//		output << std::setw(10) << std::setprecision(3) << content[i * NC + j] << " ";
	//	}
	//	output << std::endl;

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
	content = new T[NR * NC];

	for (int i = 0; i < NR * NC; i++)
		content[i] = original.content[i];
}


template<class T, int NR, int NC>
inline Matrix2D<T, NR, NC>& Matrix2D<T, NR, NC>::operator=(const Matrix2D<T, NR, NC>& original)
{
	for (int i = 0; i < NR * NC; i++)
		content[i] = original.content[i];
	return *this;
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::transpose()
{
	for (int i = 0; i < NR; i++)
		for (int j = 0; j < NC; j++)
			if (i > j) std::swap(content[i * NC + j], content[j * NC + i]);
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::scalarMultiply(const T& factor)
{
	for (int i = 0; i < NR * NC; i++)
		content[i] = factor*content[i];
}

//template<class T, int NR, int NC>
//inline void Matrix2D<T, NR, NC>::matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D<T, NR, NC>& resultMatrix)
//{
//
//	for (int i = 0; i < NR; i++)
//		for (int j = 0; j < NC; j++) {
//			resultMatrix.content[i * NC + j] = 0;
//			for (int k = 0; k < NR; k++)
//				resultMatrix.content[i * NC + j] += content[i * NC + k] * otherMatrix.content[k * NR + j];
//		}
//}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D<T, NR, NC>& resultMatrix, int startRow, int endRow, int startCol, int endCol)
{

	for (int i = startRow - 1; i < endRow; i++)
		for (int j = startCol - 1; j < endCol; j++) {
			resultMatrix.content[i * NC + j] = 0;
			for (int k = 0; k < NR; k++)
				resultMatrix.content[i * NC + j] += content[i * NC + k] * otherMatrix.content[k * NR + j];
		}
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::matrixMultiplyParallel(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D<T, NR, NR>& resultMatrix)
{
	int multiThreadCutoff = 10;
	if (NR <= multiThreadCutoff)
		matrixMultiply(otherMatrix, resultMatrix);
	else {
		using namespace std;
		int numbThreads = 4;

		// prepare for multi-threading
		vector<thread> theThreads;
		theThreads.resize(numbThreads);

		rightHandMatrixPtr = &otherMatrix;
		resultMatrixPtr = &resultMatrix;
		currThread = 1;
		int lastThread;

		// start the threads in a loop
		for (int i = 0; i < numbThreads; i++) {
			// start a thread
			lastThread = currThread;
			thread aThread(Matrix2D<T, NR, NC>::threadEntry, this);
			// swap thread into vector
			theThreads[i].swap(aThread);

			// wait until thread has really
			while (lastThread == currThread);
		}

		// wait for all threads to finish
		for (int i = 0; i < numbThreads; i++) {
			theThreads[i].join();
		}

	}
}

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::threadEntry(Matrix2D<T, NR, NC>* thisPtr)
{
	switch (thisPtr->currThread)
	{
	//============2x2===============
	case 1:
		thisPtr->currThread++;
		thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 1, NR / 2, 1, NC / 2);
		break;
	case 2:
		thisPtr->currThread++;
		thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 1, NR / 2, NC / 2 + 1, NC);
		break;
	case 3:
		thisPtr->currThread++;
		thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), NR / 2 + 1, NR, 1, NC / 2);
		break;
	case 4:
		thisPtr->currThread++;
		thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), NR / 2 + 1, NR, NC / 2 + 1, NC);
		break;
	
	//============3x3===============
	//case 1:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 1, NR / 3, 1, NC / 3);
	//	break;
	//case 2:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 1, NR / 3, NC / 3 + 1, 2 * NC / 3);
	//	break;
	//case 3:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 1, NR / 3, 2 * NC / 3 + 1, NC);
	//	break;
	//case 4:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), NR / 3 + 1, 2 * NR / 3, 1, NC / 3);
	//	break;
	//case 5:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), NR / 3 + 1, 2 * NR / 3, NC / 3 + 1, 2 * NC / 3);
	//	break;
	//case 6:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), NR / 3 + 1, 2 * NR / 3, 2 * NC / 3 + 1, NC);
	//	break;
	//case 7:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 2 * NR / 3 + 1, NR, 1, NC / 3);
	//	break;
	//case 8:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 2 * NR / 3 + 1, NR, NC / 3 + 1, 2 * NC / 3);
	//	break;
	//case 9:
	//	thisPtr->currThread++;
	//	thisPtr->matrixMultiply(*(thisPtr->rightHandMatrixPtr), *(thisPtr->resultMatrixPtr), 2 * NR / 3 + 1, NR, 2 * NC / 3 + 1, NC);
	//	break;

	default:
		break;
	}
}

