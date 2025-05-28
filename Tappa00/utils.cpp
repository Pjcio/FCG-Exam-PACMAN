#include "utils.h"

std::unordered_map<int, sf::Texture> loadAssetsFromDirectory(const std::string& directoryPath) {
    std::unordered_map<int, sf::Texture> assets;

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {

        if (entry.is_regular_file()) {
            sf::Image image;

            if (image.loadFromFile(entry.path().string())) {
                sf::Vector2u size = image.getSize();

                for (unsigned y = 0; y < size.y; ++y) {
                    for (unsigned x = 0; x < size.x; ++x) {
                        sf::Color pixel = image.getPixel({x, y});

                        if (pixel == sf::Color::Black) {
                            pixel.a = 0;
                            image.setPixel({x, y}, pixel);
                        }

                    }
                }
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