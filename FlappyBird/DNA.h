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
	void setBird(Sonar::Bird* bird) { m_pBird = bird; }

	void getGuess(double* dInputs, int iInputCount);

	void update(float dt);

	static std::tuple<DNA*, DNA*> crossover(DNA& dnaA, DNA& dnaB);

	int getScore() { return m_iScore; }

	void onHit(Hit eHit);
	bool isDead() { return m_bIsDead; }
	
	DNA* copy();
	void mutate(float fPercent) { m_pNn->mutate((double)fPercent); }
	void reset();

	DNAJson toJson();
	void fromJson(DNAJson& json);

	void DebugReport();

	void setNN(NN* nn) { m_pNn = nn; }
	NN* getNN() { return m_pNn; }

private:
	void setBird(Sonar::Bird bird) { m_pBird = &bird; }

	int				m_iInputCount;
	int				m_iHiddenCount;
	int				m_iOutputCount;
	NN*				m_pNn;
	double*			m_fPrediction;
	
	Sonar::GameDataRef		m_data;
	Sonar::Bird*			m_pBird;
	bool					m_bIsDead;
	int						m_iScore;
};

#endif // !DNA_H