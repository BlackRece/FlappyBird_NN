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

constexpr auto FILE_NAME = "chromos";
constexpr auto FILE_EXT = ".json";
constexpr auto STORED_DNA_FILE_NAME = "chromos.json";

class GAM
{
public:
	GAM();
	~GAM() {};

	void addToPopulation(GeneData gData, int dnaCount);
	void addTrainedBrain(NN* nn);
	DNA* getBirdToTrain();
	std::vector<Sonar::Bird*> getBirds();
	std::vector<DNA*> getChromos() { return m_vecChromos; }

	void update(float dt);
	bool isAllDead();
	void draw();

	void nextGeneration(DNA* eliteDNA);

private:
	DNA* findBestDNA(std::vector<DNA*> vecSource);
	DNA* popBestDNA(std::vector<DNA*>& vecSource);

	int getRandomIndex(std::vector<DNA*>& vecSource) { return rand() % vecSource.size(); }

	std::vector<DNA*> selection_rank();
	std::vector<DNA*> crossover(std::vector<DNA*>& vecMatingPool);
	std::vector<DNA*> mutation(std::vector<DNA*>& vecCrossed, float fPercent);

	void reset();

	void loadChromosFromJson();
	void saveChromosToJson();

	void DebugReport();

	std::vector<DNA*>	m_vecChromos;

	int					m_iDnaCount;
	int					m_iGeneration;
	bool				m_bIsWaitingForNewGame;
};

#endif // !GAM_H
