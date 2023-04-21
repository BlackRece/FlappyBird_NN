#include "GAM.h"

GAM::GAM() : m_iIndex(0), m_iGeneration(0), m_bIsWaitingForNewGame(false)
{
	// seed random number generator with current time
	std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
	std::chrono::system_clock::duration dtn = tp.time_since_epoch();
	srand(dtn.count());

	m_vecChromos = std::vector<DNA*>();

	// load population from json file
	//loadChromosFromJson();

	// if no population was loaded, create initial population
	//if (m_chromos.size() < 1)
		// create initial population
		//createPopulation(DNA_PER_CHROMOSOME);
}

void GAM::addToPopulation(GeneData gData, int dnaCount)
{
	m_iDnaCount = dnaCount;

	for (int i = 0; i < dnaCount; i++)
		m_vecChromos.push_back(new DNA(gData));
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

//void GAM::markGene()
//{
//	Gene& gene = getGene();
//	markAsPlaced(gene.getPosition());
//}
//
//void GAM::nextGene()
//{
//	if (m_isWaitingForNewGame)
//		return;
//
//	if (!m_chromos[m_index]->nextGene())
//		return;
//
//	m_isWaitingForNewGame = true;
//}

void GAM::nextDNA()
{
	if (m_iIndex + 1 >= m_vecChromos.size())
	{
		nextGeneration();
		return;
	}

	m_iIndex++;
	//m_chromos[m_index]->resetIndex();
}

//void GAM::storeFitnessData(int iScore, int iKills, int iDuration)
//{
//	m_chromos[m_index]->m_iScore = iScore;
//	m_chromos[m_index]->m_iKills = iKills;
//	m_chromos[m_index]->m_duration = iDuration;
//
//	m_isWaitingForNewGame = false;
//
//	nextDNA();
//}

void GAM::nextGeneration()
{
	DebugReport();

	// Selection: select the best chromosomes (aka Ranked Selection)
	std::vector<DNA*> vecSelected = selection_rank();

	// Mating Pool: create chromosome pool from selected chromosomes
	std::vector<DNA*> vecMatingPool = fastMatingPool(vecSelected);

	// Crossover: create new chromosomes from selected chromosomes
	std::vector<DNA*> vecCrossed = crossover(vecMatingPool);

	// Mutation: mutate crossed chromosomes using a mutation percentage
	m_vecChromos = mutation(vecCrossed, MUTATION_PERCENTAGE);

	/*if (USE_ELITE)
	{
		// Selection: select the best chromosomes (aka Elite/Ranked Selection)
		std::tuple<DNA*, std::vector<DNA*>> selectionResult = selection_elite();
		DNA* eliteDNA = std::get<0>(selectionResult);
		std::vector<DNA*> vecSelected = std::get<1>(selectionResult);

		// Mating Pool: create chromosome pool from selected chromosomes
		// std::vector<DNA*> vecMatingPool = matingPool(vecSelected);
		std::vector<DNA*> vecMatingPool = fastMatingPool(vecSelected);

		// Crossover: create new chromosomes from selected chromosomes
		std::vector<DNA*> vecCrossed = crossoverWithParent(*eliteDNA, vecMatingPool, CROSSOVER_STEP);

		// Mutation: mutate crossed chromosomes using a mutation percentage
		m_vecChromos = mutation(vecCrossed, MUTATION_PERCENTAGE);

		// add elite dna 
		m_vecChromos.push_back(eliteDNA);
		m_vecChromos.push_back(eliteDNA->reverse());
	}
	else
	{
		// Selection: select the best chromosomes (aka Elite/Ranked Selection)
		std::vector<DNA*> vecSelected = selection_rank();

		// Mating Pool: create chromosome pool from selected chromosomes
		// std::vector<DNA*> vecMatingPool = matingPool(vecSelected);
		std::vector<DNA*> vecMatingPool = fastMatingPool(vecSelected);

		// Crossover: create new chromosomes from selected chromosomes
		std::vector<DNA*> vecCrossed = crossover(vecMatingPool, CROSSOVER_STEP);

		// Mutation: mutate crossed chromosomes using a mutation percentage
		m_vecChromos = mutation(vecCrossed, MUTATION_PERCENTAGE);
	}*/

	// Reset: prepare for next generation
	reset();
}

//void GAM::createPopulation(int dnaCount)
//{
//	for (int i = 0; i < dnaCount; i++)
//	{
//		DNA* dna = new DNA();
//		dna->fillGenes();
//
//		m_chromos.push_back(dna);
//	}
//}

int GAM::getRandomIndex(std::vector<DNA*>& vecSource)
{
	float iChance = rand() % 100;
	for (int i = 0; i < vecSource.size(); i++)
	{
		float fFitness = vecSource[i]->fitness();
		iChance -= fFitness;
		if (iChance <= 0)
			return i;
	}

	return 0;
}

//int GAM::findPlacedIndex(sf::Vector2f pos)
//{
//	for (int i = 0; i < m_vecPlaced.size(); i++)
//	{
//		if (m_vecPlaced[i] == pos)
//			return i;
//	}
//
//	return -1;
//}

//int GAM::findDNAIndex(DNA& target, std::vector<DNA*> vecSource) {
//	for (int i = 0; i < vecSource.size(); i++)
//	{
//		if (target.isEqualTo(*vecSource[i]))
//			return i;
//	}
//	return -1;
//}

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
	int bestIndex = -1;
	for (int i = 0; i < vecSource.size(); i++)
	{
		if (bestDNA->getScore() >= vecSource[i]->getScore())
			continue;

		bestDNA = vecSource[i]->copy();
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

//bool GAM::isUnique(DNA& target, std::vector<DNA*>& vecSource)
//{
//	// if target is in list, the not unique
//	for (DNA* dna : vecSource)
//	{
//		if (target.isEqualTo(*dna))
//			return false;
//	}
//
//	return true;
//}

//std::vector<DNA*> GAM::matingPool(std::vector<DNA*>& vecSelected)
//{
//	std::vector<DNA*> vecMatingPool;
//	for (DNA* dna : vecSelected)
//	{
//		float fFitnessMultiplier = (dna->fitness() / 100.0f);
//		int iMatingPoolCount = floor(m_iDnaCount / 2);
//
//		// calculate how many times to add dna to mating pool
//		int iCount = floor(fFitnessMultiplier * iMatingPoolCount);
//
//		for (int j = 0; j < iCount; j++)
//		{
//			DNA* dnaCopy = dna->copy();
//			vecMatingPool.push_back(dnaCopy);
//		}
//	}
//	return vecMatingPool;
//}

std::vector<DNA*> GAM::fastMatingPool(std::vector<DNA*>& vecSelected)
{
	std::vector<DNA*> vecMatingPool;

	for (int i = 0; i < DNA_PER_CHROMOSOME; i++)
		vecMatingPool.push_back(vecSelected[getRandomIndex(vecSelected)]);

	return vecMatingPool;
}

//std::tuple<DNA*, std::vector<DNA*>> GAM::selection_elite()
//{
//	// copy chromos list 
//	std::vector<DNA*> vecCopiedChromos = m_vecChromos;
//
//	// select ELITE dna from copied chromos list
//	DNA* eliteDNA = popBestDNA(vecCopiedChromos);
//
//	// split copied chromos into 3 groups
//	std::vector<DNA*> vecFirst, vecSecond, vecThird;
//	
//	for (DNA* dna : vecCopiedChromos)
//	{
//		if (vecFirst.size() <= 3)
//			vecFirst.push_back(dna);
//		else if (vecSecond.size() <= 3)
//			vecSecond.push_back(dna);
//		else if (vecThird.size() <= 3)
//			vecThird.push_back(dna);
//	}
//
//	// select the best from each grouping
//	DNA* firstDNA = findBestDNA(vecFirst);
//	DNA* secondDNA = findBestDNA(vecSecond);
//	DNA* thirdDNA = findBestDNA(vecThird);
//
//	// store selected DNA into single array
//	std::vector<DNA*> vecBestChromos;
//	
//	//vecBestChromos.push_back(eliteDNA->reverse());
//	vecBestChromos.push_back(firstDNA);
//	vecBestChromos.push_back(secondDNA);
//	vecBestChromos.push_back(thirdDNA);
//
//	// return result
//	return std::make_tuple(eliteDNA, vecBestChromos);
//}

std::vector<DNA*> GAM::selection_rank()
{
	// no. of best chromosomes to select
	int iBestCount = (int)floor(m_iDnaCount / 2);

	std::vector<DNA*> vecChromoCopy = m_vecChromos;
	std::vector<DNA*> vecBestChromos;

	for (int i = 0; i < iBestCount; i++)
	{
		DNA* bestDNA = popBestDNA(vecChromoCopy);
		vecBestChromos.push_back(bestDNA);
	}

	return vecBestChromos;
}

std::vector<DNA*> GAM::crossover(std::vector<DNA*>& vecMatingPool)
{
	std::vector<DNA*> vecOffSpring;

	for (int i = 0; i < floor(vecMatingPool.size() / 2); i++)
	{
		DNA* dnaA = vecMatingPool[i];
		DNA* dnaB = vecMatingPool[i + 1];

		std::tuple<DNA*, DNA*> offspring = DNA::crossover(*dnaA, *dnaB);

		vecOffSpring.push_back(std::get<0>(offspring));
		vecOffSpring.push_back(std::get<1>(offspring));
	}

	return vecOffSpring;
}

//std::vector<DNA*> GAM::crossoverWithParent(DNA& parent, std::vector<DNA*>& vecMatingPool, int step)
//{
//	std::vector<DNA*> vecOffSpring;
//
//	for (int i = 0; i < floor(DNA_PER_CHROMOSOME / 2); i++)
//	{
//		DNA* dna = vecMatingPool[i];
//
//		std::tuple<DNA*, DNA*> offspring = DNA::crossover(parent, *dna, step);
//		vecOffSpring.push_back(std::get<0>(offspring));
//		vecOffSpring.push_back(std::get<1>(offspring));
//	}
//
//	return vecOffSpring;
//}

std::vector<DNA*> GAM::mutation(std::vector<DNA*>& vecOffSpring, float fMutationPercentage)
{
	for (DNA* dna : vecOffSpring)
		dna->mutate(fMutationPercentage);

	return vecOffSpring;
}

void GAM::reset()
{
	m_iIndex = 0;
	/*for (DNA* dna : m_vecChromos)
		dna->resetIndex();*/

	// increase generation count
	m_iGeneration++;

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

	// build file name from generation count
	std::string fileName = FILE_NAME + std::to_string(m_iGeneration) + FILE_EXT;

	// save chromos to json file
	JsonParser::SaveJson(vecChromosJson, fileName);
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