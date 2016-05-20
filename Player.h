#ifndef PLAYER_H
#define PLAYER_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

class Player
{
    public:
        double x;
        double y;
        double velocity;
        SDL_Texture *texture;
        SDL_Rect rect;
        SDL_Renderer *renderer;
        Player(double x,double y, double velocity,SDL_Renderer* renderer);
        void logic();
        void draw();
        virtual ~Player();
    protected:
    private:
};

#endif // PLAYER_H
