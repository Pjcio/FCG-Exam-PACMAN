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
        readyText(font, readyStr, TILE_SIZE),
        gameOverText(font, "Game\nOver", TILE_SIZE*4),
        introSoundBuffer("resources/sounds/pacman_intro.wav"),
        deathSoundBuffer("resources/sounds/pacman_death.wav"),
        eatGhostSoundBuffer("resources/sounds/pacman_eatghost.wav"),
        eatFruitSoundBuffer("resources/sounds/pacman_eatfruit.wav"),
        extraPacmanSoundBuffer("resources/sounds/pacman_extrapac.wav"),
        soundEffect(introSoundBuffer),
        chomp1SoundBuffer("resources/sounds/pacman_chomp1.wav"),
        chomp2SoundBuffer("resources/sounds/pacman_chomp2.wav"),
        chompSoundEffect(chomp1SoundBuffer),
        ghostWanderingSoundBuffer("resources/sounds/pacman_ghostWandering.wav"),
        ghostFrightenedSoundBuffer("resources/sounds/pacman_ghostFrightened.wav"),
        ghostEatenSoundBuffer("resources/sounds/pacman_ghostEaten.wav"),
        ghostSoundEffect(ghostWanderingSoundBuffer)
    {

    // level text initialization
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(sf::Vector2f(GAME_BOARD_WIDTH/2, 0));

    // ready text initialization
    readyText.setFillColor(sf::Color::Yellow);
    readyText.setPosition(sf::Vector2f(TILE_SIZE*11, TILE_SIZE*17 + SCORE_BOARD_HEIGHT));

    lifeSprite.setScale(sf::Vector2f(WINDOW_RESIZE/1.8 * 2, WINDOW_RESIZE/1.8 * 2));

    gameOverText.setFillColor(sf::Color::Red);
    // Center on screen
    sf::FloatRect layout = gameOverText.getLocalBounds();
    gameOverText.setOrigin(layout.getCenter());
    gameOverText.setPosition({GAME_BOARD_WIDTH / 2.f, TILE_SIZE * 17.f});

    ghostSoundEffect.setLooping(true);
    soundEffect.play(); // Play intro sound
}

void GameManager::update() {

    // Level complete animation
    if (victoryAnimationActive) {
        if (pauseTimer % 20 == 0) {
            // switch between blue and white
            if (bluePhase) {
                tileMap.convertBlueToWhite();
            } else {
                tileMap.convertWhiteToBlue();
            }
            bluePhase = !bluePhase;
    
            // Animation complete, load next level
            if (pauseTimer <= 1) { 
                victoryAnimationActive = false;
                loadNextLevel();
            }
        }
        return;
    }
    

    // Handle eating pellets
    sf::Vector2i playerTile = player.getCurrentTile();
    if (tileMap.getTileId(playerTile.x, playerTile.y) == 41 or 43) {

        if(tileMap.eatPellet(playerTile.x, playerTile.y, score, ghosts)){
            // switch between chomp1 and chomp2 sound effects
            if (&chompSoundEffect.getBuffer() != &chomp1SoundBuffer) {
                chompSoundEffect.setBuffer(chomp1SoundBuffer);
            } else {
                chompSoundEffect.setBuffer(chomp2SoundBuffer);
            }
            chompSoundEffect.play();
        }

    }

    if (awaitingRespawn) {
        if (!player.isInDyingAnimation()) {

            if(player.getLifes() == 0) {
                gameOver = true;
                return; // Game over, do not respawn
            }

            playerRespawn();
            awaitingRespawn = false;
        }
        return; // wait animation to finish
    }

    // play ghostdead  sound if any ghost is dead
    bool anyGhostDead = false; // reset anyGhostDead flag
    for (int i = 0; i < ghosts.size(); i++) {
        if (ghosts[i].getState() == GhostState::Dead) {
            anyGhostDead = true;

            if(&ghostSoundEffect.getBuffer() != &ghostEatenSoundBuffer) {
                ghostSoundEffect.setBuffer(ghostEatenSoundBuffer);
                ghostSoundEffect.stop();
            }
            if(ghostSoundEffect.getStatus() != sf::Sound::Status::Playing) {
                ghostSoundEffect.play();
            }

            break;
        }
    }

    // Control if any ghost is frightened
    bool anyGhostFrightened = false; // reset anyGhostFrightened flag
    for (int i = 0; i < ghosts.size(); i++) {
        if (ghosts[i].getState() == GhostState::Frightened) {
            anyGhostFrightened = true;

            if(!anyGhostDead) {
                // sound effect for frightened ghost
                if(&ghostSoundEffect.getBuffer() != &ghostFrightenedSoundBuffer) {
                    ghostSoundEffect.setBuffer(ghostFrightenedSoundBuffer);
                    ghostSoundEffect.stop();
                }
                if(ghostSoundEffect.getStatus() != sf::Sound::Status::Playing) {
                    ghostSoundEffect.play();
                }
            }

            break;
        }
    }
    if (!anyGhostFrightened) {
        eatenGhostsInARow = 0;

        if(!anyGhostDead) {
            // sound effect for wandering ghost
            if(&ghostSoundEffect.getBuffer() != &ghostWanderingSoundBuffer) {
                ghostSoundEffect.setBuffer(ghostWanderingSoundBuffer);
                ghostSoundEffect.stop();
            }
            if(ghostSoundEffect.getStatus() != sf::Sound::Status::Playing) {
                ghostSoundEffect.play();
            }
        }
    }

    // ghosts collision check
    for(int i = 0; i < ghosts.size(); i++) {

        sf::FloatRect ghostBox = ghosts[i].getCollisionBox();
        if(ghostBox.findIntersection(player.getCollisionBox())){
            if(ghosts[i].getState() == GhostState::Frightened) {

                // Play sound effect for eating ghost
                soundEffect.setBuffer(eatGhostSoundBuffer);
                soundEffect.play();

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

                pauseTimer = 60; // pause for 1 second
            } else if (ghosts[i].getState() != GhostState::Dead) {

                // Play sound effect for player death
                soundEffect.setBuffer(deathSoundBuffer);
                soundEffect.play();

                ghostSoundEffect.stop(); // Stop ghost sound effect

                player.die();
                awaitingRespawn = true;
                pauseTimer = 30; // pause for 0.5 second
            }
        }
    }

    // Update bonus fruit if needed
    bonusFruit.update();
    if(bonusFruit.checkCollision(player.getCollisionBox())) {

        // Play sound effect for eating fruit
        soundEffect.setBuffer(eatFruitSoundBuffer);
        soundEffect.play();

        score.addPoints(bonusFruit.getPoints());
        collectedFruits.push_back(bonusFruit.getCurrentFruitSprite());
    }

    if (score.getPoints() >= nextLifeScore) {

        // Play sound effect for extra pacman
        soundEffect.setBuffer(extraPacmanSoundBuffer);
        soundEffect.play();

        player.addLife();
        nextLifeScore += 10000; 
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
        victoryAnimationActive = true;
        pauseTimer = 200;
        bluePhase = false;
    }
}

void GameManager::loadNextLevel() {
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

    ghostSoundEffect.stop(); // Stop ghost sound effect

    pauseTimer = 120; // pause for 2 second
}


void GameManager::playerRespawn() {
    bonusFruit.loadNextLevel();
    player.loadNextLevel(); // Reset player position and lives
    for (int i = 0; i < ghosts.size(); i++) {
        ghosts[i].loadNextLevel(); // Reset ghost positions
    }

    // Reset bonus fruit
    readyTextVisible = true; // Show "Ready?" text

    ghostSoundEffect.stop(); // Stop ghost sound effect

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

bool GameManager::isGameOver() {
    return gameOver;
}

bool GameManager::isInVictoryAnimation() {
    return victoryAnimationActive;
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

    if (gameOver) {
        // Optional: darken the background
        sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);
    
        // Draw the text
        window.draw(gameOverText);
    }
}
