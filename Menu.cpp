#include "Menu.h"

Menu::Menu(SDL_Renderer *renderer)
{
    this->renderer = renderer;

    ifstream in("menu/menu.txt");
    string nombre_archivo;
    in>>nombre_archivo;

    this->background = IMG_LoadTexture(renderer, nombre_archivo.c_str());
    int w,h;
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect.x = 0;
    rect.y = 0;
    rect.w = w;
    rect.h = h;
}

Menu::~Menu()
{
    //dtor
}

void Menu::draw()
{
    SDL_RenderCopy(renderer, background, NULL, &rect);
}

void Menu::loop()
{
    SDL_Event Event;
    double last_frame_ticks=SDL_GetTicks();
    bool exit_loop=false;
    while(!exit_loop)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                exit_loop=true;
            }
        }

        draw();

        SDL_RenderPresent(renderer);
        double current_ticks = SDL_GetTicks();
        double ticks_diff = current_ticks-last_frame_ticks;
        double sleep_time = 17-ticks_diff;
        //cout<<ticks_diff<<","<<sleep_time<<endl;
        last_frame_ticks=SDL_GetTicks();
        if(sleep_time>0)
            SDL_Delay(sleep_time);

    }
}
