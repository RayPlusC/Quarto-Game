# Quarto-Game

Welcome to **Quarto Game**, a recreation of the famous logical board game "Quarto" implemented in C++ using the OLC PixelGameEngine framework. This project offers a digital version of the game with interactive graphics and gameplay features.

## Table of Contents
- [About the Game](#about-the-game)
- [Features](#features)
- [Installation](#installation)
- [How to Play](#how-to-play)
- [Technologies Used](#technologies-used)
- [Contributing](#contributing)
- [License](#license)

## About the Game
Quarto is a strategic board game played on a 4x4 grid. Each piece has four distinct attributes, and the objective is to place the fourth piece in a row, column, or diagonal that shares the same attribute.

This project recreates Quarto with:
- A graphical interface for gameplay.
- Player interactions and turn-based mechanics.

## Features
- **Interactive Graphics**: Built using the OLC PixelGameEngine.
- **Multiplayer Support**: Play with another person.
- **CPU Oponent Support**: Play against CPU.
- **Logic-Based Mechanics**: Full implementation of the Quarto game rules.
- **Real-Time Hover and Selection**: Visual feedback for mouse interactions.

## Installation -- not tested
### Prerequisites
- **C++ Compiler**: Ensure you have a C++17-compatible compiler (e.g., GCC, Clang, or MSVC).
- **OLC PixelGameEngine**: This project uses the [OLC PixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine). Clone or download the framework.
- **Git**: To clone the repository.

### Steps
1. Clone this repository:
   ```bash
   git clone https://github.com/RayPlusC/Quarto-Game.git
   cd Quarto-Game
   ```
2. Compile the code using your preferred C++ compiler. For example, with g++:
   ```bash
   g++ -o QuartoGame main.cpp -std=c++17 -I/path/to/olcPixelGameEngine
   ```
3. Run the executable:
   ```bash
   ./QuartoGame
   ```

## How to Play
1. The game is played on a 4x4 board.
2. Players take turns selecting a piece for their opponent to place on the board.
3. The objective is to align four pieces in a row, column, or diagonal that share at least one common attribute (e.g., height, color, shape, etc.).
4. You can hover over and click on the board to place pieces interactively.

## Technologies Used
- **C++**: Core programming language.
- **OLC PixelGameEngine**: For graphical interface and rendering.
- **GitHub**: For version control and collaboration.

## Contributing
We welcome contributions to improve the game! Here’s how you can help:
1. Fork the repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m "Add some feature"
   ```
4. Push to the branch:
   ```bash
   git push origin feature-name
   ```
5. Open a Pull Request.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

Feel free to reach out or open an issue for questions, feature requests, or bug reports!
