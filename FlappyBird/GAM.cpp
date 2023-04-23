#include "GAM.h"

GAM::GAM() : m_iGeneration(0), m_bIsWaitingForNewGame(false)
{
	// seed random number generator with current time
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	std::chrono::system_clock::duration dtn = tp.time_since_epoch();
	srand(dtn.count());

	m_vecChromos = std::vector<DNA*>();

	// load population from json file
	loadChromosFromJson();
}

void GAM::addToPopulation(GeneData gData, int dnaCount)
{
	m_iDnaCount = dnaCount;

	// if no population was loaded, create initial population
	if (m_vecChromos.size() < 1)
	{
		// create initial population
		for (int i = 0; i < dnaCount; i++)
			m_vecChromos.push_back(new DNA(gData));
	}
	else
	{
		for (int i = 0; i < dnaCount; i++)
		{
			if(m_vecChromos[i])
			m_vecChromos[i]->setBird(new Sonar::Bird(gData.gdrData));
		}
	}
}

void GAM::addTrainedBrain(NN* nn)
{
	m_vecChromos[99]->setNN(nn);
}

NN* GAM::getBrainToTrain()
{
	auto birdBrain = findBestDNA(m_vecChromos);
	return birdBrain->getNN();
}

std::vector<Sonar::Bird*> GAM::getBirds()
{
	std::vector<Sonar::Bird*> vecBirds = std::vector<Sonar::Bird*>();

	for(DNA* dna : m_vecChromos)
		vecBirds.push_back(dna->getBird());

	return vecBirds;
}

void GAM::update(float dt)
{
	if (m_bIsWaitingForNewGame)
		return;

	for (DNA* dna : m_vecChromos) 
		dna->update(dt);
}

bool GAM::isAllDead()
{
	for (DNA* dna : m_vecChromos)
	{
		if (!dna->isDead())
			return false;
	}

	m_bIsWaitingForNewGame = true;
	return true;
}

void GAM::draw()
{
	for (DNA* dna : m_vecChromos)
	{
		if(!dna->isDead())
			dna->getBird()->Draw();
	}
}

void GAM::nextGeneration()
{
	DebugReport();

	// Selection: select the best chromosomes (aka Ranked Selection)
	std::vector<DNA*> vecSelected = selection_rank();

	// skip crossover
	std::vector<DNA*> vecCrossed = vecSelected;
	vecCrossed.insert(vecCrossed .end(), vecSelected.begin(), vecSelected.end());
	// Crossover: create new chromosomes from selected chromosomes
	//std::vector<DNA*> vecCrossed = crossover(vecSelected);

	// Mutation: mutate crossed chromosomes using a mutation percentage
	m_vecChromos = mutation(vecCrossed, MUTATION_PERCENTAGE);

	// increase generation count
	m_iGeneration++;

	// Reset: prepare for next generation
	reset();
}

DNA* GAM::findBestDNA(std::vector<DNA*> vecSource)
{
	DNA* bestDNA = new DNA();

	for (DNA* dna : vecSource)
	{
		if(bestDNA->getScore() < dna->getScore())
			bestDNA = dna->copy();
	}

	return bestDNA;
}

DNA* GAM::popBestDNA(std::vector<DNA*>& vecSource)
{
	DNA* bestDNA = new DNA();
	int bestScore = -99999;
	int bestIndex = -1;
	
	for (int i = 0; i < vecSource.size(); i++)
	{
		DNA* dna = vecSource[i];
		if (bestScore >= dna->getScore())
			continue;

		bestDNA = dna;
		bestScore = dna->getScore();
		bestIndex = i;
	}

	// remove best dna from list
	if (bestIndex >= 0)
	{
		vecSource.erase(
			std::remove(vecSource.begin(), vecSource.end(), bestDNA), 
			vecSource.end()
		);
	}

	return bestDNA;
}

std::vector<DNA*> GAM::selection_rank()
{
	// no. of best chromosomes to select
	std::vector<DNA*> vecChromoCopy = m_vecChromos;
	std::vector<DNA*> vecBestChromos;

	for (int i = 0; i < (int)floor(m_iDnaCount / 2); i++)
	{
		DNA* bestDNA = popBestDNA(vecChromoCopy);
		vecBestChromos.push_back(bestDNA);
	}

	return vecBestChromos;
}

std::vector<DNA*> GAM::crossover(std::vector<DNA*>& vecSource)
{
	std::vector<DNA*> vecOffSpring = vecSource;
	int iCount = floor(vecSource.size() / 2);

	for (int i = 0; i < iCount; i++)
	{
		DNA* dnaA = vecSource[i];
		DNA* dnaB = vecSource[i + iCount];

		std::tuple<DNA*, DNA*> offspring = DNA::crossover(*dnaA, *dnaB);

		vecOffSpring.push_back(std::get<0>(offspring));
		vecOffSpring.push_back(std::get<1>(offspring));
	}

	for (int i = 0; i < m_vecChromos.size(); i++)
		vecOffSpring[i]->setBird(m_vecChromos[i]->getBird());
	
	return vecOffSpring;
}

std::vector<DNA*> GAM::mutation(std::vector<DNA*>& vecOffSpring, float fMutationPercentage)
{
	for (DNA* dna : vecOffSpring)
		dna->mutate(fMutationPercentage);

	return vecOffSpring;
}

void GAM::reset()
{
	for (DNA* dna : m_vecChromos)
		dna->reset();

	m_bIsWaitingForNewGame = false;
}

void GAM::loadChromosFromJson()
{
	// load chromos from json file
	std::vector<DNAJson> vecChromosJson;
	JsonParser::LoadJson(vecChromosJson, STORED_DNA_FILE_NAME);

	// convert json to dna
	std::vector<DNA*> vecChromos;
	for (DNAJson dnaJson : vecChromosJson)
	{
		DNA* dna = new DNA();
		dna->fromJson(dnaJson);
		dna->reset();
		vecChromos.push_back(dna);
	}

	// set chromos
	m_vecChromos = vecChromos;
}

void GAM::saveChromosToJson()
{
	// generate vector of chromos to save as json
	std::vector<DNAJson> vecChromosJson;
	for (DNA* dna : m_vecChromos)
	{
		DNAJson dnaJson = dna->toJson();
		vecChromosJson.push_back(dnaJson);
	}

	// save chromos to json file
	JsonParser::SaveJson(vecChromosJson, STORED_DNA_FILE_NAME);
}

void GAM::DebugReport()
{
	std::cout
		<< "\n<-------------->"
		<< "\nREPORT: Generation " << m_iGeneration;

	for (int i = 0; i < m_vecChromos.size(); i++)
	{
		std::cout << "\n\nChromosome: " << i;
		m_vecChromos[i]->DebugReport();
	}

	// save chromos to json file
	saveChromosToJson();
}