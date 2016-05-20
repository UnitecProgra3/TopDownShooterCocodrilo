
#include<iostream>
#include<list>
#include <stdlib.h>
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character, *bullet, *enemy;
SDL_Rect rect_background,rect_character, rect_bullet, rect_enemy;

int screen_width = 500;
int screen_height= 250;

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

bool isOutOfBounds(int x, int y)
{
    return x>screen_width || x<0 || y>screen_height || y<0;
}

bool collisionCheck(SDL_Rect r1, SDL_Rect r2)
{
    if(r1.x > r2.x+r2.w)//Muy a la derecha
        return false;
    if(r1.x+r1.w < r2.x)//Muy a la izquierda
        return false;
    if(r1.y > r2.y+r2.h)//Muy abajo
        return false;
    if(r1.y+r1.h < r2.y)//Muy arriba
        return false;
    return true;
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
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

    enemy = IMG_LoadTexture(renderer, "enemy.png");
    SDL_QueryTexture(enemy, NULL, NULL, &w, &h);
    rect_enemy.x = 0;
    rect_enemy.y = 100;
    rect_enemy.w = w;
    rect_enemy.h = h;

    list<Bullet*>bullets;
    list<Enemy*>enemies;
    int frame=0;

    //Main Loop
    double player_velocity=3;
    double last_frame_ticks=SDL_GetTicks();
    Player *player = new Player(30,30,3,renderer);
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
        }

        int mouse_x, mouse_y;
        SDL_GetMouseState( &mouse_x, &mouse_y );

        if(frame%30==0)
        {
            double dist_x = mouse_x - player->rect.x;
            double dist_y = player->rect.y - mouse_y;
            bullets.push_back(new Bullet(player->rect.x,player->rect.y,getAngle(dist_x,dist_y),8,renderer));
        }

        if(frame%100==0)
        {
            enemies.push_back(new Enemy(0,rand()%screen_height-rect_enemy.h,renderer));
        }

        SDL_RenderCopy(renderer, background, NULL, &rect_background);

        for(list<Bullet*>::iterator i=bullets.begin();
            i!=bullets.end();
            i++)
        {
            Bullet* bullet_actual = *i;

            bullet_actual->logic();
            bullet_actual->draw();

            if(isOutOfBounds(bullet_actual->x,bullet_actual->y))
            {
                list<Bullet*>::iterator to_erase=i;
                i--;
                bullets.erase(to_erase);
                delete bullet_actual;
            }
        }

        for(list<Enemy*>::iterator i=enemies.begin();
            i!=enemies.end();
            i++)
        {
            Enemy* current_enemy = *i;

            current_enemy->logic();
            current_enemy->draw();

            if(isOutOfBounds(current_enemy->x,current_enemy->y))
            {
                list<Enemy*>::iterator to_erase=i;
                i--;
                enemies.erase(to_erase);
                delete current_enemy;
            }
        }

        player->logic();
        player->draw();

        for(list<Enemy*>::iterator i=enemies.begin();
            i!=enemies.end();
            i++)
        {
            Enemy*current_enemy = *i;
            for(list<Bullet*>::iterator j=bullets.begin();
                j!=bullets.end();
                j++)
            {
                Bullet*current_bullet = *j;

                rect_enemy.x = current_enemy->x;
                rect_enemy.y = current_enemy->y;

                rect_bullet.x = current_bullet->x;
                rect_bullet.y = current_bullet->y;

                if(collisionCheck(rect_enemy,rect_bullet))
                {
                    list<Bullet*>::iterator to_erase=j;
                    j--;
                    bullets.erase(to_erase);
                    delete current_bullet;

                    list<Enemy*>::iterator to_erase_enemy=i;
                    i--;
                    enemies.erase(to_erase_enemy);
                    delete current_enemy;
                    break;
                }
            }
        }

        SDL_RenderPresent(renderer);
        double current_ticks = SDL_GetTicks();
        double ticks_diff = current_ticks-last_frame_ticks;
        double sleep_time = 17-ticks_diff;
        //cout<<ticks_diff<<","<<sleep_time<<endl;
        last_frame_ticks=SDL_GetTicks();
        if(sleep_time>0)
            SDL_Delay(sleep_time);

        frame++;
    }

	return 0;
}
