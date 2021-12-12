#include "commonlibs.h"

#include "Game.h"

class GameLoader
{
public:
    static std::vector<Game> LoadVerifiedGames(std::vector<std::string> gamePaths);
};
