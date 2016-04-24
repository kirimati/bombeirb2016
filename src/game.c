#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>

struct game {
	struct map* map; // the game's map
	short nb_maps; // nb maps of the game
	struct player* player;
	struct bomb* bomb;
	struct bomb* first_bomb;
};

struct game_backup {
	struct map* map_0;
	struct map* map_1;
	struct map* map_2;
	struct map* map_3;
	struct map* map_4;
	struct map* map_5;
	struct map* map_6;
	struct map* map_7;
};

struct game_backup* game_new_backup(){
	struct game_backup* game_backup = malloc(sizeof *game_backup);
	game_backup->map_0 = map_load_data_from_file("data/map_0");
	game_backup->map_1 = map_load_data_from_file("data/map_1");
	game_backup->map_2 = map_load_data_from_file("data/map_2");
	game_backup->map_3 = map_load_data_from_file("data/map_3");
	game_backup->map_4 = map_load_data_from_file("data/map_4");
	game_backup->map_5 = map_load_data_from_file("data/map_5");
	game_backup->map_6 = map_load_data_from_file("data/map_6");
	game_backup->map_7 = map_load_data_from_file("data/map_7");
	return game_backup;
}

struct game* game_new() {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->map = map_load_data_from_file("data/map_0");
	game->nb_maps = 8;

	game->player = player_init(1, 5, 1, 0, 0);
	game->bomb = bomb_init();
	game->first_bomb = game->bomb;
	player_from_map(game->player, game->map); // get x,y of the player on the first map

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	bomb_free(game->first_bomb);
	map_free(game->map);
}

void game_backup_free(struct game_backup* game_backup){
	assert(game_backup);

	free(game_backup);
}

struct map* game_get_map(struct game* game) {
	assert(game);
	return game->map;
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct bomb* game_get_bomb(struct game* game){
	assert(game);
	return game->bomb;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 7;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_bomb_range(game_get_player(game))), x, y);

	x = 4 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_banner_key(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_key(game_get_player(game))), x, y);

	x = 5 * white_bloc + 8 * SIZE_BLOC;
	window_display_image(sprite_get_banner_flag(), x, y);

	x = 5 * white_bloc + 9 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_current_level(game_get_player(game))), x, y);

}

void game_display(struct game* game) {
	assert(game);
	struct bomb* bomb_tmp = game->bomb;
	window_clear();

	game_banner_display(game);
	map_display(game->map);
	player_display(game->player);
	while (bomb_get_next_bomb(bomb_tmp) != NULL){
		bomb_display(bomb_tmp, game->player, game->map);
		bomb_tmp = bomb_get_next_bomb(bomb_tmp);
	}
	window_refresh();
}

short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_map(game);
	int move = 8;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 9;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 9;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				move = player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				move = player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				move = player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				move = player_move(player, map);
				break;
			case SDLK_SPACE:
				if (player_get_nb_bomb(player) > 0) {
					bomb_drop(game->bomb, player);
				}
				break;
			default:
				move = 8;
				break;
			}
			break;
		}
	}
	return move;
}

int game_update(struct game* game, struct game_backup* game_backup) {
	int update;
	update = input_keyboard(game);
	if (update <= 7){
		game_keep_backup(game, game_backup);
		game_change_level(game, game_backup, update);
		update = 0;
	}
	if (update == 8){
		update = 0;
	}
	if (update == 9){
		update = 1;
	}
	return update;
}

void game_keep_backup(struct game* game, struct game_backup* game_backup){
	switch (player_get_current_level(game->player)){
	case 0:
		map_fill_grid(game_backup->map_0, game->map);
		break;
	case 1:
		map_fill_grid(game_backup->map_1, game->map);
		break;
	case 2:
		map_fill_grid(game_backup->map_2, game->map);
		break;
	case 3:
		map_fill_grid(game_backup->map_3, game->map);
		break;
	case 4:
		map_fill_grid(game_backup->map_4, game->map);
		break;
	case 5:
		map_fill_grid(game_backup->map_5, game->map);
		break;
	case 6:
		map_fill_grid(game_backup->map_6, game->map);
		break;
	case 7:
		map_fill_grid(game_backup->map_7, game->map);
		break;
	}
}

struct map * game_change_level(struct game* game, struct game_backup* game_backup, int go){
	map_empty_grid(game->map);
	switch (go){
	case 0:
		map_fill_grid(game->map, game_backup->map_0);
		game->player = player_init(player_get_life(game_get_player(game)), player_get_nb_bomb(game_get_player(game)), player_get_bomb_range(game_get_player(game)), player_get_key(game_get_player(game)), 0);
		player_from_map(game->player, game->map);
		break;
	case 1:
		map_fill_grid(game->map, game_backup->map_1);
		game->player = player_init(player_get_life(game_get_player(game)), player_get_nb_bomb(game_get_player(game)), player_get_bomb_range(game_get_player(game)), player_get_key(game_get_player(game)), 1);
		player_from_map(game->player, game->map);
		break;
	case 2:
		map_fill_grid(game->map, game_backup->map_2);
		game->player = player_init(player_get_life(game_get_player(game)), player_get_nb_bomb(game_get_player(game)), player_get_bomb_range(game_get_player(game)), player_get_key(game_get_player(game)), 2);
		player_from_map(game->player, game->map);
		break;
	case 3:
		map_fill_grid(game->map, game_backup->map_3);
		game->player = player_init(player_get_life(game_get_player(game)), player_get_nb_bomb(game_get_player(game)), player_get_bomb_range(game_get_player(game)), player_get_key(game_get_player(game)), 3);
		player_from_map(game->player, game->map);
		break;
	case 4:
		map_fill_grid(game->map, game_backup->map_4);
		game->player = player_init(player_get_life(game_get_player(game)), player_get_nb_bomb(game_get_player(game)), player_get_bomb_range(game_get_player(game)), player_get_key(game_get_player(game)), 4);
		player_from_map(game->player, game->map);
		break;
	case 5:
		map_fill_grid(game->map, game_backup->map_5);
		game->player = player_init(player_get_life(game_get_player(game)), player_get_nb_bomb(game_get_player(game)), player_get_bomb_range(game_get_player(game)), player_get_key(game_get_player(game)), 5);
		player_from_map(game->player, game->map);
		break;
	case 6:
		map_fill_grid(game->map, game_backup->map_6);
		game->player = player_init(player_get_life(game_get_player(game)), player_get_nb_bomb(game_get_player(game)), player_get_bomb_range(game_get_player(game)), player_get_key(game_get_player(game)), 6);
		player_from_map(game->player, game->map);
		break;
	case 7:
		map_fill_grid(game->map, game_backup->map_7);
		game->player = player_init(player_get_life(game_get_player(game)), player_get_nb_bomb(game_get_player(game)), player_get_bomb_range(game_get_player(game)), player_get_key(game_get_player(game)), 7);
		player_from_map(game->player, game->map);
		break;
	}
	return game->map;
}
