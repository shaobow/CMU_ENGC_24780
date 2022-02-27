#pragma once
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

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
	void matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D <T, NR, NC>& resultMatrix);
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
	int rowLimit = NR, colLimit = NC;
	if (printLimit > 0) {
		rowLimit = std::min(NR, rowLimit);
		colLimit = std::min(NC, colLimit);
	}
	for (int i = 0; i < rowLimit; i++) {
		for (int j = 0; j < colLimit; j++) {
			output << std::setw(10) << std::setprecision(3) << content[i * NC + j] << " ";
		}
		output << std::endl;
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

template<class T, int NR, int NC>
inline void Matrix2D<T, NR, NC>::matrixMultiply(Matrix2D<T, NC, NR>& otherMatrix, Matrix2D<T, NR, NC>& resultMatrix)
{
	for (int i = 0; i < NR; i++)
		for (int j = 0; j < NC; j++) {
			resultMatrix.content[i * NC + j] = 0;
			for (int k = 0; k < NR; k++)
				resultMatrix.content[i * NC + j] += content[i * NC + k] * otherMatrix.content[k * NR + j];
		}
}

