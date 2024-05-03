# Twisted-Trails
Twisted Trails is a maze-themed game designed to immerse tourists and other individuals in Guyana's culture and geography. Players navigate through the maze, collecting items and gaining knowledge about the country along the way.

# Build Game
gcc src/raylib_game.c -o main.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lgdi32 -lwinmm