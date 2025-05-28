#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

#include "TileMap.h"
#include "utils.h"
#include "constant.h"

class Player {
private:
    int velocity = WINDOW_RESIZE;    // Velocity for movement
    int direction = 3; // Direction of movement (0: none, 1: up, 2: down, 3: left, 4: right)
    int buffered_dir = 0; 
    std::vector<int> movementBuffer;  // Buffer for active directions
    std::vector<sf::Texture> frames; // Animation frames
    sf::Sprite* sprite;

    int AniFrame = 0; // Current frame of the animation
    int AniFrameMax = 6; // Max frame of the animation

public:
    sf::Vector2i currentTile = {14, 23};

    Player();
    Player(int x, int y);

    bool isCentered();
    bool checkCollision(TileMap& tileMap, int dir);

    void setDirection(int dir);
    void releaseDirection(int dir);
    void updateDirection();

    void update(TileMap& tileMap);
    void draw(sf::RenderWindow& window);
};
