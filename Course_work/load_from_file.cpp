#include "load_from_file.h"

#include <SDL.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h.>
#include <math.h>
#include <time.h>
#include <SDL_image.h>

int input_from_file_gem_level_1(gem_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_1\\level_1_gem.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_wall1_level_1(wall_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_1\\level_1_wall1.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 1000 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_rolling_stone_level_1(rolling_stone_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_1\\level_1_rolling_stone.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_button_level_1(button_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_1\\level_1_button.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_door_level_1(door_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_1\\level_1_door.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
		fscanf_s(f, "%d", &a[i].type);
		fscanf_s(f, "%d", &a[i].trigger_x);
		fscanf_s(f, "%d", &a[i].trigger_y);
	}

	fclose(f);
	return size;
}
int input_from_file_leaf_level_1(leaf_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_1\\level_1_leaf.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_exit_level_1(exit_coord a[])
{
	int size;
	FILE* f;
	fopen_s(&f, "level_1\\level_1_exit.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_snake_level_1(snak_coord a[])
{
	int size;
	FILE* f;
	fopen_s(&f, "level_1\\level_1_snake.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
		fscanf_s(f, "%d", &a[i].direction);
		fscanf_s(f, "%d", &a[i].is_for_challenge);
	}

	fclose(f);
	return size;
}

int input_from_file_gem_level_2(gem_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_2\\level_2_gem.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_wall1_level_2(wall_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_2\\level_2_wall1.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 1000 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_rolling_stone_level_2(rolling_stone_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_2\\level_2_rolling_stone.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_button_level_2(button_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_2\\level_2_button.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_door_level_2(door_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_2\\level_2_door.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
		fscanf_s(f, "%d", &a[i].type);
		fscanf_s(f, "%d", &a[i].trigger_x);
		fscanf_s(f, "%d", &a[i].trigger_y);
	}

	fclose(f);
	return size;
}
int input_from_file_leaf_level_2(leaf_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_2\\level_2_leaf.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_exit_level_2(exit_coord a[])
{
	int size;
	FILE* f;
	fopen_s(&f, "level_2\\level_2_exit.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_snake_level_2(snak_coord a[])
{
	int size;
	FILE* f;
	fopen_s(&f, "level_2\\level_2_snake.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
		fscanf_s(f, "%d", &a[i].direction);
		fscanf_s(f, "%d", &a[i].is_for_challenge);
	}

	fclose(f);
	return size;
}

int input_from_file_gem_level_3(gem_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_3\\level_3_gem.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_wall1_level_3(wall_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_3\\level_3_wall1.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 1000 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_rolling_stone_level_3(rolling_stone_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_3\\level_3_rolling_stone.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_button_level_3(button_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_3\\level_3_button.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_door_level_3(door_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_3\\level_3_door.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
		fscanf_s(f, "%d", &a[i].type);
		fscanf_s(f, "%d", &a[i].trigger_x);
		fscanf_s(f, "%d", &a[i].trigger_y);
	}

	fclose(f);
	return size;
}
int input_from_file_leaf_level_3(leaf_coord a[]) // Функция для считывания файла 
{
	int size;
	FILE* f;
	fopen_s(&f, "level_3\\level_3_leaf.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_exit_level_3(exit_coord a[])
{
	int size;
	FILE* f;
	fopen_s(&f, "level_3\\level_3_exit.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
	}

	fclose(f);
	return size;
}
int input_from_file_snake_level_3(snak_coord a[])
{
	int size;
	FILE* f;
	fopen_s(&f, "level_3\\level_3_snake.txt", "r");
	if (f == NULL)
		return -1;
	fscanf_s(f, "%d", &size);
	if (size > 100 || size < 1)
		return -2;

	for (int i = 0; i < size; i++)
	{
		fscanf_s(f, "%d", &a[i].x);
		fscanf_s(f, "%d", &a[i].y);
		fscanf_s(f, "%d", &a[i].direction);
		fscanf_s(f, "%d", &a[i].is_for_challenge);
	}

	fclose(f);
	return size;
}