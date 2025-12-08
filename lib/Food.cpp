#include "Food.h"
#include <cstdlib>

Food::Food() {
    // Временная заглушка, реальная позиция задастся при старте игры
    position = {0, 0}; 
}

void Food::respawn(const std::deque<sf::Vector2i>& snakeBody) {
    bool onSnake = true;
    while (onSnake) {
        onSnake = false;
        position.x = std::rand() % GRID_W;
        position.y = std::rand() % GRID_H;

        for (const auto& part : snakeBody) {
            if (part == position) {
                onSnake = true;
                break;
            }
        }
    }
}

sf::Vector2i Food::getPosition() const {
    return position;
}