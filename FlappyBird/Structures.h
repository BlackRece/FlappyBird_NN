#pragma once
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include "JsonParser.h"

struct MatrixJson
{
	int iRows;
	int iCols;
	std::vector<double> vData;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MatrixJson, iRows, iCols, vData);
};

struct NNJson
{
	double dLearningRate;

	int iInputNodes;
	int iHiddenNodes;
	int iOutputNodes;

	MatrixJson mInputWeights;
	MatrixJson mHiddenWeights;

	MatrixJson mBiasHidden;
	MatrixJson mBiasOutput;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(
		NNJson, dLearningRate,
		iInputNodes, iHiddenNodes, iOutputNodes,
		mInputWeights, mHiddenWeights,
		mBiasHidden, mBiasOutput
	);
};

#endif // !STRUCTURES_H