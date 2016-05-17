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

// Returns the next bomb
struct bomb* bomb_get_next_bomb(struct bomb* bomb);

// Display the bomb on the screen
void bomb_display(struct bomb* bomb, struct player* player, struct map* map, struct monster* monster);

void bomb_set_cell_type(struct bomb* bomb, struct player* player, struct map* map);

void bomb_drop(struct bomb* bomb, struct player* player);

#endif /* BOMB_H_ */
