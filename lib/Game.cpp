#include "Game.h"
#include <ctime>

Game::Game() 
    : window(sf::VideoMode(WIDTH, HEIGHT), "Snake OOP"), 
      renderer(window) 
{
    window.setFramerateLimit(60);
    if (!renderer.loadFont("arial.ttf")) {
        // Обработка ошибки загрузки шрифта
    }
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    restart();
}

void Game::restart() {
    state = GameState::PLAYING;
    score = 0;
    gameTime = 0;
    snake.reset();
    food.respawn(snake.getBody());
    gameClock.restart();
    moveClock.restart();
}

void Game::run() {
    while (window.isOpen()) {
        processInput();
        update();
        render();
    }
}

void Game::processInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            // Глобальные клавиши меню
            if (state == GameState::PAUSED || state == GameState::GAME_OVER) {
                if (event.key.code == sf::Keyboard::R) restart();
                if (event.key.code == sf::Keyboard::Q) window.close();
            }

            // Переключение паузы
            if (event.key.code == sf::Keyboard::Escape) {
                if (state == GameState::PLAYING) state = GameState::PAUSED;
                else if (state == GameState::PAUSED) state = GameState::PLAYING;
            }

            // Управление змейкой
            if (state == GameState::PLAYING) {
                if (event.key.code == sf::Keyboard::Up) snake.setDirection(Direction::UP);
                else if (event.key.code == sf::Keyboard::Down) snake.setDirection(Direction::DOWN);
                else if (event.key.code == sf::Keyboard::Left) snake.setDirection(Direction::LEFT);
                else if (event.key.code == sf::Keyboard::Right) snake.setDirection(Direction::RIGHT);
            }
        }
    }
}

void Game::update() {
    if (state != GameState::PLAYING) return;

    gameTime = gameClock.getElapsedTime().asSeconds();

    if (moveClock.getElapsedTime().asSeconds() >= SNAKE_SPEED) {
        snake.update();

        // Проверка смерти
        if (snake.checkWallCollision() || snake.checkSelfCollision()) {
            state = GameState::GAME_OVER;
        }

        // Проверка еды
        if (snake.getHead() == food.getPosition()) {
            score++;
            snake.grow();
            food.respawn(snake.getBody());
        }

        moveClock.restart();
    }
}

void Game::render() {
    renderer.clear();
    
    // Рисуем игровой мир в любом случае (на фоне меню паузы)
    renderer.renderGame(snake, food, score, (int)gameTime);

    // Рисуем оверлеи меню
    if (state != GameState::PLAYING) {
        renderer.renderMenu(state, score);
    }

    renderer.display();
}