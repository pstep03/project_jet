#include "game.h"
#include <iostream>

Game::Game() : window(NULL), renderer(NULL), texture(NULL), enemyTexture(nullptr), running(false) {}

Game::~Game(){
    cleanUp();
}

void Game::init(){
    /*
    All initialization is found here regarding the SDL library.
    */
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << SDL_GetError() << '\n';
        running = false;
        return;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
        std::cerr << IMG_GetError() << '\n';
        running = false;
        return;
    }

    window = SDL_CreateWindow( // Window, title, etc.
        "Project Jet",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_SHOWN
    );

    if (!window){
        std::cerr << SDL_GetError() << '\n'; // Check if window doens't work
        running = false;
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 
    if (!renderer){
        std::cerr << SDL_GetError() << '\n'; // Check if renderer doesn't work
        running = false;
        return;
    }

    /*
    All textures go here.
    */
    texture = loadTexture("assets/fnl_img_jet.png");
    if (!texture){
        running = false;
        return;
    }

    enemyTexture = loadTexture("assets/fnl_img_enemy.png");
    if (!enemyTexture){
        running = false;
        return;
    }

    fireTexture = loadTexture("assets/img_fire_test.png");
    if (!fireTexture){
        running = false;
        return;
    }

    /*
    Code regarding fire
    */
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    SDL_ShowCursor(SDL_DISABLE);

    lastFire = SDL_GetTicks();
    fireFromLeft = true;

    running = true;
}

/*
Everything here is in regards to textures and sprites. We check for any errors while compiling and ensure
everything works as intended.
*/
SDL_Texture* Game::loadTexture(const char* path){
    SDL_Surface* surface = IMG_Load(path);
    if (!surface){
        std::cerr << IMG_GetError() << '\n';
        return nullptr;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!newTexture){
        std::cerr << SDL_GetError() << '\n';
    }

    return newTexture;
}

void Game::spawnEnemy(){
    /*
    Enemy spawning function
    */
    if (enemies.size() >= MAX_ENEMIES){
        return;
    }

    /*
    TODO:
    int w;
    int h;
    SDL_QueryTexture(enemyTexture, NULL, NULL, &w, &h);
    enemy.rect.w = w;
    enemy.rect.h = h;
    */

    /*
    Ensure spawns are random. At the moment, enemies are not scaled correctly.
    */
    Enemy enemy;
    enemy.rect.w = 80;
    enemy.rect.h = 40;
    enemy.rect.x = -enemy.rect.w;
    enemy.rect.y = std::rand() % (600 - enemy.rect.h);
    enemy.speed = 2 + std::rand() % 3;

    enemies.push_back(enemy); // Additional code regarding the enemy can be found in game.h
}

void Game::run(){
    SDL_Event event;
    bool isFullscreen = false;

    SDL_Rect jetRect = {100, 100, 200, 200};

    Uint32 lastSpawnTime = SDL_GetTicks();
    const Uint32 spawnInterval = 1000;

    while (running){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                running = false;
            }
            
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_TAB){
                    isFullscreen = !isFullscreen;
                    Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
                    SDL_SetWindowFullscreen(window, isFullscreen ? fullscreenFlag : 0);
                    SDL_ShowCursor(isFullscreen ? SDL_DISABLE : SDL_ENABLE);
                }
            }
        }
        
        int x;
        int y;
        SDL_GetMouseState(&x, &y);

        jetRect.x = x - jetRect.w / 2;
        jetRect.y = y - jetRect.h / 2;

        if (SDL_GetTicks() - lastSpawnTime >= spawnInterval){
            spawnEnemy();
            lastSpawnTime = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, texture, nullptr, &jetRect);

        for (size_t i = 0; i < enemies.size();){
            enemies[i].rect.x += static_cast<int>(enemies[i].speed);

            if (enemies[i].rect.x > 1280){
                enemies.erase(enemies.begin() + i);
            } else{
                SDL_RenderCopy(renderer, enemyTexture, nullptr, &enemies[i].rect);
                i++;
            }
        }

        for (size_t i = 0; i < fire.size();){
            fire[i].rect.x -= static_cast<int>(fire[i].speed);

            if (fire[i].rect.x < 0){
                fire.erase(fire.begin() + i);
            } else{
                SDL_RenderCopy(renderer, fireTexture, nullptr, &fire[i].rect);
                i++;
            }
        }

        for (size_t i = 0; i < fire.size();){
            bool removeFire = false;

            for (size_t j = 0; j < enemies.size(); j++){
                if (SDL_HasIntersection(&fire[i].rect, &enemies[j].rect)){
                    // std::cout << "Fire hit\n";

                    fire.erase(fire.begin() + i);
                    enemies.erase(enemies.begin() + j);
                    removeFire = true;
                    break;
                }
            }
            if (!removeFire){
                i++;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(8);

        Uint32 currTime = SDL_GetTicks();
        if (currTime - lastFire >= 300){
            Fire newFire;
            newFire.rect.w = 100;
            newFire.rect.h = 100;
            newFire.speed = 10.0f;
            newFire.rect.y = jetRect.y + jetRect.h / 2 - newFire.rect.h / 2 - 9; // adjusts y of left wing bullet


            if (fireFromLeft){
                newFire.rect.x = jetRect.x + 50; // adjusts x of left wing bullet
            } else{
                newFire.rect.x = jetRect.x + jetRect.w - 50 - newFire.rect.w; // adjusts x of right wing bullet
                newFire.rect.y = jetRect.y + jetRect.h / 2 - newFire.rect.h / 2 - 42; // adjusts y of right wing bullet

            }

            fire.push_back(newFire);
            fireFromLeft = !fireFromLeft;
            lastFire = currTime;
            
            // std::cout << "Firing from " << (fireFromLeft ? "left" : "right") << " at x = " << newFire.rect.x << '\n';

        }
    }
}

void Game::cleanUp() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }

    IMG_Quit();
    SDL_Quit();
}

bool Game::isRunning() const {
    return running;
}
