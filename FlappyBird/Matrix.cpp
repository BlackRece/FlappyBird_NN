#include "Matrix.h"
using json = nlohmann::json;

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

Matrix Matrix::sub(const double dScalar)
{
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] - dScalar;
	return m;
}

Matrix Matrix::mul(const double dScalar)
{
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] * dScalar;
	return m;
}

Matrix Matrix::div(const double dScalar)
{
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] / dScalar;
	return m;
}

Matrix Matrix::add(const Matrix& mSource)
{
	checkSize(m_iRows, mSource.m_iRows);
	checkSize(m_iCols, mSource.m_iCols);
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] + mSource.m_dData[i];
	return m;
}

Matrix Matrix::sub(const Matrix& mSource)
{
	checkSize(m_iRows, mSource.m_iRows);
	checkSize(m_iCols, mSource.m_iCols);
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] - mSource.m_dData[i];
	return m;
}


Matrix Matrix::mul(const Matrix& mSource)
{
	checkSize(m_iRows, mSource.m_iRows);
	checkSize(m_iCols, mSource.m_iCols);
	// element-wise multiplication
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] * mSource.m_dData[i];
	return m;
}

Matrix Matrix::div(const Matrix& mSource)
{
	checkSize(m_iRows, mSource.m_iRows);
	checkSize(m_iCols, mSource.m_iCols);
	// element-wise division
	Matrix m(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m.m_dData[i] = m_dData[i] / mSource.m_dData[i];
	return m;
}

Matrix Matrix::dot(const Matrix& mSource)
{
	// matrix multiplication
	if (m_iCols != mSource.m_iRows)
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
	// NOTE: bIsCol is true if the array is a column vector
	// NOTE: Matrix(ROW, COL)
	Matrix m = bIsCol
		? Matrix(iArraySize, 1)
		: Matrix(1, iArraySize);

	for (int i = 0; i < iArraySize; i++)
	{
		if (bIsCol)
			m(i, 0) = dArray[i];
		else
			m(0, i) = dArray[i];
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

MatrixJson Matrix::toJson() const
{
	MatrixJson json;

	json.iRows = m_iRows;
	json.iCols = m_iCols;

	for (double d : m_dData)
		json.vData.push_back(d);

	return json;
}

Matrix Matrix::fromJson(MatrixJson json) const
{
	Matrix m(json.iRows, json.iCols);
	for (int i = 0; i < json.vData.size(); i++)
		m.m_dData[i] = json.vData[i];
	return m;
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

void Matrix::showRow(unsigned iRow) const
{
	auto r = getRow(iRow); for (double v : r) std::cout << v << " ";
}

void Matrix::showCol(unsigned iCol) const
{
	auto c = getCol(iCol); for (double v : c) std::cout << v << " ";
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