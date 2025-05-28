#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include "utils.h"
#include "constant.h"

class TileMap {
public:
    std::vector<std::vector<int>> grid;
    std::unordered_map<int, sf::Texture> textures;
    std::vector<sf::Sprite> sprites;

public:
    TileMap();
    bool isTileSolid(int x, int y) const;
    void draw(sf::RenderWindow& window) const;
};
