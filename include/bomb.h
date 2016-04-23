#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <constant.h>
#include <player.h>

struct bomb;

// Creates a new bomb
struct bomb* bomb_init();
void bomb_drop(struct bomb* bomb, struct player* player);
void bomb_free(struct bomb* bomb);

// Returns the current position of the bomb
int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);


// Returns the next bomb
struct bomb* bomb_get_next_bomb(struct bomb* bomb);

// Load the bomb position from the map
//void bomb_from_map(struct bomb* bomb, struct map* map);

// Display the bomb on the screen
void bomb_display(struct bomb* bomb, struct player* player);

#endif /* BOMB_H_ */
