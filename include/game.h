#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <map.h>
#include <monster.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>
#include <constant.h>

extern int time_start_pause;
extern int time_paused;

// Abstract data type
struct game;

// Another abstract data type
struct game_backup;

// Create a new game backup for a new game
struct game_backup* game_new_backup();

// Create a new game backup from the saving slot
struct game_backup* game_new_backup2();

// Create a new game
struct game* game_new(struct game_backup* game_backup, char* filename);

// Free a game
void game_free(struct game* game);

// Free backups
void game_backup_free(struct game_backup* game_backup);


// Display the game on the screen
void game_display(struct game* game);

// pause the game
int game_pause(struct game* game, int update);

// update
int game_update(struct game* game, struct game_backup* game_backup);

//Keep what the player changed on the map
void game_choose_map(struct game* game, struct game_backup* game_backup);

// Change level
struct map * game_change_level(struct game* game, struct game_backup* game_backup, int go);

// Save game in data2 files
void game_save(struct game* game, struct game_backup* game_backup);

#endif /* GAME_H_ */
