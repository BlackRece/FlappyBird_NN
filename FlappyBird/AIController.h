#pragma once

#include "GameState.hpp"
#include "NN.h"
#include "GAM.h"
#include "Structures.h"

#define BIRD_COUNT 100

using namespace Sonar;

struct DnaGene
{
	NN* nn;
	Bird* bird;

	double dFitness;
	int iScore;
	bool bShouldFlap;
	bool bIsDead;

	void hitFloor() { iScore -= -100; bIsDead = true; }
	void hitPipe() { iScore -= -10; bIsDead = true; }
	void hitGap() { iScore += 10; }

	DnaGene(NN* pNN, Bird* pBird) :
		nn(pNN), bird(pBird),
		dFitness(0.0), iScore(0),
		bShouldFlap(false), bIsDead(false) {}
};

struct TrainingData
{
	double dInputs[5];
	double dOutputs[1];

	TrainingData(double dInput[5])
	{
		for (int i = 0; i < 5; i++)
			dInputs[i] = dInput[i];
	
		if (dInputs[3] < 20 && dInputs[2] < 20)
		{
			if(dInputs[4] < 0)
				dOutputs[0] = 1;
			else
				dOutputs[0] = 0;
			dOutputs[0] = 0;
		}

		if (dInputs[1] == 9999 && dInputs[2] == 9999)
		{
			if (dInputs[0] < 50)
				dOutputs[0] = 1;

			if (dInputs[0] > 350)
				dOutputs[0] = 0;
		}

		if (dInputs[0] < 50)
			dOutputs[0] = 1;

		if (dInputs[0] > 350)
			dOutputs[0] = 0;
	}
};

class AIController
{
public:
	AIController();
	~AIController();

	void initBirds(GameDataRef data);
	std::vector<Bird*> getBirds();
	std::vector<DNA*> getDna() { return m_pGAM->getChromos(); }
	void draw() { m_pGAM->draw(); }
	void update(float dt) { m_pGAM->update(dt); }

	//void setGameState(GameState* pGameState) { m_pGameState = pGameState; }
	void handleInput(GameState* m_pGameState);
	void gameOver(float dt);
	
	bool shouldFlap(); // note when this is called, it resets the flap state
	bool isAllBirdsDead() { return m_pGAM->isAllDead(); }

private:
	float distanceToFloor(Land* land, Bird* bird);
	float distanceToNearestPipes(Pipe* pipe, Bird* bird);
	float distanceToCentreOfPipeGap(Pipe* pipe, Bird* bird);

	//GameState*	m_pGameState;
	GAM*		m_pGAM;
	bool		m_bShouldFlap;

	//std::vector<DnaGene*> m_vecDnaGenes;
	NN*			m_pBrain;
	std::vector<TrainingData> m_vecTrainingData;
};

