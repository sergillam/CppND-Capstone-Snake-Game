#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, const std::string& player_name, float snake_speed, int num_obstacles)
    : snake(grid_width, grid_height, snake_speed),
      player_name_(player_name),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      num_obstacles_(num_obstacles) {
  PlaceFood();
  PlaceObstacles();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input sempre processa para permitir pause e quit
    controller.HandleInput(running, snake, *this);

    if (!snake.alive) {
      // Game Over: desenha frame final, mensagem e pausa 2s
      renderer.Render(snake, food, obstacles);
      renderer.RenderGameOverMessage();
      SDL_Delay(2000);
      running = false;
      break; // pode usar break ou continue; break é mais claro aqui
    }

    if (!paused) {
      Update();
      renderer.Render(snake, food, obstacles);
    } else {
      renderer.RenderPauseOverlay(); // Overlay PAUSED
    }

    frame_end = SDL_GetTicks();

    frame_count++;
    frame_duration = frame_end - frame_start;

    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
    int x, y;
    while (true) {
        x = random_w(engine);
        y = random_h(engine);

        // Check that the location is not occupied by snake or obstacle
        if (!snake.SnakeCell(x, y)) {
            bool conflict = false;
            for (const auto& obs : obstacles)
                if (obs.x == x && obs.y == y)
                    conflict = true;
            if (!conflict)
                break;
        }
    }

    // Escolha do tipo de comida
    static int foods_eaten = 0; // static para manter contagem entre chamadas
    foods_eaten++;
    FoodType type = FoodType::Normal;

    if (foods_eaten % 5 == 0) {
        type = FoodType::SpecialScore; // a cada 5 comidas, vermelha
    } else {
        int r = rand() % 10;
        if (r == 0) type = FoodType::SpeedUp; // ~10% chance rosa
        else if (r == 1) type = FoodType::SlowDown; // ~10% chance branca
        // senão, normal (amarela)
    }

    food.pos.x = x;
    food.pos.y = y;
    food.type = type;
}

void Game::Update() {
    // Atualiza a posição da Snake a cada frame
    snake.Update();

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);
// Após atualizar a posição da Snake:
  for (const auto& obs : obstacles) {
      if (new_x == obs.x && new_y == obs.y) {
          snake.alive = false;
          break;
      }
  }

    // Check if there's food over here
    if (food.pos.x == new_x && food.pos.y == new_y) {
        switch (food.type) {
            case FoodType::Normal:
                score += 1;
                snake.GrowBody();
                break;
            case FoodType::SpecialScore:
                score += 5;
                snake.GrowBody();
                snake.GrowBody(); // crescimento duplo
                break;
            case FoodType::SpeedUp:
                score += 1;
                snake.speed *= 1.5f; // aumenta velocidade
                speed_timer = 30 * 60; // 30 segundos em frames (60fps)
                speed_effect = 1.5f;
                break;
            case FoodType::SlowDown:
                score += 1;
                snake.speed *= 0.5f; // diminui velocidade
                speed_timer = 30 * 60;
                speed_effect = 0.5f;
                break;
        }
        PlaceFood();
    }

    // Reseta efeito temporário de velocidade, se necessário
    if (speed_timer > 0) {
        speed_timer--;
        if (speed_timer == 0) {
            snake.speed /= speed_effect; // retorna ao normal
            speed_effect = 1.0f;
        }
    }
}

void Game::PlaceObstacles() {
    obstacles.clear();
    int placed = 0;
    while (placed < num_obstacles_) {
        int x = random_w(engine);
        int y = random_h(engine);

        // Avoid snake start position and food position
        bool conflict = false;
        if (snake.SnakeCell(x, y) || (food.pos.x == x && food.pos.y == y)) {
            conflict = true;
        }
        for (const auto& obs : obstacles) {
            if (obs.x == x && obs.y == y) {
                conflict = true;
                break;
            }
        }
        if (!conflict) {
            obstacles.push_back({x, y});
            placed++;
        }
    }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }