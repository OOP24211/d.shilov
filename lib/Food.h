#pragma once

#include <SFML/System/Vector2.hpp>
#include <deque>
#include "Config.h"

class Food {
private:
    sf::Vector2i position;

public:
    Food();
    void respawn(const std::deque<sf::Vector2i>& snakeBody);
    sf::Vector2i getPosition() const;
};
