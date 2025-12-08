#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Food.h"
#include "Renderer.h"

class Game {
private:
    sf::RenderWindow window;
    Renderer renderer;
    Snake snake;
    Food food;
    
    GameState state;
    int score;
    sf::Clock moveClock;
    sf::Clock gameClock;
    float gameTime;

    void processInput();
    void update();
    void render();
    void restart();

public:
    Game();
    void run();
};
