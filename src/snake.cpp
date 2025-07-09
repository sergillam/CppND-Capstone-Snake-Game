#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update() {
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(
          head_y)};  // We first capture the head's cell before updating.
  UpdateHead();
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  // Capture the head's cell after updating.

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

void Snake::UpdateHead() {
    switch (direction) {
        case Direction::kUp:    head_y -= speed; break;
        case Direction::kDown:  head_y += speed; break;
        case Direction::kLeft:  head_x -= speed; break;
        case Direction::kRight: head_x += speed; break;
    }
  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

Snake::Snake(std::size_t grid_width, std::size_t grid_height, float initial_speed)
    : grid_width(grid_width),
      grid_height(grid_height),
      head_x(grid_width / 2),
      head_y(grid_height / 2),
      speed(initial_speed){
}

// Copy constructor
Snake::Snake(const Snake& other)
    : grid_width(other.grid_width),
      grid_height(other.grid_height),
      head_x(other.head_x),
      head_y(other.head_y),
      speed(other.speed),
      direction(other.direction),
      size(other.size),
      alive(other.alive),
      growing(other.growing),
      body(other.body) {}

// Move constructor
Snake::Snake(Snake&& other) noexcept
    : grid_width(other.grid_width),
      grid_height(other.grid_height),
      head_x(other.head_x),
      head_y(other.head_y),
      speed(other.speed),
      direction(other.direction),
      size(other.size),
      alive(other.alive),
      growing(other.growing),
      body(std::move(other.body)) {
  // Reset other's state if necessary
  other.size = 0;
  other.alive = false;
  other.growing = false;
}

// Move assignment
Snake& Snake::operator=(Snake&& other) noexcept {
  if (this == &other) return *this;
  grid_width = other.grid_width;
  grid_height = other.grid_height;
  head_x = other.head_x;
  head_y = other.head_y;
  speed = other.speed;
  direction = other.direction;
  size = other.size;
  alive = other.alive;
  growing = other.growing;
  body = std::move(other.body);

  other.size = 0;
  other.alive = false;
  other.growing = false;

  return *this;
}

// Destructor
Snake::~Snake() {
  // Nothing special needed; vector cleans up automatically
}

// Copy assignment
Snake& Snake::operator=(const Snake& other) {
  if (this == &other) return *this;
  grid_width = other.grid_width;
  grid_height = other.grid_height;
  head_x = other.head_x;
  head_y = other.head_y;
  speed = other.speed;
  direction = other.direction;
  size = other.size;
  growing = other.growing;
  alive = other.alive;
  body = other.body;  // vector copy
  return *this;
}
