#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "utils.h"
#include "constants.h"
class TileMap;
class Score;
class Ghost;

class Player {
private:
    float velocity = FULL_SPEED * 0.8f;
    int direction = 3;
    int buffered_dir = 0;
    std::vector<sf::Texture> frames;
    sf::Sprite sprite;
    int AniFrame = 0;
    int AniFrameMax = 6;
    int lives = 3;
    sf::Vector2i currentTile = {14, 23};

    // Death animation variables
    bool isDying = false;
    int deathFrame = 0;
    int deathTimer = 0;
    std::vector<sf::Texture> deathAnimationTextures;

public:

    Player();
    bool checkCollision(TileMap& tileMap, int dir);
    sf::FloatRect getCollisionBox();
    int getLifes();
    void addLife();
    void loseLife();
    sf::Vector2i getCurrentTile();
    int getDirection();
    bool isCentered();
    void setDirection(int dir);
    bool isInDyingAnimation();
    void die();
    void deathAnimation();
    void update(TileMap& tileMap, Score& score);
    void updateVelocity(int currentLevel, bool isFrightened = false);
    void loadNextLevel(int currentLevel);
    void draw(sf::RenderWindow& window);
};
