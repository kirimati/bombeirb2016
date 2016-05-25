#include <SDL/SDL_image.h>
#include <assert.h>

#include <bomb.h>
#include <player.h>
#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <map.h>
#include <game.h>


struct bomb {
	int drop_time;
	int x;
	int y;
	int end_bomb;
	int range;
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
	bomb->range = player_get_bomb_range(player);
	bomb->level = player_get_current_level(player);
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	bomb->drop_time = SDL_GetTicks();

	player_dec_nb_bomb(player);
	return bomb;
}


void bomb_free(struct bomb* bomb) {
	if (bomb != NULL){
		assert(bomb);
		while (bomb->next_bomb != NULL){
			free(bomb);
			bomb = bomb->next_bomb;
		}
		free(bomb);
	}
}

int bomb_get_x(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->x;
}

int bomb_get_y(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->y;
}

int bomb_get_range(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->range;
}

int bomb_has_exploded(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->end_bomb;
}

struct bomb* bomb_get_next_bomb(struct bomb* bomb){
	assert(bomb);
	return bomb->next_bomb;
}

void bomb_player_gets_harmed(struct player* player, int x, int y){
	if ((x == player_get_x(player)) && (y == player_get_y(player))){
		player_dec_life(player);
	}
}

void bomb_monster_gets_harmed(struct monster* monster, struct player* player, int x, int y){
	struct monster* monster_tmp = monster;
	while (monster_get_next_monster(monster_tmp) != NULL){
		if ((x == monster_get_x(monster_tmp)) && (y == monster_get_y(monster_tmp)) && (monster_get_current_level(monster) == player_get_current_level(player))){
			monster_dec_life(monster_tmp);
		}
		monster_tmp = monster_get_next_monster(monster_tmp);
	}
}

int bomb_explosion_gestion_aux(struct bomb* bomb, struct player* player, struct map* map, int x, int y){
	if (map_is_inside(map, x, y)){
		switch (map_get_cell_type(map, x, y)){
		case CELL_SCENERY:
			return 1;
		case CELL_PLAYER:
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			return 1;
		case CELL_CASE:
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			return 1;
		case CELL_MONSTER:
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			map_set_cell_type(map, x, y, CELL_EMPTY);
			return 1;
			break;
		case CELL_EMPTY:
			map_set_cell_type(map, x, y, CELL_EXPLOSION);
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			break;
		case CELL_BOMB:
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			break;
		case CELL_DOOR:
			return 1;
		case CELL_BONUS:
			map_set_cell_type(map, x, y, CELL_EXPLOSION);
			window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
			break;
		default:
			break;
		}
	}
	return 0;
}

int bomb_after_explosion_gestion_aux(struct bomb* bomb, struct player* player, struct map* map, struct monster* monster, int x, int y){
	if (map_is_inside(map, x, y)){
		switch (map_get_cell_type(map, x, y)){
		case CELL_SCENERY:
			return 1;
		case CELL_EXPLOSION:
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;
		case CELL_CASE:
			if(map_set_bonus_type(map, x, y, map_get_bonus_type(map, x, y)) == 2){
				monster_birth(monster, player, x, y);
			}
			return 1;
		case CELL_DOOR:
			return 1;
		default:
			break;
		}
	}
	return 0;
}

void bomb_explosion_gestion(struct bomb* bomb, struct player* player, struct map* map, struct monster* monster){
	int detection;
	detection = 1;
	bomb_explosion_gestion_aux(bomb, player, map, bomb->x, bomb->y);
	bomb_player_gets_harmed(player, bomb->x , bomb->y);
	bomb_monster_gets_harmed(monster, player, bomb->x , bomb->y);
	for (int i = 1 ; i <= bomb_get_range(bomb) ; i++){
		detection = bomb_explosion_gestion_aux(bomb, player, map, (bomb->x + i), bomb->y);
		bomb_player_gets_harmed(player, (bomb->x + i), bomb->y);
		bomb_monster_gets_harmed(monster, player, (bomb->x + i), bomb->y);
		if (detection){
			break;
		}
	}
	for (int i = 1 ; i <= bomb_get_range(bomb) ; i++){
		detection = bomb_explosion_gestion_aux(bomb, player, map, (bomb->x - i), bomb->y);
		bomb_player_gets_harmed(player, (bomb->x - i), bomb->y);
		bomb_monster_gets_harmed(monster, player, (bomb->x - i), bomb->y);
		if (detection){
			break;
		}
	}
	for (int i = 1 ; i <= bomb_get_range(bomb) ; i++){
		detection = bomb_explosion_gestion_aux(bomb, player, map, bomb->x, (bomb->y + i));
		bomb_player_gets_harmed(player, bomb->x, (bomb->y + i));
		bomb_monster_gets_harmed(monster, player, bomb->x , (bomb->y + i));
		if (detection){
			break;
		}
	}
	for (int i = 1 ; i <= bomb_get_range(bomb) ; i++){
		detection = bomb_explosion_gestion_aux(bomb, player, map, bomb->x, (bomb->y - i));
		bomb_player_gets_harmed(player, bomb->x, (bomb->y - i));
		bomb_monster_gets_harmed(monster, player, bomb->x, (bomb->y - i));
		if (detection){
			break;
		}
	}
}

void bomb_after_explosion_gestion(struct bomb* bomb, struct player* player, struct map* map, struct monster* monster){
	int detection;
	detection = 1;
	bomb_after_explosion_gestion_aux(bomb, player, map, monster, bomb->x, bomb->y);
	for (int i = 1 ; i <= bomb_get_range(bomb) ; i++){
		detection = bomb_after_explosion_gestion_aux(bomb, player, map, monster, (bomb->x + i), bomb->y);
		if (detection){
			break;
		}
	}
	for (int i = 1 ; i <= bomb_get_range(bomb) ; i++){
		detection = bomb_after_explosion_gestion_aux(bomb, player, map, monster, (bomb->x - i), bomb->y);
		if (detection){
			break;
		}
	}
	for (int i = 1 ; i <= bomb_get_range(bomb) ; i++){
		detection = bomb_after_explosion_gestion_aux(bomb, player, map, monster, bomb->x, (bomb->y + i));
		if (detection){
			break;
		}
	}
	for (int i = 1 ; i <= bomb_get_range(bomb) ; i++){
		detection = bomb_after_explosion_gestion_aux(bomb, player, map, monster, bomb->x, (bomb->y - i));
		if (detection){
			break;
		}
	}
}

void bomb_display(struct bomb* bomb, struct player* player, struct map* map, struct monster* monster) {
	assert(bomb);
	int t;
	if (time_start_pause > bomb->drop_time){
		t = (SDL_GetTicks() - bomb->drop_time - time_paused);
	}
	else{
		t = (SDL_GetTicks() - bomb->drop_time);
	}
	if(bomb->end_bomb != 1 && (bomb->level == player_get_current_level(player))){
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
			bomb_explosion_gestion(bomb, player, map, monster);
		}
		if (t > 5000){
			bomb_after_explosion_gestion(bomb, player, map, monster);
			bomb->end_bomb = 1;
			player_inc_nb_bomb(player);
		}
	}
	if (t > 5000 && (bomb->end_bomb != 1)){
		bomb->end_bomb = 1;
		player_inc_nb_bomb(player);
	}
}


void bomb_drop(struct bomb* bomb, struct player* player){
	while(bomb->next_bomb != NULL){
		bomb = bomb->next_bomb;
	}
	bomb->next_bomb = bomb_init(player);
}

