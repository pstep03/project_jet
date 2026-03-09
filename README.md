# Project Jet

A simple 2D jet shooter built with SDL2.

This project was created as a small learning exercise to practice
rendering, input handling, textures, and basic game loop structure in C++.

## Features

- Mouse-controlled jet
- Alternating left/right bullet fire
- Random enemy spawning
- Fullscreen toggle (TAB)

## Requirements

- C++ compiler (g++)
- SDL2
- SDL2_image

Make sure SDL2 and SDL2_image are installed and properly linked on your system before building.

## Build

If using the included Makefile:

```bash
g++ -Isrc/include -Lsrc/lib -o jet main.cpp game.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
./jet.exe
```
I have not had much time to work on this project. If I have time in the future, I will clean the code and add comments to the whole project.
