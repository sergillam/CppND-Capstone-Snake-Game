# Snake Game Capstone Project

![Snake Game Gameplay Demo](snake_game.gif)

## How to Play

- Use the **arrow keys** or **WASD** to control the snake's movement:
  - Up: Arrow Up / W
  - Down: Arrow Down / S
  - Left: Arrow Left / A
  - Right: Arrow Right / D
- Press **P** (uppercase or lowercase) to **pause** or **resume** the game.
- When the game is over:
  - Press **R** (uppercase or lowercase) to **restart** the game with the same settings.
  - Press **Q** (uppercase or lowercase) to **quit** the game.

During the game, avoid colliding with obstacles or the snake’s own body to keep playing and increase your score by eating different types of food.

---

## Concurrency Implementation

This project uses **C++ concurrency features** to enhance gameplay and meet the project rubric requirements.

### Bonus Food Thread

- A dedicated thread is launched whenever a **bonus food** item appears on the board (for example, every 10 points scored).  
- The thread runs a timer (15 seconds) using `std::thread` and manages the lifetime of the bonus food.
- **If the bonus food is not eaten in time, the thread will automatically remove it from the grid.**  
- This demonstrates the use of real concurrent computation inside the main game loop.

**Relevant code references**:

- Thread launch: `Game::StartBonusFoodThread()` in `game.cpp`
- Thread logic: `Game::BonusFoodTimer()` in `game.cpp`
- Thread declared in `game.h` as `std::thread bonus_food_thread;`

### Synchronization and Safety

- A `std::mutex` (`bonus_mutex`) is used to **protect shared data** related to bonus food state between the main thread and the bonus thread.
- A `std::condition_variable` (`bonus_cv`) is used for thread synchronization, allowing the bonus food thread to be notified immediately if the food is consumed before the timer expires.
- These objects are declared in `game.h` and used in `Game::BonusFoodTimer()` and related code in `game.cpp`.

**Relevant code references**:

- Mutex/Condition Variable: declared in `game.h`  
std::mutex bonus_mutex;
std::condition_variable bonus_cv;

- Their use is found throughout the thread management code in `game.cpp`.

### Atomic (optional)

- In this implementation, `bonus_food_active` is a simple `bool`, but could be made `std::atomic<bool>` for even stricter thread safety (though the mutex already guarantees correctness).

### Removal of Score Saving Thread

- **Note:** The previous implementation saved scores asynchronously using threads and `std::promise`/`std::future`.  
After feedback from the reviewer, the threaded score saving code was removed because saving a file is a single, quick operation and does not benefit from concurrency in this context.
- All concurrency is now focused on gameplay-related features, in line with the project rubric.

---

**Example lines to reference in your code**:

- `game.h`: `std::thread bonus_food_thread;`, `std::mutex bonus_mutex;`, `std::condition_variable bonus_cv;`
- `game.cpp`:  
- `void Game::StartBonusFoodThread()`
- `void Game::BonusFoodTimer()`
- Use of `std::unique_lock<std::mutex>` and `bonus_cv.wait_for`

---

This concurrency implementation demonstrates:

- Correct use of threads (`std::thread`)
- Mutual exclusion and data protection (`std::mutex`)
- Thread synchronization (`std::condition_variable`)
- Clean-up of old, unnecessary async code after reviewer feedback

---

## Known Issues / Future Improvements

- Currently, the game uses simple block graphics with SDL2; graphical enhancements such as sprites or animations could improve the visual experience.
- Sound effects and background music are not yet implemented.
- No multiple levels or progressive difficulty beyond obstacle count and speed.
- AI-controlled opponents or additional game modes could make gameplay more varied and challenging.
- The input handling could be further refined for smoother control.
- Cross-platform testing is basic; some adjustments might be needed for full compatibility on all systems.

These potential improvements provide room for further development and enhancement of the project.

## Code Structure & Object-Oriented Design

This project follows good Object-Oriented Programming (OOP) principles by organizing functionality into well-defined classes with clear responsibilities:

- **Game**: Controls the main game logic, including updating game state, placing food and obstacles, and handling pause functionality.
- **Snake**: Represents the snake entity, managing its position, movement, growth, and collision detection.
- **Controller**: Handles user input events and updates the snake's direction accordingly.
- **Renderer**: Manages all rendering logic using SDL2, including drawing the snake, food, obstacles, pause overlay, and game over messages.
- **ScoreManager**: Responsible for reading, saving, and displaying persistent high scores.

Each class explicitly specifies access modifiers (`public`, `private`) for its members, ensuring encapsulation and proper interface design.

The design demonstrates an understanding of encapsulation, modularity, and separation of concerns, which are fundamental OOP principles.

---

## User Input & Functions

The project utilizes multiple functions to accept, process, and respond to user input:

- At game start, the player is prompted to enter their name, select the initial snake speed, and choose the difficulty level.  
These inputs are validated and handled by dedicated functions such as `AskPlayerName()`, `AskInitialSpeed()`, and `AskDifficulty()`.

- During gameplay, user input is processed by the `Controller` class via its `HandleInput()` method, which listens for keyboard events to control the snake's movement, pause (`P` key), restart (`R` key), or quit (`Q` key).

- The snake's speed can be temporarily modified by consuming special food items, affecting gameplay dynamically.

- All user inputs are handled robustly with validation, and the game responds accordingly to ensure smooth and controlled interaction.

This use of functions for input and control clearly demonstrates an understanding of C++ functions and control structures, fulfilling the project rubric requirements for loops, functions, and I/O.

---

## Summary

Together, these design choices and function implementations illustrate a solid grasp of C++ programming concepts including:

- Object-oriented design with encapsulated classes and clear interfaces
- Robust user input handling and validation via dedicated functions
- Game state management through modular methods
- Clear separation of rendering, input, and logic

These aspects ensure that the project meets or exceeds the core rubric points related to functions, I/O, and object-oriented programming.

## Main Features & Controls

- **Classic Snake gameplay** controlled with keyboard arrows or WASD keys.
- **Player name input** at game start.
- **Difficulty levels** affecting the number of obstacles:
- Easy (5 obstacles)
- Medium (15 obstacles)
- Hard (30 obstacles)
- **Initial snake speed selection**:
- Slow
- Medium
- Fast
- **Food types with distinct effects**:
- Normal (yellow): grows snake by 1 and adds 1 point
- Special (red): grows snake by 2 and adds 5 points
- Speed-up (pink): temporarily increases snake speed
- Slow-down (white): temporarily decreases snake speed
- **Obstacles** placed on the grid; colliding ends the game.
- **Pause feature**: press `P` (uppercase or lowercase) to pause/resume the game.
- **Game Over screen** displayed on grid with "GAME OVER" message.
- **Pause screen overlay** displayed when paused.
- **Persistent high score ranking** saved in `highscores.txt`.
- **Post-game options**:
- Press `R` (uppercase or lowercase) to restart the game with the same settings.
- Press `Q` (uppercase or lowercase) to quit.

## Main Features

- Classic Snake gameplay controlled with keyboard arrows or WASD keys
- Start screen with options to:
- Enter player name
- Select initial snake speed (slow, medium, fast)
- Select difficulty level (based on number of obstacles)
- Different types of food:
- Normal food (yellow) — grows snake by 1 and adds 1 point
- Special food (red) — grows snake by 2 and adds 5 points
- Speed-up food (pink) — temporarily increases snake speed
- Slow-down food (white) — temporarily decreases snake speed
- Obstacles on the grid that cause game over on collision
- Pause functionality toggled by pressing `P`
- Game Over screen rendered on the grid with instructions
- Pause screen overlay
- Persistent high score ranking saved in `highscores.txt`
- Restart game with `R` or quit with `Q` after game over

---

## Dependencies for Running Locally
* cmake >= 3.7
* All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
* Linux: make is installed by default on most Linux distros
* Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
* Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
* All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
>Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
* Linux: gcc / g++ is installed by default on most Linux distros
* Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
* Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

---

