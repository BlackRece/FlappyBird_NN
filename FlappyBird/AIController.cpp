#include "AIController.h"

#include <iostream>
#include <vector>

using namespace std;
#define ERROR_DISTANCE 9999

#define INPUT_COUNT 5
#define HIDDEN_COUNT 6
#define OUTPUT_COUNT 1

AIController::AIController()
{
	m_pGameState = nullptr;
	m_bShouldFlap = false;

	//m_mapBirds = std::map<Bird*, NN*>();
	// inputs
	// 1. distance to floor
	// 2. distance to nearest pipe
	// 3. distance to centre of gap
	// 4. bird y position
	// 5. bird y velocity
	// 
	// hidden nodes
	// 5 minimum
	// 
	// outputs
	// 1. output >= 0.5 flap, output < 0.5 don't flap
	//m_pBrain = new NN(INPUT_COUNT, HIDDEN_COUNT, OUTPUT_COUNT);
	m_vecTrainingData = std::vector<TrainingData>();
	m_pGAM = new GAM();
}					  

AIController::~AIController()
{

}

void AIController::initBirds(GameDataRef data)
{
	GeneData gene = GeneData(INPUT_COUNT, HIDDEN_COUNT, OUTPUT_COUNT, data);
	m_pBrain = new NN(INPUT_COUNT, HIDDEN_COUNT, OUTPUT_COUNT);
	m_pGAM->addToPopulation(gene, BIRD_COUNT);
}

std::vector<Bird*> AIController::getBirds()
{
	return m_pGAM->getBirds();
}

// update - the AI method which determines whether the bird should flap or not. 
// set m_bShouldFlap to true or false.
void AIController::handleInput()
{
	if (m_pGameState == nullptr)
		return;

	Pipe* pipe = m_pGameState->GetPipeContainer();
	Land* land = m_pGameState->GetLandContainer();
	//Bird* bird = m_pGameState->GetBird();
	
	for (DNA* birdBrain : m_pGAM->getChromos())
	{
		Bird* bird = birdBrain->getBird();

		float fDistanceToFloor = distanceToFloor(land, bird);
	
		float fDistanceToNearestPipe = distanceToNearestPipes(pipe, bird);
		float fDistanceToCentreOfGap = ERROR_DISTANCE;

		if (fDistanceToNearestPipe != ERROR_DISTANCE) 
			fDistanceToCentreOfGap = distanceToCentreOfPipeGap(pipe, bird);

		double inputs[] =
		{
			(double)fDistanceToFloor, 
			(double)fDistanceToNearestPipe, 
			(double)fDistanceToCentreOfGap, 
			(double)bird->GetSprite().getPosition().y,
			(double)bird->getVelocity()
		};

		birdBrain->getGuess(inputs, INPUT_COUNT);

		m_vecTrainingData.push_back(TrainingData(inputs));

		//NN* nn = birdBrain->getGuess();
		//double* score = nn->feedForward(inputs, INPUT_COUNT);
		//if(score[0] >= 0.5)
		//	birdBrain->bShouldFlap = true;
	}


	//double* score = m_pBrain->feedForward(inputs, INPUT_COUNT);
	//if(score[0] >= 0.5)
	//	m_bShouldFlap = true;

	//std::vector<Bird*> vecBirds = m_pGameState->GetBirds();

	//// do some AI stuff, decide whether to flap
	//for (Bird* bird : vecBirds)
	//{
	//	// get the distance to the floor
	//	float fDistanceToFloor = distanceToFloor(land, bird);
	//
	//	// get the distance to the nearest pipe
	//	float fDistanceToNearestPipe = distanceToNearestPipes(pipe, bird);
	//
	//	// if there is a pipe
	//	if (fDistanceToNearestPipe != ERROR_DISTANCE) {
	//		// get the distance to the centre of the gap
	//		float fDistanceToCentreOfGap = distanceToCentreOfPipeGap(pipe, bird);
	//		fDistanceToCentreOfGap = fDistanceToCentreOfGap;	// <-- whats this for?!
	//
	//		// if the bird is above the centre of the gap, flap
	//		if (bird->GetSprite().getPosition().y < fDistanceToCentreOfGap)
	//			m_bShouldFlap = true;
	//	}
	//}
	
	
	// this means the birdie always flaps. Should only be called when the bird should need to flap. 
	//m_bShouldFlap = true;

	return;
}

//void AIController::update(float dt)
//{
//	m_pGAM->update(dt);
//	//for (DnaGene* birdBrain : m_vecDnaGenes)
//	//{
//	//	if(birdBrain->bIsDead)
//	//		continue;
//	//
//	//	birdBrain->bird->Update(dt);
//	//
//	//	if (birdBrain->bShouldFlap)
//	//	{
//	//		birdBrain->bird->Tap();
//	//		birdBrain->bShouldFlap = false;
//	//	}
//	//}
//}

void AIController::gameOver(float dt)
{
	m_pBrain = m_pGAM->getBrainToTrain();
	if (m_pBrain != nullptr)
	{
		for (TrainingData data : m_vecTrainingData)
		{
			m_pBrain->trainFeedForward(
				data.dInputs, INPUT_COUNT,
				data.dOutputs, OUTPUT_COUNT
			);
		}
		m_pGAM->addTrainedBrain(m_pBrain);
	}

	m_pGAM->nextGeneration();

	std::cout << "\nnext generation\n";
}

float AIController::distanceToFloor(Land* land, Bird* bird)
{
	// the land is always the same height so get the first sprite
	std::vector<sf::Sprite> landSprites = land->GetSprites();
	if (landSprites.size() > 0)
	{
		return landSprites.at(0).getPosition().y - bird->GetSprite().getPosition().y;
	}

	return ERROR_DISTANCE; // this is an error but also means 
}

float AIController::distanceToNearestPipes(Pipe* pipe, Bird* bird)
{
	float nearest1 = 999999;
	sf::Sprite* nearestSprite1 = nullptr;

	// get nearest pipes
	std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();
	for (unsigned int i = 0; i < pipeSprites.size(); i++) {
		sf::Sprite s = pipeSprites.at(i);

		if (s.getPosition().x <= bird->GetSprite().getPosition().x)
			continue;

		float fDistance = s.getPosition().x - bird->GetSprite().getPosition().x;
		if (fDistance > 0 && fDistance < nearest1) {
			nearestSprite1 = &(pipeSprites.at(i));
			nearest1 = fDistance;
		}
	}

	if (nearestSprite1 == nullptr)
		return ERROR_DISTANCE;

	return nearestSprite1->getPosition().x - bird->GetSprite().getPosition().x;
}

float AIController::distanceToCentreOfPipeGap(Pipe* pipe, Bird* bird)
{
	float nearest1 = 999999;
	float nearest2 = 999999;
	sf::Sprite* nearestSprite1 = nullptr;
	sf::Sprite* nearestSprite2 = nullptr;

	// get nearest pipes
	std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();
	for (unsigned int i = 0; i < pipeSprites.size(); i++) {
		sf::Sprite s = pipeSprites.at(i);
		float fDistance = s.getPosition().x - bird->GetSprite().getPosition().x;
		if (fDistance > 0 && fDistance < nearest1) {
			nearestSprite1 = &(pipeSprites.at(i));
			nearest1 = fDistance;
		}
		else if (fDistance > 0 && fDistance < nearest2) {
			nearestSprite2 = &(pipeSprites.at(i));
			nearest2 = fDistance;
		}
	}

	if (nearestSprite1 == nullptr || nearestSprite2 == nullptr)
		return ERROR_DISTANCE;


	sf::Sprite* topSprite = nullptr;
	sf::Sprite* bottomSprite = nullptr;

	if (nearestSprite1->getPosition().y < nearestSprite2->getPosition().y) {
		topSprite = nearestSprite1;
		bottomSprite = nearestSprite2;
	}
	else {
		topSprite = nearestSprite2;
		bottomSprite = nearestSprite1;
	}

	float distance = ((bottomSprite->getGlobalBounds().top) - (topSprite->getGlobalBounds().height + topSprite->getGlobalBounds().top)) / 2;
	distance += (topSprite->getGlobalBounds().top + topSprite->getGlobalBounds().height);

	return distance - bird->GetSprite().getPosition().y;
}

// note when this is called, it resets the flap state (don't edit)
bool AIController::shouldFlap() 
{
	bool output = m_bShouldFlap;
	m_bShouldFlap = false;

	return output;
}
