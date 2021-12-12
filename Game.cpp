#include "Game.h"

uint16_t Game::m_lastGameId = INVALID_GAME_ID;

Game::Game(std::string name, std::string gameText, std::vector<std::string> answers, int difficulty/* = 0*/)
{
    m_name = name;
    m_gametext = gameText;
    m_answers = answers;
    m_difficulty = difficulty;
    m_gameId = GenerateNextId();
}

uint16_t Game::GenerateNextId() { return ++m_lastGameId; }
