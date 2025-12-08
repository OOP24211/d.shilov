#include "Renderer.h"
#include <sstream>

Renderer::Renderer(sf::RenderWindow& win) : window(win) {
    block.setSize(sf::Vector2f(BLOCK_SIZE - 2.0f, BLOCK_SIZE - 2.0f));
}

bool Renderer::loadFont(const std::string& path) {
    return font.loadFromFile(path);
}

void Renderer::renderGame(const Snake& snake, const Food& food, int score, int time) {
    // Рисуем еду
    block.setFillColor(sf::Color::Red);
    block.setPosition(food.getPosition().x * BLOCK_SIZE + 1, food.getPosition().y * BLOCK_SIZE + 1);
    window.draw(block);

    // Рисуем змейку
    block.setFillColor(sf::Color::Green);
    for (const auto& part : snake.getBody()) {
        block.setPosition(part.x * BLOCK_SIZE + 1, part.y * BLOCK_SIZE + 1);
        window.draw(block);
    }

    // UI
    std::stringstream ss;
    ss << "Score: " << score << "  Time: " << time;
    text.setFont(font);
    text.setString(ss.str());
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10);
    window.draw(text);
}

void Renderer::renderMenu(GameState state, int score) {
    // Затемнение фона
    sf::RectangleShape overlay(sf::Vector2f(WIDTH, HEIGHT));
    if (state == GameState::PAUSED) overlay.setFillColor(sf::Color(0, 0, 0, 150));
    else overlay.setFillColor(sf::Color(50, 0, 0, 200));
    window.draw(overlay);

    std::stringstream ss;
    if (state == GameState::PAUSED) {
        ss << "PAUSE\n\n[ESC] - Continue\n[R] - Restart\n[Q] - Exit";
    } else {
        ss << "Game over\nYour score: " << score << "\n\n[R] - Try again\n[Q] - Exit";
    }

    text.setFont(font);
    text.setString(ss.str());
    text.setCharacterSize(30);
    
    // Центрирование
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(WIDTH/2.0f, HEIGHT/2.0f);

    window.draw(text);
}

void Renderer::display() {
    window.display();
}

void Renderer::clear() {
    window.clear(sf::Color::Black);
}