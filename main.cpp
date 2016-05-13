
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<list>
#include <math.h>
#include <stdlib.h>
#include "Bullet.h"

#define PI 3.14159265

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character, *bullet, *enemy;
SDL_Rect rect_background,rect_character, rect_bullet, rect_enemy;

double getAngle(double distance_x, double distance_y)
{
    double angle = atan (distance_y/distance_x) * 180 / PI;
    if(distance_x<0)
    {
        angle=180+angle;
    }
    if(distance_x>0 && distance_y<0)
    {
        angle=360+angle;
    }
    return angle;
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    int screen_width = 500;
    int screen_height= 250;
    if((window = SDL_CreateWindow("Image Loading", 100, 100, screen_width/*WIDTH*/, screen_height/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
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

    enemy = IMG_LoadTexture(renderer, "enemy.png");
    SDL_QueryTexture(enemy, NULL, NULL, &w, &h);
    rect_enemy.x = 0;
    rect_enemy.y = 100;
    rect_enemy.w = w;
    rect_enemy.h = h;

    list<Bullet*>bullets;
    int frame=0;

    //Main Loop
    double player_velocity=3;
    double bullet_velocity=8;
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
            rect_character.y-=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
        {
            rect_character.y+=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
        {
            rect_character.x+=player_velocity;
        }
        if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
        {
            rect_character.x-=player_velocity;
        }

        int mouse_x, mouse_y;
        SDL_GetMouseState( &mouse_x, &mouse_y );

        if(frame%30==0)
        {
            double dist_x = mouse_x - rect_character.x;
            double dist_y = rect_character.y - mouse_y;
            bullets.push_back(new Bullet(rect_character.x,rect_character.y,getAngle(dist_x,dist_y)));
        }

        SDL_RenderCopy(renderer, background, NULL, &rect_background);

        for(list<Bullet*>::iterator i=bullets.begin();
            i!=bullets.end();
            i++)
        {
            Bullet* bullet_actual = *i;
            rect_bullet.x=bullet_actual->x;
            rect_bullet.y=bullet_actual->y;
            SDL_RenderCopy(renderer, bullet, NULL, &rect_bullet);
            bullet_actual->x+=cos(-bullet_actual->angle*PI/180)*bullet_velocity;
            bullet_actual->y+=sin(-bullet_actual->angle*PI/180)*bullet_velocity;

            if(bullet_actual->x>screen_width || bullet_actual->x<0
               || bullet_actual->y>screen_height || bullet_actual->y<0)
            {
                list<Bullet*>::iterator to_erase=i;
                i--;
                bullets.erase(to_erase);
                delete bullet_actual;
            }
        }

        SDL_RenderCopy(renderer, character, NULL, &rect_character);
        SDL_RenderCopy(renderer, enemy, NULL, &rect_enemy);

        SDL_RenderPresent(renderer);
        SDL_Delay(17);
        frame++;
    }

	return 0;
}
