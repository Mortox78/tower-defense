#include <iostream>
#include <assert.h>
//#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include "../include/display.h"
#include "../include/enemy.h"
#include "../include/tower.h"
#include "../include/vector.h"
#include "../include/game_manager.h"
#include "../include/slow_tower.h"
#include "../include/collisions.h"
#include "../include/explosive_tower.h"


display::display()
{
	window       = create_window();
    renderer     = create_renderer();

	towerListHUD.push_back(std::make_unique<tower>());
	towerListHUD.push_back(std::make_unique<slow_tower>());
	towerListHUD.push_back(std::make_unique<explosive_tower>());

	font 	     = create_font();

	indexColorMap = 0;
}

display::~display()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
	TTF_CloseFont(font);
}

void display::init_SDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("SDL_Init error: %s", SDL_GetError());
		return;
	}

	atexit(SDL_Quit); // Tell the program to call SDL_Quit() when program ends
}

SDL_Window* display::create_window()
{
	SDL_Window* window = SDL_CreateWindow("Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 704, SDL_WINDOW_SHOWN);

	if (window == NULL)
	{
		SDL_Log("SDL_CreateWindow error: %s", SDL_GetError());
		return NULL;
	}
	else
	{
		return window;
	}
}

SDL_Renderer* display::create_renderer()
{
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		SDL_Log("SDL_CreateRenderer error: %s", SDL_GetError());
		return NULL;
	}
	else
	{
		return renderer;
	}
}

void display::display_renderer(game& game) const
{
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
}

void display::draw_map(game& game)
{
   	int x = 0;	//Pos of our cursor
	int y = 0;

	int nodeMap;

	for (int j = 0; j < MAP_LINE; ++j)
	{
		for (int i = 0; i < MAP_COLUMN; ++i)
		{
			// Set the color of the renderer
			nodeMap = game.get_node_map(i, j);
			set_color_draw_map(nodeMap);

			// draw one node
			SDL_Rect rect = { x, y, 64, 64};
       		SDL_RenderFillRect(renderer, &rect);

			// jump to the next node
			x += 64;
		}
		// we do this in order to alternate the color between each lines
		if (indexColorMap == 20)
		{
			indexColorMap = 1;
		}
		else if (indexColorMap == 21)
		{
			indexColorMap = 0;
		}
		// go to the next line
		x = 0;
	 	y += 64;
	}
	indexColorMap = 0;
}


void draw_circle(t_circle circle, SDL_Renderer* renderer, bool filledCircle)
{
	int x_center     = circle.centerOfCircle.x;
	int y_center     = circle.centerOfCircle.y;
    int rad 		 = circle.radCircle;

	do
	{
		int x = 0;
		int y = rad;
		int m = 5 - 4*y;
	
		while (x <= y)
		{
			SDL_RenderDrawPoint(renderer, x + x_center, y + y_center);
			SDL_RenderDrawPoint(renderer, y + x_center, x + y_center );
			SDL_RenderDrawPoint(renderer, -x + x_center, y + y_center );
			SDL_RenderDrawPoint(renderer, -y + x_center, x + y_center );
			SDL_RenderDrawPoint(renderer, x + x_center, -y + y_center );
			SDL_RenderDrawPoint(renderer, y + x_center, -x + y_center );
			SDL_RenderDrawPoint(renderer, -x + x_center, -y + y_center );
			SDL_RenderDrawPoint(renderer, -y + x_center, -x + y_center );
	
			if (m > 0)
			{
				y -= 1;
				m -= 8*y;	
				
			}
			x += 1;
			m += 8*x + 4;
		}
		if (filledCircle == false)
			rad = 0;
		else
			rad -= 1;
	}
	while (rad != 0);
}

void display::display_buying_tower(game& game) const
{
	if (game.chooseTower == 0)
		return;
	
	// game.chooseTower - 1, because our tower list HUD begin at 0
	towerListHUD[game.chooseTower - 1]->display_buying_tower(game);
}

void display::display_money(game& game) const
{
	std::string strMoney = "Money : " + std::to_string(game.get_money());
	const char* money = strMoney.c_str();

	SDL_Color color = {255, 255, 255, 255};
	SDL_Rect textRect = {15 * 64, 0, 256, 64};

	SDL_Surface* surface = TTF_RenderText_Blended(font, money, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, NULL, &textRect);

	SDL_DestroyTexture(texture);	
}

void display::display_castle_health(game& game) const
{
	std::string strMoney = "Health : " + std::to_string(game.healthCastle);
	const char* money = strMoney.c_str();

	SDL_Color color = {255, 255, 255, 255};
	SDL_Rect textRect = {10 * 64, 0, 192, 64};

	SDL_Surface* surface = TTF_RenderText_Blended(font, money, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, NULL, &textRect);

	SDL_DestroyTexture(texture);	
}

TTF_Font* display::create_font()
{
	TTF_Init();
	TTF_Font* font = TTF_OpenFont("media/roboto/Roboto-Thin.ttf", 100);
	if (font == NULL)
	{
		SDL_Log("TTF_OpenFont error: %s", SDL_GetError());
		return NULL;
	}

	return font;	
}

void display::display_HUD_grid(game& game) const
{
	int money = game.get_money();
	int x 	  = 19 * 64;
	int y 	  = 3 * 64;

	// Draw the main grid
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect rect = {x, y, 64, 320};
	SDL_RenderDrawRect(renderer, &rect);

	for (int i = 1; i < 5; ++i)
	{
		SDL_RenderDrawLine(renderer, x, y + (i * 64), x + 64, y + (i * 64));
	}

	// Draw the lines that prevent the player that he can't afford to buy this tower.
	display_HUD_grid_cross(x, y, money);
}

void display::display_HUD(game& game)
{
	for (std::unique_ptr<tower>& tower : towerListHUD)
		tower->display_tower_HUD(game);

	display_HUD_grid(game);
	display_buying_tower(game);
	display_money(game);
	display_castle_health(game);
}

void display::display_HUD_grid_cross(const int& x, const int& y, const int& money) const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	if (money < 5)
	{
		SDL_RenderDrawLine(renderer, x, y, x + 64, y + 64);
		SDL_RenderDrawLine(renderer, x, y + 64, x + 64, y);
	}
	if (money < 10)
	{
		SDL_RenderDrawLine(renderer, x, y + 64, x + 64, y + 128);
		SDL_RenderDrawLine(renderer, x, y + 128, x + 64, y + 64);		
	}
	if (money < 20)
	{
		SDL_RenderDrawLine(renderer, x, y + 128, x + 64, y + 192);
		SDL_RenderDrawLine(renderer, x, y + 192, x + 64, y + 128);
	}	
}

void display::set_color_draw_map(const int& nodeMap)
{
	// If our node is a road
    if (nodeMap == -1)
    {
        SDL_SetRenderDrawColor(renderer, 181, 139, 70, 255);
    }
	// If our node is a part of our tower HUD (gray node)
	else if (nodeMap >= 10)
	{
		SDL_SetRenderDrawColor(renderer, 153, 163, 164, 255);
	}
 	// If our node is a part of our castle (dark red)
	else if (nodeMap == -2)
	{
		SDL_SetRenderDrawColor(renderer, 169, 50, 38, 255);
	}
	// we use that index because we're drawing a checkerboard
	else if (indexColorMap % 2 == 0)
	{
		SDL_SetRenderDrawColor(renderer, 76, 180, 73, 255);
	}
	// we use that index because we're drawing a checkerboard
	else if (indexColorMap % 2 == 1)
	{
		SDL_SetRenderDrawColor(renderer, 92, 220, 89, 255);
	}
	
	++indexColorMap;
}

void display::display_score(game& game) const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	std::string strMoney = "You lost ! Level : " + std::to_string(game.get_level() - 1);
	const char* money = strMoney.c_str();

	SDL_Color color = {255, 255, 255, 255};
	SDL_Rect textRect = {2 * 64, 4 * 64, 1024, 128};

	SDL_Surface* surface = TTF_RenderText_Blended(font, money, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_RenderCopy(renderer, texture, NULL, &textRect);
	SDL_RenderPresent(renderer);

	SDL_DestroyTexture(texture);
	SDL_Delay(3000);
}