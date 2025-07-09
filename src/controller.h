#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"   // Para ter Snake e Snake::Direction
class Game;         // Forward declaration para Game

class Controller {
 public:
  // Remove qualificação Controller:: na declaração
  void HandleInput(bool &running, Snake &snake, Game &game) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif
