#pragma once

#include <deque>
#include <SFML/System/Vector2.hpp>
#include "Config.h"

class Snake {
private:
    std::deque<sf::Vector2i> body;
    Direction currentDir;
    bool growing;

public:
    Snake();
    void reset();
    void setDirection(Direction dir);
    Direction getDirection() const;
    void update();
    void grow();
    bool checkSelfCollision() const;
    bool checkWallCollision() const;
    const std::deque<sf::Vector2i>& getBody() const;
    sf::Vector2i getHead() const;
};
