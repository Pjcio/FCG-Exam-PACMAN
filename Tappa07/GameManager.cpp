#include "GameManager.h"
#include "TileMap.h"
#include "Player.h"
#include "Score.h"
#include "Ghost.h"

GameManager::GameManager(TileMap& map, Player& player, Score& score, std::vector<Ghost>& ghosts)
    : tileMap(map), player(player), score(score), ghosts(ghosts),
        lifeTexture("resources/life/0.png"), // lifes texture and sprite initialization
        lifeSprite(lifeTexture),
        font("resources/font/Emulogic-zrEw.ttf"), // load font
        levelText(font, levelStr, TILE_SIZE),
        readyText(font, readyStr, TILE_SIZE) 
    {

    // level text initialization
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(sf::Vector2f(GAME_BOARD_WIDTH/2, 0));

    // ready text initialization
    readyText.setFillColor(sf::Color::Yellow);
    readyText.setPosition(sf::Vector2f(TILE_SIZE*11, TILE_SIZE*17 + SCORE_BOARD_HEIGHT));

    lifeSprite.setScale(sf::Vector2f(WINDOW_RESIZE/1.8 * 2, WINDOW_RESIZE/1.8 * 2));
}

void GameManager::update() {

    // Control if any ghost is frightened
    anyGhostFrightened = false; // reset anyGhostFrightened flag
    for (int i = 0; i < ghosts.size(); i++) {
        if (ghosts[i].getState() == GhostState::Frightened) {
            anyGhostFrightened = true;
            break;
        }
    }
    if (!anyGhostFrightened) {
        eatenGhostsInARow = 0;
    }

    // ghosts collision check
    for(int i = 0; i < ghosts.size(); i++) {

        sf::FloatRect ghostBox = ghosts[i].getCollisionBox();
        if(ghostBox.findIntersection(player.getCollisionBox())){
            if(ghosts[i].getState() == GhostState::Frightened) {

                eatenGhostsInARow += 1; // increase eaten ghosts in a row
                switch (eatenGhostsInARow) {
                    case 1: score.addPoints(200); break; // 200 points for first ghost
                    case 2: score.addPoints(400); break; // 400 points for second ghost
                    case 3: score.addPoints(800); break; // 800 points for third ghost
                    case 4: score.addPoints(1600); break; // 1600 points for fourth ghost
                    default: score.addPoints(1600); break;
                }
                if(eatenGhostsInARow >= 4) {
                    ghosts[i].setPointSprite(3); // set ghost sprite to points sprite
                } else {
                    ghosts[i].setPointSprite(eatenGhostsInARow - 1); // set ghost sprite to normal sprite
                }   

                ghosts[i].setState(GhostState::Dead); // set ghost state to dead

                pauseTimer = 90; // pause for 1.5 second
            } else if (ghosts[i].getState() != GhostState::Dead) {

                playerRespawn(); // player respawn logic
                if(player.getLifes() == 0) {
                    std::cout << "Game Over!" << std::endl;
                }
            } else {

            }
        }
    }

    // Update bonus fruit if needed
    bonusFruit.update();
    if(bonusFruit.checkCollision(player.getCollisionBox())) {
        score.addPoints(bonusFruit.getPoints());
        collectedFruits.push_back(bonusFruit.getCurrentFruitSprite());
    }

    // Pause its finished   
    if (pauseTimer == 0) {
        readyTextVisible = false;
    } 


    // Logica di aggiornamento generale del gioco
    checkLevelProgression();
    spawnBonusIfNeeded();
}

void GameManager::updatePauseTimer() {
        pauseTimer--;
}

void GameManager::checkLevelProgression() {
    if (tileMap.getPelletCount() == 0 or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
        currentLevel++;
        tileMap.loadNextLevel();
        player.loadNextLevel();
        bonusFruit.loadNextLevel();
        for (int i = 0; i < ghosts.size(); i++) {
            ghosts[i].loadNextLevel();
        }

        levelStr = "level: " + std::to_string(currentLevel);
        levelText.setString(levelStr);

        readyTextVisible = true; // Mostra il testo "Ready?"

        pauseTimer = 120; // pause for 2 second
    }
}

void GameManager::playerRespawn() {
    bonusFruit.loadNextLevel();
    player.loadNextLevel(); // Reset player position and lives
    for (int i = 0; i < ghosts.size(); i++) {
        ghosts[i].loadNextLevel(); // Reset ghost positions
    }
    player.loseLife(); // Decrease player life

    // Reset bonus fruit
    readyTextVisible = true; // Show "Ready?" text

    // Reset pause timer
    pauseTimer = 120; // pause for 2 second
}

void GameManager::spawnBonusIfNeeded() {
    if (tileMap.getPelletCount() == 170 or sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) {
        // Spawn bonus 
        bonusFruit.spawn(currentLevel);
    }
    if(tileMap.getPelletCount() == 70 && !bonusFruit.isFruitCollected()) {
        // Spawn bonus 
        bonusFruit.spawn(currentLevel);
    }
}

void GameManager::draw(sf::RenderWindow& window) {
    // Draw score
    score.draw(window);

    // Draw player lifes
    for (int i = 0; i < player.getLifes(); i++) {
        lifeSprite.setPosition(sf::Vector2f(i*2 * TILE_SIZE + TILE_SIZE/8, TILE_SIZE  + TILE_SIZE/8));
        window.draw(lifeSprite);
    }

    // Draw level text
    window.draw(levelText);

    // Draw ready text if visible
    if (readyTextVisible) {
        window.draw(readyText);
    }

    // Draw bonus fruit if visible
    bonusFruit.draw(window); 

    // Draw collected fruits
    for (int i = collectedFruits.size(); i-- > 0;) {
        collectedFruits[i].setPosition(sf::Vector2f(TILE_SIZE*10 - (i-collectedFruits.size()) * TILE_SIZE*2, TILE_SIZE + TILE_SIZE/8));
        window.draw(collectedFruits[i]);
    }
}
