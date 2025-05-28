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
    float velocity = FULL_SPEED * 0.75f;
    int direction;

    std::vector<sf::Texture> standardTextures;
    std::vector<sf::Texture> frightenedTextures;
    std::vector<sf::Texture> deadTextures;
    std::vector<sf::Texture> pointsTextures;
    sf::Sprite sprite;

    int aniFrame = 0;
    int aniFrameMax = 6;

    sf::Vector2i targetTile;

    int nextTurnTimer = 0;
    int spawnTimer;
    int stateTimer = 500;
    bool isSpawning = true;
    GhostState currentState = GhostState::Scatter;

    std::string ghostName; 
    sf::Vector2i currentTile;


    void chaseDirDecision(TileMap& tileMap);
    void frightenedDirDecision(TileMap& tileMap);
    void updateTargetTile(Player& player, Ghost& blinky);
    void updateState(TileMap& tileMap, Player& player);
    void moveSprite();

public:
    
    void updateSprite();
    void setPointSprite(int spriteIndex);

    Ghost(std::string ghostName);
    sf::FloatRect getCollisionBox();
    int getOppositeDirection();
    sf::Vector2i getCurrentTile();
    bool isCentered();
    bool isCageCentered();
    bool isAtSpawnPoint();

    void setState(GhostState newState, bool force = false, int currentLevel = 0);
    GhostState getState();


    void updateVelocity(int currentLevel, GhostState state = GhostState::Chase);
    void loadNextLevel(int currentLevel);
    void update(TileMap& tileMap, Player& player, Ghost& blinky);
    void draw(sf::RenderWindow& window);
};
