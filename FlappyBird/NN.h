#pragma once
#ifndef NN_H
#define NN_H

#include "Matrix.h"

//Vector variants are avalable
/*
  remember: row major - rows first, columns second (row x column)

  inputCount creates a matrix of size inputCount x 1
  hiddenCount creates a matrix of size hiddenCount x 1 (could add a parameter for more than one layer)
  outputCount creates a matrix of size outputCount x 1
*/
class NN
{
public:
	NN(int iInputCount, int iHiddenCount, int iOutputCount);
	~NN() { ; }

	void feedForward(const double dInputs[]);

	// activation functions
	double sigmoid(double dVal) { return 1 / (1 + exp(-dVal)); }

private:
	int m_iInputCount;
	int m_iHiddenCount;
	int m_iOutputCount;

	Matrix m_mInput;
	Matrix m_mHidden;
	Matrix m_mOutput;
	Matrix m_mWeightsIH;
	Matrix m_mWeightsHO;
	Matrix m_mBiasH;
	Matrix m_mBiasO;
};

#endif // !NN_H
