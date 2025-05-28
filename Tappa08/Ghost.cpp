#include "Ghost.h"
#include "TileMap.h"
#include "Score.h"
#include "Ghost.h"
#include "Player.h"

const int SPAWN_VELOCITY = 0.5f * WINDOW_RESIZE; // Adjust spawn velocity as needed

Ghost::Ghost(std::string ghostName)
    : standardTextures(loadAssetsVectorFromDirectory("resources/ghosts/" + ghostName)),
      frightenedTextures(loadAssetsVectorFromDirectory("resources/ghosts/frightened")),
      deadTextures(loadAssetsVectorFromDirectory("resources/ghosts/dead")),
      pointsTextures(loadAssetsVectorFromDirectory("resources/ghostPoints")),
      sprite(standardTextures[0]) // Initialize the sprite with the first texture
{
    this->ghostName = ghostName;
    
    // Set targetTile based on ghostName
    if (ghostName == "Blinky") {
        currentTile = {14, 11}; // Blinky's starting position
        targetTile = {25, 0};  // Top-right corner
        direction = rand() % 2 + 3; // Random initial direction (3-4)
        spawnTimer = 0;
    } else if (ghostName == "Pinky") {
        currentTile = {14, 14}; // Pinky's starting position
        targetTile = {2, 0};   // Top-left corner
        direction = 2;
        spawnTimer = 1;
    } else if (ghostName == "Inky") {
        currentTile = {12, 14}; // Inky's starting position
        targetTile = {27, 31}; // Bottom-right corner
        direction = 1;
        spawnTimer = 200;
    } else if (ghostName == "Clyde") {
        currentTile = {16, 14}; // Clyde's starting position
        targetTile = {0, 31};  // Bottom-left corner
        direction = 1;
        spawnTimer = 400;
    }

    // Scale and position adjustments
    sprite.setScale(sf::Vector2f(WINDOW_RESIZE, WINDOW_RESIZE));
    sprite.setPosition(sf::Vector2f(TILE_SIZE * currentTile.x, TILE_SIZE * currentTile.y + SCORE_BOARD_HEIGHT + TILE_SIZE / 2)); // NOT FINAL
    sprite.setOrigin(sf::Vector2f(7, 7));
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

GhostState Ghost::getState(){
    return currentState;
}

sf::Vector2i Ghost::getCurrentTile() {
    return currentTile;
}

bool Ghost::isCentered() {
    int center = TILE_SIZE / 2;

    int x = static_cast<int>(sprite.getPosition().x);
    int y = static_cast<int>(sprite.getPosition().y);

    int xOffset = std::abs(((x % TILE_SIZE) + TILE_SIZE) % TILE_SIZE - center);
    int yOffset = std::abs(((y % TILE_SIZE) + TILE_SIZE) % TILE_SIZE - center);

    return (xOffset <= 2 && yOffset <= 2);
}

bool Ghost::isCageCentered() {

    int x = static_cast<int>(sprite.getPosition().x);
    int y = static_cast<int>(sprite.getPosition().y);

    int xOffset = std::abs(((x % TILE_SIZE) + TILE_SIZE) % TILE_SIZE);
    int yOffset = std::abs(((y % TILE_SIZE) + TILE_SIZE) % TILE_SIZE);

    return (xOffset <= 2 && yOffset <= 2);
}

bool Ghost::isAtSpawnPoint() {
    int centerX = GAME_BOARD_WIDTH / 2;
    int centerY = TILE_SIZE * 11 + TILE_SIZE / 2 + SCORE_BOARD_HEIGHT;

    int x = static_cast<int>(sprite.getPosition().x);
    int y = static_cast<int>(sprite.getPosition().y);

    int xOffset = std::abs(x - centerX);
    int yOffset = std::abs(y - centerY);

    if (xOffset <= 2 && yOffset <= 2) {
        sprite.setPosition(sf::Vector2f(centerX, centerY)); // Snap to spawn point

        return true; // Ghost is at spawn point
    } else {
        return false; // Ghost is not at spawn point
    }
}


void Ghost::setState(GhostState newState, bool force /* = false */) {
    if(currentState == GhostState::Dead && !force) return;

    currentState = newState;
    aniFrame = 0;

    switch (currentState) {
        case GhostState::Chase:
            stateTimer = 600;
            break;
        case GhostState::Scatter:
            stateTimer = 300;
            break;
        case GhostState::Frightened:
        
            if(!isSpawning || !isCentered())
                direction = getOppositeDirection();
            stateTimer = 500;
            break;
        case GhostState::Dead:
            // Maybe reset path to spawn here
            break;
    }
    /* DEBUGGING
    switch (currentState) {
        case GhostState::Chase:
            std::cout << ghostName << " is now chasing.\n";
            break;
        case GhostState::Scatter:
            std::cout << ghostName << " is now scattering.\n";
            break;
        case GhostState::Frightened:
            std::cout << ghostName << " is frightened!\n";
            break;
        case GhostState::Dead:
            std::cout << ghostName << " is dead!\n";
            break;
    }
    */
}

void Ghost::setPointSprite(int spriteIndex){
    if(spriteIndex < 0 || spriteIndex >= pointsTextures.size()) {
        return;
    }
    sprite.setTexture(pointsTextures[spriteIndex], true);
    // Set the texture rect to the size of the ghost point sprite
}


// Update sprite texture and animation frame
void Ghost::updateSprite() {
    // Reset sprite texture rect to match the size of the ghost sprite
    static const sf::IntRect desiredRect({0, 0}, {14, 14}); 
    if (sprite.getTextureRect() != desiredRect) {
        sprite.setTextureRect(desiredRect);
    }

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

    switch (currentState) {
        case GhostState::Frightened:
            // Frightened state flashing logic
            if((stateTimer > 120 || (stateTimer / 10) % 2 == 0))
                sprite.setTexture(frightenedTextures[frameIndex]);
            else
                sprite.setTexture(frightenedTextures[frameIndex + 2]);
            break;
        case GhostState::Dead:
            sprite.setTexture(deadTextures[textureIndex / 2]);
            break;
        default:
            sprite.setTexture(standardTextures[textureIndex]);
            break;
    }
}

float Ghost::updateVelocity() {
    // Update velocity based on the current state
    if(currentState == GhostState::Frightened && !isSpawning) {
        return velocity / 2; // Frightened speed
    }
    else if(currentState == GhostState::Dead) {
        return velocity * 1.5f; // Dead speed
    }
    else if(isSpawning) {
        return SPAWN_VELOCITY; // Spawn speed
    }
    return velocity;
}

void Ghost::frightenedDirDecision(TileMap& tileMap) {
    sf::Vector2i choosenTile = sf::Vector2i(0, 0); // Initialize chosenTile with non solit tile;ù
    int randomDir = 0; // Initialize randomDir

    // Randomly choose a tile until a non-solid tile is found
    while(tileMap.isTileSolid(choosenTile.x, choosenTile.y)){

        randomDir = rand() % 4 + 1; // Random direction (1-4)

        // Check if the chosen direction is valid (not opposite to current direction)
        if (randomDir != getOppositeDirection()) {
            choosenTile = currentTile;
            switch (randomDir) {
                case 1: choosenTile.y -= 1; break; // Up
                case 2: choosenTile.y += 1; break; // Down
                case 3: choosenTile.x -= 1; break; // Left
                case 4: choosenTile.x += 1; break; // Right
            }
        }
    }
    // Update the direction to the chosen tile
    direction = randomDir;
}

void Ghost::chaseDirDecision(TileMap& tileMap) {
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

void Ghost::updateTargetTile(Player& player, Ghost& blinky) {

    // Get the player's current tile
    sf::Vector2i playerTile = player.getCurrentTile();

    if (currentState == GhostState::Chase) {
        // Chase mode: target the player
        if (ghostName == "Blinky") {
            // Blinky chases the player directly
            targetTile = playerTile;

        } else if (ghostName == "Pinky") {
            // Pinky targets 4 tiles ahead of the player
            targetTile = playerTile;
            switch(player.getDirection()) {
                case 1: targetTile.y -= 4; break; // Up
                case 2: targetTile.y += 4; break; // Down
                case 3: targetTile.x -= 4; break; // Left
                case 4: targetTile.x += 4; break; // Right

            }
        } else if (ghostName == "Inky") {
            // Inky targets a position based on Blinky's position

            // Get 2-tile-ahead position of player
            sf::Vector2i aheadTile = playerTile;
            switch (player.getDirection()) {
                case 1: aheadTile.y -= 2; break; // Up
                case 2: aheadTile.y += 2; break; // Down
                case 3: aheadTile.x -= 2; break; // Left
                case 4: aheadTile.x += 2; break; // Right
            }

            // Target = 2 * aheadTile - BlinkyTile (180 degree reflection from Blinky)
            sf::Vector2i blinkyTile = blinky.getCurrentTile();
            targetTile = {
                2 * aheadTile.x - blinkyTile.x,
                2 * aheadTile.y - blinkyTile.y
            };
            // A 180-degree rotation of point B around center point C results in point A,
            // which is computed as: A = 2 * C - B

        } else if (ghostName == "Clyde") {
            // Clyde targets a position based on the distance to the player
            sf::Vector2i diff = {
                playerTile.x - currentTile.x,
                playerTile.y - currentTile.y
            };
            float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

            if (distance > 8.0f) {
                // Chase player
                targetTile = playerTile;
            } else {
                // Go to scatter corner
                targetTile = {0, 31};
            }

        }
    } else if (currentState == GhostState::Scatter) {
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
    } else if (currentState == GhostState::Dead) {
        targetTile = {14, 11}; // Reset to spawn position
    }
}

void Ghost::loadNextLevel(){
    velocity += 0.05f * WINDOW_RESIZE; // boost velocity for next level
    aniFrame = 0; // Reset animation frame
    stateTimer = 500; // Reset timer for chase and scatter
    currentState = GhostState::Chase; // Reset state to default;
    isSpawning = true; // Reset spawning flag
    sprite.setTexture(standardTextures[0]); // Reset animation frame to default
    
    if (ghostName == "Blinky") {
        currentTile = {14, 11}; // Blinky's starting position
        targetTile = {25, 0};  // Top-right corner
        direction = rand() % 2 + 3; // Random initial direction (3-4)
        spawnTimer = 0;
    } else if (ghostName == "Pinky") {
        currentTile = {14, 14}; // Pinky's starting position
        targetTile = {2, 0};   // Top-left corner
        direction = 2;
        spawnTimer = 1;
    } else if (ghostName == "Inky") {
        currentTile = {12, 14}; // Inky's starting position
        targetTile = {27, 31}; // Bottom-right corner
        direction = 1;
        spawnTimer = 200;
    } else if (ghostName == "Clyde") {
        currentTile = {16, 14}; // Clyde's starting position
        targetTile = {0, 31};  // Bottom-left corner
        direction = 1;
        spawnTimer = 400;
    }

    sprite.setPosition(sf::Vector2f(TILE_SIZE * currentTile.x, TILE_SIZE * currentTile.y + SCORE_BOARD_HEIGHT + TILE_SIZE / 2)); // NOT FINAL
}

void Ghost::update(TileMap& tileMap, Player& player, Ghost& blinky) {
    // Cage spawn logic
    if (spawnTimer > 0) {

        if(isCageCentered()) {
            currentTile = {
                static_cast<int>(sprite.getPosition().x) / TILE_SIZE,
                static_cast<int>((sprite.getPosition().y - SCORE_BOARD_HEIGHT) / TILE_SIZE)
            };
    
            // vertical oscillation
            direction = (currentTile.y >= 15) ? 1 : 2;
        }
    
        spawnTimer--;
    
    } else if (isSpawning && spawnTimer == 0) {
    
        // Still inside the cage
        if(isCageCentered()) {
            currentTile = {
                static_cast<int>(sprite.getPosition().x) / TILE_SIZE,
                static_cast<int>((sprite.getPosition().y - SCORE_BOARD_HEIGHT) / TILE_SIZE)
            };
    
            // Imposta la direzione iniziale in base alla posizione x
            if (currentTile.x == 14) {
                direction = 1; // up
            } else if (currentTile.x > 14) {
                direction = 3; // left
            } else {
                direction = 4; // right
            }
        }
    
        // Exiting from the cage
        if(isAtSpawnPoint()) {
            isSpawning = false; // Reset spawning flag
            direction = rand() % 2 + 3; // Random initial direction (3-4)
        }
    
    }
    // dead ghost respawn logic
    if (currentState == GhostState::Dead) {
        // Se lo sprite è al centro orizzontale e alla riga 10
        if (isAtSpawnPoint()) {
            direction = 2; // Down
        }
        // Se lo sprite è al centro orizzontale e alla riga 13
        else if (isCageCentered()) {
            currentTile = sf::Vector2i(
                static_cast<int>(sprite.getPosition().x) / TILE_SIZE,
                static_cast<int>(sprite.getPosition().y - SCORE_BOARD_HEIGHT) / TILE_SIZE
            );
            if(currentTile.y >= 14 && currentTile.x == 14) { // Time to go up and respawn
                setState(GhostState::Chase, true); // Set to chase state
                direction = 1; // Up
                isSpawning = true;
            }
        }
    } 

    // Update state timer
    stateTimer--;

    // stateTimer logic
    if(stateTimer <= 0){
        if (currentState == GhostState::Chase || currentState == GhostState::Scatter) {
            setState(currentState == GhostState::Chase ? GhostState::Scatter : GhostState::Chase);
        } else if (currentState == GhostState::Frightened) {
            setState(GhostState::Chase); // Reset to chase state after frightened
        }
    }
    updateTargetTile(player, blinky); // Update target tile


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

    // Movement decision logic
    nextTurnTimer--; // To fix the bug of the ghost that turn more than once in the same turn
    if(isCentered() && nextTurnTimer <= 0) {
        switch (currentState) {
            case GhostState::Frightened:
                frightenedDirDecision(tileMap);
                break;
            case GhostState::Dead:
            case GhostState::Chase:
            case GhostState::Scatter:
                chaseDirDecision(tileMap);
                break;
        }
        nextTurnTimer = 4;
    }

    // Update sprite position based on direction and velocity
    float newVelocity = updateVelocity(); // Update velocity based on state
    switch (direction) {
        case 1: // Up
            sprite.move(sf::Vector2f(0, -newVelocity));
            break;
        case 2: // Down
            sprite.move(sf::Vector2f(0, newVelocity));
            break;
        case 3: // Left
            sprite.move(sf::Vector2f(-newVelocity, 0));
            break;
        case 4: // Right
            sprite.move(sf::Vector2f(newVelocity, 0));
            break;
        default:
            break;
    }
    updateSprite();
}

void Ghost::draw(sf::RenderWindow& window) {
    window.draw(sprite);  // Cambiato da window.draw(*sprite) a window.draw(sprite)
}
