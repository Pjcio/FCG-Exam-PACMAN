#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "TileMap.h"
#include "Score.h"
#include "utils.h"
#include "constants.h"
#include "Player.h"

class Ghost {
private:
    float velocity = WINDOW_RESIZE;
    int direction;;

    std::vector<sf::Texture> standardTextures;
    std::vector<sf::Texture> frightenedTextures;
    std::vector<sf::Texture> deadTextures;
    sf::Sprite sprite;

    int aniFrame = 0;
    int aniFrameMax = 6;

    sf::Vector2i targetTile;
    int chaseAndScatterTimer = 500;
    bool isChasing = false;
    bool isFrightened = false;
    bool isDead = false;

    std::string ghostName; 

    sf::Vector2i currentTile = {14, 11};
public:
    

    Ghost(std::string ghostName);
    sf::FloatRect getCollisionBox();
    int getOppositeDirection();
    bool isCentered();
    void updateTargetTile(Player& player);
    void updateSprite();
    void chaseDecision(TileMap& tileMap);
    void update(TileMap& tileMap, Player& player);
    void loadNextLevel();
    void draw(sf::RenderWindow& window);
};
