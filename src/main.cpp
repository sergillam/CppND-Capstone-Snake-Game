#include "game.h"
#include "renderer.h"
#include "controller.h"
#include "score_manager.h"
#include <iostream>
#include <limits>
#include <string>

// Utility function: robust menu input validation
int AskOption(const std::string& prompt, int min, int max) {
    int option;
    while (true) {
        std::cout << prompt;
        if ((std::cin >> option) && (option >= min && option <= max)) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return option;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid option. Please enter a number between " << min << " and " << max << ".\n";
    }
}

// Function to ask for player's name
std::string AskPlayerName() {
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    return name;
}

// Function to select initial speed (now robust)
float AskInitialSpeed() {
    std::cout << "Select initial snake speed:\n";
    std::cout << "1 - Slow\n2 - Medium\n3 - Fast\n";
    int speedOption = AskOption("Enter the option number: ", 1, 3);

    switch (speedOption) {
        case 1:  return 0.07f; // Slow
        case 2:  return 0.10f; // Medium
        case 3:  return 0.16f; // Fast
        default: return 0.10f; // Should never happen
    }
}

// Difficulty enum and helpers
enum class Difficulty { Easy, Medium, Hard };

Difficulty AskDifficulty() {
    std::cout << "Select game difficulty:\n";
    std::cout << "1 - Easy\n2 - Medium\n3 - Hard\n";
    int option = AskOption("Enter the option number: ", 1, 3);

    switch(option) {
        case 1: return Difficulty::Easy;
        case 2: return Difficulty::Medium;
        case 3: return Difficulty::Hard;
        default: return Difficulty::Medium; // Should never happen
    }
}

int GetNumObstaclesForDifficulty(Difficulty diff) {
    switch (diff) {
        case Difficulty::Easy:   return 5;
        case Difficulty::Medium: return 15;
        case Difficulty::Hard:   return 30;
        default: return 10;
    }
}

int main() {
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kScreenWidth{640};
    constexpr std::size_t kScreenHeight{640};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    // 1. Ask for player name
    std::string playerName = AskPlayerName();

    // 2. Ask for initial speed
    float initialSpeed = AskInitialSpeed();

    // 3. Ask for difficulty and get number of obstacles
    Difficulty difficulty = AskDifficulty();
    int numObstacles = GetNumObstaclesForDifficulty(difficulty);

    // 4. ScoreManager
    ScoreManager scoreManager("highscores.txt");

    // 5. Create static game objects (renderer/controller)
    Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
    Controller controller;

    bool running = true;
    while (running) {
        // 6. Create a new Game each round
        Game game(kGridWidth, kGridHeight, playerName, initialSpeed, numObstacles);

        // 7. Run the game
        game.Run(controller, renderer, kMsPerFrame);

        // 8. Save the final score
        int final_score = game.GetScore();
        scoreManager.AddScore(playerName, final_score);

        // 9. Save scores asynchronously and wait for completion (optional)
        auto fut = scoreManager.SaveScoresAsync();
        fut.wait();

        // 10. Show ranking
        auto topScores = scoreManager.GetHighScores();
        std::cout << "\n===== Top Scores Ranking =====\n";
        for (const auto& entry : topScores) {
            std::cout << entry.name << ": " << entry.score << std::endl;
        }

        // 11. Visual "GAME OVER" message + instructions
        renderer.RenderGameOverMessageWithInstructions();

        // 12. Wait for user input: R = restart, Q = quit
        char action = renderer.WaitRestartOrQuit();
        if (action == 'q') {
            running = false;
        }
        // If 'r', loop restarts and a new game is created with the same settings
    }
    return 0;
}
