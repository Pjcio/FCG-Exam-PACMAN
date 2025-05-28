#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "TileMap.h"
#include "Score.h"
#include "utils.h"
#include "constants.h"

class Player {
private:
    float velocity = WINDOW_RESIZE;
    int direction = 3;
    int buffered_dir = 0;
    std::vector<sf::Texture> frames;
    sf::Sprite* sprite;
    int AniFrame = 0;
    int AniFrameMax = 6;
    int lives = 3;


public:
    sf::Vector2i currentTile = {14, 23};

    Player();
    bool checkCollision(TileMap& tileMap, int dir);
    sf::FloatRect getCollisionBox();
    int getLifes();
    bool isCentered();
    void setDirection(int dir);
    void update(TileMap& tileMap, Score& score);
    void loadNextLevel();
    void draw(sf::RenderWindow& window);
};
