#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <player.h>


struct monster;

// Creates a new monster
struct monster* monster_init();

// Free monsters
void monster_free(struct monster* monster);

// Decrease monster's life
void monster_dec_life(struct monster* monster);

// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster * monster, enum direction direction);

//Return monster's current level
int monster_get_current_level(struct monster* monster);

//Return monster's last move time
int monster_get_timer_move(struct monster* monster);

//Return the next monster
struct monster* monster_get_next_monster(struct monster* monster);

// Load the monster position from the map
void monster_from_map(struct monster* monster, struct map* map, struct player* player);

// Move the monster according to the current direction
void monster_move(struct monster* monster, struct map* map, struct player* player);

// Indicates if monster is dead or not
int monster_is_dead(struct monster* monster, struct map* map);

// Display the monster on the screen
void monster_display(struct monster* monster, struct player* player);

// Creates a monster
void monster_birth(struct monster* monster, struct player* player, int x, int y);

#endif /* MONSTER_H_ */
