#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"

struct Food;

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height);
  ~Renderer();

  void Render(Snake const &snake, Food const &food, const std::vector<SDL_Point> &obstacles);
  void UpdateWindowTitle(int score, int fps);
  void RenderGameOverMessage();
  void RenderGameOverMessageWithInstructions();
  char WaitRestartOrQuit();
  void RenderPauseOverlay();
  
  private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
};

#endif
