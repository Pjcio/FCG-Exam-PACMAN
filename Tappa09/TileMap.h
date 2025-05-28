#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <vector>
#include "utils.h"
#include "constants.h"
class Score;
class Ghost;

class TileMap {
public:
    std::vector<std::vector<int>> grid;
    std::unordered_map<int, sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    mutable sf::Clock clock;
    int pelletCount = 224;


public:
    TileMap();
    int getPelletCount() const;
    int getTileId(int x, int y);
    bool isOutOfBounds(int x, int y);
    bool isTileSolid(int x, int y);
    bool eatPellet(int x, int y, Score& score);
    void convertBlueToWhite();
    void convertWhiteToBlue();
    void loadNextLevel();
    void draw(sf::RenderWindow& window) const;
};
