#include "Player.h"  
#include "TileMap.h"
#include "Score.h"
#include "Ghost.h"

// Costruttore senza parametri
Player::Player() 
    : frames(loadAssetsVectorFromDirectory("resources/player")),
      sprite(frames[0]) 

{

    sprite.setScale(sf::Vector2f(WINDOW_RESIZE, WINDOW_RESIZE));
    sprite.setPosition(sf::Vector2f(TILE_SIZE*14, TILE_SIZE*26.5f));
    sprite.setOrigin(sf::Vector2f(6.5f, 6.5f));
}

bool Player::checkCollision(TileMap& tileMap, int dir) {
    sf::Vector2i nextTile = currentTile;

    switch (dir) {
        case 1: // Up
            nextTile.y -= 1;
            break;
        case 2: // Down
            nextTile.y += 1;
            break;
        case 3: // Left
            nextTile.x -= 1;
            break;
        case 4: // Right
            nextTile.x += 1;
            break;
        default:
            return false;
    }

    return !tileMap.isTileSolid(nextTile.x, nextTile.y);
}

bool Player::isCentered() {
    int center = TILE_SIZE / 2;

    int x = static_cast<int>(sprite.getPosition().x);
    int y = static_cast<int>(sprite.getPosition().y);

    int xOffset = std::abs(((x % TILE_SIZE) + TILE_SIZE) % TILE_SIZE - center);
    int yOffset = std::abs(((y % TILE_SIZE) + TILE_SIZE) % TILE_SIZE - center);

    return (xOffset <= 3 && yOffset <= 3);
}


int Player::getLifes() {
    return lives;
}

void Player::loseLife() {
    if (lives > 0) {
        lives--;
    }
}

int Player::getDirection() {
    return direction;
}


void Player::setDirection(int dir) {
    buffered_dir = dir;  // last pressed key   
}

void Player::update(TileMap& tileMap, Score& score, std::vector<Ghost>& ghosts) {

    // if pacman is 16x and 16y pixel in the tile update the currentTile
    if (isCentered()) {
        currentTile = sf::Vector2i(
            static_cast<int>(sprite.getPosition().x) / TILE_SIZE,
            static_cast<int>(sprite.getPosition().y - SCORE_BOARD_HEIGHT) / TILE_SIZE
        );
    }

    // Handle pacman effect (wrap around the screen)
    if(currentTile.x == -1 && currentTile.y == 14 && direction == 3) {
        currentTile.x = 28; // Pacman effect
        sprite.setPosition(sf::Vector2f(TILE_SIZE*28, TILE_SIZE*14 + SCORE_BOARD_HEIGHT + TILE_SIZE/2)); 
    }
    if (currentTile.x == 28 && currentTile.y == 14 && direction == 4) {
        currentTile.x = -1; // Pacman effect
        sprite.setPosition(sf::Vector2f(TILE_SIZE*-1, TILE_SIZE*14 + SCORE_BOARD_HEIGHT + TILE_SIZE/2));
    }

    // Handle eating pellets
    if (tileMap.getTileId(currentTile.x, currentTile.y) == 41 or 43) {
        tileMap.eatPellet(currentTile.x, currentTile.y, score, ghosts); // Eat pellet if present
    }

    // Update animation frame
    if(direction == 0) {
        // Empty
    } else if (AniFrame >= AniFrameMax) {
        AniFrame = 0;
        sprite.setTexture(frames[0]);
    } else if (AniFrame < AniFrameMax / 3) {
        AniFrame++;
        sprite.setTexture(frames[1]);
    } else {
        AniFrame++;
        sprite.setTexture(frames[2]);
    }

    if (checkCollision(tileMap, buffered_dir) && isCentered()) {
        direction = buffered_dir; // Update direction if no collision
    }

    // Update position based on direction and velocity
    if (checkCollision(tileMap, direction)) {
        switch (direction) {
            case 1: // Up
                sprite.move(sf::Vector2f(0, -velocity));
                sprite.setRotation(sf::degrees(270));
                break;
            case 2: // Down
                sprite.move(sf::Vector2f(0, velocity));
                sprite.setRotation(sf::degrees(90));
                break;
            case 3: // Left
                sprite.move(sf::Vector2f(-velocity, 0));
                sprite.setRotation(sf::degrees(180));
                break;
            case 4: // Right
                sprite.move(sf::Vector2f(velocity, 0));
                sprite.setRotation(sf::degrees(0));
                break;
            default:
                break;
        }
    } else {
        direction = 0;
    }

}

sf::FloatRect Player::getCollisionBox() {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2f center = bounds.getCenter();

    sf::Vector2f halfSize = bounds.size / 2.f;

    return sf::FloatRect(
        center - halfSize / 2.f,
        halfSize
    );
}

sf::Vector2i Player::getCurrentTile() {
    return currentTile;
}

void Player::loadNextLevel() {
    velocity += 0.05f * WINDOW_RESIZE; // boost velocity for next level
    // COLLISION BUGS OUT AT LEVEL 19 //

    direction = 3; // Reset direction to default (left)
    buffered_dir = 0; // Reset buffered direction
    AniFrame = 0; // Reset animation frame
    sprite.setTexture(frames[0]); // Reset animation frame to default
    currentTile = {14, 23}; // Reset to edfault spawn position
    sprite.setPosition(sf::Vector2f(TILE_SIZE*14, TILE_SIZE*26.5f));
}

void Player::draw(sf::RenderWindow& window) {
    // Draw the player sprite
    window.draw(sprite);
}
