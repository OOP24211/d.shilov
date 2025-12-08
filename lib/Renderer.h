#pragma once

#include <SFML/Graphics.hpp>
#include "Snake.h"
#include "Food.h"
#include "Config.h"
#include <string>

class Renderer {
private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text text;
    sf::RectangleShape block;

public:
    Renderer(sf::RenderWindow& win);
    bool loadFont(const std::string& path);
    void renderGame(const Snake& snake, const Food& food, int score, int time);
    void renderMenu(GameState state, int score);
    void display();
    void clear();
};

