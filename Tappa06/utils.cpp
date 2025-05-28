#include "utils.h"
#include <algorithm>

void makeBlackPixelsTransparent(sf::Image& image) {
    sf::Vector2u size = image.getSize();

    for (unsigned y = 0; y < size.y; ++y) {
        for (unsigned x = 0; x < size.x; ++x) {

            sf::Color pixel = image.getPixel(sf::Vector2u(x, y));

            if (pixel == sf::Color::Black) {
                pixel.a = 0;
                image.setPixel(sf::Vector2u(x, y), pixel);
            }

        }
    }
}

std::unordered_map<int, sf::Texture> loadAssetsMapFromDirectory(const std::string& directoryPath) {
    std::unordered_map<int, sf::Texture> assets;

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {

        if (entry.is_regular_file()) {
            sf::Image image;

            if (image.loadFromFile(entry.path().string())) {
                makeBlackPixelsTransparent(image);
                sf::Texture texture;

                if (texture.loadFromImage(image)) {

                    std::string filename = entry.path().filename().string();
                    std::string filenameWithoutExtension = filename.substr(0, filename.find_last_of('.'));
                    int key = std::stoi(filenameWithoutExtension);
                    assets[key] = std::move(texture);
                }

            }
        }
    }
    return assets;
}

std::vector<sf::Texture> loadAssetsVectorFromDirectory(const std::string& directoryPath) {
    std::vector<sf::Texture> assets;
    std::vector<std::filesystem::path> files;

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {

        if (entry.is_regular_file()) {
            files.push_back(entry.path());
        }
    }

    std::sort(files.begin(), files.end());

    for (const auto& file : files) {
        sf::Image image;

        if (image.loadFromFile(file.string())) {
            makeBlackPixelsTransparent(image);
            sf::Texture texture;

            if (texture.loadFromImage(image)) {
                assets.push_back(std::move(texture));
            }

        }
    }
    return assets;
}