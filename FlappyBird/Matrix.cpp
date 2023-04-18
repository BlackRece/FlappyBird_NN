#include "Matrix.h"

Matrix::Matrix(unsigned iRows, unsigned iCols)
	: m_iRows(iRows), m_iCols(iCols)
{
	m_dData = std::valarray<double>(m_iRows * m_iCols);
}

double& Matrix::operator()(unsigned iRows, unsigned iCols)
{
	checkBounds(iRows, iCols);
	return m_dData[m_iCols * iRows + iCols];
}

double Matrix::operator()(unsigned iRows, unsigned iCols) const
{
	checkBounds(iRows, iCols);
	return m_dData[m_iCols * iRows + iCols];
}

Matrix Matrix::add(const double dScalar)
{
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] + dScalar;
	return m;
}

Matrix Matrix::add(const Matrix& mSource)
{
	checkSize(mSource.m_iRows, mSource.m_iCols);
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] + mSource.m_dData[i];
	return m;
}

Matrix Matrix::mul(const double dScalar)
{
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] * dScalar;
	return m;
}

Matrix Matrix::mul(const Matrix& mSource)
{
	// element-wise multiplication
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] * mSource.m_dData[i];
	return m;
}

Matrix Matrix::dot(const Matrix& mSource)
{
	// matrix multiplication
	if (m_iRows != mSource.m_iCols)
	{
		throw std::invalid_argument("Matrix sizes do not match");
		Matrix errorMatrix(1, 1);
		return errorMatrix;
	}
	Matrix m(m_iRows, mSource.m_iCols);
	for (int i = 0; i < m.m_iRows; i++)
		for (int j = 0; j < m.m_iCols; j++)
			m(i, j) = (getRow(i) * mSource.getCol(j)).sum();

	/*
	for (int i = 0; i < m_iRows; i++)
		for (int j = 0; j < mSource.m_iCols; j++)
			for (int k = 0; k < m_iCols; k++)
				m(i, j) += (*this)(i, k) * mSource(k, j);
	*/
	return m;
}

Matrix Matrix::transpose() const
{
	Matrix m(m_iCols, m_iRows);
	for (int i = 0; i < m_iRows; i++)
		for (int j = 0; j < m_iCols; j++)
			m(j, i) = (*this)(i, j);
	return m;
}

Matrix Matrix::map(const std::function<double(int, int, double)>& func)
{
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows; i++)
		for (int j = 0; j < m_iCols; j++)
			m(i, j) = func(i, j, (*this)(i, j));
	return m;
}

Matrix Matrix::map(std::function<double(double)>& func)
{
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows; i++)
		for (int j = 0; j < m_iCols; j++)
			m(i, j) = func((*this)(i, j));
	return m;
}

Matrix Matrix::map(std::function<double()>& func) {
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows; i++) {
		for (int j = 0; j < m_iCols; j++) {
			m(i, j) = func();
		}
	}
	return m;
}

Matrix Matrix::fromArray(const double dArray[], const int iArraySize, bool bIsCol)
{
	Matrix m = bIsCol
		? Matrix(1, iArraySize)
		: Matrix(iArraySize, 1);

	for (int i = 0; i < iArraySize; i++)
	{
		if (bIsCol)
			m(0, i) = dArray[i];
		else
			m(i, 0) = dArray[i];
	}

	return m;
}

double* Matrix::toArray() const
{
	double* dArray = new double[m_iRows * m_iCols];
	for (int i = 0; i < m_iRows * m_iCols; i++)
		dArray[i] = m_dData[i];
	return dArray;
}

void Matrix::print() const
{
	for (int i = 0; i < m_iRows; i++)
	{
		for (int j = 0; j < m_iCols; j++)
			std::cout << (*this)(i, j) << " ";
		std::cout << std::endl;
	}
}

void Matrix::random(int dVal)
{
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] = rand() % dVal;
}

inline void Matrix::checkBounds(unsigned iRows, unsigned iCols) const
{
	if (iRows >= m_iRows || iCols >= m_iCols)
		throw std::out_of_range("Matrix subscript out of bounds");
}

std::valarray<double> Matrix::getRow(unsigned iRow) const
{
	std::valarray<double> v(m_iCols);
	for (int i = 0; i < m_iCols; i++)
		v[i] = (*this)(iRow, i);
	return v;
}

std::valarray<double> Matrix::getCol(unsigned iCol) const
{
	std::valarray<double> v(m_iRows);
	for (int i = 0; i < m_iRows; i++)
		v[i] = (*this)(i, iCol);
	return v;
}

inline void Matrix::checkSize(unsigned iRows, unsigned iCols) const
{
	if (iRows <= 0 || iCols <= 0)
		throw std::invalid_argument("Matrix constructor has 0 size");
}

inline void Matrix::checkSize(Matrix mMatrix) const
{
	if (mMatrix.m_iRows != m_iRows || mMatrix.m_iCols != m_iCols)
		throw std::invalid_argument("Matrix sizes do not match");
}