#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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
    bool isGameOver();
    bool isInVictoryAnimation();
    void loadNextLevel();
    void playerRespawn();
    void spawnBonusIfNeeded();
    void draw(sf::RenderWindow& window);

    int pauseTimer = 240; // 4 second intro
private:
    TileMap& tileMap;
    Player& player;
    Score& score;
    std::vector<Ghost>& ghosts;
    int currentLevel = 1;

    bool awaitingRespawn = false;

    int eatenGhostsInARow = 0;

    BonusFruit bonusFruit;
    std::vector<sf::Sprite> collectedFruits = {};

    sf::Texture lifeTexture;
    sf::Sprite lifeSprite;
    int nextLifeScore = 10000;

    sf::Font font;

    std::string levelStr = "Level: 1";
    sf::Text levelText;

    std::string readyStr = "Ready?";
    sf::Text readyText;
    bool readyTextVisible = true;

    bool gameOver = false;
    sf::Text gameOverText;

    bool victoryAnimationActive = false;
    bool bluePhase = true;


    // Sound effects
    sf::SoundBuffer introSoundBuffer;
    sf::SoundBuffer deathSoundBuffer;
    sf::SoundBuffer eatGhostSoundBuffer;
    sf::SoundBuffer eatFruitSoundBuffer;
    sf::SoundBuffer extraPacmanSoundBuffer;
    sf::Sound soundEffect;
    sf::SoundBuffer chomp1SoundBuffer;
    sf::SoundBuffer chomp2SoundBuffer;
    sf::Sound chompSoundEffect;
    sf::SoundBuffer ghostWanderingSoundBuffer;
    sf::SoundBuffer ghostFrightenedSoundBuffer;
    sf::SoundBuffer ghostEatenSoundBuffer;
    sf::Sound ghostSoundEffect;
};

