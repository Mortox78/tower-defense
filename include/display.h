#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <memory>
//#include "enemy.h"
//#include "tower.h"
#include "vector.h"
//#include "game_manager.h"


class enemy;
class tower;
class game;
class slow_tower;
class explosive_tower;

class display
{
    private:
        int                                     indexColorMap;

        std::vector<std::unique_ptr<tower>>     towerListHUD;

        void                                    display_HUD_grid(game& game) const;
        void                                    display_money(game& game) const;
        void                                    init_SDL();
        void                                    display_HUD_grid_cross(const int& x, const int& y, const int& money) const;
        void                                    set_color_draw_map(const int& nodeMap);
        void                                    display_castle_health(game& game) const;

        SDL_Window*                             create_window();
        SDL_Renderer*                           create_renderer();
        TTF_Font*                               create_font();

    public:
        SDL_Renderer*                           renderer;
        SDL_Window*                             window;
        TTF_Font*                               font;

        display();
        ~display();

        void                                    draw_map(game& game);
        void                                    display_renderer(game& game) const;
        void                                    display_HUD(game& game);
        void                                    display_buying_tower(game& game) const;
        void                                    display_score(game& game) const;
};


void draw_circle(t_circle circle, SDL_Renderer* renderer, bool filledCircle);

#endif