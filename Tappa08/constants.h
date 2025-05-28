#pragma once

// Definizione delle costanti globali
const int WINDOW_RESIZE = 3;
const int GAME_BOARD_HEIGHT = 248 * WINDOW_RESIZE;  // Pac-Man original pixel size * resize factor
const int GAME_BOARD_WIDTH = 224 * WINDOW_RESIZE;
const int TILE_SIZE = 8 * WINDOW_RESIZE;
const int SCORE_BOARD_HEIGHT = TILE_SIZE * 3; // 3 rows of tiles
const int SCORE_BOARD_WIDTH = GAME_BOARD_WIDTH;
const int WINDOW_HEIGHT = GAME_BOARD_HEIGHT + SCORE_BOARD_HEIGHT;
const int WINDOW_WIDTH = GAME_BOARD_WIDTH;
