#include "GameState.h"
#include "GameLoader.h"
#include "xmlLoaderDll.h"

uint16_t                            GameState::m_selectedGame = INVALID_GAME_ID;
std::string                         GameState::m_selectedUsername;
std::vector<Game>                   GameState::m_verifiedGames;
std::unordered_map<int, uint16_t>   GameState::m_drowdownToGameIdMap;
uint16_t                            GameState::m_currentGuess = 0;
std::vector<std::string>            GameState::m_guesses;
int                                 GameState::m_score = -1;
HWND                                GameState::m_mainHWND;
HWND                                GameState::m_childHWND;
HINSTANCE                           GameState::m_hInstance;


std::string GameState::getSelectedUsername()                { return m_selectedUsername; }

void GameState::setSelectedUsername(const char* username)   { m_selectedUsername = username; }

uint16_t GameState::getSelectedGameId()                     { return m_selectedGame; }

void GameState::setSelectedGameId(uint16_t gameId)          { m_selectedGame = gameId; }

void GameState::loadGames()
{
    const char* dirpath = "games"; // make a DEFINE probs
    std::vector<std::string> gameNamesFound = LoadFromDir(dirpath);

    // prepend path to file
    for (auto &it: gameNamesFound)
    {
        char out[100];
        sprintf(out, "%s\\%s", dirpath, it.c_str());
        it = out;
    }

    m_verifiedGames = GameLoader::LoadVerifiedGames(gameNamesFound);
}

std::vector<Game> GameState::getVerifiedGames()
{
    if (m_verifiedGames.empty()) // only ask if some time passed since last check that didn't find any games
        loadGames();

    return m_verifiedGames;
}

void GameState::addDropdownMap(int dropdownIdx, uint16_t gameId)
{
    if (-1 == dropdownIdx)
        return;

    // add a check for duplicate value.
    // every added gameId should be unique

    m_drowdownToGameIdMap[dropdownIdx] = gameId;
}

uint16_t GameState::getMappedId(int dropdownIdx)
{
    if (-1 == dropdownIdx)
        return 0;

    return m_drowdownToGameIdMap[dropdownIdx];
}

std::string GameState::getSelectedGameText()
{
    Game selectedGame;
    auto success = getSelectedGame(selectedGame);
    if (!success)
        return nullptr;

    return selectedGame.m_gametext;
}

bool GameState::getSelectedGame(Game &output)
{
    if (INVALID_GAME_ID == m_selectedGame)
        return false;

    auto verifiedGames = getVerifiedGames();
    for (auto &it: verifiedGames)
    {
        if (it.m_gameId == m_selectedGame)
        {
            output = it;
            return true;
        }
    }

    return false;
}

// Returns whether it was the last word guessed
//  (if the last word was guessed, dialog should handle switching to Score table or whatever is appropriate after game is finished)
bool GameState::guessClicked(const char* guess)
{
    auto missingWordCount = GetSelectedGameMissingWordCount();
    if (m_currentGuess >= missingWordCount)
        return false;

    m_guesses.push_back(guess);

    // increment inside if condition since it needs to be incremented even if it doesn't step inside this if body
    if (m_currentGuess++ == missingWordCount - 1)
    {
        // finish the game
        CalculateScore();
        return true;
    }

    return false;
}

uint16_t GameState::GetSelectedGameMissingWordCount()
{
    Game game;
    bool success = getSelectedGame(game);

    if (!success)
        return 0;

    return (uint16_t)game.m_answers.size();
}

void GameState::CalculateScore()
{
    Game game;
    bool success = getSelectedGame(game);

    if (!success)
        return;

    if (m_guesses.size() != game.m_answers.size())
        return;

    m_score = 0;
    for (uint16_t i = 0; i < game.m_answers.size(); i++)
        if (game.m_answers[i].compare(m_guesses[i]) == 0)
            m_score++;
}

void GameState::getScoreString(char* str)
{
    if (-1 == m_score)
        CalculateScore();

    if (-1 == m_score)
        sprintf(str, "x/X");

    sprintf(str, "%d/%d", m_score, GetSelectedGameMissingWordCount());
}

void GameState::ResetGameState()
{
    m_selectedGame = INVALID_GAME_ID;
    m_currentGuess = 0;
    m_guesses.clear();
    m_score = -1;
}

uint16_t GameState::GetCurrentGuess() { return m_currentGuess; }
