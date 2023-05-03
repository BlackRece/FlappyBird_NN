#include "Matrix.h"
using json = nlohmann::json;

Matrix::Matrix(unsigned int iRows, unsigned int iCols)
	: m_iRows(iRows), m_iCols(iCols)
{
	isZeroSize(iRows, iCols);
	m_dData = std::valarray<double>(m_iRows * m_iCols);
}

Matrix::Matrix(const Matrix& m) :
	m_iRows(m.m_iRows), m_iCols(m.m_iCols),
	m_dData(std::valarray<double>(m.m_dData))
{
	// Note: This implementation copies the valarray object and its contents
}

Matrix::~Matrix()
{
	// delete the valarray object
	m_dData.~valarray();
}

double& Matrix::operator()(unsigned int iRows, unsigned int iCols)
{
	checkBounds(iRows, iCols);
	return m_dData[m_iCols * iRows + iCols];
}

double Matrix::operator()(unsigned int iRows, unsigned int iCols) const
{
	checkBounds(iRows, iCols);
	return m_dData[m_iCols * iRows + iCols];
}

#pragma region static scalar functions
Matrix* Matrix::add(const Matrix& m1, const double dScalar)
{
	Matrix* m = new Matrix(m1.m_iRows, m1.m_iCols);
	for (int i = 0; i < m1.m_iRows * m1.m_iCols; i++)
		m->m_dData[i] = m1.m_dData[i] + dScalar;
	return m;
}

Matrix* Matrix::sub(const Matrix& m1, const double dScalar)
{
	Matrix* m = new Matrix(m1.m_iRows, m1.m_iCols);
	for (int i = 0; i < m1.m_iRows * m1.m_iCols; i++)
		m->m_dData[i] = m1.m_dData[i] - dScalar;
	return m;
}

Matrix* Matrix::mul(const Matrix& m1, const double dScalar)
{
	Matrix* m = new Matrix(m1.m_iRows, m1.m_iCols);
	for (int i = 0; i < m1.m_iRows * m1.m_iCols; i++)
		m->m_dData[i] = m1.m_dData[i] * dScalar;
	return m;
}

Matrix* Matrix::div(const Matrix& m1, const double dScalar)
{
	Matrix* m = new Matrix(m1.m_iRows, m1.m_iCols);
	for (int i = 0; i < m1.m_iRows * m1.m_iCols; i++)
		m->m_dData[i] = m1.m_dData[i] / dScalar;
	return m;
}
#pragma endregion // static scalar functions

#pragma region static matrix functions
Matrix* Matrix::add(const Matrix& m1, const Matrix& m2)
{
	isSameSize(m1, m2);
	Matrix* m = new Matrix(m1.m_iRows, m1.m_iCols);
	for (int i = 0; i < m1.m_iRows * m1.m_iCols; i++)
		m->m_dData[i] = m1.m_dData[i] + m2.m_dData[i];
	return m;
}

Matrix* Matrix::sub(const Matrix& m1, const Matrix& m2)
{
	isSameSize(m1, m2);
	Matrix* m = new Matrix(m1.m_iRows, m1.m_iCols);
	for (int i = 0; i < m1.m_iRows * m1.m_iCols; i++)
		m->m_dData[i] = m1.m_dData[i] - m2.m_dData[i];
	return m;
}

Matrix* Matrix::mul(const Matrix& m1, const Matrix& m2)
{
	isSameSize(m1, m2);
	Matrix* m = new Matrix(m1.m_iRows, m1.m_iCols);
	for (int i = 0; i < m1.m_iRows * m1.m_iCols; i++)
		m->m_dData[i] = m1.m_dData[i] * m2.m_dData[i];
	return m;
}

Matrix* Matrix::div(const Matrix& m1, const Matrix& m2)
{
	isSameSize(m1, m2);
	Matrix* m = new Matrix(m1.m_iRows, m1.m_iCols);
	for (int i = 0; i < m1.m_iRows * m1.m_iCols; i++)
		m->m_dData[i] = m1.m_dData[i] / m2.m_dData[i];
	return m;
}
#pragma endregion // static matrix functions

#pragma region member scalar functions
void Matrix::add(const double dScalar)
{
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] += dScalar;
}

void Matrix::sub(const double dScalar)
{
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] -= dScalar;
}

void Matrix::mul(const double dScalar)
{
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] *= dScalar;
}

void Matrix::div(const double dScalar)
{
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] /= dScalar;
}
#pragma endregion // member scalar functions

#pragma region member matrix functions
void Matrix::add(const Matrix& m1)
{
	isSameSize(m1);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] += m1.m_dData[i];
}

void Matrix::sub(const Matrix& m1)
{
	isSameSize(m1);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] -= m1.m_dData[i];
}

void Matrix::mul(const Matrix& m1)
{
	isSameSize(m1);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] *= m1.m_dData[i];
}

void Matrix::div(const Matrix& m1)
{
	isSameSize(m1);
	for (int i = 0; i < m_iRows * m_iCols; i++)
		m_dData[i] /= m1.m_dData[i];
}
#pragma endregion // member matrix functions

Matrix* Matrix::dot(const Matrix& m1, const Matrix& m2)
{
	// check dimensions
	if (m1.m_iCols != m2.m_iRows)
	{
		throw std::invalid_argument("Matrices are not compatabile");
		return nullptr;
	}

	Matrix* m = new Matrix(m1.m_iRows, m2.m_iCols);
	for (int i = 0; i < m1.m_iRows; i++)
		for (int j = 0; j < m2.m_iCols; j++)
			for (int k = 0; k < m1.m_iCols; k++)
				(*m)(i, j) += m1(i, k) * m2(k, j);
	return m;
}

Matrix* Matrix::transpose(const Matrix& mSource)
{
	Matrix* m = new Matrix(mSource.m_iCols, mSource.m_iRows);
	for (int i = 0; i < mSource.m_iRows; i++)
		for (int j = 0; j < mSource.m_iCols; j++)
			(*m)(j, i) = (mSource)(i, j);
	return m;
}

Matrix* Matrix::map(const std::function<double(int, int, double)>& func)
{
	Matrix* m = new Matrix(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows; i++)
		for (int j = 0; j < m_iCols; j++)
			(*m)(i, j) = func(i, j, (*this)(i, j));
	return m;
}

Matrix* Matrix::map(std::function<double(double)>& func)
{
	Matrix* m = new Matrix(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows; i++)
		for (int j = 0; j < m_iCols; j++)
			(*m)(i, j) = func((*this)(i, j));
	return m;
}

Matrix* Matrix::map(std::function<double()>& func) {
	Matrix* m = new Matrix(m_iRows, m_iCols);
	for (int i = 0; i < m_iRows; i++) {
		for (int j = 0; j < m_iCols; j++) {
			(*m)(i, j) = func();
		}
	}
	return m;
}

Matrix* Matrix::fromArray(const double dArray[], const int iArraySize, bool bIsCol)
{
	// NOTE: bIsCol is true if the array is a column vector
	// NOTE: Matrix(ROW, COL)
	Matrix* m = bIsCol
		? new Matrix(iArraySize, 1)
		: new Matrix(1, iArraySize);

	for (int i = 0; i < iArraySize; i++)
	{
		if (bIsCol)
			(*m)(i, 0) = dArray[i];
		else
			(*m)(0, i) = dArray[i];
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

Matrix* Matrix::fromJson(MatrixJson& json) 
{
	Matrix* m = new Matrix(json.iRows, json.iCols);
	for (int i = 0; i < json.vData.size(); i++)
		m->m_dData[i] = json.vData[i];
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

void Matrix::showRow(unsigned int iRow) const
{
	auto r = getRow(iRow); for (double v : r) std::cout << v << " ";
}

void Matrix::showCol(unsigned int iCol) const
{
	auto c = getCol(iCol); for (double v : c) std::cout << v << " ";
}

inline void Matrix::checkBounds(const unsigned int iRows, const unsigned int iCols) const
{
	if (iRows >= m_iRows || iCols >= m_iCols)
		throw std::out_of_range("Matrix subscript out of bounds");
}

inline void Matrix::isMatchingSize(const unsigned int lhs, const unsigned int rhs)
{
	if (lhs != rhs)
		throw std::invalid_argument("Matrix sizes do not match");
}

inline void Matrix::isSameSize(Matrix mMatrix)
{
	if (mMatrix.m_iRows != m_iRows || mMatrix.m_iCols != m_iCols)
		throw std::invalid_argument("Matrix sizes do not match");
}

inline void Matrix::isZeroSize(const unsigned iRows, const unsigned iCols)
{
	if (iRows <= 0 || iCols <= 0)
		throw std::invalid_argument("Matrix constructor has 0 size");
}

inline void Matrix::isSameSize(const Matrix& mMatrix1, const Matrix& mMatrix2)
{
	bool bSameRows = mMatrix1.m_iRows == mMatrix2.m_iRows;
	bool bSameCols = mMatrix1.m_iCols == mMatrix2.m_iCols;
	if (!bSameRows || !bSameCols)
		throw std::invalid_argument("Matrix sizes do not match");
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
