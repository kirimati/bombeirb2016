#include <SDL/SDL_image.h>
#include <assert.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

#ifndef NULL
#define NULL   ((void *) 0)
#endif

struct monster {
	int life;
	int x, y;
	enum direction current_direction;
	int current_level;

};

struct monster* monster_init(int life, int current_level) {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->life = life;
	monster->current_direction = SOUTH;
	monster->current_level = current_level;
	return monster;
}

void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->current_direction = way;
}

int monster_get_current_level(struct monster* monster){
	assert(monster);
	return monster->current_level;
}

void monster_from_map(struct monster* monster, struct map* map) {
	assert(monster);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {
	    if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
	      monster->x = i;
	      monster->y = j;
	    }
	  }
	}
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;

	case CELL_CASE:
		switch (monster->current_direction) {
		case NORTH :
			if (!map_is_inside(map, x, y-1)){
				return 0;
			}
			if (map_get_cell_type(map, x, y-1) == CELL_EMPTY){
				return 1;
			}
			else {
				return 0;
			}
		case SOUTH :
			if (!map_is_inside(map, x, y + 1)){
				return 0;
			}
			if (map_get_cell_type(map, x, y + 1) == CELL_EMPTY){
				return 1;
			}
			else {
				return 0;
			}
		case WEST :
			if (!map_is_inside(map, x - 1, y)){
				return 0;
			}
			if(map_get_cell_type(map, x - 1, y) == CELL_EMPTY){
				return 1;
			}
			else {
				return 0;
			}
		case EAST :
			if (!map_is_inside(map, x + 1, y)){
				return 0;
			}
			if(map_get_cell_type(map, x + 1, y) == CELL_EMPTY){
				return 1;
			}
			else {
				return 0;
			}
		}
		break;

	case CELL_BONUS:
		break;

	case CELL_PLAYER:
		break;

	case CELL_DOOR:
		if (map_get_door_state(map,x ,y)){
			return 1;
		}
		return 0;
		break;
	case CELL_BOMB:
		return 0;
	case CELL_EXPLOSION:
		monster_dec_life(monster);
		break;
	default:
		break;
	}

	// Zombie has moved
	return 1;
}

int monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 8;

	switch (monster->current_direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1)) {
			monster->y--;
			switch (map_get_cell_type(map, x, y - 1)) {
			case CELL_CASE:
				map_set_cell_type(map, x, y - 2, CELL_CASE);
				break;
			case CELL_DOOR:
				move = map_get_door_to_level(map ,x , y - 1);
				return move;
			default :
				break;
			}
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			move = 8;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1)) {
			monster->y++;
			switch (map_get_cell_type(map, x, y + 1)) {
			case CELL_CASE:
				map_set_cell_type(map, x, y + 2, CELL_CASE);
				break;
			case CELL_DOOR:
				move = map_get_door_to_level(map, x, y + 1);
				return move;
			default :
				break;
			}
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			move = 8;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y)) {
			monster->x--;
			switch (map_get_cell_type(map, x - 1, y)) {
			case CELL_CASE:
				map_set_cell_type(map, x - 2, y, CELL_CASE);
				break;
			case CELL_DOOR:
				move = map_get_door_to_level(map, x - 1, y);
				return move;
			default :
				break;
			}
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			move = 8;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y)) {
			monster->x++;
			switch (map_get_cell_type(map, x + 1, y)) {
			case CELL_CASE:
				map_set_cell_type(map, x + 2, y, CELL_CASE);
				break;
			case CELL_DOOR:
				move = map_get_door_to_level(map, x + 1, y);
				return move;
			default:
				break;
			}
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			move = 8;
		}
		break;
	}
	return move;
}

void monster_display(struct monster* monster) {
	assert(monster);
	monster_invincibility(monster);
	// faire clignoter le monster
	window_display_image(sprite_get_monster(monster->current_direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}

