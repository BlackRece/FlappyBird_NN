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
	inline Matrix(unsigned iRows, unsigned iCols);
	~Matrix() { ; }

	Matrix(const Matrix& m);               // Copy constructor
	Matrix& operator= (const Matrix& m);   // Assignment operator

	double& operator() (unsigned iRows, unsigned iCols);
	double  operator() (unsigned iRows, unsigned iCols) const;

	inline Matrix operator+(const double& dScalar) { return add(dScalar); }
	inline Matrix operator+(const Matrix& mMatrix) { return add(mMatrix); }
	inline Matrix operator*(const double& dScalar) { return mul(dScalar); }
	inline Matrix operator*(const Matrix& mMatrix) { return mul(mMatrix); }

	Matrix add(const double dScalar);
	Matrix add(const Matrix& mSource);
	Matrix mul(const double dScalar);
	Matrix mul(const Matrix& mSource);
	Matrix dot(const Matrix& mSource);
	Matrix transpose() const;

	/// <summary>
	/// take the value at each index and apply the function to it, reutrn a new matrix
	/// </summary>
	Matrix map(const std::function<double(int, int, double)>& func);

	void print() const;
	void random();
	inline void showRow(unsigned iRow) const {
		auto r = getRow(iRow); for (double v : r) std::cout << v << " ";
	}
	inline void showCol(unsigned iCol) const {
		auto c = getCol(iCol); for (double v : c) std::cout << v << " ";
	}

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
