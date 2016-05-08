#include <SDL/SDL_image.h>
#include <assert.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <game.h>
#include <player.h>


struct monster {
	int life;
	int x, y;
	enum direction current_direction;
	int current_level;
	int timer_move;
	struct monster* next_monster;
};

struct monster* monster_init() {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->life = 1;
	monster->current_direction = SOUTH;
	monster->timer_move = SDL_GetTicks();
	monster->next_monster = NULL;

	return monster;
}

void monster_free(struct monster* monster) {
	assert(monster);
	while (monster->next_monster != NULL){
		free(monster);
		monster = monster->next_monster;
	}
	free(monster);
}


void monster_dec_life(struct monster* monster){
	assert(monster);
	monster->life -= 1;
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

int monster_get_timer_move(struct monster* monster){
	assert(monster);
	return monster->timer_move;
}

struct monster* monster_get_next_monster(struct monster* monster){
	assert(monster);
	return monster->next_monster;
}

void monster_from_map(struct monster* monster, struct map* map, struct player* player) {
	assert(monster);
	assert(map);
	assert(player);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {
	    if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
	      monster->x = i;
	      monster->y = j;
	      monster->current_level = player_get_current_level(player);
	      monster->next_monster = monster_init();
	      monster = monster->next_monster;
	    }
	  }
	}
}


int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {
	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_EMPTY:
		return 1;

	case CELL_EXPLOSION:
		monster_dec_life(monster);
		break;
	default:
		return 0;
	}

	// monster has moved
	return 1;
}

void monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	monster->timer_move = SDL_GetTicks();
	if (!monster_is_dead(monster)){
		switch (monster->current_direction) {
		case NORTH:
			if (monster_move_aux(monster, map, x, y - 1)) {
				monster->y--;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			}
			break;

		case SOUTH:
			if (monster_move_aux(monster, map, x, y + 1)) {
				monster->y++;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			}
			break;

		case WEST:
			if (monster_move_aux(monster, map, x - 1, y)) {
				monster->x--;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			}
			break;

		case EAST:
			if (monster_move_aux(monster, map, x + 1, y)) {
				monster->x++;
				map_set_cell_type(map, x, y, CELL_EMPTY);
				map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			}
			break;
		}
	}
	else{
		map_set_cell_type(map, monster->x, monster->y, CELL_EMPTY);
	}
}

int monster_is_dead(struct monster* monster){
	if (monster->life <= 0){
		return 1;
	}
	return 0;

}

void monster_display(struct monster* monster, struct player* player) {
	assert(monster);
	if (monster->current_level == player_get_current_level(player) && monster->life > 0){
		window_display_image(sprite_get_monster(monster->current_direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
	}
}

