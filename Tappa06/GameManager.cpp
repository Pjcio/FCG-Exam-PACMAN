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

void GameManager::update(int* pauseTimer) {

    // Update bonus fruit if needed
    bonusFruit.update();
    if(bonusFruit.checkCollision(player.getCollisionBox())) {
        score.addPoints(bonusFruit.getPoints());
        collectedFruits.push_back(bonusFruit.getCurrentFruitSprite());
    }

    // Pause its finished   
    if (*pauseTimer == 0) {
        readyTextVisible = false;
    }


    // Logica di aggiornamento generale del gioco
    checkLevelProgression(pauseTimer);
    spawnBonusIfNeeded();
}

void GameManager::checkLevelProgression(int* pauseTimer) {
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

        *pauseTimer = 120; // pause for 2 second
    }
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
