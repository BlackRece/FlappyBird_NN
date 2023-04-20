#pragma once

#include "GameState.hpp"
#include "NN.h"

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

class AIController
{
public:
	AIController();
	~AIController();

	void initBirds(GameDataRef data);
	std::vector<Bird*> getBirds();
	std::vector<DnaGene*> getDnaGenes() { return m_vecDnaGenes; }

	void setGameState(GameState* pGameState) { m_pGameState = pGameState; }
	void handleInput();
	void update(float dt);
	void gameOver(float dt);
	
	bool shouldFlap(); // note when this is called, it resets the flap state
	bool isAllBirdsDead();

private:
	float distanceToFloor(Land* land, Bird* bird);
	float distanceToNearestPipes(Pipe* pipe, Bird* bird);
	float distanceToCentreOfPipeGap(Pipe* pipe, Bird* bird);

	GameState*	m_pGameState;
	bool		m_bShouldFlap;

	std::vector<DnaGene*> m_vecDnaGenes;

	NN*			m_pBrain;
};

