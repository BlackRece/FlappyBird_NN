#pragma once

#include <sstream>
#include "DEFINITIONS.hpp"
#include "GameState.hpp"
#include "GameOverState.hpp"
#include "AIController.h"

#include <iostream>

#define PLAY_WITH_AI true
#define BIRD_COUNT 10

namespace Sonar
{
	GameState::GameState(GameDataRef data) : _data(data)
	{
		m_pAIController = new AIController();
		//m_pAIController->setGameState(this);
	}

	GameState::~GameState()
	{
		delete pipe;
		delete land;

		delete bird;
		for (auto& bird : vecBirds)
		{
			delete bird;
			bird = nullptr;
		}
		vecBirds.clear();

		delete m_pAIController;

		delete flash;
		delete hud;
	}

	void GameState::Init()
	{
		//if (!_hitSoundBuffer.loadFromFile(HIT_SOUND_FILEPATH))
		//{
		//	std::cout << "Error Loading Hit Sound Effect" << std::endl;
		//}

		//if (!_wingSoundBuffer.loadFromFile(WING_SOUND_FILEPATH))
		//{
		//	std::cout << "Error Loading Wing Sound Effect" << std::endl;
		//}

		//if (!_pointSoundBuffer.loadFromFile(POINT_SOUND_FILEPATH))
		//{
		//	std::cout << "Error Loading Point Sound Effect" << std::endl;
		//}

		/*_hitSound.setBuffer(_hitSoundBuffer);
		_wingSound.setBuffer(_wingSoundBuffer);
		_pointSound.setBuffer(_pointSoundBuffer);*/

		this->_data->assets.LoadTexture("Game Background", GAME_BACKGROUND_FILEPATH);
		this->_data->assets.LoadTexture("Pipe Up", PIPE_UP_FILEPATH);
		this->_data->assets.LoadTexture("Pipe Down", PIPE_DOWN_FILEPATH);
		this->_data->assets.LoadTexture("Land", LAND_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 1", BIRD_FRAME_1_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 2", BIRD_FRAME_2_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 3", BIRD_FRAME_3_FILEPATH);
		this->_data->assets.LoadTexture("Bird Frame 4", BIRD_FRAME_4_FILEPATH);
		this->_data->assets.LoadTexture("Scoring Pipe", SCORING_PIPE_FILEPATH);
		this->_data->assets.LoadFont("Flappy Font", FLAPPY_FONT_FILEPATH);

		pipe = new Pipe(_data);
		land = new Land(_data);
		bird = new Bird(_data);

		m_pAIController->initBirds(_data);
		
		flash = new Flash(_data);
		hud = new HUD(_data);

		_background.setTexture(this->_data->assets.GetTexture("Game Background"));

		_score = 0;
		hud->UpdateScore(_score);

		_gameState = GameStates::eReady;
	}

	void GameState::HandleInput()
	{
		if (PLAY_WITH_AI)
		{
			if (GameStates::eGameOver != _gameState)
			{
				_gameState = GameStates::ePlaying;

				m_pAIController->handleInput(this);

				//if (m_pAIController->shouldFlap())
				//{
				//	//std::cout << "tap!" << std::endl;
				//	bird->Tap();
				//	_wingSound.play();
				//}
				//else 
				//{
				//	//std::cout << "not tap :(" << std::endl;
				//}
			}
		}

		sf::Event event;
		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->_data->window.close();
			}

			if (this->_data->input.IsSpriteClicked(this->_background, sf::Mouse::Left, this->_data->window))
			{
				if (GameStates::eGameOver != _gameState)
				{
					_gameState = GameStates::ePlaying;
					bird->Tap();

					_wingSound.play();
				}
			}
		}
	}

	void GameState::Update(float dt)
	{
		std::vector<DNA*> vecBirdDna = m_pAIController->getDna();
		
		if (GameStates::eGameOver != _gameState)
		{
			for (DNA* birdDna : vecBirdDna)
				birdDna->getBird()->Animate(dt);

			land->MoveLand(dt);
		}

		if (GameStates::ePlaying == _gameState)
		{
			pipe->MovePipes(dt);

			if (clock.getElapsedTime().asSeconds() > PIPE_SPAWN_FREQUENCY)
			{
				pipe->RandomisePipeOffset();

				pipe->SpawnInvisiblePipe();
				pipe->SpawnBottomPipe();
				pipe->SpawnTopPipe();
				pipe->SpawnScoringPipe();

				clock.restart();
			}

			for (DNA* birdDna : vecBirdDna)
				birdDna->update(dt);

			std::vector<sf::Sprite> landSprites = land->GetSprites();
			std::vector<sf::Sprite> pipeSprites = pipe->GetSprites();

			for (DNA* birdDna : vecBirdDna)
			{
				for (unsigned int i = 0; i < landSprites.size(); i++)
				{
					if (collision.CheckSpriteCollision(
						birdDna->getBird()->GetSprite(),
						0.7f, 
						landSprites.at(i), 
						1.0f, 
						false))
					{
						birdDna->onHit(Hit::Floor);
					}
				}

				for (unsigned int i = 0; i < pipeSprites.size(); i++)
				{
					if (collision.CheckSpriteCollision(
						birdDna->getBird()->GetSprite(),
						0.625f, 
						pipeSprites.at(i),
						1.0f, 
						true))
					{
						birdDna->onHit(Hit::Pipe);
					}
				}
			}

			if(m_pAIController->isAllBirdsDead())
			{
				_gameState = GameStates::eGameOver;

				clock.restart();

				//_hitSound.play();
			}

			if (GameStates::ePlaying == _gameState)
			{
				std::vector<sf::Sprite> &scoringSprites = pipe->GetScoringSprites();

				for (unsigned int i = 0; i < scoringSprites.size(); i++)
				{
					bool hasScored = false;
					for (DNA* birdDna : vecBirdDna)
					{
						if (hasScored)
						{
							birdDna->onHit(Hit::Gap);
							continue;
						}

						if (collision.CheckSpriteCollision(
							birdDna->getBird()->GetSprite(),
							0.625f, 
							scoringSprites.at(i),
							1.0f,
							false))
						{
							_score++;
							hasScored = true;
							birdDna->onHit(Hit::Gap);

							hud->UpdateScore(_score);

							scoringSprites.erase(scoringSprites.begin() + i);

							//_pointSound.play();
						}
					}
				}
			}
		}

		if (GameStates::eGameOver == _gameState)
		{

			//flash->Show(dt);

			//if (clock.getElapsedTime().asSeconds() > TIME_BEFORE_GAME_OVER_APPEARS)
			{
				m_pAIController->gameOver(dt);
				
				//this->_data->machine.AddState(StateRef(new GameOverState(_data, _score)), true);
				//this->_data->machine.AddState(StateRef(new GameState(_data)), true);
				this->_data->machine.AddState(StateRef(this), false);
			}
		}
	}

	void GameState::Draw(float dt)
	{
		this->_data->window.clear( sf::Color::Red );

		this->_data->window.draw(this->_background);

		pipe->DrawPipes();
		land->DrawLand();

		//bird->Draw();
		if (GameStates::eGameOver != _gameState)
			m_pAIController->draw();

		//flash->Draw();

		hud->Draw();

		this->_data->window.display();
	}
}