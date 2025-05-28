#pragma once
#include <SFML/Graphics.hpp>
class Ghost;
class Player;
class TileMap;
class Score;
#include "bonusFruit.h"

class GameManager {
public:
    GameManager(TileMap& map, Player& player, Score& score, std::vector<Ghost>& ghosts);
    void update(int* pauseTimer);
    void checkLevelProgression(int* pauseTimer);
    void spawnBonusIfNeeded();
    void draw(sf::RenderWindow& window);

private:
    TileMap& tileMap;
    Player& player;
    Score& score;
    std::vector<Ghost>& ghosts;
    int currentLevel = 1;

    BonusFruit bonusFruit;
    std::vector<sf::Sprite> collectedFruits = {};

    sf::Texture lifeTexture;
    sf::Sprite lifeSprite;

    sf::Font font;

    std::string levelStr = "Level: 1";
    sf::Text levelText;

    std::string readyStr = "Ready?";
    sf::Text readyText;
    bool readyTextVisible = true;
};

