#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>
#include <string>

void makeBlackPixelsTransparent(sf::Image& image);

std::unordered_map<int, sf::Texture> loadAssetsMapFromDirectory(const std::string& directoryPath);

std::vector<sf::Texture> loadAssetsVectorFromDirectory(const std::string& directoryPath);
