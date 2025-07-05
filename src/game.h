#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

enum class FoodType { Normal, SpecialScore, SpeedUp, SlowDown };

struct Food {
    SDL_Point pos;
    FoodType type;
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, const std::string& player_name, float snake_speed, int num_obstacles);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  bool IsPaused() const { return paused; }
  void SetPaused(bool value) { paused = value; }
  void TogglePause() { paused = !paused; }
  
 private:
  Snake snake;
  Food food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  std::string player_name_;
  std::vector<SDL_Point> obstacles;

  int score{0};
  int num_obstacles_;

  bool paused = false;

  void PlaceFood();
  void Update();
  void PlaceObstacles();
  int speed_timer{0};
  float speed_effect{1.0f};
};

#endif