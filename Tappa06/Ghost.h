#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "utils.h"
#include "constants.h"
class TileMap;
class Player;
class Score;
class Ghost;

enum class GhostState {
    Scatter,
    Chase,
    Frightened,
    Dead
};

class Ghost {
private:
    float velocity = WINDOW_RESIZE;
    int direction;

    std::vector<sf::Texture> standardTextures;
    std::vector<sf::Texture> frightenedTextures;
    std::vector<sf::Texture> deadTextures;
    sf::Sprite sprite;

    int aniFrame = 0;
    int aniFrameMax = 6;

    sf::Vector2i targetTile;
    int stateTimer = 500;
    GhostState currentState = GhostState::Scatter;

    std::string ghostName; 
    sf::Vector2i currentTile = {14, 11};


    void chaseDirDecision(TileMap& tileMap);
    void frightenedDirDecision(TileMap& tileMap);
    void updateTargetTile(Player& player);
    void updateState(TileMap& tileMap, Player& player);
    void moveSprite();

public:
    
    void updateSprite();

    Ghost(std::string ghostName);
    sf::FloatRect getCollisionBox();
    int getOppositeDirection();
    sf::Vector2i getCurrentTile();
    bool isCentered();

    void setState(GhostState newState);
    GhostState getState() const { return currentState; }

    void loadNextLevel();
    void update(TileMap& tileMap, Player& player);
    void draw(sf::RenderWindow& window);
};
