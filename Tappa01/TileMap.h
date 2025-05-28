#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include "utils.h"

class TileMap {
public:
    std::vector<std::vector<int>> grid;
    std::unordered_map<int, sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    int tileSize = 8 * 4;

public:
    TileMap();
    void draw(sf::RenderWindow& window) const;
};
