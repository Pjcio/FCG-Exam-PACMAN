////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include "utils.h"
#include "TileMap.h"
#include "constant.h"

////////////////////////////////////////////////////////////

struct State
{
    // General resources
    sf::RenderWindow window;

    State(unsigned w, unsigned h, std::string title)
    {
        window = sf::RenderWindow(sf::VideoMode({w, h}), title);
        // window.setVerticalSyncEnabled(true); // alternative to setFramerateLimit
        window.setFramerateLimit(60);
    }
};
///
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
/// Callback functions
void handle(const sf::Event::Closed &, State &gs)
{
    gs.window.close();
}

void handle(const sf::Event::TextEntered &textEnter, State &gs)
{
    
}

void handle(const sf::Event::KeyPressed &keyPressed, State &gs)
{
    
}

// void handle(const sf::Event::MouseMoved &mouseMoved, State &gs)
// {
// }

// void handle(const sf::Event::MouseButtonPressed &, State &gs)
// {
// }

void handle(const sf::Event::Resized &resized, State &gs)
{
    // Chiama adjustView dopo il resize
}

template <typename T>
void handle(const T &, State &gs)
{
    // All unhandled events will end up here
}
///
////////////////////////////////////////////////////////////

void doGraphics(State &gs) {
    gs.window.clear();

    // Draw a grid of blue and black tiles
    for (int y = 0; y < WINDOW_HEIGHT; y += TILE_SIZE) {
        for (int x = 0; x < WINDOW_WIDTH; x += TILE_SIZE) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));

            // Alternate colors between blue and black
            if (((x / TILE_SIZE) + (y / TILE_SIZE)) % 2 == 0) {
                tile.setFillColor(sf::Color(173, 216, 230)); 
            } else {
                tile.setFillColor(sf::Color::Black);
            }

            gs.window.draw(tile);
        }
    }

    // Draw a tile map
    TileMap tileMap;
    tileMap.draw(gs.window);

    gs.window.display();
}


int main()
{   

    State gs(WINDOW_WIDTH, WINDOW_HEIGHT , "Pac-Man");
    while (gs.window.isOpen()) // main loop
    {
        // event loop and handler through callbacks
        gs.window.handleEvents([&](const auto &event)
                               { handle(event, gs); });

        // Show log
        doGraphics(gs);
    }
}