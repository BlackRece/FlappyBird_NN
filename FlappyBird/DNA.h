#pragma once
#ifndef DNA_H
#define DNA_H

#include <vector>
#include "Structures.h"
#include "NN.h"
#include "Bird.hpp"

#define TRIGGER_TARGET 0.5

struct DNAJson
{
	NNJson			m_NNJson;

	int				m_iInputCount;
	int				m_iHiddenCount;
	int				m_iOutputCount;

	int				m_iScore;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(DNAJson, 
		m_NNJson, m_iInputCount, m_iHiddenCount, m_iOutputCount, m_iScore
	);
};

enum class Hit
{
	None = 0,
	Gap = 1,
	Pipe = 2,
	Floor = 3
};

class DNA
{
public:
	DNA();
	DNA(GeneData gData);
	~DNA();

	Sonar::Bird* getBird() { return m_pBird; }
	void getGuess(double* dInputs, int iInputCount);

	void update(float dt);

	//static DNA* setHigh();
	static std::tuple<DNA*, DNA*> crossover(DNA& dnaA, DNA& dnaB);

	float fitness()
	{
		float score = m_iScore - 40;
		float fraction = score / 200;
		float percent = fraction * 100;
		return percent;
	}
	int getScore() { return m_iScore; }

	void onHit(Hit eHit);
	bool isDead() { return m_bIsDead; }
	
	//inline void resetIndex() { m_index = 0; }

	DNA* copy();
	//bool isGreaterThan(DNA& dna);
	//inline bool isLesserThan(DNA& dna) { return !isGreaterThan(dna); }
	//bool isEqualTo(DNA& dna);

	//void fillGenes();
	//inline Gene& getGene() { return *m_genes[m_index]; }
	//bool nextGene();
	void mutate(float fPercent) { m_pNn->mutate((double)fPercent); }
	//Gene* getUniqueGene();

	DNAJson toJson();
	void fromJson(DNAJson& json);

	void DebugReport();

	//std::vector<Gene*> m_genes;
	//int m_iKills;
	//int m_duration;

private:
	void setNN(NN nn) { m_pNn = &nn; }
	void setBird(Sonar::Bird bird) { m_pBird = &bird; }

	int				m_iInputCount;
	int				m_iHiddenCount;
	int				m_iOutputCount;
	NN*				m_pNn;
	double*			m_fPrediction;
	
	GeneData*		m_pGeneData;
	Sonar::Bird*	m_pBird;
	bool			m_bIsDead;
	int				m_iScore;
};

#endif // !DNA_H