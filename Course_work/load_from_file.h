#include <SDL.h>

struct gem_coord
{
	int x;
	int y;

	bool is_taking = false;

	SDL_Rect dst_static_rect;
	SDL_Rect dst_fading_rect;

	int timer = 20;
	bool is_stay_on_the_floor;

	bool can_move_right;
	bool can_move_left;
};
struct snak_coord
{
	int x;
	int y;
	int direction;
	SDL_Rect dst_snak_rect;
	SDL_Rect dst_snak_up_rect;
	SDL_Rect dst_snak_dead_rect;
	bool change_direction = false;
	bool is_dead = false;
	int timer = 25;

	int is_for_challenge;
};
struct leaf_coord
{
	int x; // положение по оси Х
	int y; // положение по оси Y

	SDL_Rect dst_break_rect;
	SDL_Rect  dst_static_rect;

	bool is_brake = false; // состояние
	int chek = 0;
	int timer = 20; // таймер исчезнавения
};
struct door_coord
{
	int x; // положение по оси Х
	int y; // положение по оси Y

	// Открытие и закрытие двери
	int change_pozition_y = 0;
	int texture_y = 0;

	int type; // тип двери 
	int trigger_x; // положение тригера по оси Х
	int trigger_y; // положение тригера по оси Y
	bool is_trigger_activeted = false; // состояние тригера
	bool is_challenge_running = false; // состояние испытания
	bool is_challenge_complit = false; // состояние испытания

	// координаты 1 части двери
	int door_1_x;
	int door_1_y;

	// координаты 2 части двери
	int door_2_x;
	int door_2_y;

	// координаты 3 части двери
	int door_3_x;
	int door_3_y;

	SDL_Rect  dst_door_1_rect;
	SDL_Rect  dst_door_2_rect;
	SDL_Rect  dst_door_3_rect;
};
struct button_coord
{
	int x;
	int y;

	bool is_presed = false;

	// for movement
	int change_y = 0;
	int texture_y = 0;

	SDL_Rect dsr_rect_button_static;
};
struct wall_coord
{
	int x;
	int y;
};
struct rolling_stone_coord
{
	int x;
	int y;

	bool is_katitsa = false;

	bool can_move_left = true;
	bool can_move_right = true;
	bool is_stay_on_the_floor = false;

	bool is_moveing_left = false;
	bool is_moveing_right = false;

	int rotation_pozition = 0;
	int last_direction = 0;

	bool is_player_push = false;

	SDL_Rect dst_rect;

	int change_pozition;
};
struct exit_coord
{
	int x;
	int y;
	bool is_win = false;
	SDL_Rect dst_exit;
};
struct cursor_coord
{
	int x = 10 * 48;
	int y = 10 * 48;
	SDL_Rect dst_rect;
};

int input_from_file_gem_level_1(gem_coord a[]);
int input_from_file_wall1_level_1(wall_coord a[]);
int input_from_file_rolling_stone_level_1(rolling_stone_coord a[]);
int input_from_file_button_level_1(button_coord a[]);
int input_from_file_door_level_1(door_coord a[]);
int input_from_file_leaf_level_1(leaf_coord a[]);
int input_from_file_exit_level_1(exit_coord a[]);
int input_from_file_snake_level_1(snak_coord a[]);

int input_from_file_gem_level_2(gem_coord a[]);
int input_from_file_wall1_level_2(wall_coord a[]);
int input_from_file_rolling_stone_level_2(rolling_stone_coord a[]);
int input_from_file_button_level_2(button_coord a[]);
int input_from_file_door_level_2(door_coord a[]);
int input_from_file_leaf_level_2(leaf_coord a[]);
int input_from_file_exit_level_2(exit_coord a[]);
int input_from_file_snake_level_2(snak_coord a[]);

int input_from_file_gem_level_3(gem_coord a[]);
int input_from_file_wall1_level_3(wall_coord a[]);
int input_from_file_rolling_stone_level_3(rolling_stone_coord a[]);
int input_from_file_button_level_3(button_coord a[]);
int input_from_file_door_level_3(door_coord a[]);
int input_from_file_leaf_level_3(leaf_coord a[]);
int input_from_file_exit_level_3(exit_coord a[]);
int input_from_file_snake_level_3(snak_coord a[]);
