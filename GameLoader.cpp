#include "commonlibs.h"
#include <iostream>

#include "GameLoader.h"
#include "tinyxml2.h"

size_t CountMissingWordsFromText(const char *text)
{
    std::string txt = text;
    size_t index = 0;
    size_t wordCount = 0;
    while (true)
    {
        index = txt.find("_", index);
        if (index++ == std::string::npos)
            break;
        wordCount++;
    }
    return wordCount;
}

Game* ValidateGame(tinyxml2::XMLElement* game)
{
    tinyxml2::XMLElement* gametext = game->FirstChildElement("Text");
    const tinyxml2::XMLAttribute* gameDifficulty = game->FindAttribute("difficulty");
    const tinyxml2::XMLAttribute* gameName = game->FindAttribute("name");

    if (nullptr == gameDifficulty)
        return nullptr;

    const char* diff = gameDifficulty->Value();
    const char* name = "untitled";
    if (nullptr != gameName)
        name = gameName->Value();

    if (nullptr == gametext)
        return nullptr;

    const char* gameText = gametext->GetText();
    if (nullptr == gameText)
        return nullptr;

    const size_t missingWordCount = CountMissingWordsFromText(gameText);
    std::cout << "missingWordCount " << missingWordCount << std::endl;
    if (0 == missingWordCount)
        return nullptr;

    tinyxml2::XMLElement* missingWordsElement = game->FirstChildElement("MissingWords");
    if (nullptr == missingWordsElement)
        return nullptr;

    tinyxml2::XMLElement* missingWordElement = missingWordsElement->FirstChildElement("Word");
    if (nullptr == missingWordElement)
        return nullptr;

    std::vector<std::string> missingWords;
    while (true)
    {
        missingWords.push_back(missingWordElement->GetText());
        missingWordElement = missingWordElement->NextSiblingElement("Word");
        if (nullptr == missingWordElement)
            break;
    }

    if (missingWordCount != missingWords.size())
        return nullptr;

    auto ret = new Game(name, gameText, missingWords, std::stoi(diff));
    return ret;
}

void numberTheMissing(Game* game)
{
    size_t currentPos = 0;
    int nth_missing = 1;
    while (true)
    {
        currentPos = game->m_gametext.find("_", currentPos + (currentPos == 0 ? 0 : 9));
        if (currentPos == std::string::npos)
            break;

        char numberedMissing[11]; // will output "__n__"
        sprintf(numberedMissing, "____%d____", nth_missing++);
        game->m_gametext.replace(currentPos, 1, numberedMissing);
    }
}

std::vector<Game> GameLoader::LoadVerifiedGames(std::vector<std::string> gamePaths)
{
    std::vector<Game> result;
    tinyxml2::XMLDocument doc;

    for (auto &path: gamePaths)
    {
        std::cout << path.c_str() << std::endl;
        if (tinyxml2::XML_SUCCESS != doc.LoadFile(path.c_str()))
            continue;

        tinyxml2::XMLElement* pRootElement = doc.RootElement();
        if (nullptr == pRootElement)
            continue;

        tinyxml2::XMLElement* gameEl = pRootElement->FirstChildElement("Game");

        // at least a single game tag should exist for an XML describing a game
        if (nullptr == gameEl)
            continue;

        tinyxml2::XMLElement* nextGame = gameEl;
        while (nullptr != nextGame)
        {
            auto gameDef = ValidateGame(nextGame);
            if (nullptr != gameDef){
                numberTheMissing(gameDef);
                result.push_back(*gameDef);
                std::cout << "Successfully loaded game: " << gameDef->m_name << std::endl;
                std::cout << "Game difficulty: "<< gameDef->m_difficulty << std::endl;
                std::cout << "Game text: "<< gameDef->m_gametext << std::endl;
                for (auto &ans: gameDef->m_answers)
                    std::cout << ans << std::endl;
                std::cout << "----------------------------------" << std::endl;

            }
            nextGame = nextGame->NextSiblingElement("Game");
        }
    }
    return result;
}


