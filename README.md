# Twisted Trails - Guyanese-themed Maze Game

Twisted Trails is a maze game implemented in C, where players navigate through mazes and answer Guyanese-themed questions to progress. The goal is to reach the end of each level while accumulating points and avoiding obstacles.

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [How to Play](#how-to-play)
5. [Documentation](#documentation)
6. [Contributing](#contributing)
7. [License](#license)

## 1. Introduction <a name="introduction"></a>

In this project, we've created a maze game called "Twisted Trails" that challenges players to navigate through a series of mazes while answering questions related to Guyanese culture and themes. The game includes multiple levels, each with its unique maze and set of questions.

## 2. Features <a name="features"></a>

- **Guyanese-themed Questions**: Players must answer questions related to Guyanese topics to progress through the game.
- **Leaderboard**: The game maintains a leaderboard of the highest scores achieved by players.
- **Multiple Levels**: Twisted Trails consists of multiple levels, each presenting increasingly challenging mazes and questions.
- **Lives System**: Players have a limited number of lives to complete all levels, adding an element of strategy and challenge.
- **Graphics and Audio**: The game features graphics and audio elements to enhance the gaming experience.

## 3. Installation <a name="installation"></a>

To install and run Twisted Trails on your system, follow these steps:

1. Clone the repository from GitHub:

    ```bash
    git clone https://github.com/RolexAlexander/Twisted-Trails.git
    ```

2. Navigate to the project directory:

    ```bash
    cd Twisted-Trails
    ```

3. Compile the source code using a C compiler. To build the game, use the following command in GCC (version 13.2.0):

    ```bash
    gcc src/game.c icon.o -o 'Twisted-Trails.exe' -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lgdi32 -lwinmm
    ```

4. Run the compiled executable:

    ```bash
    ./Twisted-Trails.exe
    ```

## 4. How to Play <a name="how-to-play"></a>

- **Controls**:
  - Use the arrow keys to navigate the player character through the maze.
  - use your mouse to select answers to questions.
- **Objective**:
  - Navigate through each level's maze while answering questions correctly.
  - Accumulate points and strive to achieve the highest score possible.
  - Complete all levels with the limited number of lives available.

## 5. Documentation <a name="documentation"></a>

The source code includes detailed comments to explain the functionality of each function, variable, and game state. Additionally, a `src/maze.h` file contains declarations for maze-related functions and structures.

## 6. Contributing <a name="contributing"></a>

Contributions to Twisted Trails are welcome! Feel free to submit bug fixes, feature enhancements, or suggestions via pull requests on [GitHub](https://github.com/RolexAlexander/Twisted-Trails.git).

## 7. License <a name="license"></a>

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for more information.