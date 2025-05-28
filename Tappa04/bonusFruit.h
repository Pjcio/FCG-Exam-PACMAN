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
        sf::Sprite* fruitSprite = nullptr;
        sf::Sprite* pointsSprite = nullptr;
    
        std::vector<sf::Texture> fruitsTextures;
        std::vector<sf::Texture> pointsTextures;
    
        int points = 0;
        int fruitTimer = 0;
        int pointsTimer = 0;
        bool isVisible = false;
        bool fruitCollected = false;
    };
    