#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include "Score.h"
#include "utils.h"
#include "constants.h"

class TileMap {
public:
    std::vector<std::vector<int>> grid;
    std::unordered_map<int, sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    int pelletCount = 224;

public:
    TileMap();
    int getPelletCount() const;
    int getTileId(int x, int y);
    bool isTileSolid(int x, int y);
    void eatPellet(int x, int y, Score& score);
    void loadNextLevel();
    void draw(sf::RenderWindow& window) const;
};
