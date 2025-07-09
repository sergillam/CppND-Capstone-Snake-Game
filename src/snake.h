#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  // Construtor padrão
  Snake(std::size_t grid_width, std::size_t grid_height, float initial_speed = 0.1f);

  // Rule of Five
  Snake(const Snake& other);                    // Copy constructor
  Snake& operator=(const Snake& other);         // Copy assignment
  Snake(Snake&& other) noexcept;                 // Move constructor
  Snake& operator=(Snake&& other) noexcept;      // Move assignment
  ~Snake();                                     // Destructor

  void Update();
  void GrowBody();
  bool SnakeCell(int x, int y);

  // Estado público (para acesso simples)
  Direction direction = Direction::kUp;
  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif  // SNAKE_H
