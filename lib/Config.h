#pragma once

const int BLOCK_SIZE = 20;
const int WIDTH = 800;
const int HEIGHT = 600;
const int GRID_W = WIDTH / BLOCK_SIZE;
const int GRID_H = HEIGHT / BLOCK_SIZE;
const float SNAKE_SPEED = 0.1f;

enum class Direction { UP, DOWN, LEFT, RIGHT, NONE };
enum class GameState { PLAYING, PAUSED, GAME_OVER };
