#include <SDL/SDL.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <misc.h>
#include <bomb.h>


int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}



	window_create(SIZE_BLOC * MAP_WIDTH,
	SIZE_BLOC * MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	struct game* game = game_new();
	struct game_backup* game_backup = game_new_backup();

	// game loop
	// static time rate implementation
	int update = 0;
	while (!update) {
		timer = SDL_GetTicks();

		update = game_update(game, game_backup);
		game_display(game);

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed){
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
		}
	}
	game_backup_free(game_backup);
	game_free(game);

	SDL_Quit();

	return EXIT_SUCCESS;
}
