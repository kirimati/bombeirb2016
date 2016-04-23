#include <SDL/SDL_image.h>
#include <assert.h>

#include <bomb.h>
#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

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


void bomb_display(struct bomb* bomb, struct player* player) {
	assert(bomb);

	int t = (SDL_GetTicks() - bomb->drop_time);
	printf("%d\n", bomb->drop_time);

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
	if ((t <= 5000) & (t > 4000 )){
			for (int i = 1 ; i <= player_get_bomb_range(player) ; i++){
			window_display_image(sprite_get_explosion(), (bomb->x - i) * SIZE_BLOC, (bomb->y) * SIZE_BLOC);
			window_display_image(sprite_get_explosion(), (bomb->x + i) * SIZE_BLOC, (bomb->y) * SIZE_BLOC);
			window_display_image(sprite_get_explosion(), (bomb->x) * SIZE_BLOC, (bomb->y - i) * SIZE_BLOC);
			window_display_image(sprite_get_explosion(), (bomb->x) * SIZE_BLOC, (bomb->y + i) * SIZE_BLOC);
			}
		}
}


void bomb_drop(struct bomb* bomb, struct player* player){
	while (bomb->next_bomb != NULL){
		bomb = bomb->next_bomb;
	}
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	bomb->drop_time = SDL_GetTicks();
	printf("%d", bomb->drop_time);
	bomb->next_bomb = bomb_init();
}
