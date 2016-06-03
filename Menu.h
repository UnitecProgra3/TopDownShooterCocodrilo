#ifndef MENU_H
#define MENU_H

#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <fstream>

using namespace std;

class Menu
{
    public:
        SDL_Texture *background;
        SDL_Rect rect;
        SDL_Renderer *renderer;
        Menu(SDL_Renderer *renderer);
        void draw();
        void loop();
        virtual ~Menu();
    protected:
    private:
};

#endif // MENU_H
