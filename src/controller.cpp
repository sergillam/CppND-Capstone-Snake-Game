#include "controller.h"
#include "game.h"
#include "snake.h"
#include "SDL.h"
#include <iostream>

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake, Game &game) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      char key = static_cast<char>(tolower(e.key.keysym.sym));
      // Handle pause toggle
      if (key == 'p') {
        game.TogglePause();
      }

      if (game.IsPaused()) {
        continue; // ignora outras teclas se pausado
      }

      // Direções (não permitir inversão)
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          if (snake.direction != Snake::Direction::kDown) {
            snake.direction = Snake::Direction::kUp;
          }
          break;
        case SDLK_DOWN:
          if (snake.direction != Snake::Direction::kUp) {
            snake.direction = Snake::Direction::kDown;
          }
          break;
        case SDLK_LEFT:
          if (snake.direction != Snake::Direction::kRight) {
            snake.direction = Snake::Direction::kLeft;
          }
          break;
        case SDLK_RIGHT:
          if (snake.direction != Snake::Direction::kLeft) {
            snake.direction = Snake::Direction::kRight;
          }
          break;
      }
    }
  }
}
