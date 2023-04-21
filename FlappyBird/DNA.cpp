#include "DNA.h"

DNA::DNA() :
	m_iScore(0), m_bIsDead(false), m_fPrediction(0),
	m_iInputCount(0), m_iOutputCount(0)
{
	m_pBird = nullptr;
	m_pNn = nullptr;
	m_pGeneData = nullptr;
}

DNA::DNA(GeneData gData) :
	m_iScore(0), m_bIsDead(false), m_fPrediction(0), 
	m_iInputCount(gData.iInputCount), m_iOutputCount(gData.iOutputCount)
{
	m_pBird = new Sonar::Bird(gData.gdrData);
	m_pNn = new NN(gData.iInputCount, gData.iHiddenCount, gData.iOutputCount);
	m_pGeneData = &gData;
}

DNA::~DNA()
{
	delete m_pBird;
	delete m_pNn;
	delete m_fPrediction;
}

void DNA::onHit(Hit eHit)
{
	switch (eHit)
	{
	case Hit::Gap:		m_iScore += 10; return;
	case Hit::Pipe:		m_iScore -= 100; break;
	case Hit::Floor:	m_iScore -= 1000; break;

	case Hit::None:
	default:
		break;
	}

	m_bIsDead = true;
}

DNA* DNA::copy()
{
	DNA* dna = new DNA(*m_pGeneData);
	dna->m_pBird = new Sonar::Bird(m_pGeneData->gdrData);
	dna->m_iScore = m_iScore;
	dna->setNN(m_pNn->clone());
	return dna;
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
	{
		avgPrediction += m_fPrediction[i];
		avgPrediction /= m_iOutputCount;
	}
	if(avgPrediction >= TRIGGER_TARGET)
		m_pBird->Tap();
}

//DNA* DNA::setHigh() {
//	DNA* dna = new DNA();
//	dna->m_iScore = RAND_MAX;
//	dna->m_duration = RAND_MAX;
//	dna->m_iKills = RAND_MAX;
//	return dna;
//}

std::tuple<DNA*, DNA*> DNA::crossover(DNA& dnaA, DNA& dnaB)
{
	DNA* dna1 = new DNA();
	DNA* dna2 = new DNA();

	DNAJson dnaBuffer1 = dnaA.toJson();
	DNAJson dnaBuffer2 = dnaB.toJson();

	DNAJson dnaBufferA = dnaA.toJson();
	DNAJson dnaBufferB = dnaB.toJson();

	dnaBuffer1.m_NNJson.mInputWeights = dnaBufferB.m_NNJson.mInputWeights;
	dnaBuffer1.m_NNJson.mBiasHidden = dnaBufferB.m_NNJson.mBiasHidden;

	//dnaBuffer1.m_NNJson.mHiddenWeights = dnaBufferA.m_NNJson.mHiddenWeights;
	//dnaBuffer1.m_NNJson.mBiasOutput = dnaBufferB.m_NNJson.mBiasOutput;

	dnaBuffer2.m_NNJson.mInputWeights = dnaBufferB.m_NNJson.mInputWeights;
	dnaBuffer2.m_NNJson.mBiasHidden = dnaBufferB.m_NNJson.mBiasHidden;

	//dnaBuffer2.m_NNJson.mHiddenWeights = dnaBufferB.m_NNJson.mHiddenWeights;
	//dnaBuffer2.m_NNJson.mBiasOutput = dnaBufferA.m_NNJson.mBiasOutput;

	dna1->fromJson(dnaBuffer1);
	dna2->fromJson(dnaBuffer2);

	return std::make_tuple(dna1, dna2);
}

//bool DNA::isGreaterThan(DNA& dna)
//{
//	if (m_iScore == dna.m_iScore)
//	{
//		if (m_iKills == dna.m_iKills)
//			return m_duration > dna.m_duration;
//		else
//			return m_iKills > dna.m_iKills;
//	}
//	return m_iScore > dna.m_iScore;
//}
//
//bool DNA::isEqualTo(DNA& dna)
//{
//	if (m_genes.size() != dna.m_genes.size())
//		return false;
//
//	for (int i = 0; i < m_genes.size(); i++)
//	{
//		if (m_genes[i]->m_towerType != dna.m_genes[i]->m_towerType)
//			return false;
//		if (m_genes[i]->m_towerPosition != dna.m_genes[i]->m_towerPosition)
//			return false;
//	}
//
//	return m_iScore == dna.m_iScore && m_iKills == dna.m_iKills && m_duration == dna.m_duration;
//}
//
//void DNA::fillGenes()
//{
//	for (int i = 0; i < GENES_PER_DNA; i++)
//		m_genes.push_back(getUniqueGene());
//}
//
//bool DNA::nextGene()
//{
//	m_index++;
//	if (m_index >= m_genes.size())
//	{
//		m_index = 0;
//		return true;
//	}
//
//	return false;
//}

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
	m_pNn->fromJson(json.m_NNJson);

	m_iInputCount = json.m_iInputCount;
	m_iHiddenCount = json.m_iHiddenCount;
	m_iOutputCount = json.m_iOutputCount;

	m_iScore = json.m_iScore;
}

//Gene* DNA::getUniqueGene()
//{
//	Gene* newGene = new Gene();
//	bool isUnique = false;
//
//	while (isUnique == false)
//	{
//		// set flag
//		isUnique = true;
//
//		// for each gene
//		for (Gene* gene : m_genes)
//		{
//			// check for dupicates
//			if (gene->m_towerPosition == newGene->m_towerPosition)
//			{
//				// if duplicated, set flag
//				isUnique = false;
//				break;
//			}
//		}
//
//		// if duplicated flag was set, get new position
//		if (!isUnique)
//			newGene = new Gene();
//	}
//
//	return newGene;
//}

void DNA::DebugReport()
{
	std::cout
		<< "\n\tScore: " << m_iScore;
}