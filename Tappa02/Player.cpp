#include "Player.h"
    
Player::Player() {
    // Carica le texture
    frames = loadAssetsVectorFromDirectory("resources/player"); 
    sprite = new sf::Sprite(frames[0]);
    
    sprite->setScale(sf::Vector2f(WINDOW_RESIZE*2, WINDOW_RESIZE*2));

    sprite->setPosition(sf::Vector2f(0, 0));
    sf::FloatRect bounds = sprite->getLocalBounds();
    sprite->setOrigin(sf::Vector2f(6.5f, 6.5f));


}

// Costruttore con posizione
Player::Player(int x, int y) {
    // Carica le texture
    frames = loadAssetsVectorFromDirectory("resources/player"); 
    sprite = new sf::Sprite(frames[0]);
    
    sprite->setScale(sf::Vector2f(WINDOW_RESIZE, WINDOW_RESIZE));
    
    sprite->setPosition(sf::Vector2f(x, y));
    sprite->setOrigin(sf::Vector2f(6.5f, 6.5f));

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
        return (int(sprite->getPosition().x) % TILE_SIZE == TILE_SIZE/2 && 
                   int(sprite->getPosition().y) % TILE_SIZE == TILE_SIZE/2);
    }

    // Add a direction to the movement buffer
    void Player::setDirection(int dir) {
        if (dir != 0) {
            // Remove the direction from the buffer if it's already present
            movementBuffer.erase(
                std::remove(movementBuffer.begin(), movementBuffer.end(), dir),
                movementBuffer.end()
            );
            // Add the direction to the back of the buffer
            movementBuffer.push_back(dir);
        } else {
            movementBuffer.clear();  // Clear the buffer when no movement
        }
    
        buffered_dir = movementBuffer.back();  // Use the last pressed key   
    }
    

    // Function to update the player's state
    void Player::update(TileMap& tileMap) {

        if (isCentered()) {

            currentTile = sf::Vector2i(
                static_cast<int>(sprite->getPosition().x) / TILE_SIZE,
                static_cast<int>(sprite->getPosition().y - SCORE_BOARD_HEIGHT) / TILE_SIZE
            );
        }

        // Update animation frame
        if(direction == 0) {
            // Empty
        } else if (AniFrame >= AniFrameMax) {
            AniFrame = 0;
            sprite->setTexture(frames[0]);
        } else if (AniFrame < AniFrameMax / 3) {
            AniFrame++;
            sprite->setTexture(frames[1]);
        } else {
            AniFrame++;
            sprite->setTexture(frames[2]);
        }

        if(checkCollision(tileMap, buffered_dir) && isCentered()) {
            direction = buffered_dir; // Update direction if no collision
        }

        // Update position based on direction and velocity
        if (checkCollision(tileMap, direction)) {
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
                default:
                    break;
            }
        }

        
    }

    void Player::draw(sf::RenderWindow& window) {
        window.draw(*sprite);
    }


