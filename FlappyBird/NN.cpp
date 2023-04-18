#include "NN.h"

NN::NN(int iInputCount, int iHiddenCount, int iOutputCount)
	: m_iInputCount(iInputCount), m_iHiddenCount(iHiddenCount), m_iOutputCount(iOutputCount)
{
	// prepare function to random every element in matrix
	std::function<double()> fnRandomDouble = std::bind(&NN::random, this, -2.0, 2.0);

	// create matrices
	m_mInput = Matrix(m_iInputCount, 1);
	m_mHidden = Matrix(m_iHiddenCount, 1);
	m_mOutput = Matrix(m_iOutputCount, 1);

	m_mWeightsIH = Matrix(m_iHiddenCount, m_iInputCount);
	m_mWeightsHO = Matrix(m_iOutputCount, m_iHiddenCount);
	m_mBiasH = Matrix(m_iHiddenCount, 1);
	m_mBiasO = Matrix(m_iOutputCount, 1);

	// randomize weights and biases
	m_mWeightsIH = m_mWeightsIH.map(fnRandomDouble);
	m_mWeightsHO = m_mWeightsHO.map(fnRandomDouble);

	m_mBiasH = m_mBiasH.map(fnRandomDouble);
	m_mBiasO = m_mBiasO.map(fnRandomDouble);
}

double* NN::feedForward(const double dInputs[], const int iInputCount)
{
	// convert input array to matrix
	m_mInput = m_mInput.fromArray(dInputs, iInputCount);

	// apply weights and biases to input
	m_mHidden = m_mWeightsIH.dot(m_mInput);
	auto hiddenWithWeightsAndBias = m_mHidden.add(m_mBiasH);

	// prepare and apply activation function
	std::function<double(double)> fnSigmoid = std::bind(&NN::sigmoid, this, std::placeholders::_1);
	auto hiddenLayerResult = hiddenWithWeightsAndBias.map(fnSigmoid);

	// apply weights and biases to hidden layer
	m_mOutput = m_mWeightsHO.dot(hiddenLayerResult);
	auto outputLayerWithWeightsAndBias = m_mOutput.add(m_mBiasO);
	
	// apply activation function
	auto result = outputLayerWithWeightsAndBias.map(fnSigmoid);
	return result.toArray();
}
