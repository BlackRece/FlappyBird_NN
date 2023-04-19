#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <valarray>
#include <stdexcept>
#include <iostream>
#include <functional>

class Matrix
{
public:
	Matrix() :m_iRows(1), m_iCols(1), m_dData(std::valarray<double>(1)) { ; }
	inline Matrix(unsigned iRows, unsigned iCols);
	~Matrix() { ; }

	//Matrix(const Matrix& m);               // Copy constructor
	//Matrix& operator= (const Matrix& m);   // Assignment operator

	double& operator() (unsigned iRows, unsigned iCols);
	double  operator() (unsigned iRows, unsigned iCols) const;

	// math functions
	inline Matrix operator+(const double& dScalar) { return add(dScalar); }
	inline Matrix operator+(const Matrix& mMatrix) { return add(mMatrix); }
	inline Matrix operator-(const double& dScalar) { return sub(dScalar); }
	inline Matrix operator-(const Matrix& mMatrix) { return sub(mMatrix); }
	inline Matrix operator*(const double& dScalar) { return mul(dScalar); }
	inline Matrix operator*(const Matrix& mMatrix) { return mul(mMatrix); }
	inline Matrix operator/(const double& dScalar) { return div(dScalar); }
	inline Matrix operator/(const Matrix& mMatrix) { return div(mMatrix); }

	Matrix add(const double dScalar);
	Matrix sub(const double dScalar);
	Matrix mul(const double dScalar);
	Matrix div(const double dScalar);

	Matrix add(const Matrix& mSource);
	Matrix sub(const Matrix& mSource);
	Matrix mul(const Matrix& mSource);
	Matrix div(const Matrix& mSource);

	Matrix dot(const Matrix& mSource);

	// manipulation functions
	Matrix transpose() const;

	/// <summary>
	/// take the value at each index and apply the function to it, reutrn a new matrix
	/// </summary>
	Matrix map(const std::function<double(int, int, double)>& func);
	Matrix map(std::function<double(double)>& func);
	Matrix map(std::function<double()>& func);

	// helper functions
	Matrix fromArray(const double dArray[], const int iArraySize, bool bIsCol = true);
	double* toArray() const;

	// debug functions
	void print() const;
	void random(int dVal = 10);
	inline void showRow(unsigned iRow) const;
	inline void showCol(unsigned iCol) const;

private:
	unsigned m_iRows;
	unsigned m_iCols;

	std::valarray<double> m_dData;

	inline void checkSize(unsigned iRows, unsigned iCols) const;
	inline void checkSize(Matrix mMatrix) const;
	inline void checkBounds(unsigned iRows, unsigned iCols) const;

	std::valarray<double> getRow(unsigned iRow) const;
	std::valarray<double> getCol(unsigned iCol) const;
};

#endif // !MATRIX_H
