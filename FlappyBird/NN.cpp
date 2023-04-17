#include "NN.h"

NN::NN(int iInputCount, int iHiddenCount, int iOutputCount)
	: m_iInputCount(iInputCount), m_iHiddenCount(iHiddenCount), m_iOutputCount(iOutputCount)
{
	m_mInput = Matrix(m_iInputCount, 1);
	m_mHidden = Matrix(m_iHiddenCount, 1);
	m_mOutput = Matrix(m_iOutputCount, 1);

	m_mWeightsIH = Matrix(m_iHiddenCount, m_iInputCount);
	m_mWeightsHO = Matrix(m_iOutputCount, m_iHiddenCount);
	m_mBiasH = Matrix(m_iHiddenCount, 1);
	m_mBiasO = Matrix(m_iOutputCount, 1);

	m_mWeightsIH.random(2);
	m_mWeightsIH.add(-1);
	m_mWeightsHO.random(2);
	m_mWeightsHO.add(-1);

	m_mBiasH.random(2);
	m_mBiasH.add(-1);
	m_mBiasO.random(2);
	m_mBiasO.add(-1);
}

void NN::feedForward(const double dInputs[])
{
	std::function<double(double)> fnSigmoid = std::bind(&NN::sigmoid, this, std::placeholders::_1);
	m_mInput = m_mInput.fromArray(dInputs);
	m_mHidden = m_mWeightsIH.dot(m_mInput);
	m_mHidden.add(m_mBiasH);
	//m_mHidden.sigmoid();
	m_mHidden.map(fnSigmoid);
	m_mOutput = m_mWeightsHO.dot(m_mHidden);
	//m_mOutput.add(m_mBiasO);
	m_mOutput.map(fnSigmoid);
	m_mOutput.sigmoid();
}
