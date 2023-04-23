#include "DNA.h"

DNA::DNA() :
	m_iScore(0), m_bIsDead(false), m_fPrediction(0),
	m_iInputCount(0), m_iHiddenCount(0), m_iOutputCount(0)
{
	m_pBird = nullptr;
	m_pNn = nullptr;
}

DNA::DNA(GeneData gData) :
	m_iScore(0), m_bIsDead(false), m_fPrediction(0), 
	m_iInputCount(gData.iInputCount), 
	m_iHiddenCount(gData.iHiddenCount),
	m_iOutputCount(gData.iOutputCount)
{
	m_pBird = new Sonar::Bird(gData.gdrData);
	m_pNn = new NN(gData.iInputCount, gData.iHiddenCount, gData.iOutputCount);
	m_data = gData.gdrData;
}

DNA::DNA(const DNA& dna) :
	m_iScore(dna.m_iScore), m_bIsDead(dna.m_bIsDead), m_fPrediction(dna.m_fPrediction),
	m_iInputCount(dna.m_iInputCount),
	m_iHiddenCount(dna.m_iHiddenCount),
	m_iOutputCount(dna.m_iOutputCount)
{
	m_pBird = nullptr; // new Sonar::Bird(dna.m_data);
	m_pNn = dna.m_pNn->clone();
	m_data = dna.m_data;
}

DNA::~DNA()
{
	delete m_pBird;
	delete m_pNn;
	delete m_fPrediction;
}

void DNA::onHit(Hit eHit)
{
	if(m_bIsDead)
		return;

	switch (eHit)
	{
		case Hit::Gap:		m_iScore += 1000; return;
		case Hit::Pipe:		m_iScore -= 10; break;
		case Hit::Floor:	m_iScore -= 100; break;
	}

	m_bIsDead = true;
}

void DNA::reset()
{
	m_bIsDead = false;
	m_iScore = 0;
}

void DNA::getGuess(double* dInputs, int iInputCount)
{
	m_fPrediction = m_pNn->feedForward(dInputs, iInputCount);
}

void DNA::update(float dt)
{
	if (m_bIsDead)
		return;

	m_pBird->Update(dt);

	double avgPrediction = 0.0;
	for (int i = 0; i < m_iOutputCount; i++)
		avgPrediction += m_fPrediction[i];
	
	avgPrediction /= m_iOutputCount;

	if(avgPrediction >= TRIGGER_TARGET)
		m_pBird->Tap();
}

std::tuple<DNA*, DNA*> DNA::crossover(DNA& dnaA, DNA& dnaB)
{
	DNA* dna1 = dnaA.copy();
	DNA* dna2 = dnaB.copy();

	NN* nnA = dnaA.getNN();
	NN* nnB = dnaB.getNN();

	NN* nn1 = dna1->getNN();
	NN* nn2 = dna2->getNN();

	//nn1->setWeightsHO(nnA->getWeightsHO());
	//nn1->setWeightsIH(nnB->getWeightsIH());

	//nn1->setBiasH(nnB->getBiasH());
	//nn1->setBiasO(nnB->getBiasO());

	//nn2->setWeightsHO(nnB->getWeightsHO());
	//nn2->setWeightsIH(nnA->getWeightsIH());

	//nn2->setBiasH(nnA->getBiasH());
	//nn2->setBiasO(nnA->getBiasO());

	nn1->setWeights(nnB->getWeights());
	nn2->setWeights(nnA->getWeights());
	//nn2->setBiases(nnA->getBiases());

	dna1->setNN(nn1);
	dna2->setNN(nn2);

	return std::make_tuple(dna1, dna2);
}

DNAJson DNA::toJson()
{
	DNAJson json;

	json.m_NNJson = m_pNn->toJson();
	
	json.m_iInputCount = m_iInputCount;
	json.m_iHiddenCount = m_iHiddenCount;
	json.m_iOutputCount = m_iOutputCount;
	
	json.m_iScore = m_iScore;

	return json;
}

void DNA::fromJson(DNAJson& json)
{
	m_iInputCount = json.m_iInputCount;
	m_iHiddenCount = json.m_iHiddenCount;
	m_iOutputCount = json.m_iOutputCount;
	
	m_pNn = new NN(
		json.m_iInputCount,
		json.m_iHiddenCount,
		json.m_iOutputCount
	);
	m_pNn->fromJson(json.m_NNJson);

	m_iScore = json.m_iScore;
}

void DNA::DebugReport()
{
	std::cout
		<< "\n\tScore: " << m_iScore;
}