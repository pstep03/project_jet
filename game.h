#ifndef GAME_H
#define GAME_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<vector>
#include<cstdlib>
#include<ctime>


struct Enemy{
    SDL_Rect rect;
    float speed;
};

struct Fire{
    SDL_Rect rect;
    float speed;
};

class Game{
    public:
        Game();
        ~Game();

        void init();
        void run();
        void cleanUp();

        bool isRunning() const;

    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Texture *enemyTexture;
        SDL_Texture *fireTexture;
        bool running;

        SDL_Texture *loadTexture(const char *path);

        std::vector<Enemy> enemies;
        const int MAX_ENEMIES = 20;

        std::vector<Fire> fire;
        Uint32 lastFire = 0;
        bool fireFromLeft;

        void spawnEnemy();
};

#endif