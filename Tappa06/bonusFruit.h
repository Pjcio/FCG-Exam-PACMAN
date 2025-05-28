#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.h"
#include "constants.h"
#include <stdexcept>

class BonusFruit {
    public:
        BonusFruit();
    
        void spawn(int currentLevel);
        void loadNextLevel();
        bool isFruitCollected();
        void update();
        sf::FloatRect getCollisionBox();
        sf::Sprite getCurrentFruitSprite();
        int getPoints();
        void draw(sf::RenderWindow& window);
        bool checkCollision(const sf::FloatRect& playerBounds);
    
    private:
        std::vector<sf::Texture> fruitsTextures;
        std::vector<sf::Texture> pointsTextures;

        sf::Sprite fruitSprite;
        sf::Sprite pointsSprite;
    
        int points = 0;
        int fruitTimer = 0;
        int pointsTimer = 0;
        bool isVisible = false;
        bool fruitCollected = false;
    };
    