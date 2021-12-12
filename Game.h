#ifndef GAME_H
#define GAME_H

#include "commonlibs.h"

#define INVALID_GAME_ID 100

class Game
{
private:
    static uint16_t m_lastGameId;

    static uint16_t GenerateNextId();
public:
    Game() { };
    std::string m_name;
    std::string m_gametext;
    std::vector<std::string> m_answers;
    int m_difficulty;
    uint16_t m_gameId;

    Game(std::string name, std::string gameText, std::vector<std::string> answers, int difficulty = 0);
};

#endif
