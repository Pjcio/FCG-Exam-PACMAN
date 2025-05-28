#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "TileMap.h"
#include "Player.h"
#include "Score.h"
#include "bonusFruit.h"

class GameManager {
public:
    GameManager(TileMap& map, Player& player, Score& score);
    void update(int* pauseTimer);
    void checkLevelProgression(int* pauseTimer);
    void spawnBonusIfNeeded();
    void draw(sf::RenderWindow& window);

private:
    TileMap& tileMap;
    Player& player;
    Score& score;
    int currentLevel = 1;

    BonusFruit bonusFruit;
    std::vector<sf::Sprite> collectedFruits = {};

    sf::Texture lifeTexture;
    sf::Sprite* lifeSprite;

    sf::Font font;

    std::string levelStr = "Level: 1";
    sf::Text* levelText = nullptr;

    std::string readyStr = "Ready?";
    sf::Text* readyText = nullptr;
    bool readyTextVisible = true;
};

#endif // GAMEMANAGER_H
