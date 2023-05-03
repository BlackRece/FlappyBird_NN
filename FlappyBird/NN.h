#pragma once
#ifndef NN_H
#define NN_H

#include <string>
#include "Matrix.h"
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
	NN(const NN& nn);
	~NN();

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
	NNJson toJson();
	void fromJson(NNJson nnJson);

	void save(const std::string sFileName) { JsonParser::SaveJson(toJson(), sFileName); }
	NNJson load(const std::string sFileName);

	// genetic algorithm functions
	NN* clone() const { return new NN(*this); }
	void mutate(double dRate);
	Matrix* getWeightsIH();// { return m_mWeightsIH; }
	Matrix* getWeightsHO() { return m_mWeightsHO; }
	Matrix* getBiasH() { return m_mBiasH; }
	Matrix* getBiasO() { return m_mBiasO; }

	void setWeightsIH(Matrix* mWeightsIH) { m_mWeightsIH = mWeightsIH; }
	void setWeightsHO(Matrix* mWeightsHO) { m_mWeightsHO = mWeightsHO; }
	void setBiasH(Matrix* mBiasH) { m_mBiasH = mBiasH; }
	void setBiasO(Matrix* mBiasO) { m_mBiasO = mBiasO; }

	std::vector<Matrix*> getWeights() { return { m_mWeightsIH, m_mWeightsHO }; }
	void setWeights(std::vector<Matrix*> vWeights) 
		{ m_mWeightsIH = vWeights[0]; m_mWeightsHO = vWeights[1]; }

	std::vector<Matrix*> getBiases() { return { m_mBiasH, m_mBiasO }; }
	void setBiases(std::vector<Matrix*> vBiases) 
		{ m_mBiasH = vBiases[0]; m_mBiasO = vBiases[1]; }

private:
	double m_dLearningRate;
	int m_iInputCount;
	int m_iHiddenCount;
	int m_iOutputCount;

	Matrix* m_mInput;
	Matrix* m_mHidden;
	Matrix* m_mOutput;

	Matrix* m_mWeightsIH;
	Matrix* m_mWeightsHO;

	Matrix* m_mBiasH;
	Matrix* m_mBiasO;

	std::function<double()> m_fnRandom = std::bind(&NN::random, this, -1.0, 1.0);
	std::function<double(double)> m_fnSigmoid = std::bind(&NN::sigmoid, this, std::placeholders::_1);
	std::function<double(double)> m_fnSigmoidDerivative = std::bind(&NN::sigmoidDerivative, this, std::placeholders::_1);

};

#endif // !NN_H
