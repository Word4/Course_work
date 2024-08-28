#include <SDL.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h.>
#include <math.h>
#include <time.h>
#include <SDL_image.h>

#include "load_from_file.h"

int width = 48 * 20, height = 48 * 20;

bool isLevelRunning = false;
bool isMenuRunning = true;
bool you_died_is_running = false;
bool player_is_dead = false;

int choosing_the_input_method = 0;
int chek1 = 0;
int chek2 = 0;
int chek3 = 0;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Surface* surface = NULL;

bool isWPresed = false;
bool isSPresed = false;
bool isAPresed = false;
bool isDPresed = false;

int main_scale = 2;

bool player_can_move_x_left = true;
bool player_can_move_x_right = true;
bool player_can_move_y_up = true;
bool player_can_move_y_down = true;
     


snak_coord snak[10];
cursor_coord cursor;
exit_coord exit1[10];
gem_coord gem[100];
leaf_coord leaf[100];
door_coord door[100];
button_coord button[100];
wall_coord wall_1[2000];
rolling_stone_coord rolling_stone[10];

void Init()
{
	// Подключение SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("could not init SDL! Error: %s", SDL_GetError());
		system("pause");
		exit(1);
	}

	// Создание окна
	window = SDL_CreateWindow("Program", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("could not create window! Error: %s", SDL_GetError());
		system("pause");
		SDL_Quit();
		exit(1);
	}

	// Создание рендера
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		printf("could not create renderer! Error: %s", SDL_GetError());
		system("pause");
		SDL_Quit();
		exit(1);
	}

	// Инициализация SDL_IMG
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
	{
		printf("could not  Init SDL_Image! Error: %s", SDL_GetError());
		system("pause");
		SDL_Quit();
		exit(1);
	}
}
void DeInit(int error)
{
	if (renderer != NULL) SDL_DestroyRenderer(renderer);
	if (window != NULL) SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	exit(error);
}
void window_key_status()
{
	SDL_Event ev;
	SDL_PollEvent(&ev);

	switch (ev.type)
	{
	case SDL_QUIT: isLevelRunning = false; isMenuRunning = false; break;

	case SDL_WINDOWEVENT:
		if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			width = ev.window.data1;
			height = ev.window.data2;
		}
		break;

	case SDL_KEYDOWN:
		switch (ev.key.keysym.scancode)
		{
		case SDL_SCANCODE_ESCAPE: isLevelRunning = false; isMenuRunning = false; break;
		case SDL_SCANCODE_W: isWPresed = true; break;
		case SDL_SCANCODE_S: isSPresed = true; break;
		case SDL_SCANCODE_D: isDPresed = true; break;
		case SDL_SCANCODE_A: isAPresed = true; break;
		case SDL_SCANCODE_M: player_is_dead = true; break;
		}
		break;

	case SDL_KEYUP:
		switch (ev.key.keysym.scancode)
		{
		case SDL_SCANCODE_D: isDPresed = false; break;
		case SDL_SCANCODE_A: isAPresed = false; break;
		case SDL_SCANCODE_W: isWPresed = false; break;
		case SDL_SCANCODE_S: isSPresed = false; break;
		}

	case SDL_MOUSEMOTION:
		cursor.x = ev.motion.x;
		cursor.y = ev.motion.y;
		if (cursor.x >= 48 && cursor.x <= 11 * 48 && cursor.y >= 9 * 48 && cursor.y <= 12 * 48)
		{
			chek1 = 1;
		}
		else 
			chek1 = 0;
		if (cursor.x >= 48 && cursor.x <= 11 * 48 && cursor.y >= 12 * 48 && cursor.y <= 15 * 48)
		{
			chek2 = 1;
		}
		else
			chek2 = 0;
		if (cursor.x >= 48 && cursor.x <= 11 * 48 && cursor.y >= 15 * 48 && cursor.y <= 18 * 48)
		{
			chek3 = 1;
		}
		else
			chek3 = 0;
		break;
		

	case SDL_MOUSEBUTTONDOWN:
		if (ev.button.button == SDL_BUTTON_LEFT)
			if (ev.button.x >= 48 && ev.button.x <= 11 * 48 && ev.button.y >= 9 * 48 && ev.button.y <= 12 * 48)
			{
				isLevelRunning = true;
				choosing_the_input_method = 2;
			}
		if (ev.button.x >= 0 && ev.button.x <= 48 && ev.button.y >= 0 && ev.button.y <= 48)
		{
			isLevelRunning = true;
			choosing_the_input_method = 1;
		}
		if (ev.button.x >= 48 && ev.button.x <= 11 * 48 && ev.button.y >= 12 * 48 && ev.button.y <= 15 * 48)
		{
			isLevelRunning = true;
			choosing_the_input_method = 3;
		}
		if (ev.button.x >= 48 && ev.button.x <= 11 * 48 && ev.button.y >= 15 * 48 && ev.button.y <= 18 * 48)
		{
			isLevelRunning = true;
			choosing_the_input_method = 4;
		}

	}
}



SDL_Texture* loadTextureFromFille(const char* filename, SDL_Rect* rect)
{
	SDL_Surface* surface = IMG_Load(filename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	*rect = { 0, 0, surface->w, surface->h };

	SDL_FreeSurface(surface);
	return texture;
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	SDL_Init(SDL_INIT_EVERYTHING);
	Init();
	SDL_ShowCursor(SDL_DISABLE);

#pragma region creating texture

#pragma region player texture 

	SDL_Rect player_walk_rect;
	SDL_Texture* player_walk_texture = loadTextureFromFille("texture\\player\\walking_anim.png", &player_walk_rect); // текстура ходьбы
	player_walk_rect.w = player_walk_rect.h;

	SDL_Rect player_puch_rect;
	SDL_Texture* player_push_texture = loadTextureFromFille("texture\\player\\push_anim.png", &player_puch_rect); // текстура толкания камня
	player_puch_rect.w = player_puch_rect.h;

	SDL_Rect player_upping_rect;
	SDL_Texture* player_upping_texture = loadTextureFromFille("texture\\player\\upping.png", &player_upping_rect); // текстура подъема
	player_upping_rect.w = player_upping_rect.h;

	SDL_Rect player_stand_rect;
	SDL_Texture* player_stand_texture = loadTextureFromFille("texture\\player\\stand.png", &player_stand_rect); // текстура стояния на месте
	player_stand_rect.w = player_stand_rect.h;

	SDL_Rect player_up_stand_rect;
	SDL_Texture* player_up_stand_texture = loadTextureFromFille("texture\\player\\up_stand.png", &player_up_stand_rect); // текстура весения на стене смотря вверх
	player_up_stand_rect.w = player_up_stand_rect.h;

	SDL_Rect player_climb_stand_rect;
	SDL_Texture* player_climb_stand_texture = loadTextureFromFille("texture\\player\\climb_stand.png", &player_climb_stand_rect); // текстура весения на стене смотря ввлево или вправо
	player_climb_stand_rect.w = player_climb_stand_rect.h;

	SDL_Rect player_side_climb_rect;
	SDL_Texture* player_side_climb_texture = loadTextureFromFille("texture\\player\\pols.png", &player_side_climb_rect); // текстура карапканья в сторону
	player_side_climb_rect.w = player_side_climb_rect.h;

#pragma endregion

#pragma region rolling stone texture

	// текстура катящегося камня
	SDL_Rect rolling_rolling_stone_rect;
	SDL_Texture* rolling_rolling_stone_texture = loadTextureFromFille("texture\\stone\\rolling_stone.png", &rolling_rolling_stone_rect);
	rolling_rolling_stone_rect.w = rolling_rolling_stone_rect.h;

	// текстуры для стояния камня
	SDL_Rect standing_rolling_stone_1;
	SDL_Texture* standing_rolling_stone_1_texture = loadTextureFromFille("texture\\stone\\rolling_stone_standing_1.png", &standing_rolling_stone_1);

	SDL_Rect standing_rolling_stone_2;
	SDL_Texture* standing_rolling_stone_2_texture = loadTextureFromFille("texture\\stone\\rolling_stone_standing_2.png", &standing_rolling_stone_2);

	SDL_Rect standing_rolling_stone_3;
	SDL_Texture* standing_rolling_stone_3_texture = loadTextureFromFille("texture\\stone\\rolling_stone_standing_3.png", &standing_rolling_stone_3);

	SDL_Rect standing_rolling_stone_4;
	SDL_Texture* standing_rolling_stone_4_texture = loadTextureFromFille("texture\\stone\\rolling_stone_standing_4.png", &standing_rolling_stone_4);

	SDL_Rect standing_rolling_stone_5;
	SDL_Texture* standing_rolling_stone_5_texture = loadTextureFromFille("texture\\stone\\rolling_stone_standing_5.png", &standing_rolling_stone_5);

	SDL_Rect standing_rolling_stone_6;
	SDL_Texture* standing_rolling_stone_6_texture = loadTextureFromFille("texture\\stone\\rolling_stone_standing_6.png", &standing_rolling_stone_6);

	SDL_Rect standing_rolling_stone_7;
	SDL_Texture* standing_rolling_stone_7_texture = loadTextureFromFille("texture\\stone\\rolling_stone_standing_7.png", &standing_rolling_stone_7);

	SDL_Rect standing_rolling_stone_8;
	SDL_Texture* standing_rolling_stone_8_texture = loadTextureFromFille("texture\\stone\\rolling_stone_standing_8.png", &standing_rolling_stone_8);

#pragma endregion

#pragma region other texture

	// текстура фона
	SDL_Rect leaves_rect;
	SDL_Texture* leaves_texture = loadTextureFromFille("texture\\other\\back.png", &leaves_rect);

	// текстура каменной стены
	SDL_Rect wall1_rect;
	SDL_Texture* wall1_texture = loadTextureFromFille("texture\\other\\wall.png", &wall1_rect);

	// текстура кнопки
	SDL_Rect button_static_rect;
	SDL_Texture* button_static_texture = loadTextureFromFille("texture\\other\\button_static.png", &button_static_rect);

	SDL_Rect door_1_rect;
	SDL_Texture* door_1_texture = loadTextureFromFille("texture\\doors\\door_1.png", &door_1_rect);

	SDL_Rect door_2_rect;
	SDL_Texture* door_2_texture = loadTextureFromFille("texture\\doors\\door_2.png", &door_2_rect);

	SDL_Rect door_3_rect;
	SDL_Texture* door_3_texture = loadTextureFromFille("texture\\doors\\door_3.png", &door_3_rect);

	// текстура листвы
	SDL_Rect static_leaf_rect;
	SDL_Texture* static_leaf_texture = loadTextureFromFille("texture\\other\\leaf_static.png", &static_leaf_rect);

	// текстура листвы
	SDL_Rect leaf_braking_rect;
	SDL_Texture* leaf_braking_texture = loadTextureFromFille("texture\\other\\leaf_braking.png", &leaf_braking_rect);
	leaf_braking_rect.w = leaf_braking_rect.h;

	// текстура гема
	SDL_Rect gem_rect;
	SDL_Texture* gem_texture = loadTextureFromFille("texture\\other\\gem_static.png", &gem_rect);
	gem_rect.w = gem_rect.h;

	// текстура изчезания гема
	SDL_Rect gem_taking_rect;
	SDL_Texture* gem_taking_texture = loadTextureFromFille("texture\\other\\gem_brake.png", &gem_taking_rect);
	gem_taking_rect.w = gem_taking_rect.h;

	// текстура exit
	SDL_Rect exit_rect;
	SDL_Texture* exit_texture = loadTextureFromFille("texture\\other\\exit.png", &exit_rect);
	exit_rect.w = exit_rect.h;

	// текстура курсора
	SDL_Rect cursor_rect;
	SDL_Texture* cursor_texture = loadTextureFromFille("texture\\other\\cursor.png", &cursor_rect);

	SDL_Rect diamond_rect;
	SDL_Texture* diamond_texture = loadTextureFromFille("texture\\text\\diamond.png", &diamond_rect);
	SDL_Rect dst_diamond_rect = { 48 * 1, 48, 48 * 17, 48 * 5 };

	SDL_Rect rush_rect;
	SDL_Texture* rush_texture = loadTextureFromFille("texture\\text\\rush.png", &rush_rect);
	SDL_Rect dst_rush_rect = { 48 * 2,48 * 5, 48 * 15, 48 * 3 };

	SDL_Rect back_menu_rect;
	SDL_Texture* menu_back_texture = loadTextureFromFille("texture\\other\\back_menu.png", &back_menu_rect);

	SDL_Rect level1_main_rect;
	SDL_Texture* level1_main_texture = loadTextureFromFille("texture\\text\\level_1_main.png", &level1_main_rect);
	SDL_Rect dst_level1_main_rect = { 48,48 * 9, 48 * 10, 48 * 3 };

	SDL_Rect level1_sec_rect;
	SDL_Texture* level1_sec_texture = loadTextureFromFille("texture\\text\\level_1_sec.png", &level1_sec_rect);
	SDL_Rect dst_level1_sec_rect = { 48,48 * 9, 48 * 10, 48 * 3 };

	SDL_Rect level2_main_rect;
	SDL_Texture* level2_main_texture = loadTextureFromFille("texture\\text\\level_2_main.png", &level2_main_rect);
	SDL_Rect dst_level2_main_rect = { 48,48 * 12, 48 * 10, 48 * 3 };

	SDL_Rect level2_sec_rect;
	SDL_Texture* level2_sec_texture = loadTextureFromFille("texture\\text\\level_2_sec.png", &level2_sec_rect);
	SDL_Rect dst_level2_sec_rect = { 48,48 * 12, 48 * 10, 48 * 3 };

	SDL_Rect level3_main_rect;
	SDL_Texture* level3_main_texture = loadTextureFromFille("texture\\text\\level_3_main.png", &level3_main_rect);
	SDL_Rect dst_level3_main_rect = { 48,48 * 15, 48 * 10, 48 * 3 };

	SDL_Rect level3_sec_rect;
	SDL_Texture* level3_sec_texture = loadTextureFromFille("texture\\text\\level_3_sec.png", &level3_sec_rect);
	SDL_Rect dst_level3_sec_rect = { 48,48 * 15, 48 * 10, 48 * 3 };

	SDL_Rect snak_rect;
	SDL_Texture* snak_texture = loadTextureFromFille("texture\\snake\\snak.png", &snak_rect);
	snak_rect.w = snak_rect.h;

	SDL_Rect snak_up_rect;
	SDL_Texture* snak_up_texture = loadTextureFromFille("texture\\snake\\snak_up.png", &snak_up_rect);
	snak_up_rect.w = snak_up_rect.h;

	SDL_Rect snak_dead_rect;
	SDL_Texture* snak_dead_texture = loadTextureFromFille("texture\\snake\\snak_dead.png", &snak_dead_rect);
	snak_dead_rect.w = snak_dead_rect.h;

	SDL_Rect player_dead_rect;
	SDL_Texture* player_dead_texture = loadTextureFromFille("texture\\player\\player_is_dead.png", &player_dead_rect);
	player_dead_rect.w = player_dead_rect.h;

	SDL_Rect you_died_rect;
	SDL_Texture* you_died_texture = loadTextureFromFille("texture\\other\\you_died.png", &you_died_rect);
	SDL_Rect dst_you_died_rect = { 0,10*20, 48 * 20, 48 * 10 };

#pragma endregion

#pragma endregion

	while (isMenuRunning)
	{
#pragma region creating variables 

		int frame = 0;
		int frame_count = 4;

		int cur_frametime = 0;
		int max_frametime = 1000 / 8;

		int lasttime = SDL_GetTicks();
		int newtime;
		int dt = 0;

		int frame_for_eight = 0;
		int frame_for_eight_count = 8;

		int cur_frametime_for_eight = 0;
		int max_frametime_for_eight = 1000 / 8;

		int lasttime_for_eight = 0;
		int newtime_for_eight;
		int dt_for_eight = 0;

		int player_direction = 0;
		int animation_check = 0;

		int player_hitbox_pozition_x = 0;
		int player_hitbox_pozition_y = 0;

		bool is_player_stay_on_the_flor = false;


		bool dovodchik_is_activated = false;
		bool dovodchik_is_activated_for_move = false;
		bool chek_is_any_rolling_stone_moving = false;

		int score = 0;

		int exit_count = 1;
		int gem_count = 0;
		int rolling_stone_count = 8;
		int wall_1_count = 23;
		int button_count = 2;
		int door_count = 2;
		int leaf_count = 5;
		int snak_count = 2;	

		
		int player_timer = 20;

		for (int i = 0; i < 100; i++)
		{
			leaf[i].x = 0;
			leaf[i].y = 0;
			leaf[i].chek = 0;
			leaf[i].is_brake = false;
			leaf[i].timer = 10;
		}

		int player_pozition_x;
		int player_pozition_y;

		// player's starting coordinates
		

		for (int i = 0; i < 100; i++)
		{
			gem[i].x = 0;
			gem[i].y = 0;
			gem[i].is_taking = false;
			gem[i].timer = 20;
		}

		for (int i = 0; i < 100; i++)
		{
			rolling_stone[i].x = 0;
			rolling_stone[i].y = 0;
			rolling_stone[i].last_direction = 0;
		}

		for (int i = 0; i < 100; i++)
		{
			snak[i].x = 0;
			snak[i].y = 0;
			snak[i].is_dead = false;
		}


		player_is_dead = false;

#pragma endregion

#pragma region menu logic & rendering
		
		window_key_status();

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		
		// отрисовка фона
		for (int i = 0; i < width; i += 48)
			for (int j = 0; j < height; j += 48)
			{
				SDL_Rect back_menu_hitbox_rect{ i, j, 24 * main_scale, 24 * main_scale };
				SDL_RenderCopy(renderer, menu_back_texture, NULL, &back_menu_hitbox_rect);
			}

		SDL_RenderCopy(renderer, diamond_texture, &diamond_rect, &dst_diamond_rect);
		SDL_RenderCopy(renderer, rush_texture, &rush_rect, &dst_rush_rect);

		if (chek1 == 0)
		SDL_RenderCopy(renderer, level1_main_texture, &level1_main_rect, &dst_level1_main_rect);
		if (chek1 == 1)
		SDL_RenderCopy(renderer, level1_sec_texture, &level1_sec_rect, &dst_level1_sec_rect);
		if (chek2 == 0)
			SDL_RenderCopy(renderer, level2_main_texture, &level2_main_rect, &dst_level2_main_rect);
		if (chek2 == 1)
			SDL_RenderCopy(renderer, level2_sec_texture, &level2_sec_rect, &dst_level2_sec_rect);
		if (chek3 == 0)
			SDL_RenderCopy(renderer, level3_main_texture, &level3_main_rect, &dst_level3_main_rect);
		if (chek3 == 1)
			SDL_RenderCopy(renderer, level3_sec_texture, &level3_sec_rect, &dst_level3_sec_rect);

		cursor.dst_rect = { cursor.x , cursor.y , cursor_rect.w * main_scale, cursor_rect.h * main_scale };
		SDL_RenderCopy(renderer, cursor_texture, &cursor_rect, &cursor.dst_rect);

		SDL_RenderPresent(renderer);
		SDL_Delay(20);

#pragma endregion

		if (choosing_the_input_method == 2)
		{
			gem_count = input_from_file_gem_level_1(gem);
			wall_1_count = input_from_file_wall1_level_1(wall_1);
			leaf_count = input_from_file_leaf_level_1(leaf);
			rolling_stone_count = input_from_file_rolling_stone_level_1(rolling_stone);
			button_count = input_from_file_button_level_1(button);
			door_count = input_from_file_door_level_1(door);
			exit_count = input_from_file_exit_level_1(exit1);
			snak_count = input_from_file_snake_level_1(snak);
		}
		if (choosing_the_input_method == 3)
		{
			gem_count = input_from_file_gem_level_2(gem);
			wall_1_count = input_from_file_wall1_level_2(wall_1);
			leaf_count = input_from_file_leaf_level_2(leaf);
			rolling_stone_count = input_from_file_rolling_stone_level_2(rolling_stone);
			button_count = input_from_file_button_level_2(button);
			door_count = input_from_file_door_level_2(door);
			exit_count = input_from_file_exit_level_2(exit1);
			snak_count = input_from_file_snake_level_2(snak);
		}
		if (choosing_the_input_method == 4)
		{
			gem_count = input_from_file_gem_level_3(gem);
			wall_1_count = input_from_file_wall1_level_3(wall_1);
			leaf_count = input_from_file_leaf_level_3(leaf);
			rolling_stone_count = input_from_file_rolling_stone_level_3(rolling_stone);
			button_count = input_from_file_button_level_3(button);
			door_count = input_from_file_door_level_3(door);
			exit_count = input_from_file_exit_level_3(exit1);
			snak_count = input_from_file_snake_level_3(snak);
		}


#pragma region createing coords 

#pragma region recording coordinates for tests

		if (choosing_the_input_method == 1)
		{
			isLevelRunning = true;
			isMenuRunning = false;

			// кординаты змеи

			snak[0].x = 5 * 48;
			snak[0].y = 5 * 48;
			snak[0].direction = 1;

			snak[1].x = 12 * 48;
			snak[1].y = 13 * 48;
			snak[1].direction = 2;

			// координаты листвы

			leaf[0].x = 11;
			leaf[0].y = 15;

			leaf[1].x = 11;
			leaf[1].y = 14;

			leaf[2].x = 10;
			leaf[2].y = 14;

			leaf[3].x = 0;
			leaf[3].y = 13;

			leaf[4].x = 12;
			leaf[4].y = 5;

			// координаты кнопки

			button[0].x = 7;
			button[0].y = 14;

			button[1].x = 9;
			button[1].y = 14;

			// координаты стен

			wall_1[0].x = 2;
			wall_1[0].y = 5;

			wall_1[1].x = 10;
			wall_1[1].y = 5;

			wall_1[2].x = 3;
			wall_1[2].y = 15;

			wall_1[3].x = 4;
			wall_1[3].y = 15;

			wall_1[4].x = 5;
			wall_1[4].y = 15;

			wall_1[5].x = 6;
			wall_1[5].y = 15;

			wall_1[6].x = 1;
			wall_1[6].y = 15;

			wall_1[7].x = 7;
			wall_1[7].y = 15;

			wall_1[8].x = 8;
			wall_1[8].y = 15;

			wall_1[9].x = 0;
			wall_1[9].y = 14;

			wall_1[10].x = 7;
			wall_1[10].y = 18;

			wall_1[11].x = 9;
			wall_1[11].y = 15;

			wall_1[12].x = 10;
			wall_1[12].y = 18;

			wall_1[13].x = 11;
			wall_1[13].y = 18;

			wall_1[14].x = 12;
			wall_1[14].y = 18;

			wall_1[15].x = 13;
			wall_1[15].y = 18;

			wall_1[16].x = 14;
			wall_1[16].y = 18;

			wall_1[17].x = 14;
			wall_1[17].y = 17;

			wall_1[18].x = 10;
			wall_1[18].y = 15;

			wall_1[19].x = 9;
			wall_1[19].y = 18;

			wall_1[20].x = 8;
			wall_1[20].y = 18;

			wall_1[21].x = 13;
			wall_1[21].y = 5;

			wall_1[22].x = 12;
			wall_1[22].y = 15;

			wall_1[23].x = 6;
			wall_1[23].y = 4;

			//координаты двери

			door[0].x = 10;
			door[0].y = 16;

			door[1].x = 8;
			door[1].y = 16;


			//координаты катящегося камня

			rolling_stone[0].x = 5;
			rolling_stone[0].y = 14;

			rolling_stone[1].x = 7;
			rolling_stone[1].y = 14;

			rolling_stone[2].x = 1;
			rolling_stone[2].y = 14;

			rolling_stone[3].x = 1;
			rolling_stone[3].y = 10;

			rolling_stone[4].x = 2;
			rolling_stone[4].y = 14;

			rolling_stone[5].x = 2;
			rolling_stone[5].y = 4;

			rolling_stone[6].x = 6;
			rolling_stone[6].y = 3;

			rolling_stone[7].x = 12;
			rolling_stone[7].y = 4;

			exit1[0].x = 0;
			exit1[0].y = 0;
		}

#pragma endregion

#pragma region coordinate transformation

		for (int i = 0; i <= wall_1_count; i++)
		{
			wall_1[i].x = wall_1[i].x * 48;
			wall_1[i].y = wall_1[i].y * 48;
		}

		for (int i = 0; i <= exit_count; i++)
		{
			exit1[i].x = exit1[i].x * 48;
			exit1[i].y = exit1[i].y * 48;
		}

		for (int i = 0; i <= leaf_count; i++)
		{
			leaf[i].x = leaf[i].x * 48;
			leaf[i].y = leaf[i].y * 48;
		}

		for (int i = 0; i <= button_count; i++)
		{
			button[i].x = button[i].x * 48;
			button[i].y = button[i].y * 48;
		}

		for (int i = 0; i <= door_count; i++)
		{
			door[i].x = door[i].x * 48;
			door[i].y = door[i].y * 48;
			door[i].trigger_x = door[i].trigger_x * 48;
			door[i].trigger_y = door[i].trigger_y * 48;
		}

		for (int i = 0; i <= snak_count; i++)
		{
			snak[i].x = snak[i].x * 48;
			snak[i].y = snak[i].y * 48;
		}

		for (int i = 0; i <= gem_count; i++)
		{
			gem[i].x = gem[i].x * 48;
			gem[i].y = gem[i].y * 48;
		}

		for (int i = 0; i <= rolling_stone_count; i++)
		{
			rolling_stone[i].x = rolling_stone[i].x * 48;
			rolling_stone[i].y = rolling_stone[i].y * 48;
		}

		for (int i = 0; i < door_count; i++)
		{
			door[i].door_1_x = door[i].x;
			door[i].door_1_y = door[i].y;

			door[i].door_2_x = door[i].x;
			door[i].door_2_y = door[i].y;

			door[i].door_3_x = door[i].x;
			door[i].door_3_y = door[i].y + (1 * 48);
		}

#pragma endregion

#pragma endregion


		lasttime_for_eight = SDL_GetTicks();
		lasttime = SDL_GetTicks();

		if (choosing_the_input_method == 2)
		{
			player_pozition_x = 100;
			player_pozition_y = (2 * 48) - 6;
		}
		if (choosing_the_input_method == 3)
		{
			player_pozition_x = 100;
			player_pozition_y = (18 * 48)-6;
		}
		if (choosing_the_input_method == 1)
		{
			player_pozition_x = 100;
			player_pozition_y = (2 * 48) - 6;
		}
		if (choosing_the_input_method == 4)
		{
			player_pozition_x = 100;
			player_pozition_y = (18 * 48) - 6;
		}

		while (isLevelRunning)
		{
			window_key_status();

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			SDL_RenderClear(renderer);

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

#pragma region createing dst rect 

			SDL_Rect dst_rect_walk = { player_pozition_x, player_pozition_y, player_walk_rect.w * main_scale, player_walk_rect.h * main_scale };

			SDL_Rect dst_rect_stand = { player_pozition_x, player_pozition_y, player_stand_rect.w * main_scale, player_stand_rect.h * main_scale };

			SDL_Rect player_hitbox_rect = { player_pozition_x, player_pozition_y + 6, 24 * main_scale, 24 * main_scale };

			SDL_Rect dst_rect_upping = { player_pozition_x, player_pozition_y, player_upping_rect.w * main_scale, player_upping_rect.h * main_scale };

			SDL_Rect dst_rect_up_stand = { player_pozition_x, player_pozition_y, player_up_stand_rect.w * main_scale, player_up_stand_rect.h * main_scale };

			SDL_Rect dst_rect_side_climd = { player_pozition_x, player_pozition_y, player_side_climb_rect.w * main_scale, player_side_climb_rect.h * main_scale };

			SDL_Rect dst_rect_climb_stnd = { player_pozition_x, player_pozition_y, player_climb_stand_rect.w * main_scale, player_climb_stand_rect.h * main_scale };

			for (int i = 0; i < rolling_stone_count; i++)
				rolling_stone[i].dst_rect = { rolling_stone[i].x, rolling_stone[i].y, rolling_rolling_stone_rect.w * main_scale, rolling_rolling_stone_rect.h * main_scale };

			SDL_Rect dst_rect_player_push = { player_pozition_x, player_pozition_y, player_puch_rect.w * main_scale, player_puch_rect.h * main_scale };
			for (int i = 0; i < button_count; i++)
				button[i].dsr_rect_button_static = { button[i].x, button[i].texture_y, button_static_rect.w * main_scale, button_static_rect.h * main_scale };

			for (int i = 0; i < door_count; i++)
			{
				door[i].dst_door_1_rect = { door[i].door_1_x, door[i].door_1_y, door_1_rect.w * main_scale, door_1_rect.h * main_scale };

				door[i].dst_door_2_rect = { door[i].door_2_x, door[i].door_2_y, door_2_rect.w * main_scale, door_2_rect.h * main_scale };

				door[i].dst_door_3_rect = { door[i].door_3_x, door[i].texture_y, door_3_rect.w * main_scale, door_3_rect.h * main_scale };
			}

			for (int i = 0; i < leaf_count; i++)
			{
				leaf[i].dst_static_rect = { leaf[i].x, leaf[i].y, static_leaf_rect.w * main_scale, static_leaf_rect.h * main_scale };

				leaf[i].dst_break_rect = { leaf[i].x, leaf[i].y, leaf_braking_rect.w * main_scale, leaf_braking_rect.h * main_scale };
			}

			for (int i = 0; i < gem_count; i++)
				gem[i].dst_static_rect = { gem[i].x , gem[i].y , gem_taking_rect.w * main_scale, gem_taking_rect.h * main_scale };

			for (int i = 0; i < gem_count; i++)
				gem[i].dst_fading_rect = { gem[i].x , gem[i].y , gem_rect.w * main_scale, gem_rect.h * main_scale };

			for (int i = 0; i < exit_count; i++)
				exit1[i].dst_exit = { exit1[i].x , exit1[i].y , exit_rect.w * main_scale, exit_rect.h * main_scale };

			for (int i = 0; i < snak_count; i++)
			{
				snak[i].dst_snak_rect = { snak[i].x , snak[i].y, snak_rect.w * main_scale , snak_rect.h * main_scale };
				snak[i].dst_snak_up_rect = { snak[i].x , snak[i].y, snak_up_rect.w * main_scale , snak_up_rect.h * main_scale };
				snak[i].dst_snak_dead_rect = { snak[i].x , snak[i].y, snak_dead_rect.w * main_scale , snak_dead_rect.h * main_scale };
			}

			SDL_Rect dst_rect_player_is_dead = { player_pozition_x, player_pozition_y, player_dead_rect.w * main_scale, player_dead_rect.h * main_scale };

#pragma endregion

#pragma region floor chek for player

			for (int i = 0; i <= wall_1_count; i++)
			{
				if (player_hitbox_pozition_y + 48 == wall_1[i].y && ((player_hitbox_pozition_x <= wall_1[i].x && player_hitbox_pozition_x + 48 > wall_1[i].x) || (player_hitbox_pozition_x > wall_1[i].x && player_hitbox_pozition_x < wall_1[i].x + 48)))
				{
					is_player_stay_on_the_flor = true;
				}
			}

#pragma endregion

#pragma region collision chek for player

			// стнена слева от геоя
			for (int i = 0; i <= wall_1_count; i++)
			{
				if (player_hitbox_pozition_x == wall_1[i].x + 48 && ((player_hitbox_pozition_y >= wall_1[i].y && player_hitbox_pozition_y < wall_1[i].y + 48) || (player_hitbox_pozition_y < wall_1[i].y && player_hitbox_pozition_y + 48 > wall_1[i].y)))
					player_can_move_x_left = false;
			}

			// стнена справа от геоя
			for (int i = 0; i <= wall_1_count; i++)
			{
				if (player_hitbox_pozition_x + 48 == wall_1[i].x && ((player_hitbox_pozition_y >= wall_1[i].y && player_hitbox_pozition_y < wall_1[i].y + 48) || (player_hitbox_pozition_y < wall_1[i].y && player_hitbox_pozition_y + 48 > wall_1[i].y)))
					player_can_move_x_right = false;
			}

			// стнена сверху от геоя
			for (int i = 0; i <= wall_1_count; i++)
			{
				if (player_hitbox_pozition_y == wall_1[i].y + 48 && ((player_hitbox_pozition_x <= wall_1[i].x && player_hitbox_pozition_x + 48 > wall_1[i].x) || (player_hitbox_pozition_x > wall_1[i].x && player_hitbox_pozition_x < wall_1[i].x + 48)))
					player_can_move_y_up = false;
			}

			// стнена снизу от геоя
			for (int i = 0; i <= wall_1_count; i++)
			{
				if (player_hitbox_pozition_y + 48 == wall_1[i].y && ((player_hitbox_pozition_x <= wall_1[i].x && player_hitbox_pozition_x + 48 > wall_1[i].x) || (player_hitbox_pozition_x > wall_1[i].x && player_hitbox_pozition_x < wall_1[i].x + 48)))
					player_can_move_y_down = false;
			}

#pragma endregion

#pragma region dowodchiki

			// доводчик влево
			if (player_can_move_x_right)
			{
				if (isDPresed == false && player_direction == 0)
				{
					if ((player_pozition_x + 24 * main_scale) % 48 != 0)
					{
						if ((player_pozition_x + 24 * main_scale) % 48 != 0)
						{
							player_pozition_x += 4;
							animation_check = 1;
							dovodchik_is_activated = true;
							dovodchik_is_activated_for_move = true;
						}
						if ((player_pozition_x + 24 * main_scale) % 48 == 0)
						{
							animation_check = 0;
						}

					}
				}
			}

			// доводчик вправо
			if (player_can_move_x_left)
			{
				if (isAPresed == false && player_direction == 1)
				{
					if (player_pozition_x % 48 != 0)
					{
						if (player_pozition_x % 48 != 0)
						{
							player_pozition_x -= 4;
							animation_check = 2;
							dovodchik_is_activated = true;
							dovodchik_is_activated_for_move = true;
						}
						if (player_pozition_x % 48 == 0)
						{
							animation_check = 0;
						}

					}
				}
			}

			// доводчик вверх
			if (player_can_move_y_up == true)
			{
				if (isWPresed == false && player_direction == 3)
				{
					if ((player_pozition_y + 6) % 48 != 0)
					{
						if ((player_pozition_y + 6) % 48 != 0)
						{
							player_pozition_y -= 4;
							animation_check = 3;
							dovodchik_is_activated_for_move = true;
						}
						if ((player_pozition_y + 6) % 48 == 0)
						{
							animation_check = 0;
						}
					}
				}
			}

			// доводчик вниз
			if (player_can_move_y_up == true)
			{
				if (isSPresed == false && player_direction == 4)
				{
					if ((player_pozition_y + 6) % 48 != 0)
					{
						if ((player_pozition_y + 6) % 48 != 0)
						{
							player_pozition_y += 4;
							animation_check = 4;
							dovodchik_is_activated_for_move = true;
						}
						if ((player_pozition_y + 6) % 48 == 0)
						{
							animation_check = 0;
						}
					}
				}
			}

#pragma endregion

#pragma region gem logic

#pragma region gem colision

			// стнена слева от гема
			for (int i = 0; i <= wall_1_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x == wall_1[i].x + 48 && ((gem[j].y >= wall_1[i].y && gem[j].y < wall_1[i].y + 48) || (gem[j].y < wall_1[i].y && gem[j].y + 48 > wall_1[i].y)))
						gem[j].can_move_left = false;
				}

			// стнена справа от гема
			for (int i = 0; i <= wall_1_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x + 48 == wall_1[i].x && ((gem[j].y >= wall_1[i].y && gem[j].y < wall_1[i].y + 48) || (gem[j].y < wall_1[i].y && gem[j].y + 48 > wall_1[i].y)))
						gem[j].can_move_right = false;
				}

			// стнена снизу от гема
			for (int i = 0; i <= wall_1_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].y + 48 == wall_1[i].y && ((gem[j].x <= wall_1[i].x && gem[j].x + 48 > wall_1[i].x) || (gem[j].x > wall_1[i].x && gem[j].x < wall_1[i].x + 48)))
						gem[j].is_stay_on_the_floor = true;
				}

			// листва слева от гема
			for (int i = 0; i <= leaf_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x == leaf[i].x + 48 && ((gem[j].y >= leaf[i].y && gem[j].y < leaf[i].y + 48) || (gem[j].y < leaf[i].y && gem[j].y + 48 > leaf[i].y)))
						gem[j].can_move_left = false;
				}

			// листва справа от гема
			for (int i = 0; i <= leaf_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x + 48 == leaf[i].x && ((gem[j].y >= leaf[i].y && gem[j].y < leaf[i].y + 48) || (gem[j].y < leaf[i].y && gem[j].y + 48 > leaf[i].y)))
						gem[j].can_move_right = false;
				}

			// листва снизу от гема
			for (int i = 0; i <= leaf_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].y + 48 == leaf[i].y && ((gem[j].x <= leaf[i].x && gem[j].x + 48 > leaf[i].x) || (gem[j].x > leaf[i].x && gem[j].x < leaf[i].x + 48)))
						gem[j].is_stay_on_the_floor = true;
				}

			// камень слева от гема
			for (int i = 0; i < rolling_stone_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x == rolling_stone[i].x + 48 && ((gem[j].y >= rolling_stone[i].y && gem[j].y < rolling_stone[i].y + 48) || (gem[j].y < rolling_stone[i].y && gem[j].y + 48 > rolling_stone[i].y)))
						gem[j].can_move_left = false;
				}

			// камень справа от гема
			for (int i = 0; i < rolling_stone_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x + 48 == rolling_stone[i].x && ((gem[j].y >= rolling_stone[i].y && gem[j].y < rolling_stone[i].y + 48) || (gem[j].y < rolling_stone[i].y && gem[j].y + 48 > rolling_stone[i].y)))
						gem[j].can_move_right = false;
				}

			// гем слева от гема
			for (int i = 0; i < gem_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x == gem[i].x + 48 && ((gem[j].y >= gem[i].y && gem[j].y < gem[i].y + 48) || (gem[j].y < gem[i].y && gem[j].y + 48 > gem[i].y)))
						gem[j].can_move_left = false;
				}

			// гем справа от гема
			for (int i = 0; i < gem_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x + 48 == gem[i].x && ((gem[j].y >= gem[i].y && gem[j].y < gem[i].y + 48) || (gem[j].y < gem[i].y && gem[j].y + 48 > gem[i].y)))
						gem[j].can_move_right = false;
				}

			// дверь3 слева от гема
			for (int i = 0; i < door_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x == door[i].door_3_x + 48 && ((gem[j].y >= door[i].door_3_y && gem[j].y < door[i].door_3_y + 48) || (gem[j].y < door[i].door_3_y && gem[j].y + 48 > door[i].door_3_y)))
						gem[j].can_move_left = false;
				}

			// дверь3 справа от гема
			for (int i = 0; i < door_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x + 48 == door[i].door_3_y && ((gem[j].y >= door[i].door_3_y && gem[j].y < door[i].door_3_y + 48) || (gem[j].y < door[i].door_3_y && gem[j].y + 48 > door[i].door_3_y)))
						gem[j].can_move_right = false;
				}

			// дверь1 слева от гема
			for (int i = 0; i < door_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x == door[i].door_1_x + 48 && ((gem[j].y >= door[i].door_1_y && gem[j].y < door[i].door_1_y + 48) || (gem[j].y < door[i].door_1_y && gem[j].y + 48 > door[i].door_1_y)))
						gem[j].can_move_left = false;
				}

			// дверь1 справа от гема
			for (int i = 0; i < door_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].x + 48 == door[i].door_1_y && ((gem[j].y >= door[i].door_1_y && gem[j].y < door[i].door_1_y + 48) || (gem[j].y < door[i].door_1_y && gem[j].y + 48 > door[i].door_1_y)))
						gem[j].can_move_right = false;
				}

#pragma endregion

#pragma region logic for collecting gems

			// подбор гема слева
			for (int i = 0; i < gem_count; i++)
			{
				if (player_hitbox_pozition_x == gem[i].x + 40 && ((player_hitbox_pozition_y >= gem[i].y && player_hitbox_pozition_y < gem[i].y + 48) || (player_hitbox_pozition_y < gem[i].y && player_hitbox_pozition_y + 48 > gem[i].y)))
				{
					gem[i].is_taking = true;
					score++;
				}
			}

			// подбор гема справа
			for (int i = 0; i < gem_count; i++)
			{
				if (player_hitbox_pozition_x + 40 == gem[i].x && ((player_hitbox_pozition_y >= gem[i].y && player_hitbox_pozition_y < gem[i].y + 48) || (player_hitbox_pozition_y < gem[i].y && player_hitbox_pozition_y + 48 > gem[i].y)))
				{
					gem[i].is_taking = true;
				}
			}

			// подбор гема сверху
			for (int i = 0; i < gem_count; i++)
			{
				if (player_hitbox_pozition_y == gem[i].y + 40 && ((player_hitbox_pozition_x <= gem[i].x && player_hitbox_pozition_x + 48 > gem[i].x) || (player_hitbox_pozition_x > gem[i].x && player_hitbox_pozition_x < gem[i].x + 48)))
				{
					gem[i].is_taking = true;
				}
			}

			// подбор гема снизу
			for (int i = 0; i < gem_count; i++)
			{
				if (player_hitbox_pozition_y + 40 == gem[i].y && ((player_hitbox_pozition_x <= gem[i].x && player_hitbox_pozition_x + 48 > gem[i].x) || (player_hitbox_pozition_x > gem[i].x && player_hitbox_pozition_x < gem[i].x + 48)))
				{
					gem[i].is_taking = true;
				}
			}

			// логика анимации исчезновения гема
			for (int i = 0; i < gem_count; i++)
			{
				if (gem[i].is_taking == true)
				{
					if (gem[i].timer != 0)
						gem[i].timer--;
					if (gem[i].timer == 0)
					{
						gem[i].y = 99999999;
						gem[i].is_stay_on_the_floor = true;
					}
				}
			}

			for (int i = 0; i < gem_count; i++)
			{
				if (gem[i].is_taking == true)
				{
					score++;
				}
			}
			
			//printf("score: %d\n", score);

#pragma endregion

#pragma endregion

#pragma region rolling stone logic 

#pragma region rolling logic

			// скатывание камня  
			for (int i = 0; i < rolling_stone_count; i++)
			{
				if (rolling_stone[i].is_player_push == false)
				{
					if (rolling_stone[i].change_pozition < 48)
						rolling_stone[i].change_pozition += 1;
					if (rolling_stone[i].change_pozition >= 48)
						rolling_stone[i].change_pozition = 0;
				}
			}

#pragma endregion

#pragma region colision check for rolling stone & gem 

			// стнена слева от камня
			for (int i = 0; i <= wall_1_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x == wall_1[i].x + 48 && ((rolling_stone[j].y >= wall_1[i].y && rolling_stone[j].y < wall_1[i].y + 48) || (rolling_stone[j].y < wall_1[i].y && rolling_stone[j].y + 48 > wall_1[i].y)))
						rolling_stone[j].can_move_left = false;
				}

			// стнена справа от камня
			for (int i = 0; i <= wall_1_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x + 48 == wall_1[i].x && ((rolling_stone[j].y >= wall_1[i].y && rolling_stone[j].y < wall_1[i].y + 48) || (rolling_stone[j].y < wall_1[i].y && rolling_stone[j].y + 48 > wall_1[i].y)))
						rolling_stone[j].can_move_right = false;
				}

			// стнена снизу от камня
			for (int i = 0; i <= wall_1_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].y + 48 == wall_1[i].y && ((rolling_stone[j].x <= wall_1[i].x && rolling_stone[j].x + 48 > wall_1[i].x) || (rolling_stone[j].x > wall_1[i].x && rolling_stone[j].x < wall_1[i].x + 48)))
						rolling_stone[j].is_stay_on_the_floor = true;
				}


			// листва слева от камня
			for (int i = 0; i <= leaf_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x == leaf[i].x + 48 && ((rolling_stone[j].y >= leaf[i].y && rolling_stone[j].y < leaf[i].y + 48) || (rolling_stone[j].y < leaf[i].y && rolling_stone[j].y + 48 > leaf[i].y)))
						rolling_stone[j].can_move_left = false;
				}

			// листва справа от камня
			for (int i = 0; i <= leaf_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x + 48 == leaf[i].x && ((rolling_stone[j].y >= leaf[i].y && rolling_stone[j].y < leaf[i].y + 48) || (rolling_stone[j].y < leaf[i].y && rolling_stone[j].y + 48 > leaf[i].y)))
						rolling_stone[j].can_move_right = false;
				}

			// листва снизу от камня
			for (int i = 0; i <= leaf_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].y + 48 == leaf[i].y && ((rolling_stone[j].x <= leaf[i].x && rolling_stone[j].x + 48 > leaf[i].x) || (rolling_stone[j].x > leaf[i].x && rolling_stone[j].x < leaf[i].x + 48)))
						rolling_stone[j].is_stay_on_the_floor = true;
				}

			// камень слева от камня
			for (int i = 0; i < rolling_stone_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x == rolling_stone[i].x + 48 && ((rolling_stone[j].y >= rolling_stone[i].y && rolling_stone[j].y < rolling_stone[i].y + 48) || (rolling_stone[j].y < rolling_stone[i].y && rolling_stone[j].y + 48 > rolling_stone[i].y)))
						rolling_stone[j].can_move_left = false;
				}

			// камень справа от камня
			for (int i = 0; i < rolling_stone_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x + 48 == rolling_stone[i].x && ((rolling_stone[j].y >= rolling_stone[i].y && rolling_stone[j].y < rolling_stone[i].y + 48) || (rolling_stone[j].y < rolling_stone[i].y && rolling_stone[j].y + 48 > rolling_stone[i].y)))
						rolling_stone[j].can_move_right = false;
				}

			// дверь3 слева от камня
			for (int i = 0; i < door_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x == door[i].door_3_x + 48 && ((rolling_stone[j].y >= door[i].door_3_y && rolling_stone[j].y < door[i].door_3_y + 48) || (rolling_stone[j].y < door[i].door_3_y && rolling_stone[j].y + 48 > door[i].door_3_y)))
						rolling_stone[j].can_move_left = false;
				}

			// дверь3 справа от камня
			for (int i = 0; i < door_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x + 48 == door[i].door_3_x && ((rolling_stone[j].y >= door[i].door_3_y && rolling_stone[j].y < door[i].door_3_y + 48) || (rolling_stone[j].y < door[i].door_3_y && rolling_stone[j].y + 48 > door[i].door_3_y)))
						rolling_stone[j].can_move_right = false;
				}

			// дверь1 слева от камня
			for (int i = 0; i < door_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x == door[i].door_1_x + 48 && ((rolling_stone[j].y >= door[i].door_1_y && rolling_stone[j].y < door[i].door_1_y + 48) || (rolling_stone[j].y < door[i].door_1_y && rolling_stone[j].y + 48 > door[i].door_1_y)))
						rolling_stone[j].can_move_left = false;
				}

			// дверь1 справа от камня
			for (int i = 0; i < door_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x + 48 == door[i].door_1_y && ((rolling_stone[j].y >= door[i].door_1_y && rolling_stone[j].y < door[i].door_1_y + 48) || (rolling_stone[j].y < door[i].door_1_y && rolling_stone[j].y + 48 > door[i].door_1_y)))
						rolling_stone[j].can_move_right = false;
				}

			// гем слева от камня
			for (int i = 0; i <= gem_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x == gem[i].x + 48 && ((rolling_stone[j].y >= gem[i].y && rolling_stone[j].y < gem[i].y + 48) || (rolling_stone[j].y < gem[i].y && rolling_stone[j].y + 48 > gem[i].y)))
						rolling_stone[j].can_move_left = false;
				}

			// гем справа от камня
			for (int i = 0; i <= gem_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].x + 48 == gem[i].x && ((rolling_stone[j].y >= gem[i].y && rolling_stone[j].y < gem[i].y + 48) || (rolling_stone[j].y < gem[i].y && rolling_stone[j].y + 48 > gem[i].y)))
						rolling_stone[j].can_move_right = false;
				}

			// игрок снизу от камня
			for (int j = 0; j < rolling_stone_count; j++)
			{
				if (rolling_stone[j].y + 48 == player_hitbox_pozition_y && ((rolling_stone[j].x <= player_hitbox_pozition_x && rolling_stone[j].x + 48 > player_hitbox_pozition_x) || (rolling_stone[j].x > player_hitbox_pozition_x && rolling_stone[j].x < player_hitbox_pozition_x + 48)))
					rolling_stone[j].is_stay_on_the_floor = true;
			}

			// костыль
			for (int j = 0; j < rolling_stone_count; j++)
			{
				if (((rolling_stone[j].y + 48 >= player_hitbox_pozition_y) && ((rolling_stone[j].y <= player_hitbox_pozition_y + 48)) && ((rolling_stone[j].x <= player_hitbox_pozition_x && rolling_stone[j].x + 48 > player_hitbox_pozition_x) || (rolling_stone[j].x > player_hitbox_pozition_x && rolling_stone[j].x < player_hitbox_pozition_x + 48))))
					rolling_stone[j].is_stay_on_the_floor = true;
			}

			// игрок слева от камня
			for (int j = 0; j < rolling_stone_count; j++)
			{
				if (rolling_stone[j].x == player_hitbox_pozition_x + 48 && ((rolling_stone[j].y >= player_hitbox_pozition_y && rolling_stone[j].y < player_hitbox_pozition_y + 48) || (rolling_stone[j].y < player_hitbox_pozition_y && rolling_stone[j].y + 48 > player_hitbox_pozition_y)))
					rolling_stone[j].can_move_left = false;
			}

			// игрок справа от камня
			for (int j = 0; j < rolling_stone_count; j++)
			{
				if (rolling_stone[j].x + 48 == player_hitbox_pozition_x && ((rolling_stone[j].y >= player_hitbox_pozition_y && rolling_stone[j].y < player_hitbox_pozition_y + 48) || (rolling_stone[j].y < player_hitbox_pozition_y && rolling_stone[j].y + 48 > player_hitbox_pozition_y)))
					rolling_stone[j].can_move_right = false;
			}

			// смерть змеи
			for (int i = 0; i <= snak_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (((rolling_stone[j].y + 48 >= snak[i].y) && (rolling_stone[j].y + 48 <= snak[i].y + 48)) && ((rolling_stone[j].x <= snak[i].x && rolling_stone[j].x + 48 > snak[i].x) || (rolling_stone[j].x > snak[i].x && rolling_stone[j].x < snak[i].x + 48)))
					{
						//printf("true");
						snak[i].is_dead = true;
					}
				}

			// камень снизу от камня
			for (int i = 0; i < rolling_stone_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].y + 48 == rolling_stone[i].y && ((rolling_stone[j].x <= rolling_stone[i].x && rolling_stone[j].x + 48 > rolling_stone[i].x) || (rolling_stone[j].x > rolling_stone[i].x && rolling_stone[j].x < rolling_stone[i].x + 48)))
					{
						rolling_stone[j].is_stay_on_the_floor = true;
						if (rolling_stone[j].can_move_right == true && rolling_stone[i].can_move_right == true && rolling_stone[i].can_move_left == false)
						{
							rolling_stone[j].x = rolling_stone[j].x + 2;
							rolling_stone[j].is_moveing_right = true;
							//printf("44444444444444");
							rolling_stone[j].is_katitsa = true;
						}
						if (rolling_stone[j].can_move_left == true && rolling_stone[i].can_move_left == true)
						{
							rolling_stone[j].x = rolling_stone[j].x - 2;
							rolling_stone[j].is_moveing_left = true;
							//printf("44444444444444");
							rolling_stone[j].is_katitsa = true;

						}
					}
				}

			// камень на геме
			for (int i = 0; i < gem_count; i++)
				for (int j = 0; j < rolling_stone_count; j++)
				{
					if (rolling_stone[j].y + 48 == gem[i].y && ((rolling_stone[j].x <= gem[i].x && rolling_stone[j].x + 48 > gem[i].x) || (rolling_stone[j].x > gem[i].x && rolling_stone[j].x < gem[i].x + 48)))
					{

						rolling_stone[j].is_stay_on_the_floor = true;
						if (rolling_stone[j].can_move_right == true && gem[i].can_move_right == true && gem[i].can_move_left == false)
						{
							rolling_stone[j].x = rolling_stone[j].x + 2;
						}
						if (rolling_stone[j].can_move_left == true && gem[i].can_move_left == true)
							rolling_stone[j].x = rolling_stone[j].x - 2;

					}
				}

			// гем на камень
			for (int i = 0; i < rolling_stone_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].y + 48 == rolling_stone[i].y && ((gem[j].x <= rolling_stone[i].x && gem[j].x + 48 > rolling_stone[i].x) || (gem[j].x > rolling_stone[i].x && gem[j].x < rolling_stone[i].x + 48)))
					{
						gem[j].is_stay_on_the_floor = true;
						if (gem[j].can_move_right == true && rolling_stone[i].can_move_right == true && rolling_stone[i].can_move_left == false)
						{
							gem[j].x = gem[j].x + 2;
						}
						if (gem[j].can_move_left == true && rolling_stone[i].can_move_left == true)
							gem[j].x = gem[j].x - 2;
					}
				}

			// гем на геме
			for (int i = 0; i < gem_count; i++)
				for (int j = 0; j < gem_count; j++)
				{
					if (gem[j].y + 48 == gem[i].y && ((gem[j].x <= gem[i].x && gem[j].x + 48 > gem[i].x) || (gem[j].x > gem[i].x && gem[j].x < gem[i].x + 48)))
					{

						gem[j].is_stay_on_the_floor = true;
						if (gem[j].can_move_right == true && gem[i].can_move_right == true && gem[i].can_move_left == false)
						{
							gem[j].x = gem[j].x + 2;
						}
						if (gem[j].can_move_left == true && gem[i].can_move_left == true)
							gem[j].x = gem[j].x - 2;
					}
				}

			for (int i = 0; i < rolling_stone_count; i++)
				if (rolling_stone[i].is_stay_on_the_floor == false)
				{
					rolling_stone[i].can_move_left = false;
					rolling_stone[i].can_move_right = false;
				}

			// падение гема вниз
			for (int i = 0; i < gem_count; i++)
				if (gem[i].is_stay_on_the_floor == false)
					gem[i].y += 4;

			for (int i = 0; i < gem_count; i++)
				if (gem[i].is_taking == true)
					score++;
			//printf("%d\n", score);

#pragma endregion

#pragma region rolling stone change pozition

		// падение камня вниз
			for (int i = 0; i < rolling_stone_count; i++)
				if (rolling_stone[i].is_stay_on_the_floor == false)
					rolling_stone[i].y += 4;

			// толкание каменя справа от геоя
			for (int i = 0; i < rolling_stone_count; i++)
			{
				if (rolling_stone[i].is_katitsa == false)
				if (rolling_stone[i].can_move_right == true && (isDPresed == true || dovodchik_is_activated))
					if (player_hitbox_pozition_x + 48 == rolling_stone[i].x && ((player_hitbox_pozition_y >= rolling_stone[i].y && player_hitbox_pozition_y < rolling_stone[i].y + 48) || (player_hitbox_pozition_y < rolling_stone[i].y && player_hitbox_pozition_y + 48 > rolling_stone[i].y)))
					{
						rolling_stone[i].x += 4;
						rolling_stone[i].is_moveing_right = true;
						rolling_stone[i].rotation_pozition = frame_for_eight;
						rolling_stone[i].last_direction = 0;
						rolling_stone[i].is_player_push = true;
					}
			}

			// толкания каменя слева от геоя
			for (int i = 0; i < rolling_stone_count; i++)
			{
				if (rolling_stone[i].is_katitsa == false)
				if (rolling_stone[i].can_move_left == true && (isAPresed == true || dovodchik_is_activated))
					if (player_hitbox_pozition_x == rolling_stone[i].x + 48 && ((player_hitbox_pozition_y >= rolling_stone[i].y && player_hitbox_pozition_y < rolling_stone[i].y + 48) || (player_hitbox_pozition_y < rolling_stone[i].y && player_hitbox_pozition_y + 48 > rolling_stone[i].y)))
					{
						rolling_stone[i].x -= 4;
						rolling_stone[i].is_moveing_left = true;
						rolling_stone[i].rotation_pozition = frame_for_eight;
						rolling_stone[i].last_direction = 1;
						rolling_stone[i].is_player_push = true;
					}
			}

			// для анимации
			for (int i = 0; i < rolling_stone_count; i++)
				if (rolling_stone[i].is_moveing_left == true || rolling_stone[i].is_moveing_right == true)
					chek_is_any_rolling_stone_moving = true;

#pragma endregion

#pragma region player & rolling stone colision

			// упирание героя в камень справа

			for (int i = 0; i < rolling_stone_count; i++)
			{
				if (rolling_stone[i].can_move_right == false || rolling_stone[i].is_katitsa == true)
					if (player_hitbox_pozition_x + 48 /* может нужно добавить -4*/ == rolling_stone[i].x && ((player_hitbox_pozition_y >= rolling_stone[i].y && player_hitbox_pozition_y < rolling_stone[i].y + 48) || (player_hitbox_pozition_y < rolling_stone[i].y && player_hitbox_pozition_y + 48 > rolling_stone[i].y)))
					{
						player_can_move_x_right = false;
					}
			}

			// упирание героя в камень слева

			for (int i = 0; i <= rolling_stone_count; i++)
			{
				if (rolling_stone[i].can_move_left == false || rolling_stone[i].is_katitsa == true)
					if (player_hitbox_pozition_x == rolling_stone[i].x + 48 /* может нужно добавить -4*/ && ((player_hitbox_pozition_y >= rolling_stone[i].y && player_hitbox_pozition_y < rolling_stone[i].y + 48) || (player_hitbox_pozition_y < rolling_stone[i].y && player_hitbox_pozition_y + 48 > rolling_stone[i].y)))
					{
						player_can_move_x_left = false;
					}
			}

			// упирание героя в камень сверху
			for (int i = 0; i <= rolling_stone_count; i++)
			{
				if (player_hitbox_pozition_y == rolling_stone[i].y + 48 && ((player_hitbox_pozition_x <= rolling_stone[i].x && player_hitbox_pozition_x + 48 > rolling_stone[i].x) || (player_hitbox_pozition_x > rolling_stone[i].x && player_hitbox_pozition_x < rolling_stone[i].x + 48)))
					player_can_move_y_up = false;
			}
			
			// костыль
			for (int i = 0; i <= rolling_stone_count; i++)
			{
				if (((player_hitbox_pozition_y <= rolling_stone[i].y + 48) && (player_hitbox_pozition_y >= rolling_stone[i].y)) && ((player_hitbox_pozition_x <= rolling_stone[i].x && player_hitbox_pozition_x + 48 > rolling_stone[i].x) || (player_hitbox_pozition_x > rolling_stone[i].x && player_hitbox_pozition_x < rolling_stone[i].x + 48)))
					player_can_move_y_up = false;
			}

			// упирание героя в камень снизу
			for (int i = 0; i <= rolling_stone_count; i++)
			{
				if (player_hitbox_pozition_y + 48 == rolling_stone[i].y && ((player_hitbox_pozition_x <= rolling_stone[i].x && player_hitbox_pozition_x + 48 > rolling_stone[i].x) || (player_hitbox_pozition_x > rolling_stone[i].x && player_hitbox_pozition_x < rolling_stone[i].x + 48)))
					player_can_move_y_down = false;
			}

#pragma endregion

#pragma endregion

#pragma region exit logic

			// выход слева
			for (int i = 0; i < exit_count; i++)
			{
				if (player_hitbox_pozition_x == exit1[i].x + 40 && ((player_hitbox_pozition_y >= exit1[i].y && player_hitbox_pozition_y < exit1[i].y + 48) || (player_hitbox_pozition_y < exit1[i].y && player_hitbox_pozition_y + 48 > exit1[i].y)))
				{
					//exit1[i].is_win = true;
					isLevelRunning = false;
				}
			}

			// выход справа
			for (int i = 0; i < exit_count; i++)
			{
				if (player_hitbox_pozition_x + 40 == exit1[i].x && ((player_hitbox_pozition_y >= exit1[i].y && player_hitbox_pozition_y < exit1[i].y + 48) || (player_hitbox_pozition_y < exit1[i].y && player_hitbox_pozition_y + 48 > exit1[i].y)))
				{
					//exit1[i].is_win = true;
					isLevelRunning = false;
				}
			}

			// выход сверху
			for (int i = 0; i < exit_count; i++)
			{
				if (player_hitbox_pozition_y == exit1[i].y + 40 && ((player_hitbox_pozition_x <= exit1[i].x && player_hitbox_pozition_x + 48 > exit1[i].x) || (player_hitbox_pozition_x > exit1[i].x && player_hitbox_pozition_x < exit1[i].x + 48)))
				{
					//exit1[i].is_win = true;
					isLevelRunning = false;
				}
			}

			// выход снизу
			for (int i = 0; i < exit_count; i++)
			{
				if (player_hitbox_pozition_y + 40 == exit1[i].y && ((player_hitbox_pozition_x <= exit1[i].x && player_hitbox_pozition_x + 48 > exit1[i].x) || (player_hitbox_pozition_x > exit1[i].x && player_hitbox_pozition_x < exit1[i].x + 48)))
				{
					//exit1[i].is_win = true;
					isLevelRunning = false;
				}
			}

#pragma endregion

#pragma region button logic

			// игрок наступает на кнопку
			for (int i = 0; i < button_count; i++)
			{
				if (((player_hitbox_pozition_x + 48 > button[i].x && player_hitbox_pozition_x + 48 <= button[i].x + 48) || (player_hitbox_pozition_x < button[i].x + 48 && player_hitbox_pozition_x >= button[i].x)) && (player_hitbox_pozition_y + 48 >= button[i].y + 24 && player_hitbox_pozition_y + 48 <= button[i].y + 48))
				{
					button[i].is_presed = true;
				}
			}

			// каменень на кнопке
			for (int i = 0; i <= rolling_stone_count; i++)
				for (int j = 0; j < button_count; j++)
				{
					if (((rolling_stone[i].x + 48 > button[j].x && rolling_stone[i].x + 48 <= button[j].x + 48) || (rolling_stone[j].x < button[j].x + 48 && rolling_stone[j].x >= button[j].x)) && (rolling_stone[i].y + 48 >= button[j].y + 24 && rolling_stone[i].y + 48 <= button[j].y + 48))
					{
						button[j].is_presed = true;
					}
				}

			for (int i = 0; i < button_count; i++)
			{
				button[i].texture_y = button[i].y + button[i].change_y;

				if (button[i].is_presed == true && button[i].change_y <= 10)
				{
					button[i].change_y++;
				}

				if (button[i].is_presed == false && button[i].change_y > 0)
				{
					button[i].change_y--;
				}
			}


#pragma endregion

#pragma region snake logic

					// стнена слева от змеи
					for (int i = 0; i <= wall_1_count; i++)
						for (int j = 0; j < snak_count; j++)
							if(snak[j].direction == 1 || snak[j].direction == -1)
							{
								if (snak[j].x == wall_1[i].x + 48 && ((snak[j].y >= wall_1[i].y && snak[j].y < wall_1[i].y + 48) || (snak[j].y < wall_1[i].y && snak[j].y + 48 > wall_1[i].y)))
									snak[j].change_direction = true;
							}

					// стнена справа от змеи
					for (int i = 0; i <= wall_1_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 1 || snak[j].direction == -1)
							{
								if (snak[j].x + 48 == wall_1[i].x && ((snak[j].y >= wall_1[i].y && snak[j].y < wall_1[i].y + 48) || (snak[j].y < wall_1[i].y && snak[j].y + 48 > wall_1[i].y)))
									snak[j].change_direction = true;
							}
				
					// стнена снизу от змеи
					for (int i = 0; i <= wall_1_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 2 || snak[j].direction == -2)
							{
								if (snak[j].y + 48 == wall_1[i].y && ((snak[j].x <= wall_1[i].x && snak[j].x + 48 > wall_1[i].x) || (snak[j].x > wall_1[i].x && snak[j].x < wall_1[i].x + 48)))
									snak[j].change_direction = true;
							}

					// стнена сверху от змеи
					for (int i = 0; i <= wall_1_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 2 || snak[j].direction == -2)
							{
								if (snak[j].y == wall_1[i].y + 48 && ((snak[j].x <= wall_1[i].x && snak[j].x + 48 > wall_1[i].x) || (snak[j].x > wall_1[i].x && snak[j].x < wall_1[i].x + 48)))
									snak[j].change_direction = true;
							}
				
			

					// листва слева от змеи
					for (int i = 0; i <= leaf_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 1 || snak[j].direction == -1)
							{
								if (snak[j].x == leaf[i].x + 48 && ((snak[j].y >= leaf[i].y && snak[j].y < leaf[i].y + 48) || (snak[j].y < leaf[i].y && snak[j].y + 48 > leaf[i].y)))
									snak[j].change_direction = true;
							}

					// листва справа от змеи
					for (int i = 0; i <= leaf_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 1 || snak[j].direction == -1)
							{
								if (snak[j].x + 48 == leaf[i].x && ((snak[j].y >= leaf[i].y && snak[j].y < leaf[i].y + 48) || (snak[j].y < leaf[i].y && snak[j].y + 48 > leaf[i].y)))
									snak[j].change_direction = true;
							}

					// листва снизу от змеи
					for (int i = 0; i <= leaf_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 2 || snak[j].direction == -2)
							{
								if (snak[j].y + 48 == leaf[i].y && ((snak[j].x <= leaf[i].x && snak[j].x + 48 > leaf[i].x) || (snak[j].x > leaf[i].x && snak[j].x < leaf[i].x + 48)))
									snak[j].change_direction = true;
							}

					// листва сверху от змеи
					for (int i = 0; i <= leaf_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 2 || snak[j].direction == -2)
							{
								if (snak[j].y == leaf[i].y + 48 && ((snak[j].x <= leaf[i].x && snak[j].x + 48 > leaf[i].x) || (snak[j].x > leaf[i].x && snak[j].x < leaf[i].x + 48)))
									snak[j].change_direction = true;
							}

					// камень слева от змеи
					for (int i = 0; i <= rolling_stone_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 1 || snak[j].direction == -1)
							{
								if (snak[j].x == rolling_stone[i].x + 48 && ((snak[j].y >= rolling_stone[i].y && snak[j].y < rolling_stone[i].y + 48) || (snak[j].y < rolling_stone[i].y && snak[j].y + 48 > rolling_stone[i].y)))
									snak[j].change_direction = true;
							}

					// камень справа от змеи
					for (int i = 0; i <= rolling_stone_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 1 || snak[j].direction == -1)
							{
								if (snak[j].x + 48 == rolling_stone[i].x && ((snak[j].y >= rolling_stone[i].y && snak[j].y < rolling_stone[i].y + 48) || (snak[j].y < rolling_stone[i].y && snak[j].y + 48 > rolling_stone[i].y)))
									snak[j].change_direction = true;
							}

					// камень снизу от змеи
					for (int i = 0; i <= rolling_stone_count; i++)
						for (int j = 0; j < snak_count; j++)
							if (snak[j].direction == 2 || snak[j].direction == -2)
							{
								if (snak[j].y + 48 == rolling_stone[i].y && ((snak[j].x <= rolling_stone[i].x && snak[j].x + 48 > rolling_stone[i].x) || (snak[j].x > rolling_stone[i].x && snak[j].x < rolling_stone[i].x + 48)))
									snak[j].change_direction = true;
							}

		
			// движение змеи
			for (int i = 0; i < snak_count; i++)
			{
				if (snak[i].is_dead == false)
				{
					if (snak[i].change_direction == true)
						snak[i].direction = snak[i].direction * -1;

					if (snak[i].direction == 1)
						snak[i].x += 2;

					if (snak[i].direction == -1)
						snak[i].x += -2;

					if (snak[i].direction == 2)
						snak[i].y += 2;

					if (snak[i].direction == -2)
						snak[i].y += -2;
				}
				if (snak[i].is_dead == true)
				{
					snak[i].y -= 2;
				}
						
				//if (snak[i].direction = 2)
					//snak[i].y+=4;
			}

			for (int i = 0; i < snak_count; i++)
			{
				if (snak[i].is_dead == true)
				{
					if (snak[i].timer != 0)
						snak[i].timer--;
					if (snak[i].timer == 0)
					{
						snak[i].y = 99999999;
					}
				}
			}

			// смерть от снека
			for (int i = 0; i < snak_count; i++)
				if(snak[i].is_dead == false)
				{
					if (player_hitbox_pozition_x == snak[i].x + 40 && ((player_hitbox_pozition_y >= snak[i].y && player_hitbox_pozition_y < snak[i].y + 48) || (player_hitbox_pozition_y < snak[i].y && player_hitbox_pozition_y + 48 > snak[i].y)))
					{
						player_is_dead = true;
					}
				}

			// смерть от снека
			for (int i = 0; i < snak_count; i++)
				if (snak[i].is_dead == false)
				{
					if (player_hitbox_pozition_x + 40 == snak[i].x && ((player_hitbox_pozition_y >= snak[i].y && player_hitbox_pozition_y < snak[i].y + 48) || (player_hitbox_pozition_y < snak[i].y && player_hitbox_pozition_y + 48 > snak[i].y)))
					{
						player_is_dead = true;
					}
				}

			// смерть от снека
			for (int i = 0; i < snak_count; i++)
				if (snak[i].is_dead == false)
				{
					if (player_hitbox_pozition_y == snak[i].y + 40 && ((player_hitbox_pozition_x <= snak[i].x && player_hitbox_pozition_x + 48 > snak[i].x) || (player_hitbox_pozition_x > snak[i].x && player_hitbox_pozition_x < snak[i].x + 48)))
					{
						player_is_dead = true;
					}
				}

			// смерть от снека
			for (int i = 0; i < snak_count; i++)
				if (snak[i].is_dead == false)
			 	{
					if (player_hitbox_pozition_y + 40 == snak[i].y && ((player_hitbox_pozition_x <= snak[i].x && player_hitbox_pozition_x + 48 > snak[i].x) || (player_hitbox_pozition_x > snak[i].x && player_hitbox_pozition_x < snak[i].x + 48)))
					{
						player_is_dead = true;
					}
				}

			if (player_is_dead == true)
			{
				if (player_timer != 0)
					player_timer--;
				if (player_timer == 0)
				{
					player_pozition_y = 99999999;
					you_died_is_running = true;
					isLevelRunning = false;
				}
			}

#pragma endregion

#pragma region door logic

			for (int i = 0; i < door_count; i++)
				door[i].texture_y = door[i].door_3_y + door[i].change_pozition_y;

			for (int i = 0; i < button_count; i++)
			{
				if (door[i].type == 1)
				{
					if (button[i].is_presed == true && door[i].change_pozition_y >= -48)
					{
						door[i].change_pozition_y -= 2;
					}

					if (button[i].is_presed == false && door[i].change_pozition_y < 0)
					{
						door[i].change_pozition_y += 2;
					}
				}
			}

			for (int i = 0; i < door_count; i++)
			{
				if (door[i].type == 2)
				{ 
					
					if (door[i].is_challenge_complit == false)
					{
						if (door[i].is_trigger_activeted == false && door[i].change_pozition_y >= -48)
						{
							door[i].change_pozition_y -= 2;
						}
						if (door[i].is_trigger_activeted == true && door[i].change_pozition_y < 0)
						{
							door[i].change_pozition_y += 2;
						}
					}
					if (door[i].is_challenge_complit == true)
					{
						if (door[i].change_pozition_y >= -48)
							door[i].change_pozition_y -= 2;
					}
					


					// выход слева
					for (int i = 0; i < door_count; i++)
					{
						if (player_hitbox_pozition_x == door[i].trigger_x + 40 && ((player_hitbox_pozition_y >= door[i].trigger_y && player_hitbox_pozition_y < door[i].trigger_y + 48) || (player_hitbox_pozition_y < door[i].trigger_y && player_hitbox_pozition_y + 48 > door[i].trigger_y)))
						{
							door[i].is_trigger_activeted = true;
							door[i].is_challenge_running = true;
							door[i+1].is_trigger_activeted = true;
							door[i+1].is_challenge_running = true;
							//printf("31311241241\n");
						}
						if (player_hitbox_pozition_x + 40 == door[i].trigger_x && ((player_hitbox_pozition_y >= door[i].trigger_y && player_hitbox_pozition_y < door[i].trigger_y + 48) || (player_hitbox_pozition_y < door[i].trigger_y && player_hitbox_pozition_y + 48 > door[i].trigger_y)))
						{
							door[i].is_trigger_activeted = true;
							door[i].is_challenge_running = true;
							door[i + 1].is_trigger_activeted = true;
							door[i + 1].is_challenge_running = true;
							//printf("31311241241\n");
						}
						if (player_hitbox_pozition_y == door[i].trigger_y + 40 && ((player_hitbox_pozition_x <= door[i].trigger_x && player_hitbox_pozition_x + 48 > door[i].trigger_x) || (player_hitbox_pozition_x > door[i].trigger_x && player_hitbox_pozition_x < door[i].trigger_x + 48)))
						{
							door[i].is_trigger_activeted = true;
							door[i].is_challenge_running = true;
							door[i + 1].is_trigger_activeted = true;
							door[i + 1].is_challenge_running = true;
							//printf("31311241241\n");
						}
						if (player_hitbox_pozition_y + 40 == door[i].trigger_y && ((player_hitbox_pozition_x <= door[i].trigger_x && player_hitbox_pozition_x + 48 > door[i].trigger_x) || (player_hitbox_pozition_x > door[i].trigger_x && player_hitbox_pozition_x < door[i].trigger_x + 48)))
						{
							door[i].is_trigger_activeted = true;
							door[i].is_challenge_running = true;
							door[i + 1].is_trigger_activeted = true;
							door[i + 1].is_challenge_running = true;	
							//printf("31311241241\n");
						}
					}
					

				

					




				}
			}


			for (int i = 0; i < door_count; i++)
			{
				// дверь слева от геоя
				if (player_hitbox_pozition_x == door[i].door_1_x + 48 && ((player_hitbox_pozition_y >= door[i].door_1_y && player_hitbox_pozition_y < door[i].door_1_y + 48) || (player_hitbox_pozition_y < door[i].door_1_y && player_hitbox_pozition_y + 48 > door[i].door_1_y)))
					player_can_move_x_left = false;

				// дверь справа от геоя
				if (player_hitbox_pozition_x + 48 == door[i].door_1_x && ((player_hitbox_pozition_y >= door[i].door_1_y && player_hitbox_pozition_y < door[i].door_1_y + 48) || (player_hitbox_pozition_y < door[i].door_1_y && player_hitbox_pozition_y + 48 > door[i].door_1_y)))
					player_can_move_x_right = false;

				// дверь сверху от геоя
				if (player_hitbox_pozition_y == door[i].door_1_y + 48 && ((player_hitbox_pozition_x <= door[i].door_1_x && player_hitbox_pozition_x + 48 > door[i].door_1_x) || (player_hitbox_pozition_x > door[i].door_1_x && player_hitbox_pozition_x < door[i].door_1_x + 48)))
					player_can_move_y_up = false;
			}

			for (int i = 0; i < button_count; i++)
				if (button[i].is_presed == false)
				{
					// дверь слева от геоя
					if (player_hitbox_pozition_x == door[i].door_3_x + 48 && ((player_hitbox_pozition_y >= door[i].door_3_y && player_hitbox_pozition_y < door[i].door_3_y + 48) || (player_hitbox_pozition_y < door[i].door_3_y && player_hitbox_pozition_y + 48 > door[i].door_3_y)))
						player_can_move_x_left = false;

					// дверь справа от геоя
					if (player_hitbox_pozition_x + 48 == door[i].door_3_x && ((player_hitbox_pozition_y >= door[i].door_3_y && player_hitbox_pozition_y < door[i].door_3_y + 48) || (player_hitbox_pozition_y < door[i].door_3_y && player_hitbox_pozition_y + 48 > door[i].door_3_y)))
						player_can_move_x_right = false;

					// дверь сверху от геоя
					if (player_hitbox_pozition_y == door[i].door_3_y + 48 && ((player_hitbox_pozition_x <= door[i].door_3_x && player_hitbox_pozition_x + 48 > door[i].door_3_x) || (player_hitbox_pozition_x > door[i].door_3_x && player_hitbox_pozition_x < door[i].door_3_x + 48)))
						player_can_move_y_up = false;
				}

			for (int i = 0; i < door_count; i++)
			{
				if (door[i].is_challenge_complit == false)
				{
					if (door[i].is_challenge_running == true && door[i].type == 2)
					{
						// дверь слева от геоя
						if (player_hitbox_pozition_x == door[i].door_3_x + 48 && ((player_hitbox_pozition_y >= door[i].door_3_y && player_hitbox_pozition_y < door[i].door_3_y + 48) || (player_hitbox_pozition_y < door[i].door_3_y && player_hitbox_pozition_y + 48 > door[i].door_3_y)))
							player_can_move_x_left = false;

						// дверь справа от геоя
						if (player_hitbox_pozition_x + 48 == door[i].door_3_x && ((player_hitbox_pozition_y >= door[i].door_3_y && player_hitbox_pozition_y < door[i].door_3_y + 48) || (player_hitbox_pozition_y < door[i].door_3_y && player_hitbox_pozition_y + 48 > door[i].door_3_y)))
							player_can_move_x_right = false;
					}
				}
			}

			for (int i = 0; i < snak_count; i++)
			{
				if (snak[i].is_for_challenge == 1 && snak[i].is_dead == true)
				{
					for (int j = 0; j < door_count; j++)
					{
						if (door[j].type == 2)
						{
							door[j].is_challenge_complit = true;
							//printf("PRAVDA\n");
						}
					}
				}
			}

#pragma endregion

#pragma region leaf logic

			// лмства слева от геоя
			for (int i = 0; i <= leaf_count; i++)
			{
				if (player_hitbox_pozition_x == leaf[i].x + 40 && ((player_hitbox_pozition_y >= leaf[i].y && player_hitbox_pozition_y < leaf[i].y + 48) || (player_hitbox_pozition_y < leaf[i].y && player_hitbox_pozition_y + 48 > leaf[i].y)))
				{
					player_can_move_x_left = false;
					leaf[i].is_brake = true;
				}
			}

			// лмства справа от геоя
			for (int i = 0; i <= leaf_count; i++)
			{
				if (player_hitbox_pozition_x + 40 == leaf[i].x && ((player_hitbox_pozition_y >= leaf[i].y && player_hitbox_pozition_y < leaf[i].y + 48) || (player_hitbox_pozition_y < leaf[i].y && player_hitbox_pozition_y + 48 > leaf[i].y)))
				{
					player_can_move_x_right = false;
					leaf[i].is_brake = true;
				}
			}

			// лмства сверху от геоя
			for (int i = 0; i <= leaf_count; i++)
			{
				if (player_hitbox_pozition_y == leaf[i].y + 40 && ((player_hitbox_pozition_x <= leaf[i].x && player_hitbox_pozition_x + 48 > leaf[i].x) || (player_hitbox_pozition_x > leaf[i].x && player_hitbox_pozition_x < leaf[i].x + 48)))
				{
					player_can_move_y_up = false;
					leaf[i].is_brake = true;
				}
			}

			// лмства снизу от геоя
			for (int i = 0; i <= leaf_count; i++)
			{
				if (player_hitbox_pozition_y + 40
					== leaf[i].y && ((player_hitbox_pozition_x <= leaf[i].x && player_hitbox_pozition_x + 48 > leaf[i].x) || (player_hitbox_pozition_x > leaf[i].x && player_hitbox_pozition_x < leaf[i].x + 48)))
				{
					player_can_move_y_down = false;
					leaf[i].is_brake = true;
				}
			}

			// логика анимации исчезновения гема
			for (int i = 0; i < leaf_count; i++)
			{
				if (leaf[i].is_brake == true)
				{
					if (leaf[i].timer != 0)
						leaf[i].timer--;
					if (leaf[i].timer == 0)
					{
						leaf[i].y = 99999999;
					}
				}
			}

#pragma endregion

#pragma region processing click data

			if (dovodchik_is_activated_for_move == false)
			{
				// запрет на движение по диагонали
				if ((isSPresed && isDPresed) || (isSPresed && isAPresed))
				{
					player_can_move_y_down = false;
				}
				if ((isWPresed && isDPresed) || (isWPresed && isAPresed))
				{
					player_can_move_y_up = false;
				}

				// лево
				if (player_can_move_x_left == true)
				{
					if (isAPresed)
					{
						player_pozition_x -= 4;
						player_direction = 1;
					}
				}

				// право
				if (player_can_move_x_right == true)
				{
					if (isDPresed)
					{
						player_pozition_x += 4;
						player_direction = 0;
					}
				}

				// верх
				if (player_can_move_y_up == true)
				{
					if (isWPresed)
					{
						player_pozition_y -= 4;
						player_direction = 3;
					}
				}

				// низ
				if (player_can_move_y_down == true)
				{
					if (isSPresed)
					{
						player_pozition_y += 4;
						player_direction = 4;
					}
				}
			}

		

#pragma endregion		

#pragma region change frames

			

			// переменные для анимации камня из 8 кадров
			newtime_for_eight = SDL_GetTicks();
			dt_for_eight = newtime_for_eight - lasttime_for_eight;
			lasttime_for_eight = newtime_for_eight;

			cur_frametime_for_eight += dt_for_eight;

			// переменные для анимации из 4 кадров
			newtime = SDL_GetTicks(); // получение текущего времени
			dt = newtime - lasttime; // разница 
			lasttime = newtime; // обновление

			cur_frametime += dt;

			// смена 4 кадров
			if (cur_frametime >= max_frametime)
			{
				cur_frametime -= max_frametime;
				frame = (frame + 1) % frame_count;

				player_walk_rect.x = player_walk_rect.w * frame;
				player_stand_rect.x = player_stand_rect.w * frame;
				player_upping_rect.x = player_upping_rect.w * frame;
				player_side_climb_rect.x = player_side_climb_rect.w * frame;
				player_puch_rect.x = player_puch_rect.w * frame;
				leaf_braking_rect.x = leaf_braking_rect.w * frame;
				gem_taking_rect.x = gem_taking_rect.w * frame;
				snak_rect.x = snak_rect.w * frame;
				snak_up_rect.x = snak_up_rect.w * frame;
				snak_dead_rect.x = snak_dead_rect.w * frame;
				player_dead_rect.x = player_dead_rect.w * frame;
			}

			// смена 8 кадров для камня
			if (cur_frametime_for_eight >= max_frametime_for_eight)
			{
				cur_frametime_for_eight -= max_frametime_for_eight;
				frame_for_eight = (frame_for_eight + 1) % frame_for_eight_count;

				rolling_rolling_stone_rect.x = rolling_rolling_stone_rect.w * frame_for_eight;
			}

#pragma endregion

#pragma region rendering

			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

#pragma region background rendering

			// отрисовка фона из лиан
			for (int i = 0; i < width; i += 48)
				for (int j = 0; j < height; j += 48)
				{
					SDL_Rect leaves_hitbox_rect{ i, j, 24 * main_scale, 24 * main_scale };
					SDL_RenderCopy(renderer, leaves_texture, NULL, &leaves_hitbox_rect);
				}

#pragma endregion

#pragma region button rendering

			for (int i = 0; i < button_count; i++)
				SDL_RenderCopy(renderer, button_static_texture, &button_static_rect, &button[i].dsr_rect_button_static);

#pragma endregion

#pragma region wall rendering

			// отрисовка каменной стены
			for (int i = 0; i <= wall_1_count; i++)
			{
				SDL_Rect  sqre2 = { wall_1[i].x, wall_1[i].y, 48, 48 };
				SDL_RenderCopy(renderer, wall1_texture, NULL, &sqre2);
				//SDL_RenderDrawRect(renderer, &sqre2); отрисовка хитбокас стены
			}

#pragma endregion

#pragma region gem rendering

			for (int i = 0; i < gem_count; i++)
				if (gem[i].is_taking == false)
					SDL_RenderCopy(renderer, gem_texture, &gem_rect, &gem[i].dst_fading_rect);

			for (int i = 0; i < gem_count; i++)
				if (gem[i].timer > 0)
					if (gem[i].is_taking == true)
						SDL_RenderCopy(renderer, gem_taking_texture, &gem_taking_rect, &gem[i].dst_static_rect);

#pragma endregion

#pragma region exit rendering

			for (int i = 0; i < exit_count; i++)
				SDL_RenderCopy(renderer, exit_texture, &exit_rect, &exit1[i].dst_exit);

#pragma endregion

#pragma region snake rendering

			for (int i = 0; i <= snak_count; i++)
			{
				if (snak[i].is_dead == false)
				{
					if (snak[i].direction == -1)
						SDL_RenderCopy(renderer, snak_texture, &snak_rect, &snak[i].dst_snak_rect);
					if (snak[i].direction == 1)
						SDL_RenderCopyEx(renderer, snak_texture, &snak_rect, &snak[i].dst_snak_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (snak[i].direction == 2)
						SDL_RenderCopy(renderer, snak_up_texture, &snak_up_rect, &snak[i].dst_snak_up_rect);
					if (snak[i].direction == -2)
						SDL_RenderCopyEx(renderer, snak_up_texture, &snak_up_rect, &snak[i].dst_snak_up_rect, 0, NULL, SDL_FLIP_VERTICAL);
				}
				if (snak[i].is_dead == true)
					SDL_RenderCopy(renderer, snak_dead_texture, &snak_dead_rect, &snak[i].dst_snak_dead_rect);
			}
				
#pragma endregion

#pragma region rolling stone rendering

			for (int i = 0; i < rolling_stone_count; i++)
			{
				if (rolling_stone[i].last_direction == 0 && rolling_stone[i].is_moveing_right == false && rolling_stone[i].is_moveing_left == false)
				{
					if (rolling_stone[i].rotation_pozition == 0)
						SDL_RenderCopy(renderer, standing_rolling_stone_1_texture, &standing_rolling_stone_1, &rolling_stone[i].dst_rect);
					if (rolling_stone[i].rotation_pozition == 1)
						SDL_RenderCopy(renderer, standing_rolling_stone_2_texture, &standing_rolling_stone_2, &rolling_stone[i].dst_rect);
					if (rolling_stone[i].rotation_pozition == 2)
						SDL_RenderCopy(renderer, standing_rolling_stone_3_texture, &standing_rolling_stone_3, &rolling_stone[i].dst_rect);
					if (rolling_stone[i].rotation_pozition == 3)
						SDL_RenderCopy(renderer, standing_rolling_stone_4_texture, &standing_rolling_stone_4, &rolling_stone[i].dst_rect);
					if (rolling_stone[i].rotation_pozition == 4)
						SDL_RenderCopy(renderer, standing_rolling_stone_5_texture, &standing_rolling_stone_5, &rolling_stone[i].dst_rect);
					if (rolling_stone[i].rotation_pozition == 5)
						SDL_RenderCopy(renderer, standing_rolling_stone_6_texture, &standing_rolling_stone_6, &rolling_stone[i].dst_rect);
					if (rolling_stone[i].rotation_pozition == 6)
						SDL_RenderCopy(renderer, standing_rolling_stone_7_texture, &standing_rolling_stone_7, &rolling_stone[i].dst_rect);
					if (rolling_stone[i].rotation_pozition == 7)
						SDL_RenderCopy(renderer, standing_rolling_stone_8_texture, &standing_rolling_stone_8, &rolling_stone[i].dst_rect);
				}

				if (rolling_stone[i].last_direction == 1 && rolling_stone[i].is_moveing_right == false && rolling_stone[i].is_moveing_left == false)
				{
					if (rolling_stone[i].rotation_pozition == 0)
						SDL_RenderCopyEx(renderer, standing_rolling_stone_1_texture, &standing_rolling_stone_1, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (rolling_stone[i].rotation_pozition == 1)
						SDL_RenderCopyEx(renderer, standing_rolling_stone_2_texture, &standing_rolling_stone_2, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (rolling_stone[i].rotation_pozition == 2)
						SDL_RenderCopyEx(renderer, standing_rolling_stone_3_texture, &standing_rolling_stone_3, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (rolling_stone[i].rotation_pozition == 3)
						SDL_RenderCopyEx(renderer, standing_rolling_stone_4_texture, &standing_rolling_stone_4, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (rolling_stone[i].rotation_pozition == 4)
						SDL_RenderCopyEx(renderer, standing_rolling_stone_5_texture, &standing_rolling_stone_5, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (rolling_stone[i].rotation_pozition == 5)
						SDL_RenderCopyEx(renderer, standing_rolling_stone_6_texture, &standing_rolling_stone_6, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (rolling_stone[i].rotation_pozition == 6)
						SDL_RenderCopyEx(renderer, standing_rolling_stone_7_texture, &standing_rolling_stone_7, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (rolling_stone[i].rotation_pozition == 7)
						SDL_RenderCopyEx(renderer, standing_rolling_stone_8_texture, &standing_rolling_stone_8, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
				}
			}

			for (int i = 0; i < rolling_stone_count; i++)
			{
				// движение 1 камня вправо
				if (rolling_stone[i].is_moveing_right == true)
					SDL_RenderCopy(renderer, rolling_rolling_stone_texture, &rolling_rolling_stone_rect, &rolling_stone[i].dst_rect);
				// движение 1 камня влево
				if (rolling_stone[i].is_moveing_left == true)
					SDL_RenderCopyEx(renderer, rolling_rolling_stone_texture, &rolling_rolling_stone_rect, &rolling_stone[i].dst_rect, 0, NULL, SDL_FLIP_HORIZONTAL);

				SDL_Rect  rolling_stone_rect = { rolling_stone[i].x, rolling_stone[i].y, 48, 48 };
				//SDL_RenderDrawRect(renderer, &rolling_stone_rect);
			}
#pragma endregion

#pragma region door rendering

			for (int i = 0; i < door_count; i++)
			{
				SDL_RenderCopy(renderer, door_1_texture, &door_1_rect, &door[i].dst_door_1_rect);

				SDL_RenderCopy(renderer, door_3_texture, &door_3_rect, &door[i].dst_door_3_rect);

				SDL_RenderCopy(renderer, door_2_texture, &door_2_rect, &door[i].dst_door_2_rect);
			}

#pragma endregion

#pragma region player rendering

			if (player_is_dead == false)
			{
				// карапканье вниз
				if (isSPresed == true && isWPresed == false && isAPresed == false && isDPresed == false)
					SDL_RenderCopy(renderer, player_upping_texture, &player_upping_rect, &dst_rect_upping);

				// карапканье вверх
				if (isWPresed == true && isSPresed == false && isAPresed == false && isDPresed == false)
					SDL_RenderCopy(renderer, player_upping_texture, &player_upping_rect, &dst_rect_upping);

				// доводчик карапканья вверх
				if (animation_check == 3 && isWPresed == false && isSPresed == false && isAPresed == false && isDPresed == false)
					SDL_RenderCopy(renderer, player_upping_texture, &player_upping_rect, &dst_rect_upping);

				// доводчик карапканья вниз
				if (animation_check == 4 && isWPresed == false && isSPresed == false && isAPresed == false && isDPresed == false)
					SDL_RenderCopy(renderer, player_upping_texture, &player_upping_rect, &dst_rect_upping);

				// доводчик карапканья влево
				if (animation_check == 2 && isAPresed == false && isDPresed == false && is_player_stay_on_the_flor == false)
					SDL_RenderCopyEx(renderer, player_side_climb_texture, &player_side_climb_rect, &dst_rect_side_climd, 0, NULL, SDL_FLIP_HORIZONTAL);

				// доводчик карапканья вправо
				if (animation_check == 1 && isDPresed == false && isAPresed == false && is_player_stay_on_the_flor == false)
					SDL_RenderCopy(renderer, player_side_climb_texture, &player_side_climb_rect, &dst_rect_side_climd);

				// доводчик ходьбы влево
				if (animation_check == 2 && isAPresed == false && isDPresed == false && is_player_stay_on_the_flor == true)
					SDL_RenderCopyEx(renderer, player_walk_texture, &player_walk_rect, &dst_rect_walk, 0, NULL, SDL_FLIP_HORIZONTAL);

				// доводчик ходьбы вправо
				if (animation_check == 1 && isDPresed == false && isAPresed == false && is_player_stay_on_the_flor == true)
					SDL_RenderCopy(renderer, player_walk_texture, &player_walk_rect, &dst_rect_walk);

				// карапканье в сторону
				if (is_player_stay_on_the_flor == false)
				{
					if (isDPresed == true && isAPresed == false) // карапканье вправо
						SDL_RenderCopy(renderer, player_side_climb_texture, &player_side_climb_rect, &dst_rect_side_climd);

					if (isAPresed == true && isDPresed == false) // карапканье влево
						SDL_RenderCopyEx(renderer, player_side_climb_texture, &player_side_climb_rect, &dst_rect_side_climd, 0, NULL, SDL_FLIP_HORIZONTAL);
				}

				// ходьба в сорону
				for (int i = 0; i < rolling_stone_count; i++)
				{
					if (chek_is_any_rolling_stone_moving == false)
						if (is_player_stay_on_the_flor == true && rolling_stone[i].is_moveing_left == false && rolling_stone[i].is_moveing_right == false)
						{
							if (isDPresed == true && isAPresed == false) // ходьбв вправо
								SDL_RenderCopy(renderer, player_walk_texture, &player_walk_rect, &dst_rect_walk);

							if (isAPresed == true && isDPresed == false) // ходьба влево
								SDL_RenderCopyEx(renderer, player_walk_texture, &player_walk_rect, &dst_rect_walk, 0, NULL, SDL_FLIP_HORIZONTAL);
						}

					// ходьба и толкание камня
					if (is_player_stay_on_the_flor == true && rolling_stone[i].is_moveing_left == true || rolling_stone[i].is_moveing_right == true)
					{
						if (is_player_stay_on_the_flor == true && rolling_stone[i].is_moveing_right == true && isAPresed == false) // ходьбв вправо
							SDL_RenderCopy(renderer, player_push_texture, &player_puch_rect, &dst_rect_player_push);

						if (is_player_stay_on_the_flor == true && rolling_stone[i].is_moveing_left == true && isDPresed == false) // ходьба влево
							SDL_RenderCopyEx(renderer, player_push_texture, &player_puch_rect, &dst_rect_player_push, 0, NULL, SDL_FLIP_HORIZONTAL);
					}
				}

				// статическая анимация весения
				if (is_player_stay_on_the_flor == false && isDPresed == false && isAPresed == false && animation_check == 0 && isSPresed == false && isWPresed == false || isAPresed == true && isDPresed == true || isSPresed == true && isWPresed == true) // стояние наместе
				{
					if (player_direction == 0) // карапконье вправо
						SDL_RenderCopy(renderer, player_climb_stand_texture, &player_climb_stand_rect, &dst_rect_climb_stnd);

					if (player_direction == 1) // карапконье влево
						SDL_RenderCopyEx(renderer, player_climb_stand_texture, &player_climb_stand_rect, &dst_rect_climb_stnd, 0, NULL, SDL_FLIP_HORIZONTAL);
					if (player_direction == 3 || player_direction == 4) // весение и взгляд вверх
						SDL_RenderCopy(renderer, player_up_stand_texture, &player_up_stand_rect, &dst_rect_up_stand);
				}

				// статическая анимация стояния
				if (is_player_stay_on_the_flor == true && isDPresed == false && isAPresed == false && animation_check == 0 && isSPresed == false && isWPresed == false || isAPresed == true && isDPresed == true || isSPresed == true && isWPresed == true) // стояние наместе
				{
					if (player_direction == 0 || player_direction == 4 || player_direction == 3) // стояние и взгляд влево
						SDL_RenderCopy(renderer, player_stand_texture, &player_stand_rect, &dst_rect_walk);
					if (player_direction == 1) // стояние и взгляд вправо
						SDL_RenderCopyEx(renderer, player_stand_texture, &player_stand_rect, &dst_rect_walk, 0, NULL, SDL_FLIP_HORIZONTAL);
				}
			}

			if (player_is_dead == true)
				SDL_RenderCopy(renderer, player_dead_texture, &player_dead_rect, &dst_rect_player_is_dead);

			//SDL_RenderDrawRect(renderer, &player_hitbox_rect); // отрисока хитбокаса главного героя

#pragma endregion

#pragma region leaf rendering

			for (int i = 0; i < leaf_count; i++)
			{
				if (leaf[i].is_brake == false)
				{
					SDL_RenderCopy(renderer, static_leaf_texture, &static_leaf_rect, &leaf[i].dst_static_rect);
				}
				if (leaf[i].is_brake == true)
					SDL_RenderCopy(renderer, leaf_braking_texture, &leaf_braking_rect, &leaf[i].dst_break_rect);
			}

#pragma endregion

#pragma endregion

#pragma region change other data 

			player_hitbox_pozition_x = player_pozition_x;
			player_hitbox_pozition_y = player_pozition_y + 6;

			player_can_move_x_right = true;
			player_can_move_x_left = true;
			player_can_move_y_up = true;
			player_can_move_y_down = true;

			is_player_stay_on_the_flor = false;

			for (int i = 0; i < rolling_stone_count; i++)
			{
				rolling_stone[i].is_stay_on_the_floor = false;

				rolling_stone[i].can_move_left = true;
				rolling_stone[i].can_move_right = true;

				rolling_stone[i].is_moveing_right = false;
				rolling_stone[i].is_moveing_left = false;
				rolling_stone[i].is_player_push = false;
				rolling_stone[i].is_katitsa = false;
			}

			for (int i = 0; i < gem_count; i++)
			{
				gem[i].is_stay_on_the_floor = false;

				gem[i].can_move_left = true;
				gem[i].can_move_right = true;
			}

			for (int i = 0; i < button_count; i++)
				button[i].is_presed = false;

			dovodchik_is_activated = false;
			dovodchik_is_activated_for_move = false;
			chek_is_any_rolling_stone_moving = false;

			score = 0;

			for (int i = 0; i < snak_count; i++)
				snak[i].change_direction = false;

#pragma endregion

			SDL_RenderPresent(renderer);
			SDL_Delay(20);
		}

		while (you_died_is_running)
		{
			window_key_status();

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);


			SDL_Rect blak_rect = { 0, 0, 20 * 48, 20 * 48 };
			SDL_RenderFillRect(renderer, &blak_rect);
			SDL_RenderCopy(renderer, you_died_texture, &you_died_rect, &dst_you_died_rect);
			


			SDL_RenderPresent(renderer);
			SDL_Delay(2000);
			you_died_is_running = false;
			isLevelRunning = true;
			isMenuRunning = true;
		}
	}

#pragma region destroy texture

	/*SDL_DestroyTexture(texture_head);*/

#pragma endregion

	return 0;
}
