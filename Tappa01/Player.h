#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>

#include "TileMap.h"
#include "utils.h"
#include "constant.h"

class Player {
private:
    int velocity = 3;    // Velocity for movement
    int direction; // Direction of movement (0: none, 1: up, 2: down, 3: left, 4: right)
    std::vector<int> movementBuffer;  // Buffer for active directions
    std::vector<sf::Texture> frames; // Animation frames
    sf::Sprite* sprite;

    bool AniSwitch = false; // 2 frame animation switch

public:
    Player();
    Player(int x, int y);

    void setDirection(int dir);
    void releaseDirection(int dir);
    void updateDirection();

    void update();
    void draw(sf::RenderWindow& window);
};
