#include <SFML/Graphics.hpp>
#include <iostream>
#include "bonusFruit.h"
#include "Player.h"
#include "utils.h"
#include "constants.h"
#include <stdexcept>

BonusFruit::BonusFruit() {

    // Load fruit textures
    fruitsTextures = loadAssetsVectorFromDirectory("resources/bonusFruits"); 
    pointsTextures = loadAssetsVectorFromDirectory("resources/bonusPoints");

    if (fruitsTextures.empty() || pointsTextures.empty()) {
        throw std::runtime_error("Failed to load bonus fruit or points textures!");
    }

    fruitSprite = new sf::Sprite(fruitsTextures[0]);
    fruitSprite->setScale(sf::Vector2f(WINDOW_RESIZE, WINDOW_RESIZE)); // Scale the sprite
    fruitSprite->setPosition(sf::Vector2f(TILE_SIZE*13, TILE_SIZE*17 + SCORE_BOARD_HEIGHT - TILE_SIZE/3)); // Set initial position

    pointsSprite = new sf::Sprite(pointsTextures[0]);
    pointsSprite->setScale(sf::Vector2f(WINDOW_RESIZE, WINDOW_RESIZE)); // Scale the sprite

    

}

// Spawning the fruit on the screen with a position and a point value
void BonusFruit::spawn(int currentLevel) {

    // Cambia sprite e punti in base al livello
    switch (currentLevel) {
        case 1:
            fruitSprite->setTexture(fruitsTextures[0]); // cherry
            pointsSprite->setTexture(pointsTextures[0]);
            points = 100;
            break;
    
        case 2:
            fruitSprite->setTexture(fruitsTextures[1]); // strawberry
            pointsSprite->setTexture(pointsTextures[1]);
            points = 300;
            break;
    
        case 3:
        case 4:
            fruitSprite->setTexture(fruitsTextures[2]); // orange
            pointsSprite->setTexture(pointsTextures[2]);
            points = 500;
            break;
    
        case 5:
        case 6:
            fruitSprite->setTexture(fruitsTextures[3]); // apple
            pointsSprite->setTexture(pointsTextures[3]);
            points = 700;
            break;
    
        case 7:
        case 8:
            fruitSprite->setTexture(fruitsTextures[4]); // melon
            pointsSprite->setTexture(pointsTextures[4]);
            points = 1000;
            break;
    
        case 9:
        case 10:
            fruitSprite->setTexture(fruitsTextures[5]); // galaxian
            pointsSprite->setTexture(pointsTextures[5]);
            points = 2000;
            break;
    
        case 11:
        case 12:
            fruitSprite->setTexture(fruitsTextures[6]); // bell
            pointsSprite->setTexture(pointsTextures[6]);
            points = 3000;
            break;
    
        default:
            fruitSprite->setTexture(fruitsTextures[7]); // key
            pointsSprite->setTexture(pointsTextures[7]);
            points = 5000;
            break;
    }
    

    isVisible = true;  // Fruit is now visible
    fruitTimer = 600;     // Keep the fruit visible for 10 seconds (60 FPS * 10 seconds)
}

void BonusFruit::loadNextLevel() {
    // Reset the fruit's state for the next level
    isVisible = false;
    fruitTimer = 0;
    fruitCollected = false;
}

sf::FloatRect BonusFruit::getCollisionBox(){
    sf::FloatRect bounds = fruitSprite->getGlobalBounds();
    sf::Vector2f center = bounds.getCenter();

    sf::Vector2f halfSize = bounds.size / 2.f;

    return sf::FloatRect(
        center - halfSize / 2.f,
        halfSize
    );
}

sf::Sprite BonusFruit::getCurrentFruitSprite() {
    return *fruitSprite;  // Return the current fruit sprite
}

int BonusFruit::getPoints() {
    return points;  // Return the points associated with the fruit
}

bool BonusFruit::isFruitCollected() {
    return fruitCollected;  // Return whether the fruit has been collected
}

// Update the fruit's state (mainly for handling visibility duration)
void BonusFruit::update() {
    if (isVisible && --fruitTimer <= 0) {
        isVisible = false;  // Fruit disappears when the fruitTimer reaches zero
    }
    if (pointsTimer > 0) {
        pointsTimer--;  // Decrease the points timer
    }
}

// Check if the fruit has been collected by the player (collision detection)
bool BonusFruit::checkCollision(const sf::FloatRect& playerBounds) { 
    sf::FloatRect fruitBox = getCollisionBox();
    if (isVisible && fruitBox.findIntersection(playerBounds)) {
        fruitCollected = true;
        isVisible = false;
        pointsTimer = 180;  // Keep the points visible for a short duration (3 seconds)
        return true;
    }
    return false;  // No collision detected
}

// Draw the fruit on the window if it's visible
void BonusFruit::draw(sf::RenderWindow& window) {
    if (isVisible) {
        // fruit sprite flash 2 seconds before despawn
        if (fruitTimer > 120 || (fruitTimer / 10) % 2 == 0) {
            window.draw(*fruitSprite);  // Disegna solo se non sta lampeggiando
        }
    }

    if (pointsTimer > 0) {
        pointsSprite->setPosition(sf::Vector2f(TILE_SIZE*13 - TILE_SIZE/WINDOW_RESIZE, TILE_SIZE*17 + SCORE_BOARD_HEIGHT - TILE_SIZE/3));  // Position the points sprite at the fruit's position
        window.draw(*pointsSprite);  // Draw the points sprite
    }
}

