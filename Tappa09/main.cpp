////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <cmath> 
#include <vector>
#include <string>
#include <cstdlib> 
#include <iostream>
#include "utils.h"
#include "TileMap.h"
#include "Player.h"
#include "Score.h"
#include "constants.h"
#include "GameManager.h"
#include "Ghost.h"
////////////////////////////////////////////////////////////



struct State {
    sf::RenderWindow window;
    TileMap tileMap;
    Player player;
    Score score;

    std::vector<Ghost> ghosts {
        Ghost("Blinky"),
        Ghost("Pinky"),
        Ghost("Inky"),
        Ghost("Clyde")
    };

    GameManager gameManager;

    State(unsigned w, unsigned h, std::string title)
        : window(sf::VideoMode({w, h}), title),
          gameManager(tileMap, player, score, ghosts) // ✅ Ora i ghosts esistono già
    {
        window.setFramerateLimit(60);
        
        // patch to fix the white ghost bug
        for(int i = 0; i < ghosts.size(); i++){
            ghosts[i].updateSprite();
        }
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

void handle(const sf::Event::KeyPressed &keyPressed, State &gs)
{
    // Handle key presses and add directions to the buffer
    if (keyPressed.code == sf::Keyboard::Key::W) gs.player.setDirection(1);  // Up
    else if (keyPressed.code == sf::Keyboard::Key::A) gs.player.setDirection(3);  // Left
    else if (keyPressed.code == sf::Keyboard::Key::S) gs.player.setDirection(2);  // Down
    else if (keyPressed.code == sf::Keyboard::Key::D) gs.player.setDirection(4);  // Right
}

void handle(const sf::Event::Resized &resized, State &gs)
{
    // Adjust the view to maintain the aspect ratio after resizing
    sf::View view = gs.window.getView(); // Prendi la view attuale

    view.setSize({
        static_cast<float>(resized.size.x),
        static_cast<float>(resized.size.y)
    });

    gs.window.setView(view);
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

    // Draw sempre, anche se in pausa
    gs.tileMap.draw(gs.window);

    if(!gs.gameManager.isGameOver()) {
        gs.player.draw(gs.window);
    }

    if(!gs.player.isInDyingAnimation() && !gs.gameManager.isInVictoryAnimation()) {
        for(int i = 0; i < gs.ghosts.size(); i++){
            gs.ghosts[i].draw(gs.window);
        }
    }

    gs.gameManager.draw(gs.window);

    // Display all things drawn
    gs.window.display();

    /*
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
    */
   if(gs.gameManager.isInVictoryAnimation()){
        gs.gameManager.update(); // Update the game manager for the victory animation
    }

    // block update if game is paused or game over or in victory animation
    if ((gs.gameManager.pauseTimer == 0 or gs.gameManager.isGameOver())) {

        gs.player.update(gs.tileMap, gs.score);

        if(!gs.player.isInDyingAnimation()){
            for(int i = 0; i < gs.ghosts.size(); i++){
                gs.ghosts[i].update(gs.tileMap, gs.player, gs.ghosts[0]);
            }

            gs.gameManager.update();
        }
    } else {
        gs.gameManager.updatePauseTimer(); // Decrementa il timer di pausa
    }
    
}


int main()
{ 
    srand(static_cast<unsigned>(time(nullptr))); // Seed the random number generator

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