#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#include <player.h>
#include <monster.h>
#include <bomb.h>
#include <game.h>

struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( i + j * map->width)

void map_empty_grid(struct map* map){
		int width = map->width;
		int height = map->height;
		int i, j;
		for (i = 0; i < width; i++)
		  for (j = 0; j < height; j++)
		    map->grid[CELL(i,j)] = CELL_EMPTY;
}

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	map_empty_grid(map);

	return map;
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);
	if (x>=0 && x<map->width && y>=0 && y<map->height){
		return 1;
	}
	else {
		return 0;
	}
}

void map_free(struct map* map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}

enum door_to_level map_get_door_to_level(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] >> 4) & 7;
}

enum door_state map_get_door_state(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] >> 7);
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void map_open_door(struct map* map){
	assert(map);
	for (int i = 0; i < map_get_width(map); i++) {
		for (int j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_DOOR) {
		     	map->grid[CELL(i,j)] = (map->grid[CELL(i,j)] | (CELL_DOOR_OPENED << 7));
			}
		}
	}
}

enum bonus_type map_get_bonus_type(struct map* map, int x, int y){
	//assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x, y)] >> 4);
}

void display_bonus(struct map* map, int x, int y)
{
	// bonus is encoded with the 4 most significant bits
	switch (map_get_bonus_type(map, x, y)) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;
	default:
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type >> 4) { // sub-types are encoded with the 4 most significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;

	}
}

void display_door(struct map* map, int x, int  y, unsigned char type)
{
	switch (type >> 7){
	case CELL_DOOR_CLOSED:
		window_display_image(sprite_get_door_closed(), x, y);
		break;
	case CELL_DOOR_OPENED:
		window_display_image(sprite_get_door_opened(), x, y);
		break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];
	    switch (type & 0x0f) {
		case CELL_SCENERY:
			display_scenery(map, x, y, type);
		  	break;
		case CELL_MONSTER:
			window_display_image(sprite_get_monster(), x, y);
			break;
	    case CELL_CASE:
	    	window_display_image(sprite_get_box(), x, y);
	    	break;
	    case CELL_BONUS:
	    	display_bonus(map, x, y);
	      	break;
	    case CELL_KEY:
	    	window_display_image(sprite_get_key(), x, y);
	    	break;
	    case CELL_DOOR:
	    	display_door(map, x ,y, type);
	    	break;
	    case CELL_BOMB:
	    	break;
	    case CELL_EXPLOSION:
	    	window_display_image(sprite_get_explosion(), x, y);
	    	break;
	    }
	  }
	}
}

struct map* map_load_data_from_file(char* filename)
{
	int c;
	int width;
	int height;
	FILE* file;
	file = fopen(filename, "r");

	// get height and width from file
	fscanf (file, "%d", &width);
	fseek(file, 1, SEEK_CUR);
	fscanf (file, "%d", &height);
	struct map* map = map_new(width, height);

	// fill the map with data from file
	for (int i = 0 ; i < width*height; i++){
		fscanf (file, "%d", &c);
		map->grid[i] = c;
	}
	fclose (file);
	return map;
}

void map_fill_grid(struct map* map_A, struct map* map_B){
	for (int i = 0 ; i < map_get_width(map_B)*map_get_height(map_B) ; i++){
		map_A->grid[i] = map_B->grid[i];
	}
}
