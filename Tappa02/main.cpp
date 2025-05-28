////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath> 
#include <vector>
#include <string>
#include "utils.h"
#include "TileMap.h"
#include "Player.h"

struct State
{
    // General resources
    sf::RenderWindow window;
    TileMap tileMap;
    Player player;

    State(unsigned w, unsigned h, std::string title)
    {
        window = sf::RenderWindow(sf::VideoMode({w, h}), title);

        player = Player(TILE_SIZE*14, TILE_SIZE*26.5f); // Start position of the player
        
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
    // Handle key presses and add directions to the buffer
    if (keyPressed.code == sf::Keyboard::Key::W) gs.player.setDirection(1);  // Up
    else if (keyPressed.code == sf::Keyboard::Key::A) gs.player.setDirection(3);  // Left
    else if (keyPressed.code == sf::Keyboard::Key::S) gs.player.setDirection(2);  // Down
    else if (keyPressed.code == sf::Keyboard::Key::D) gs.player.setDirection(4);  // Right
}

void handle(const sf::Event::KeyReleased &keyPressed, State &gs)
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
    // Adjust the view to maintain the aspect ratio after resizing
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

    //////////////////////////////////////////////////////////////////////
    // Draw score board (TO BE IMPLEMENTED)
    ////////////////////////////////////////////////////////////////////

    // Draw a grid of blue and black tiles
    for (int y = SCORE_BOARD_HEIGHT; y < GAME_BOARD_HEIGHT + SCORE_BOARD_HEIGHT; y += TILE_SIZE) {
        for (int x = 0; x < GAME_BOARD_WIDTH; x += TILE_SIZE) {
            sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
            tile.setPosition(sf::Vector2f(static_cast<float>(x), static_cast<float>(y)));

            // #DEBUG# //
            if (gs.player.currentTile.x * TILE_SIZE == x && gs.player.currentTile.y * TILE_SIZE + SCORE_BOARD_HEIGHT == y) {
                tile.setFillColor(sf::Color::Red); // Red highlight for player
            } else if (((x / TILE_SIZE) + (y / TILE_SIZE)) % 2 == 0) {
                tile.setFillColor(sf::Color(0, 0, 139)); // Extremely dark blue color
            } else {
                tile.setFillColor(sf::Color::Black);
            }

            gs.window.draw(tile);
        }
    }

    // Draw a tile map
    gs.tileMap.draw(gs.window);

    // Update and draw a player
    gs.player.update(gs.tileMap);
    gs.player.draw(gs.window);

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