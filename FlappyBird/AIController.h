#pragma once

#include "GameState.hpp"
#include "NN.h"

#define BIRD_COUNT 100

using namespace Sonar;

class AIController
{
public:
	AIController();
	~AIController();

	void initBirds(GameDataRef data);
	void setGameState(GameState* pGameState) { m_pGameState = pGameState; }
	void handleInput();
	void update(float dt);
	void hitFloor(float dt);
	void hitPipe(float dt);
	void hitGap(float dt);
	void gameOver(float dt);
	bool shouldFlap(); // note when this is called, it resets the flap state

public:

private:
	float distanceToFloor(Land* land, Bird* bird);
	float distanceToNearestPipes(Pipe* pipe, Bird* bird);
	float distanceToCentreOfPipeGap(Pipe* pipe, Bird* bird);


private:
	GameState*	m_pGameState;
	bool		m_bShouldFlap;
	bool		m_bGameOver;

	NN*			m_pBrain;
	std::map<Bird*, NN*> m_mapBirds;
	float		m_ga_score;
};

