#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>

extern int time_start_pause;
extern int time_paused;

// Abstract data type
struct game;

// Another abstract data type
struct game_backup;

// Create a new game backup
struct game_backup* game_new_backup();

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Free backups
void game_backup_free(struct game_backup* game_backup);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current map
struct map* game_get_map(struct game* game);

// Return bomb structure
struct bomb* game_get_bomb(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// pause the game
int game_pause(struct game* game, int update);

// update
int game_update(struct game* game, struct game_backup* game_backup);

//Keep what the player changed on the map
void game_keep_backup(struct game* game, struct game_backup* game_backup);

// Change level
struct map * game_change_level(struct game* game, struct game_backup* game_backup, int go);

#endif /* GAME_H_ */
