#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include "game.h"   // para FoodType, Food

struct Food;

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &snake, Food const &food, const std::vector<SDL_Point> &obstacles);
  void UpdateWindowTitle(int score, int fps);

  // Novas funções para Pause e GameOver
  void RenderPauseOverlay();
  void RenderGameOverMessage();
  void RenderGameOverMessageWithInstructions();

  // Espera input para reiniciar (R) ou sair (Q)
  char WaitRestartOrQuit();

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;

  // Definições para renderização de texto (exemplo Game Over)
  static constexpr int letter_height = 7;
  static constexpr int letter_width = 7;
  static constexpr int spacing = 2;
};

#endif
