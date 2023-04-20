#pragma once
#ifndef NN_H
#define NN_H

#include <string>
#include "Matrix.h"
//#include "JsonParser.h"
#include "Structures.h"

#define LEARNING_RATE 0.1
#define HIDDEN_LAYER_SIZE 1 // TODO: refactor to handle more than 1 layer

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

	double* feedForward(const double dInputs[], const int iInputCount);
	void trainFeedForward(const double dInputs[], const int iInputCount, const double dTargets[], const int iTargetCount);

	// activation functions
	double sigmoid(double dVal) { return 1 / (1 + exp(-dVal)); }
	double sigmoidDerivative(double dVal) { return dVal * (1 - dVal); }
	double sigmoidDerivativeFull(double dVal) { return sigmoid(dVal) * (1 - sigmoid(dVal)); }

	// helper functions
	void setLearningRate(double dLearningRate) { m_dLearningRate = dLearningRate; }
	double getLearningRate() { return m_dLearningRate; }

	double random(double dMin = -1, double dMax = 1) { return dMin + (dMax - dMin) * ((double)rand() / RAND_MAX); }
	double randomDouble() { return -2 + (2 - -2) * ((double)rand() / RAND_MAX); }

	// json functions
	void save(const std::string sFileName);
	void load(const std::string sFileName);

private:
	double m_dLearningRate;
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
