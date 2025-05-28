#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "TileMap.h"
#include "Score.h"
#include "utils.h"
#include "constants.h"
#include "Ghost.h"
#include "Player.h"

Ghost::Ghost(std::string ghostName)
    : standardTextures(loadAssetsVectorFromDirectory("resources/ghosts/" + ghostName)),
      frightenedTextures(loadAssetsVectorFromDirectory("resources/ghosts/frightened")),
      deadTextures(loadAssetsVectorFromDirectory("resources/ghosts/dead")),
      sprite(standardTextures[0]) // Initialize the sprite with the first texture
{
    // Scale and position adjustments
    sprite.setScale(sf::Vector2f(WINDOW_RESIZE, WINDOW_RESIZE));
    sprite.setPosition(sf::Vector2f(TILE_SIZE * currentTile.x, TILE_SIZE * currentTile.y + SCORE_BOARD_HEIGHT + TILE_SIZE / 2)); // NOT FINAL
    sprite.setOrigin(sf::Vector2f(7, 7));

    this->ghostName = ghostName;
    
    // Set targetTile based on ghostName
    if (ghostName == "Blinky") {
        targetTile = {25, 0};  // Top-right corner
    } else if (ghostName == "Pinky") {
        targetTile = {2, 0};   // Top-left corner
    } else if (ghostName == "Inky") {
        targetTile = {27, 31}; // Bottom-right corner
    } else if (ghostName == "Clyde") {
        targetTile = {0, 31};  // Bottom-left corner
    }

    direction = rand() % 2 + 3; // Random initial direction (3-4)
}


sf::FloatRect Ghost::getCollisionBox() {
    sf::FloatRect bounds = sprite.getGlobalBounds();  // Cambiato da sprite->getGlobalBounds() a sprite.getGlobalBounds()
    sf::Vector2f center = bounds.getCenter();

    sf::Vector2f halfSize = bounds.size / 2.f;

    return sf::FloatRect(
        center - halfSize / 2.f,
        halfSize
    );
}

int Ghost::getOppositeDirection(){
    switch(direction){
        case 1: return 2;
        case 2: return 1;
        case 3: return 4;
        case 4: return 3;
        default: return 0;
    }
}

bool Ghost::isCentered() {
    int center = TILE_SIZE / 2;
    int xOffset = std::abs(int(sprite.getPosition().x) % TILE_SIZE - center);  // Cambiato da sprite->getPosition() a sprite.getPosition()
    int yOffset = std::abs(int(sprite.getPosition().y) % TILE_SIZE - center);  // Cambiato da sprite->getPosition() a sprite.getPosition()

    return (xOffset <= 2 && yOffset <= 2);
}

// Update sprite texture and animation frame
void Ghost::updateSprite() {

    // Animation frame logic
    int frameIndex = 0;
    if(aniFrame < 3) {
        aniFrame++;
    } else if (aniFrame < aniFrameMax) {
        frameIndex = 1;
        aniFrame++;
    } else if (aniFrame >= aniFrameMax) {
        aniFrame = 0;
    }

    int textureIndex;
    // Calcola offset base in base alla direzione
    switch (direction) {
        case 1: textureIndex = 0; break; // Su
        case 2: textureIndex = 2; break; // Giù
        case 3: textureIndex = 4; break; // Sinistra
        case 4: textureIndex = 6; break; // Destra
        default: textureIndex = 0; break;
    }

    textureIndex += frameIndex;

    if (isFrightened) {
        sprite.setTexture(frightenedTextures[textureIndex]);  // Cambiato da sprite->setTexture() a sprite.setTexture()
    } else if (isDead) {
        // Dead ha solo 1 frame per direzione
        sprite.setTexture(deadTextures[textureIndex / 2]);  // Cambiato da sprite->setTexture() a sprite.setTexture()
    } else {
        sprite.setTexture(standardTextures[textureIndex]);  // Cambiato da sprite->setTexture() a sprite.setTexture()
    }
}


void Ghost::chaseDecision(TileMap& tileMap) {
    if (currentTile == sf::Vector2i(15, 11) ||  // Spawn area hard coded tiles
        currentTile == sf::Vector2i(12, 11) ||
        currentTile == sf::Vector2i(15, 23) ||  // Bottom hard coded tiles
        currentTile == sf::Vector2i(12, 23)) {
            if(direction == 2) direction = rand() % 2 + 3; // Random direction (3-4)
            return; 
    }

    const std::vector<sf::Vector2i> directions = {
        {  0, -1 },  // 1 = up
        {  0,  1 },  // 2 = down
        { -1,  0 },  // 3 = left
        {  1,  0 }   // 4 = right
    };

    int bestDir = 0;
    float minDistance = std::numeric_limits<float>::max();

    for (int i = 0; i < directions.size(); ++i) {
        if((i+1) == getOppositeDirection()) continue;

        sf::Vector2i neighbor = currentTile + directions[i];

        if (!tileMap.isTileSolid(neighbor.x, neighbor.y)) {
            float dx = static_cast<float>(targetTile.x - neighbor.x);
            float dy = static_cast<float>(targetTile.y - neighbor.y);
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < minDistance) {
                minDistance = distance;
                bestDir = i + 1; // direction start from 1
            }
        }
    }

    // update to most optimal direction
    direction = bestDir;
}

void Ghost::updateTargetTile(Player& player) {
    if (isChasing) {
        // Chase mode: target the player
        if (ghostName == "Blinky") {
            targetTile = player.getCurrentTile();
        } else if (ghostName == "Pinky") {
            targetTile = player.getCurrentTile();
        } else if (ghostName == "Inky") {
            targetTile = player.getCurrentTile();
        } else if (ghostName == "Clyde") {
            targetTile = player.getCurrentTile();
        }
    } else {
        // Scatter mode: target the ghost's home corner
        if (ghostName == "Blinky") {
            targetTile = {25, 0};  // Top-right corner
        } else if (ghostName == "Pinky") {
            targetTile = {2, 0};   // Top-left corner
        } else if (ghostName == "Inky") {
            targetTile = {27, 31}; // Bottom-right corner
        } else if (ghostName == "Clyde") {
            targetTile = {0, 31};  // Bottom-left corner
        }
    }
}

void Ghost::loadNextLevel(){
    velocity += 0.05f * WINDOW_RESIZE; // boost velocity for next level
    direction = rand() % 2 + 3; // Random initial direction (3-4)
    aniFrame = 0; // Reset animation frame
    sprite.setTexture(standardTextures[0]); // Reset animation frame to default
    currentTile = {14, 11}; // Reset to default spawn position
    sprite.setPosition(sf::Vector2f(TILE_SIZE * currentTile.x, TILE_SIZE * currentTile.y + SCORE_BOARD_HEIGHT + TILE_SIZE / 2)); // NOT FINAL
}

void Ghost::update(TileMap& tileMap, Player& player) {
    // chase and scatter logic
    chaseAndScatterTimer--;
    if(chaseAndScatterTimer <= 0) {
        isChasing = !isChasing;
        chaseAndScatterTimer = 500; // Reset timer
        std::cout << ghostName << " is now " << (isChasing ? "chasing" : "scattering") << std::endl;
    }
    updateTargetTile(player); // Update target tile

    if (isCentered()) {
        currentTile = sf::Vector2i(
            static_cast<int>(sprite.getPosition().x) / TILE_SIZE,
            static_cast<int>(sprite.getPosition().y - SCORE_BOARD_HEIGHT) / TILE_SIZE
        );
    }

    // Movement decision logic
    if(isCentered()){
        if(isFrightened){
            // Logic for frightened state
        } else if(isDead){
            // Logic for dead state
        } else {    // chase the target tile
            chaseDecision(tileMap);
        }
    }

    // Update sprite position
    switch (direction) {
        case 1: // Up
            sprite.move(sf::Vector2f(0, -velocity));
            break;
        case 2: // Down
            sprite.move(sf::Vector2f(0, velocity));
            break;
        case 3: // Left
            sprite.move(sf::Vector2f(-velocity, 0));
            break;
        case 4: // Right
            sprite.move(sf::Vector2f(velocity, 0));
            break;
        default:
            break;
    }
    updateSprite();
}

void Ghost::draw(sf::RenderWindow& window) {
    window.draw(sprite);  // Cambiato da window.draw(*sprite) a window.draw(sprite)
}
