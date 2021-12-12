#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "commonlibs.h"
#include <unordered_map>
#include "Game.h"

#define CMD_OPEN_GAME_SELECTOR 12
#define CMD_OPEN_GAME 13
#define CMD_OPEN_GAME_RESULTS 14
#define CMD_QUIT_GAME 15

class GameState
{
private:

    static std::string                          m_selectedUsername;
    static uint16_t                             m_selectedGame;
    static std::vector<Game>                    m_verifiedGames;
    static std::unordered_map<int, uint16_t>    m_drowdownToGameIdMap;
    static uint16_t                             m_currentGuess;
    static std::vector<std::string>             m_guesses;
    static int                                  m_score;

public:

    static HWND                                 m_mainHWND;
    static HWND                                 m_childHWND;
    static HINSTANCE                            m_hInstance;

    static std::string getSelectedUsername();
    static void setSelectedUsername(const char* username);

    static uint16_t getSelectedGameId();
    static void setSelectedGameId(uint16_t gameId);

    static void loadGames();
    static std::vector<Game> getVerifiedGames();

    static void addDropdownMap(int dropdownIdx, uint16_t gameId);
    static uint16_t getMappedId(int dropdownIdx);

    static std::string getSelectedGameText();
    static bool getSelectedGame(Game &output);

    static bool guessClicked(const char* guess);

    static uint16_t GetSelectedGameMissingWordCount();
    static void CalculateScore();
    static void getScoreString(char* str);
    static uint16_t GetCurrentGuess();

    static void ResetGameState();
};
#endif // __GAMESTATE_H__
