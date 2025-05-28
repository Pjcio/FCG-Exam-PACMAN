#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <filesystem>
#include <unordered_map>

void makeBlackPixelsTransparent(sf::Image& image);

std::unordered_map<int, sf::Texture> loadAssetsMapFromDirectory(const std::string& directoryPath);

std::vector<sf::Texture> loadAssetsVectorFromDirectory(const std::string& directoryPath);
