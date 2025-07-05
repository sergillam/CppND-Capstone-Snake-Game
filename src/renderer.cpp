#include "renderer.h"
#include "game.h"    // Para ter acesso a struct Food e enum FoodType
#include <iostream>
#include <string>

// Letras G, A, M, E (espaço), O, V, E, R
const int letter_height = 7, letter_width = 7;
const int spacing = 2; // Espaço entre letras

// Letras, 1 = bloco, 0 = vazio
const int letters[8][7][7] = {
  // G
  {{1,1,1,1,1,1,0},
   {1,0,0,0,0,0,0},
   {1,0,0,0,0,0,0},
   {1,0,0,1,1,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,1,1,1,1,1,0}},
  // A
  {{0,1,1,1,1,0,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,1,1,1,1,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0}},
  // M
  {{1,0,0,0,0,0,1},
   {1,1,0,0,0,1,1},
   {1,0,1,0,1,0,1},
   {1,0,0,1,0,0,1},
   {1,0,0,0,0,0,1},
   {1,0,0,0,0,0,1},
   {1,0,0,0,0,0,1}},
  // E
  {{1,1,1,1,1,1,0},
   {1,0,0,0,0,0,0},
   {1,0,0,0,0,0,0},
   {1,1,1,1,1,0,0},
   {1,0,0,0,0,0,0},
   {1,0,0,0,0,0,0},
   {1,1,1,1,1,1,0}},
  // O
  {{0,1,1,1,1,0,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {0,1,1,1,1,0,0}},
  // V
  {{1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {0,1,0,0,1,0,0},
   {0,0,1,1,0,0,0}},
  // E (de novo)
  {{1,1,1,1,1,1,0},
   {1,0,0,0,0,0,0},
   {1,0,0,0,0,0,0},
   {1,1,1,1,1,0,0},
   {1,0,0,0,0,0,0},
   {1,0,0,0,0,0,0},
   {1,1,1,1,1,1,0}},
  // R
  {{1,1,1,1,1,0,0},
   {1,0,0,0,0,1,0},
   {1,0,0,0,0,1,0},
   {1,1,1,1,1,0,0},
   {1,0,1,0,0,0,0},
   {1,0,0,1,0,0,0},
   {1,0,0,0,1,0,0}}
};

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(Snake const &snake, Food const &food, const std::vector<SDL_Point> &obstacles) {
    SDL_Rect block;
    block.w = screen_width / grid_width;
    block.h = screen_height / grid_height;

    // Clear screen (background)
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);

    // Draw obstacles
    SDL_SetRenderDrawColor(sdl_renderer, 80, 80, 80, 255); // Grey
    for (auto const &block_point : obstacles) {
        block.x = block_point.x * block.w;
        block.y = block_point.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render food (color by type)
    switch (food.type) {
        case FoodType::Normal:
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF); // Yellow
            break;
        case FoodType::SpecialScore:
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF); // Red
            break;
        case FoodType::SpeedUp:
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x69, 0xB4, 0xFF); // Pink (HotPink)
            break;
        case FoodType::SlowDown:
            SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF); // White
            break;
    }
    block.x = food.pos.x * block.w;
    block.y = food.pos.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);

    // Render snake's body
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const &point : snake.body) {
        block.x = point.x * block.w;
        block.y = point.y * block.h;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render snake's head
    block.x = static_cast<int>(snake.head_x) * block.w;
    block.y = static_cast<int>(snake.head_y) * block.h;
    if (snake.alive) {
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    } else {
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
    SDL_RenderFillRect(sdl_renderer, &block);

    // Update Screen
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::RenderGameOverMessage() {
    // Configurações dos blocos
    int block_size = screen_width / 80; // ajusta conforme seu grid
    int total_letters = 8;
    int msg_width = total_letters * (letter_width * block_size + spacing * block_size);
    int msg_height = letter_height * block_size;

    int base_x = (screen_width - msg_width) / 2;
    int base_y = (screen_height - msg_height) / 2;

    SDL_SetRenderDrawColor(sdl_renderer, 50, 50, 50, 220);
    SDL_Rect background = {base_x - 20, base_y - 20, msg_width + 40, msg_height + 40};
    SDL_RenderFillRect(sdl_renderer, &background);

    // Desenha as letras
    SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255); // branco
    for (int l = 0; l < total_letters; ++l) {
        for (int y = 0; y < letter_height; ++y) {
            for (int x = 0; x < letter_width; ++x) {
                if (letters[l][y][x]) {
                    SDL_Rect block = {
                        base_x + l * (letter_width * block_size + spacing * block_size) + x * block_size,
                        base_y + y * block_size,
                        block_size, block_size
                    };
                    SDL_RenderFillRect(sdl_renderer, &block);
                }
            }
        }
    }
    SDL_RenderPresent(sdl_renderer);
    SDL_Delay(2000); // mostra 2 segundos
}

char Renderer::WaitRestartOrQuit() {
    SDL_Event e;
    while (true) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                char key = static_cast<char>(tolower(e.key.keysym.sym));
                if (key == 'r' || key == 'q') {
                    return key;
                }
            } else if (e.type == SDL_QUIT) {
                return 'q';
            }
        }
        SDL_Delay(10);
    }
}

void Renderer::RenderGameOverMessageWithInstructions() {
    // Mostra GAME OVER (já faz isso)
    RenderGameOverMessage();
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                             "Game Over",
                             "Pressione R para reiniciar ou Q para sair.",
                             sdl_window);
}

void Renderer::RenderPauseOverlay() {
    // Dim background para escurecer
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 150); // Preto translúcido
    SDL_Rect full_screen = {0, 0, static_cast<int>(screen_width), static_cast<int>(screen_height)};
    SDL_RenderFillRect(sdl_renderer, &full_screen);

    // Texto "PAUSED" desenhado no centro usando blocos (similar ao Game Over)
    const int block_size = screen_width / 40;
    const char* paused_text = "PAUSED";

    // Para simplificar, desenhe um retângulo branco centralizado com texto desenhado de forma simples:
    // (Se quiser algo mais elaborado, pode usar SDL_ttf, mas aqui uma caixa simples)
    SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);

    SDL_Rect rect = {
        static_cast<int>(screen_width / 2) - (block_size * 5),
        static_cast<int>(screen_height / 2) - (block_size),
        block_size * 10,
        block_size * 2
    };
    SDL_RenderFillRect(sdl_renderer, &rect);

    // Para desenhar as letras PAUSED usando blocos, você pode definir uma matriz parecida com Game Over,
    // ou apenas deixar a caixa branca (para teste).

    SDL_RenderPresent(sdl_renderer);
}
