#include "GameManager.h"
#include "constants.h"
#include <iostream>
#include <string>
#include <stdexcept>

GameManager::GameManager(TileMap& map, Player& player, Score& score)
    : tileMap(map), player(player), score(score)
    {

    // Load font
    if (!font.openFromFile("resources/font/Emulogic-zrEw.ttf")) {
        throw std::runtime_error("Failed to open font");
    }

    // level text initialization
    levelText = new sf::Text(font, levelStr, TILE_SIZE);
    levelText->setFillColor(sf::Color::White);
    levelText->setPosition(sf::Vector2f(GAME_BOARD_WIDTH/2, 0));

    // ready text initialization
    readyText = new sf::Text(font, readyStr, TILE_SIZE);
    readyText->setFillColor(sf::Color::Yellow);
    readyText->setPosition(sf::Vector2f(TILE_SIZE*11, TILE_SIZE*17 + SCORE_BOARD_HEIGHT));

    // lifes texture and sprite initialization
    if (!lifeTexture.loadFromFile("resources/life/0.png")) {
        throw std::runtime_error("Failed to load life texture");
    }
    lifeSprite = new sf::Sprite(lifeTexture);
    lifeSprite->setScale(sf::Vector2f(WINDOW_RESIZE/1.8 * 2, WINDOW_RESIZE/1.8 * 2));
}

void GameManager::update(int* pauseTimer) {

    // Pause its finished   
    if (*pauseTimer == 0) {
        readyTextVisible = false;
    }


    // Logica di aggiornamento generale del gioco
    checkLevelProgression(pauseTimer);
    spawnBonusIfNeeded();
}

void GameManager::checkLevelProgression(int* pauseTimer) {
    if (tileMap.getPelletCount() == 0) {
        currentLevel++;
        tileMap.loadNextLevel();
        player.loadNextLevel();
        //Ghost.loadNextLevel(); 

        levelStr = "level: " + std::to_string(currentLevel);
        levelText->setString(levelStr);

        bonusSpawned = false; // Reset del bonus per il nuovo livello

        readyTextVisible = true; // Mostra il testo "Ready?"

        *pauseTimer = 120; // pause for 2 second
    }
}

void GameManager::spawnBonusIfNeeded() {
    if (!bonusSpawned && score.getPoints() % 500 == 0 && score.getPoints() != 0) {
        //tileMap.spawnBonusItem();
        //bonusSpawned = true;
    }
}

void GameManager::draw(sf::RenderWindow& window) {
    // Draw score
    score.draw(window);

    // Draw player lifes
    for (int i = 0; i < player.getLifes(); i++) {
        lifeSprite->setPosition(sf::Vector2f(i*2 * TILE_SIZE + TILE_SIZE/8, TILE_SIZE  + TILE_SIZE/8));
        window.draw(*lifeSprite);
    }

    // Draw level text
    window.draw(*levelText);

    // Draw ready text if visible
    if (readyTextVisible) {
        window.draw(*readyText);
    }

    // Draw acquired bonus item
    // bonusFruit.draw(window); 
}
