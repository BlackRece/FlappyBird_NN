#pragma once
#ifndef GAM_H
#define GAM_H

#include <chrono>
#include <vector>

#include "DNA.h"
#include <nlohmann/json.hpp>
#include "JsonParser.h"
#include "Structures.h"

class Sonar::Bird;

constexpr int DNA_PER_CHROMOSOME = 10;
constexpr int CROSSOVER_STEP = 5;
constexpr float MUTATION_PERCENTAGE = 5.0f;	// 1.0f = 1.0%
constexpr bool USE_ELITE = true;

constexpr auto FILE_NAME = "chromos_";
constexpr auto FILE_EXT = ".json";
constexpr auto STORED_DNA_FILE_NAME = "chromos.json";

class GAM
{
public:
	GAM();
	~GAM() {};

	void addToPopulation(GeneData gData, int dnaCount);
	std::vector<Sonar::Bird*> getBirds();
	std::vector<DNA*> getChromos() { return m_vecChromos; }

	void update(float dt);
	bool isAllDead();
	void draw();

	//inline void storeScore(int iScore) { m_chromos[m_index]->m_iScore = iScore; }
	//inline void storeKills(int iKills) { m_chromos[m_index]->m_iKills = iKills; }
	//inline void storeDuration(int iDuration) { m_chromos[m_index]->m_duration = iDuration; }

	//inline bool isPlaced(sf::Vector2f pos) { return findPlacedIndex(pos) >= 0; }
	//inline void markAsPlaced(sf::Vector2f pos) { if (findPlacedIndex(pos) < 0) m_vecPlaced.push_back(pos); }
	//inline sf::Vector2f getPlacedPosition() { return m_vecPlaced[rand() % m_vecPlaced.size() - 1]; }

	//inline Gene& getGene() { return m_chromos[m_index]->getGene(); }
	//void markGene();
	//void nextGene();
	void nextDNA();

	void nextGeneration();
	//void storeFitnessData(int iScore, int iKills, int iDuration);

private:
	//void createPopulation(int iDnaCount);
	int getRandomIndex(std::vector<DNA*>& source);
	//int findPlacedIndex(sf::Vector2f pos);

	//int findDNAIndex(DNA& target, std::vector<DNA*> vecSource);
	DNA* findBestDNA(std::vector<DNA*> vecSource);
	DNA* popBestDNA(std::vector<DNA*>& vecSource);
	//bool isUnique(DNA& target, std::vector<DNA*>& vecSource);

	//std::vector<DNA*> matingPool(std::vector<DNA*>& vecSelected);
	std::vector<DNA*> fastMatingPool(std::vector<DNA*>& vecSelected);

	//std::tuple<DNA*, std::vector<DNA*>> selection_elite();
	std::vector<DNA*> selection_rank();
	std::vector<DNA*> crossover(std::vector<DNA*>& vecMatingPool);
	//std::vector<DNA*> crossoverWithParent(DNA& parent, std::vector<DNA*>& vecMatingPool, int step);
	std::vector<DNA*> mutation(std::vector<DNA*>& vecCrossed, float fPercent);

	void reset();

	void loadChromosFromJson();
	void saveChromosToJson();

	void DebugReport();

	std::vector<DNA*>	m_vecChromos;

	int					m_iDnaCount;
	int					m_iIndex;
	int					m_iGeneration;
	bool				m_bIsWaitingForNewGame;
};

#endif // !GAM_H
