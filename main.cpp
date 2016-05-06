
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<list>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character, *bullet;
SDL_Rect rect_background,rect_character, rect_bullet;

class Bullet
{
public:
    int x;
    int y;
    Bullet(int x,int y)
    {
        this->x=x;
        this->y=y;
    }
};


int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 500/*WIDTH*/, 250/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;

    character = IMG_LoadTexture(renderer, "personaje.png");
    SDL_QueryTexture(character, NULL, NULL, &w, &h);
    rect_character.x = 0;
    rect_character.y = 100;
    rect_character.w = w;
    rect_character.h = h;

    bullet = IMG_LoadTexture(renderer, "bullet.png");
    SDL_QueryTexture(bullet, NULL, NULL, &w, &h);
    rect_bullet.x = 0;
    rect_bullet.y = 100;
    rect_bullet.w = w;
    rect_bullet.h = h;

    list<Bullet*>bullets;
    int frame=0;

    //Main Loop
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if( currentKeyStates[ SDL_SCANCODE_UP ] )
        {
            rect_character.y-=1;
        }
        if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
        {
            rect_character.y+=1;
        }
        if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
        {
            rect_character.x+=1;
        }
        if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
        {
            rect_character.x-=1;
        }

        int x, y;
        SDL_GetMouseState( &x, &y );
        cout<<x<<endl;
        cout<<y<<endl;
        rect_bullet.x=x;
        rect_bullet.y=y;

        if(frame%30==0)
            bullets.push_back(new Bullet(rect_character.x,rect_character.y));

        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        SDL_RenderCopy(renderer, character, NULL, &rect_character);

        for(list<Bullet*>::iterator i=bullets.begin();
            i!=bullets.end();
            i++)
        {
            Bullet* bullet_actual = *i;
            rect_bullet.x=bullet_actual->x;
            rect_bullet.y=bullet_actual->y;
            SDL_RenderCopy(renderer, bullet, NULL, &rect_bullet);
            bullet_actual->x+=3;
            if(bullet_actual->x>500)
            {
                list<Bullet*>::iterator to_erase=i;
                i--;
                bullets.erase(to_erase);
                delete bullet_actual;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(17);
        frame++;
    }

	return 0;
}
