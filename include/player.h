#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <constant.h>
#include <player.h>
#include <sprite.h>
#include <window.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(char* filename);
void player_free(struct player* player);

// Return player's life
int player_get_life(struct player* player);

// Increase player's life
void player_inc_life(struct player* player);

// Decrease player's life
void player_dec_life(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Increase or decrease number of bomb
void player_inc_range_bomb(struct player* player);
void player_dec_range_bomb(struct player* player);

// Change the the current level of the player
void player_change_level(struct player* player, int level);

// Return bomb range
int player_get_bomb_range(struct player* player);

//Return if player has key or not
int player_get_key(struct player* player);

//Return player's current level
int player_get_current_level(struct player* player);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map);

// Load the player position next to the where he comes from
void player_from_map_next_door(struct player* player, struct map* map);

// Affects the player when he picks up a bonus
void player_bonus_effects(struct player* player, struct map* map, int x, int y);

// Allows the player to move a case or not
int player_move_case_aux(struct player* player, struct map* map, int x, int y);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

// Indicates if player is dead or not
int player_is_dead(struct player* player);

// Save player's current state in a data file
void player_save_data_in_file(struct player* player, char* filename);

#endif /* PLAYER_H_ */
