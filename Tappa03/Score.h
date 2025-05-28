#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "constants.h"

class Score {
private:
    int score;
    std::string scoreStr;
    sf::Font font;
    sf::Text* scoreText;

public:
    Score();
    int getPoints() const;
    void addPoints(int points);
    void draw(sf::RenderWindow& window);
};
