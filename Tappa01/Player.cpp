#include "Player.h"

Player::Player() {
    // Carica le texture
    frames = loadAssetsVectorFromDirectory("resources/player"); 
    sprite = new sf::Sprite(frames[1]); 
    sprite->setPosition(sf::Vector2f(0, 0));
    sf::FloatRect bounds = sprite->getLocalBounds();
    sprite->setScale(sf::Vector2f(3.f, 3.f)); 
    sprite->setOrigin(sf::Vector2f(6.5f, 6.5f)); 

}

// Costruttore con posizione
Player::Player(int x, int y) {
    // Carica le texture
    frames = loadAssetsVectorFromDirectory("resources/player"); 
    sprite = new sf::Sprite(frames[1]); 
    sprite->setPosition(sf::Vector2f(x, y));
    sprite->setScale(sf::Vector2f(3.f, 3.f)); 
    sprite->setOrigin(sf::Vector2f(6.5f, 6.5f)); 
}

// Add a direction to the movement buffer
void Player::setDirection(int dir) {
    if (dir != 0) {
        movementBuffer.push_back(dir);
    } else {
        movementBuffer.clear();  // Clear the buffer when no movement
    }

    updateDirection();
}

// Release a direction (remove it from the buffer)
void Player::releaseDirection(int dir) {
    movementBuffer.erase(
        std::remove(movementBuffer.begin(), movementBuffer.end(), dir),
        movementBuffer.end()
    );
    updateDirection();
}


// Update the current direction based on the buffer
void Player::updateDirection() {
    if (!movementBuffer.empty()) {
        direction = movementBuffer.back();  // Use the last pressed key         
    } else {
        direction = 0;  // No movement if buffer is empty
    }
}

// Function to update the player's state
void Player::update() {
    // Update position based on direction and velocity
    switch (direction) {
        case 1: // Up
            sprite->move(sf::Vector2f(0, -velocity));
            sprite->setRotation(sf::degrees(270)); 
            break;
        case 2: // Down
            sprite->move(sf::Vector2f(0, velocity));
            sprite->setRotation(sf::degrees(90));
            break;
        case 3: // Left
            sprite->move(sf::Vector2f(-velocity, 0));
            sprite->setRotation(sf::degrees(180));
            break;
        case 4: // Right
            sprite->move(sf::Vector2f(velocity, 0));
            sprite->setRotation(sf::degrees(0));
            break;
        default: // No movement
            break;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(*sprite);
}