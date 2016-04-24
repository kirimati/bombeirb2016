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
	struct bomb* next_bomb;
};

struct bomb* bomb_init() {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb){
		error("Memory error");
	}
	bomb->next_bomb = NULL;
	return bomb;
}

void bomb_free(struct bomb* bomb) {
	assert(bomb);
	while (bomb->next_bomb != NULL){
		free(bomb);
		bomb = bomb->next_bomb;
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

void bomb_explosion_gestion(struct bomb* bomb, struct player* player, struct map* map, int x, int y){
	switch (map_get_cell_type(map, x, y)){
	case CELL_PLAYER:
		player_dec_life(player);
		map_set_cell_type(map, x, y, CELL_EXPLOSION);
		break;
	case CELL_CASE:
		// Gestion des bonus
		break;
	case CELL_MONSTER:
		//Gestion des monstres
		break;
	default:
		break;
	}
	window_display_image(sprite_get_explosion(), x * SIZE_BLOC, y * SIZE_BLOC);
}

void bomb_after_explosion_gestion(struct bomb* bomb, struct player* player, struct map* map, int x, int y){
	switch (map_get_cell_type(map, x, y)){
	case CELL_EXPLOSION:
		map_set_cell_type(map, x, y, CELL_EMPTY);
		break;
	case CELL_CASE:
		// Gestion des bonus
		break;
	case CELL_MONSTER:
		//Gestion des monstres
		break;
	default:
		break;
	}
}

void bomb_display(struct bomb* bomb, struct player* player, struct map* map) {
	assert(bomb);

	int t = (SDL_GetTicks() - bomb->drop_time);

	if (t <= 1000){
		window_display_image(sprite_get_bomb(3), bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
		map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);
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
		bomb_explosion_gestion(bomb, player, map, bomb->x, bomb->y);
		bomb_explosion_gestion(bomb, player, map, (bomb->x + 1), bomb->y);
		bomb_explosion_gestion(bomb, player, map, (bomb->x - 1), bomb->y);
		bomb_explosion_gestion(bomb, player, map, bomb->x, (bomb->y + 1));
		bomb_explosion_gestion(bomb, player, map, bomb->x, (bomb->y - 1));
	}
	if (t > 5000){
		bomb_after_explosion_gestion(bomb, player, map, bomb->x, bomb->y);
		bomb_after_explosion_gestion(bomb, player, map, (bomb->x + 1), bomb->y);
		bomb_after_explosion_gestion(bomb, player, map, (bomb->x - 1), bomb->y);
		bomb_after_explosion_gestion(bomb, player, map, bomb->x, (bomb->y + 1));
		bomb_after_explosion_gestion(bomb, player, map, bomb->x, (bomb->y - 1));
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
	bomb->next_bomb = bomb_init();
}

