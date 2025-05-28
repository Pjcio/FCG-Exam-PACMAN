#include "Score.h"

Score::Score() 
    : score(0), scoreStr("Score: 0"), 
    font("resources/font/Emulogic-zrEw.ttf"),
    scoreText(font, scoreStr, TILE_SIZE) 
    {

    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(sf::Vector2f(0, 0));
}

int Score::getPoints() const {
    return score;
}

void Score::addPoints(int points) {
    score += points;
    scoreStr = "Score: " + std::to_string(score);
    scoreText.setString(scoreStr);
}

void Score::draw(sf::RenderWindow& window) {
    window.draw(scoreText);
}
