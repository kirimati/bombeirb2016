#include <SDL/SDL_image.h>
#include <assert.h>

#include <bomb.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <map.h>
#include <game.h>


struct bomb {
	int drop_time;
	int x;
	int y;
	int end_bomb;
	int level;
	struct bomb* next_bomb;
};

struct bomb* bomb_init(struct player* player) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb){
		error("Memory error");
	}
	bomb->next_bomb = NULL;
	bomb->end_bomb = 0;
	bomb->level = player_get_current_level(player);
	return bomb;
}

void bomb_free(struct bomb* bomb) {
	assert(bomb);
	while (bomb->next_bomb != NULL){
		free(bomb);
		bomb = bomb->next_bomb;
	}
	free(bomb);
}

int bomb_get_x(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->x;
}

int bomb_get_y(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->y;
}

struct bomb* bomb_get_next_bomb(struct bomb* bomb){
	assert(bomb);
	return bomb->next_bomb;
}


/*
void bomb_from_map(struct bomb* bomb, struct map* map) {
	assert(bomb);explosion dessin
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
	  for (j = 0; j < map_get_height(map); j++) {	
	    if (map_get_cell_type(map, i, j) == CELL_bomb) {
	      bomb->x = i;
	      bomb->y = j;
	    }
	  }
	}
}*/

void bomb_player_gets_harmed(struct player* player, int x, int y){
	if ((x == player_get_x(player)) && (y == player_get_y(player))){
		player_dec_life(player);
	}
}

int bomb_explosion_gestion_aux(struct bomb* bomb, struct player* player, struct map* map, int x, int y){
	if (map_is_inside(map, x, y)){
		switch (map_get_cell_type(map, x, y)){
		case CELL_PLAYER:
<<<<<<< HEAD
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
=======
			map_set_cell_type(map, x, y, CELL_EXPLOSION);
>>>>>>> origin/master
			break;
		case CELL_CASE:
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			return 0;
		case CELL_MONSTER:
			//Gestion des monstres
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			break;
		case CELL_EMPTY:
			map_set_cell_type(map, x, y, CELL_EXPLOSION);
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			break;
		case CELL_BOMB:
			map_set_cell_type(map, x, y, CELL_EXPLOSION);
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			break;
<<<<<<< HEAD
		case CELL_DOOR:
			return 0;
=======
>>>>>>> origin/master
		default:
			break;
		}
	}
	return 1;
<<<<<<< HEAD
}

int bomb_after_explosion_gestion_aux(struct bomb* bomb, struct player* player, struct map* map, int x, int y){
	if (map_is_inside(map, x, y)){
		switch (map_get_cell_type(map, x, y)){
		case CELL_EXPLOSION:
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		case CELL_CASE:
			map_set_bonus_type(map, x, y, map_get_bonus_type(map, x, y));
			return 0;
		case CELL_MONSTER:
			//Gestion des monstres
			break;
		case CELL_BOMB:
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		case CELL_DOOR:
			return 0;
		default:
			break;
		}
	}
	return 1;
}

void bomb_explosion_gestion(struct bomb* bomb, struct player* player, struct map* map){
	int detection;
	detection = 1;
	bomb_explosion_gestion_aux(bomb, player, map, bomb->x, bomb->y);
	bomb_player_gets_harmed(player,bomb->x ,bomb->y);
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
		detection = bomb_explosion_gestion_aux(bomb, player, map, (bomb->x + i), bomb->y);
		bomb_player_gets_harmed(player, (bomb->x + i), bomb->y);
		if (!detection){
			break;
		}
	}
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
		detection = bomb_explosion_gestion_aux(bomb, player, map, (bomb->x - i), bomb->y);
		bomb_player_gets_harmed(player, (bomb->x - i), bomb->y);
		if (!detection){
			break;
		}
	}
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
		detection = bomb_explosion_gestion_aux(bomb, player, map, bomb->x, (bomb->y + i));
		bomb_player_gets_harmed(player, bomb->x, (bomb->y + i));
		if (!detection){
			break;
		}
	}
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
		detection = bomb_explosion_gestion_aux(bomb, player, map, bomb->x, (bomb->y - i));
		bomb_player_gets_harmed(player, bomb->x, (bomb->y - i));
		if (!detection){
			break;
		}
	}
}

void bomb_after_explosion_gestion(struct bomb* bomb, struct player* player, struct map* map){
	int detection;
	detection = 1;
	bomb_after_explosion_gestion_aux(bomb, player, map, bomb->x, bomb->y);
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
		detection = bomb_after_explosion_gestion_aux(bomb, player, map, (bomb->x + i), bomb->y);
		if (!detection){
=======
}

void bomb_after_explosion_gestion(struct bomb* bomb, struct player* player, struct map* map, int x, int y){
	if (map_is_inside(map, x, y)){
		switch (map_get_cell_type(map, x, y)){
		case CELL_EXPLOSION:
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		case CELL_CASE:
			map_set_cell_type(map, x, y, CELL_BONUS);
			break;
		case CELL_MONSTER:
			//Gestion des monstres
			break;
		case CELL_BOMB:
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		default:
			break;
		}
	}
}

void bomb_explosion_gestion(struct bomb* bomb, struct player* player, struct map* map){
	int case_detection;
	bomb_explosion_gestion_aux(bomb, player, map, bomb->x, bomb->y);
	bomb_player_gets_harmed(player,bomb->x ,bomb->y);
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
		case_detection = bomb_explosion_gestion_aux(bomb, player, map, (bomb->x + i), bomb->y);
		bomb_player_gets_harmed(player, (bomb->x + i), bomb->y);
		if (!case_detection){
>>>>>>> origin/master
			break;
		}
	}
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
<<<<<<< HEAD
		detection = bomb_after_explosion_gestion_aux(bomb, player, map, (bomb->x - i), bomb->y);
		if (!detection){
=======
		case_detection = bomb_explosion_gestion_aux(bomb, player, map, (bomb->x - i), bomb->y);
		bomb_player_gets_harmed(player, (bomb->x - i), bomb->y);
		if (!case_detection){
>>>>>>> origin/master
			break;
		}
	}
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
<<<<<<< HEAD
		detection = bomb_after_explosion_gestion_aux(bomb, player, map, bomb->x, (bomb->y + i));
		if (!detection){
=======
		case_detection = bomb_explosion_gestion_aux(bomb, player, map, bomb->x, (bomb->y + i));
		bomb_player_gets_harmed(player, bomb->x, (bomb->y + i));
		if (!case_detection){
>>>>>>> origin/master
			break;
		}
	}
	for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
<<<<<<< HEAD
		detection = bomb_after_explosion_gestion_aux(bomb, player, map, bomb->x, (bomb->y - i));
		if (!detection){
=======
		case_detection = bomb_explosion_gestion_aux(bomb, player, map, bomb->x, (bomb->y - i));
		bomb_player_gets_harmed(player, bomb->x, (bomb->y - i));
		if (!case_detection){
>>>>>>> origin/master
			break;
		}
	}
}

void bomb_display(struct bomb* bomb, struct player* player, struct map* map) {
	assert(bomb);

	int t = (SDL_GetTicks() - bomb->drop_time);
	if(!(bomb->end_bomb) && (bomb->level == player_get_current_level(player))){
		if (t <= 1000){
			window_display_image(sprite_get_bomb(3), bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
		}
		if ((t <= 2000) & (t > 1000 )){
			window_display_image(sprite_get_bomb(2), bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
			}
		if ((t <= 3000) & (t > 2000 )){
			window_display_image(sprite_get_bomb(1), bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
			}
		if ((t <= 4000) & (t > 3000 )){
			window_display_image(sprite_get_bomb(0), bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
			}
		if ((t <= 5000) & (t > 4000)){
			bomb_explosion_gestion(bomb, player, map);
		}
		if (t > 5000){
			bomb_after_explosion_gestion(bomb, player, map);
			bomb->end_bomb = 1;
		}
<<<<<<< HEAD
=======
	if ((t <= 5000) & (t > 4000)){
		bomb_explosion_gestion(bomb, player, map);
	}
	if (t > 5000){
		bomb_after_explosion_gestion(bomb, player, map, bomb->x, bomb->y);
		for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
			bomb_after_explosion_gestion(bomb, player, map, (bomb->x + i), bomb->y);
			bomb_after_explosion_gestion(bomb, player, map, (bomb->x - i), bomb->y);
			bomb_after_explosion_gestion(bomb, player, map, bomb->x, (bomb->y + i));
			bomb_after_explosion_gestion(bomb, player, map, bomb->x, (bomb->y - i));
		}
>>>>>>> origin/master
	}
}

void bomb_drop(struct bomb* bomb, struct player* player){
	while (bomb->next_bomb != NULL){
		bomb = bomb->next_bomb;
	}
	player_dec_nb_bomb(player);
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	bomb->drop_time = SDL_GetTicks();
	bomb->next_bomb = bomb_init(player);
}

