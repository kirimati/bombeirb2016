#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>

struct monster;

// Creates a new monster with a given number of available bombs
struct monster* monster_init(int life, int bomb_number, int bomb_range,int key, int current_level);
void   monster_free(struct monster* monster);

// Return monster's life
int monster_get_life(struct monster* monster);

// Increase monster's life
void monster_inc_life(struct monster* monster);

// Decrease monster's life
void monster_dec_life(struct monster* monster);

// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster * monster, enum direction direction);

// Set, Increase, Decrease the number of bomb that monster can put
int  monster_get_nb_bomb(struct monster * monster);
void monster_inc_nb_bomb(struct monster * monster);
void monster_dec_nb_bomb(struct monster * monster);

// Increase monster's current level
void monster_level_up(struct monster* monster);

// Return bomb range
int monster_get_bomb_range(struct monster* monster);

//Return if monster has key or not
int monster_get_key(struct monster* monster);

//Return monster's current level
int monster_get_current_level(struct monster* monster);

// Load the monster position from the map
void monster_from_map(struct monster* monster, struct map* map);

// Disable invincibility when invincibility was enabled more than 2 seconds
void monster_invincibility(struct monster* monster);

// Move the monster according to the current direction
int monster_move(struct monster* monster, struct map* map);

// Display the monster on the screen
void monster_display(struct monster* monster);

// The monster places a bomb
void monster_place_bomb(struct map* map, struct monster* monster);

// Display the bomb on the screen
void monster_bomb_display(struct monster* monster);

#endif /* MONSTER_H_ */
