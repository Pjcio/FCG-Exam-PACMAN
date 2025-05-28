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
    void update();
    void updatePauseTimer();
    void checkLevelProgression();
    void playerRespawn();
    void spawnBonusIfNeeded();
    void draw(sf::RenderWindow& window);

    int pauseTimer = 120; // 2 second pause
private:
    TileMap& tileMap;
    Player& player;
    Score& score;
    std::vector<Ghost>& ghosts;
    int currentLevel = 1;
    
    int eatenGhostsInARow  = 0;
    bool anyGhostFrightened = false;

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

