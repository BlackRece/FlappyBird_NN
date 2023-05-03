#pragma once
#ifndef MATRIX_H
#define MATRIX_H

#include <valarray>
#include <stdexcept>
#include <iostream>
#include <functional>

#include "Structures.h"

class Matrix
{
public:
	Matrix() : m_iRows(1), m_iCols(1), m_dData(std::valarray<double>(1)) { ; }
	Matrix(unsigned int iRows, unsigned int iCols);
	Matrix(const Matrix& m);               // Copy constructor
	~Matrix();

	//Matrix& operator= (const Matrix& m);   // Assignment operator

	double& operator() (unsigned int iRows, unsigned int iCols);
	double  operator() (unsigned int iRows, unsigned int iCols) const;

	// BUG: methods returning a pointer are not being freed
	// TODO: create static methods that return a pointer to a new matrix
	// TODO: change member methods that return a pointer to a new matrix, sp that they operate and return a reference to the current matrix
	// TODO: ensure the usages of Matrix methods are used accordingly

	// static scalar functions
	static Matrix* add(const Matrix& m1, const double dScalar);
	static Matrix* sub(const Matrix& m1, const double dScalar);
	static Matrix* mul(const Matrix& m1, const double dScalar);
	static Matrix* div(const Matrix& m1, const double dScalar);

	// static matrix functions
	static Matrix* add(const Matrix& m1, const Matrix& m2);
	static Matrix* sub(const Matrix& m1, const Matrix& m2);
	static Matrix* mul(const Matrix& m1, const Matrix& m2);
	static Matrix* div(const Matrix& m1, const Matrix& m2);

	// member scalar functions
	void add(const double dScalar);
	void sub(const double dScalar);
	void mul(const double dScalar);
	void div(const double dScalar);

	// member matrix functions
	void add(const Matrix& m);
	void sub(const Matrix& m);
	void mul(const Matrix& m);
	void div(const Matrix& m);

	// manipulation functions
	static Matrix* dot(const Matrix& m1, const Matrix& m2);
	static Matrix* transpose(const Matrix& mSource);

	/// <summary>
	/// take the value at each index and apply the function to it, reutrn a new matrix
	/// </summary>
	Matrix* map(const std::function<double(int, int, double)>& func);
	Matrix* map(std::function<double(double)>& func);
	Matrix* map(std::function<double()>& func);

	// helper functions
	double* toArray() const;
	static Matrix* fromArray(const double dArray[], const int iArraySize, bool bIsCol = true);

	// json functions
	MatrixJson toJson() const;
	static Matrix* fromJson(MatrixJson& json);

	// debug functions
	void print() const;
	void random(int dVal = 10);
	void showRow(unsigned int iRow) const;
	void showCol(unsigned int iCol) const;

	// getters
	inline unsigned int getRowsCount() const { return m_iRows; }
	inline unsigned int getColCount() const { return m_iCols; }

private:
	unsigned m_iRows;
	unsigned m_iCols;

	std::valarray<double> m_dData;

	inline void checkBounds(const unsigned int iRows, const unsigned int iCols) const;
	inline void isSameSize(Matrix mMatrix);
	inline void isMatchingSize(const unsigned int lhs, const unsigned int rhs);

	inline static void isZeroSize(const unsigned int iRows, const unsigned int iCols);
	inline static void isSameSize(const Matrix& mMatrix1, const Matrix& mMatrix2);

	std::valarray<double> getRow(unsigned int iRow) const;
	std::valarray<double> getCol(unsigned int iCol) const;
};

#endif // !MATRIX_H
