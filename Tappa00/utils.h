#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <filesystem>
#include <unordered_map>

std::unordered_map<int, sf::Texture> loadAssetsFromDirectory(const std::string& directoryPath);
