#ifndef BOMB_H_
#define BOMB_H_

#include <player.h>
#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <map.h>
#include <game.h>

struct bomb;

// Creates a new bomb
struct bomb* bomb_init(struct player* player);


void bomb_free(struct bomb* bomb);

// Returns the current position of the bomb
int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);

// Returns the range of the bomb
int bomb_get_range(struct bomb* bomb);

// Indicates if bomb has exploded or not
int bomb_has_exploded(struct bomb* bomb);

// Returns the next bomb
struct bomb* bomb_get_next_bomb(struct bomb* bomb);

// Display the bomb on the screen
void bomb_display(struct bomb* bomb, struct player* player, struct map* map, struct monster* monster);

// Initialise a struct bomb at the end of the chained list
void bomb_drop(struct bomb* bomb, struct player* player);

#endif /* BOMB_H_ */
