#include <SFML/Graphics.hpp>
#include <stdexcept>
#include "constants.h"
#include "Score.h"

Score::Score() : score(0), scoreStr("Score: 0"), scoreText(nullptr) {
    if (!font.openFromFile("resources/font/Emulogic-zrEw.ttf")) {
        throw std::runtime_error("Failed to open font");
    }

    scoreText = new sf::Text(font, scoreStr, TILE_SIZE);
    scoreText->setFillColor(sf::Color::White);
    scoreText->setPosition(sf::Vector2f(0, 0));
}

int Score::getPoints() const {
    return score;
}

void Score::addPoints(int points) {
    score += points;
    scoreStr = "Score: " + std::to_string(score);
    scoreText->setString(scoreStr);
}

void Score::draw(sf::RenderWindow& window) {
    window.draw(*scoreText);
}
