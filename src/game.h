#ifndef GAME_H
#define GAME_H

#include <random>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Renderer;

// Enum para tipos de comida
enum class FoodType { Normal, SpecialScore, SpeedUp, SlowDown };

// Estrutura para comida
struct Food {
    SDL_Point pos;
    FoodType type;
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, const std::string& player_name, float snake_speed, int num_obstacles);

  // Rodar o jogo principal
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);

  // Getters
  int GetScore() const;
  int GetSize() const;

  // Pause control
  bool IsPaused() const { return paused; }
  void SetPaused(bool value) { paused = value; }
  void TogglePause() { paused = !paused; }

 private:
  Snake snake;
  Food food;
  Food bonus_food;
  bool bonus_food_active{false};
  std::thread bonus_food_thread;
  std::mutex bonus_mutex;
  std::condition_variable bonus_cv;

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
  void StartBonusFoodThread();
  void BonusFoodTimer();
  void PlaceBonusFood();
  
  int speed_timer{0};
  float speed_effect{1.0f};

  std::vector<SDL_Point> body;
};

#endif
