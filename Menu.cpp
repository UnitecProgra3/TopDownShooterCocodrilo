#include "Menu.h"

Menu::Menu(SDL_Renderer *renderer,string path)
{
    this->renderer = renderer;

    ifstream in(path.c_str());

    string type;
    while(in>>type)
    {
        cout<<type<<endl;
        if(type=="text")
        {
            string text;
            int x,y;
            in>>text;
            in>>x;
            in>>y;
            items.push_back(new MenuText(renderer,text,x,y));
        }
        else if(type=="image")
        {
            string path;
            int x,y;
            in>>path;
            in>>x;
            in>>y;
            items.push_back(new MenuImage(renderer,path,x,y));
        }
        else if(type=="button")
        {
            string text;
            int x,y;
            in>>text;
            in>>x;
            in>>y;
            items.push_back(new MenuButton(renderer,text,x,y));
        }
    }

}

Menu::~Menu()
{
    //dtor
}

void Menu::draw()
{
    for(int i=0;i<items.size();i++)
    {
        MenuItem* current = items[i];
        current->draw();
    }
}

void Menu::loop()
{
    SDL_Event Event;
    double last_frame_ticks=SDL_GetTicks();
    bool exit_loop=false;
    while(!exit_loop)
    {
        bool mouse_up=false;
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
            if(Event.type == SDL_MOUSEBUTTONUP)
            {
                mouse_up=true;
            }
        }

        int mouse_x, mouse_y;
        SDL_GetMouseState( &mouse_x, &mouse_y );

        if(mouse_up)
        {
            for(int i=0;i<items.size();i++)
            {
                if(items[i]->type=="MenuButton")
                {
                    MenuButton* menu_button = (MenuButton*)items[i];
                    if(mouse_x > menu_button->rect.x
                       && mouse_x < menu_button->rect.x + menu_button->rect.w
                       && mouse_y > menu_button->rect.y
                       && mouse_y < menu_button->rect.y + menu_button->rect.h)
                    {
                        if(menu_button->text=="exit")
                        {
                            exit(0);
                        }
                        if(menu_button->text=="start")
                        {
                            exit_loop=true;
                        }
                        if(menu_button->text=="back")
                        {
                            exit_loop=true;
                        }
                        if(menu_button->text=="instructions")
                        {
                            Menu menu(renderer,"menu/instructions.txt");
                            menu.loop();
                        }
                    }
                }
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
