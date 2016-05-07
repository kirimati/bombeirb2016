#ifndef ZOMBIE_H_
#define ZOMBIE_H_

#include <map.h>
#include <constant.h>

struct zombie;

// Creates a new zombie with a given number of available bombs
struct zombie* zombie_init(int life, int bomb_number, int bomb_range,int key, int current_level);
void   zombie_free(struct zombie* zombie);

// Return zombie's life
int zombie_get_life(struct zombie* zombie);

// Increase zombie's life
void zombie_inc_life(struct zombie* zombie);

// Decrease zombie's life
void zombie_dec_life(struct zombie* zombie);

// Returns the current position of the zombie
int zombie_get_x(struct zombie* zombie);
int zombie_get_y(struct zombie* zombie);

// Set the direction of the next move of the zombie
void zombie_set_current_way(struct zombie * zombie, enum direction direction);

// Set, Increase, Decrease the number of bomb that zombie can put
int  zombie_get_nb_bomb(struct zombie * zombie);
void zombie_inc_nb_bomb(struct zombie * zombie);
void zombie_dec_nb_bomb(struct zombie * zombie);

// Increase zombie's current level
void zombie_level_up(struct zombie* zombie);

// Return bomb range
int zombie_get_bomb_range(struct zombie* zombie);

//Return if zombie has key or not
int zombie_get_key(struct zombie* zombie);

//Return zombie's current level
int zombie_get_current_level(struct zombie* zombie);

// Load the zombie position from the map
void zombie_from_map(struct zombie* zombie, struct map* map);

// Disable invincibility when invincibility was enabled more than 2 seconds
void zombie_invincibility(struct zombie* zombie);

// Move the zombie according to the current direction
int zombie_move(struct zombie* zombie, struct map* map);

// Display the zombie on the screen
void zombie_display(struct zombie* zombie);

// The zombie places a bomb
void zombie_place_bomb(struct map* map, struct zombie* zombie);

// Display the bomb on the screen
void zombie_bomb_display(struct zombie* zombie);

#endif /* ZOMBIE_H_ */
