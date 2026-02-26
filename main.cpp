#include"game.h"

int main(int argc, char *argv[]){
    Game game;

    game.init();

    if(!game.isRunning()){
        return 1;
    }

    game.run();

    return 0;
}