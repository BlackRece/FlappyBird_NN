#include "NN.h"

NN::NN(int iInputCount, int iHiddenCount, int iOutputCount)
	: m_iInputCount(iInputCount), m_iHiddenCount(iHiddenCount), m_iOutputCount(iOutputCount)
{
	// create matrices
	m_mInput = new Matrix(m_iInputCount, 1);
	m_mHidden = new Matrix(m_iHiddenCount, 1);
	m_mOutput = new Matrix(m_iOutputCount, 1);

	m_mWeightsIH = new Matrix(m_iHiddenCount, m_iInputCount);
	m_mWeightsHO = new Matrix(m_iOutputCount, m_iHiddenCount);

	m_mBiasH = new Matrix(m_iHiddenCount, 1);
	m_mBiasO = new Matrix(m_iOutputCount, 1);

	// randomize weights and biases
	m_mWeightsIH = m_mWeightsIH->map(m_fnRandom);
	m_mWeightsHO = m_mWeightsHO->map(m_fnRandom);

	m_mBiasH = m_mBiasH->map(m_fnRandom);
	m_mBiasO = m_mBiasO->map(m_fnRandom);

	// set default learning rate
	m_dLearningRate = LEARNING_RATE;
}

NN::NN(const NN& nn)
{
	m_iInputCount = nn.m_iInputCount;
	m_iHiddenCount = nn.m_iHiddenCount;
	m_iOutputCount = nn.m_iOutputCount;

	m_mInput = nn.m_mInput;
	m_mHidden = nn.m_mHidden;
	m_mOutput = nn.m_mOutput;

	m_mWeightsIH = nn.m_mWeightsIH;
	m_mWeightsHO = nn.m_mWeightsHO;

	m_mBiasH = nn.m_mBiasH;
	m_mBiasO = nn.m_mBiasO;

	m_dLearningRate = nn.m_dLearningRate;
}

double* NN::feedForward(const double dInputs[], const int iInputCount)
{
	// convert input array to matrix
	bool bIsColumnVector = true;
	m_mInput = m_mInput->fromArray(dInputs, iInputCount, bIsColumnVector);

	// apply weights and biases to input
	m_mHidden = m_mWeightsIH->dot(*m_mInput);
	m_mHidden = m_mHidden->add(*m_mBiasH);

	// apply activation function
	m_mHidden = m_mHidden->map(m_fnSigmoid);

	// apply weights and biases to hidden layer
	m_mOutput = m_mWeightsHO->dot(*m_mHidden);
	m_mOutput = m_mOutput->add(*m_mBiasO);

	// apply activation function
	m_mOutput = m_mOutput->map(m_fnSigmoid);

	// return result as array
	return m_mOutput->toArray();
}

void NN::trainFeedForward(const double dInputs[], const int iInputCount, const double dTargets[], const int iTargetCount)
{
	double* rawOutputs = feedForward(dInputs, iInputCount);

	Matrix *mOutputs{}, *mTargets{};
	// get output matrix from feed forward
	mOutputs = mOutputs->fromArray(rawOutputs, m_iOutputCount);

	// convert target array to matrix
	mTargets = mTargets->fromArray(dTargets, iTargetCount);

	// calculate output errors
	Matrix* mOutputErrors = mTargets->sub(*mOutputs);

	// calculate hidden-output gradients via gradient descent
	Matrix* mOutputGradients = mOutputs->map(m_fnSigmoidDerivative);
	mOutputGradients = mOutputGradients->mul(*mOutputErrors);

	double dLearningRate = 1.0;		// TODO: make this a member variable that starts at 0.1 and decreases over time
	mOutputGradients = mOutputGradients->mul(dLearningRate);

	// calculate hidden-output deltas
	Matrix* mHidden_T = m_mHidden->transpose();
	Matrix* mWeightsHO_deltas = mOutputGradients->dot(*mHidden_T);

	// adjust hidden-output weights and biases
	m_mWeightsHO = m_mWeightsHO->add(*mWeightsHO_deltas);

	// apply hidden->output biases
	m_mBiasO = m_mBiasO->add(*mOutputGradients);

	// calculate hidden->output deltas/errors
	Matrix* mWeightsHO_T = m_mWeightsHO->transpose();
	Matrix* mHiddenErrors = mWeightsHO_T->dot(*mOutputErrors);

	// calculate input->hidden gradients via gradient descent
	Matrix* mHiddenGradients = m_mHidden->map(m_fnSigmoidDerivative);
	mHiddenGradients = mHiddenGradients->mul(*mHiddenErrors);
	mHiddenGradients = mHiddenGradients->mul(dLearningRate);

	// calculate input->hidden deltas
	Matrix *mInput_T = m_mInput->transpose();
	Matrix *mWeightsIH_deltas = mHiddenGradients->dot(*mInput_T);

	// adjust input->hidden weights and biases
	m_mWeightsIH = m_mWeightsIH->add(*mWeightsIH_deltas);

	// apply input->hidden biases
	m_mBiasH = m_mBiasH->add(*mHiddenGradients);

}

NNJson NN::toJson()
{
	NNJson json;

	json.dLearningRate = m_dLearningRate;

	json.iInputNodes = m_iInputCount;
	json.iHiddenNodes = m_iHiddenCount;
	json.iOutputNodes = m_iOutputCount;

	json.mInputWeights = m_mWeightsIH->toJson();
	json.mHiddenWeights = m_mWeightsHO->toJson();

	json.mBiasHidden = m_mBiasH->toJson();
	json.mBiasOutput = m_mBiasO->toJson();

	return json;
}

void NN::fromJson(NNJson nnJson)
{
	if (nnJson.iInputNodes == 0 || nnJson.iHiddenNodes == 0 || nnJson.iOutputNodes == 0)
	{
		std::cout << "Error: Invalid JSON file" << std::endl;
		return;
	}

	m_dLearningRate = nnJson.dLearningRate;

	m_iInputCount = nnJson.iInputNodes;
	m_iHiddenCount = nnJson.iHiddenNodes;
	m_iOutputCount = nnJson.iOutputNodes;

	m_mWeightsIH = m_mWeightsIH->fromJson(nnJson.mInputWeights);
	m_mWeightsHO = m_mWeightsHO->fromJson(nnJson.mHiddenWeights);

	m_mBiasH = m_mBiasH->fromJson(nnJson.mBiasHidden);
	m_mBiasO = m_mBiasO->fromJson(nnJson.mBiasOutput);
}

NNJson NN::load(const std::string sFileName)
{
	// load json file using JsonParser
	NNJson json;
	JsonParser::LoadJson(json, sFileName);
	if (json.iInputNodes == 0 || json.iHiddenNodes == 0 || json.iOutputNodes == 0)
		std::cout << "Error: Invalid JSON file" << std::endl;
	
	return json;
}

void NN::mutate(double dRate)
{
	if (random(100.0) > dRate)
		return;

	m_mWeightsIH = m_mWeightsIH->map(m_fnRandom);
	m_mWeightsHO = m_mWeightsHO->map(m_fnRandom);

	m_mBiasH = m_mBiasH->map(m_fnRandom);
	m_mBiasO = m_mBiasO->map(m_fnRandom);
}

void NN::crossWeights(NN& nn)
{
	//Matrix 
	// cross weights
	//m_mWeightsIH = m_mWeightsIH.cross(nn.m_mWeightsIH);
	//m_mWeightsHO = m_mWeightsHO.cross(nn.m_mWeightsHO);
	// cross biases
	//m_mBiasH = m_mBiasH.cross(nn.m_mBiasH);
	//m_mBiasO = m_mBiasO.cross(nn.m_mBiasO);
}
