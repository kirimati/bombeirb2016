#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <map.h>
#include <sprite.h>
#include <window.h>
#include <game.h>


struct player {
	int life;
	int x, y;
	enum direction current_direction;
	int nb_bomb;
	int bomb_range;
	int key;
	int current_level;
	int invincibility;
	int invincibility_timer;
};

struct player* player_init(char* filename) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");
	int life;
	int bomb_number;
	int bomb_range;
	int key;
	int current_level;
	FILE* file;
	file = fopen(filename, "r");

	// get height and width from file
	fscanf (file, "%d", &life);
	fscanf (file, "%d", &bomb_number);
	fscanf (file, "%d", &bomb_range);
	fscanf (file, "%d", &key);
	fscanf (file, "%d", &current_level);

	fclose (file);

	player->life = life;
	player->current_direction = SOUTH;
	player->nb_bomb = bomb_number;
	player->bomb_range = bomb_range;
	player->current_level = current_level;
	player->key = key;
	player->invincibility = 0;

	return player;
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_life(struct player* player){
	assert(player);
	return player->life;
}

void player_inc_life(struct player* player){
	assert(player);
	if (player->life < 9){
		player->life += 1;
	}
}

void player_dec_life(struct player* player){
	assert(player);
	if ((player->invincibility == 0) && (player->life > 0)){
		player->life -= 1;
		player->invincibility_timer = SDL_GetTicks();
		player->invincibility = 1;
	}

}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bomb;
}

int player_get_bomb_range(struct player* player){
	assert(player);
	return player->bomb_range;
}

int player_get_key(struct player* player){
	assert(player);
	return player->key;
}

int player_get_current_level(struct player* player){
	assert(player);
	return player->current_level;
}


void player_inc_nb_bomb(struct player* player) {
	assert(player);
	if (player->nb_bomb < 9){
		player->nb_bomb += 1;
	}
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	if (player->nb_bomb > 0){
		player->nb_bomb -= 1;
	}
}

void player_inc_range_bomb(struct player* player) {
	assert(player);
	if (player->bomb_range < 9){
		player->bomb_range += 1;
	}
}
void player_dec_range_bomb(struct player* player) {
	assert(player);
	if (player->bomb_range > 1){
		player->bomb_range -= 1;
	}
}

void player_change_level(struct player* player, int level) {
	assert(player);
	player->current_level = level;
}

void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {	
	    if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
	      player->x = i;
	      player->y = j;
	    }
	  }
	}
}

void player_from_map_next_door(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {
	    if (map_get_cell_type(map, i, j) == CELL_DOOR && map_get_door_to_level(map, i, j) == player_get_current_level(player)) {
	      player->x = i + 1;
	      player->y = j;
	    }
	  }
	}
}

void player_invincibility(struct player* player){
	int t;
	if (time_start_pause > player->invincibility_timer){
		t = (SDL_GetTicks() - player->invincibility_timer - time_paused);
	}
	else{
		t = (SDL_GetTicks() - player->invincibility_timer);
	}
	if (t > 2000){
		player->invincibility = 0;
	}
}

void player_bonus_effects(struct player* player, struct map* map, int x, int y){
	switch (map_get_bonus_type(map, x, y)){
	case BONUS_BOMB_RANGE_DEC:
		player_dec_range_bomb(player);
		break;
	case BONUS_BOMB_RANGE_INC:
		player_inc_range_bomb(player);
		break;
	case BONUS_BOMB_NB_DEC:
		player_dec_nb_bomb(player);
		break;
	case BONUS_BOMB_NB_INC:
		player_inc_nb_bomb(player);
		break;
	case BONUS_LIFE:
		if (player->life < 9){
			player->life++;
		}
		break;
	default:
		break;
	}
}

void player_open_door(struct player* player, struct map* map){
	switch(player->current_direction){
	case NORTH:
		map_open_door(map, player_get_x(player), player_get_y(player) - 1);
		break;
	case SOUTH:
		map_open_door(map, player_get_x(player), player_get_y(player) + 1);
		break;
	case WEST:
		map_open_door(map, player_get_x(player) - 1, player_get_y(player));
		break;
	case EAST:
		map_open_door(map, player_get_x(player) + 1, player_get_y(player));
		break;
	}
	player->key--;
}

int player_move_case_aux(struct player* player, struct map* map, int x, int y){
	switch (player->current_direction) {
	case NORTH :
		if (!map_is_inside(map, x, y-1)){
			return 0;
		}
		if ((map_get_cell_type(map, x, y-1) == CELL_EMPTY) || (map_get_cell_type(map, x, y - 1) == CELL_EXPLOSION)){
			return 1;
		}
		else {
			return 0;
		}
	case SOUTH :
		if (!map_is_inside(map, x, y + 1)){
			return 0;
		}
		if ((map_get_cell_type(map, x, y + 1) == CELL_EMPTY) || (map_get_cell_type(map, x, y + 1) == CELL_EXPLOSION)){
			return 1;
		}
		else {
			return 0;
		}
	case WEST :
		if (!map_is_inside(map, x - 1, y)){
			return 0;
		}
		if((map_get_cell_type(map, x - 1, y) == CELL_EMPTY) || (map_get_cell_type(map, x - 1, y) == CELL_EXPLOSION)){
			return 1;
		}
		else {
			return 0;
		}
	case EAST :
		if (!map_is_inside(map, x + 1, y)){
			return 0;
		}
		if((map_get_cell_type(map, x + 1, y) == CELL_EMPTY) || (map_get_cell_type(map, x + 1, y) == CELL_EXPLOSION)){
			return 1;
		}
		else {
			return 0;
		}
	}
	return 0;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;

	case CELL_CASE:
		return player_move_case_aux(player, map, x, y);

	case CELL_BONUS:
		player_bonus_effects(player, map, x, y);
		break;

	case CELL_MONSTER:
		player_dec_life(player);
		break;

	case CELL_DOOR:
		if (map_get_door_state(map, x, y)){
			return 1;
		}
		else{
			if (player_get_key(player) > 0){
				player_open_door(player, map);
			}
		}
		return 0;
		break;

	case CELL_KEY:
		if (player->key < 9){
			player->key++;
		}
		return 1;

	case CELL_BOMB:
		return 0;

	case CELL_EXPLOSION:
		player_dec_life(player);
		break;
	case CELL_PRINCESS:
		return 1;
	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 8; // Move = 8 when the player doesn't change level
				  // Move = 0,1,2,3,4,..,7 when the player goes to level 0,1,2,3,4,...,7
	unsigned char type;

	switch (player->current_direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			switch (map_get_cell_type(map, x, y - 1)) {
			case CELL_CASE:
				if (map_get_cell_type(map, x, y - 2) == CELL_EXPLOSION){
					type = map_get_bonus_type(map, x, y - 1);
					map_set_bonus_type(map, x, y - 2, type);
				}
				else{
					type = map_get_case_type(map, x, y - 1);
					map_set_case_type(map, x, y - 2, type);
				}
				break;
			case CELL_DOOR:
				move = map_get_door_to_level(map ,x , y - 1);
				return move;
			case CELL_PRINCESS:
				move = 9; // end game
				return move;
			default :
				break;
			}
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
			move = 8;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			switch (map_get_cell_type(map, x, y + 1)) {
			case CELL_CASE:
				if (map_get_cell_type(map, x, y + 2) == CELL_EXPLOSION){
					type = map_get_bonus_type(map, x, y + 1);
					map_set_bonus_type(map, x, y + 2, type);
				}
				else{
					type = map_get_case_type(map, x, y + 1);
					map_set_case_type(map, x, y + 2, type);
				}
				break;
			case CELL_DOOR:
				move = map_get_door_to_level(map, x, y + 1);
				return move;
			case CELL_PRINCESS:
				move = 9; // end game
				return move;
			default :
				break;
			}
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
			move = 8;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			switch (map_get_cell_type(map, x - 1, y)) {
			case CELL_CASE:
				if (map_get_cell_type(map, x - 2, y) == CELL_EXPLOSION){
					type = map_get_bonus_type(map, x - 1, y);
					map_set_bonus_type(map, x - 2, y, type);
				}
				else{
					type = map_get_case_type(map, x - 1, y);
					map_set_case_type(map, x - 2, y, type);
				}
				break;
			case CELL_DOOR:
				move = map_get_door_to_level(map, x - 1, y);
				return move;
			case CELL_PRINCESS:
				move = 9; // end game
				return move;
			default :
				break;
			}
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
			move = 8;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			switch (map_get_cell_type(map, x + 1, y)) {
			case CELL_CASE:
				if (map_get_cell_type(map, x + 2, y) == CELL_EXPLOSION){
					type = map_get_bonus_type(map, x + 1, y);
					map_set_bonus_type(map, x + 2, y, type);
				}
				else{
					type = map_get_case_type(map, x + 1, y);
					map_set_case_type(map, x + 2, y, type);
				}
				break;
			case CELL_DOOR:
				move = map_get_door_to_level(map, x + 1, y);
				return move;
			case CELL_PRINCESS:
				move = 9; // end game
				return move;
			default:
				break;
			}
			map_set_cell_type(map, x, y, CELL_EMPTY);
			map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
			move = 8;
		}
		break;
	}
	return move;
}

void player_display(struct player* player) {
	assert(player);
	player_invincibility(player);
	// faire clignoter le player
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

int player_is_dead(struct player* player){
	if (player->life <= 0){
		return 1;
	}
	else{
		return 0;
	}
}

void player_save_data_in_file(struct player* player, char* filename){
	FILE* file;
	file = fopen(filename, "w");

	fprintf(file, "%d ", player->life);
	fprintf(file, "%d ", player->nb_bomb);
	fprintf(file, "%d ", player->bomb_range);
	fprintf(file, "%d ", player->key);
	fprintf(file, "%d ", player->current_level);

	fclose (file);
}
