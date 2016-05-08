#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bombeirb 2016"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT		4
#define BANNER_HEIGHT	40

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define MAP_WIDTH  30
#define MAP_HEIGHT 15

enum direction {
	NORTH = 0,
	SOUTH, // 1
	WEST, // 2
	EAST //3
};

// monster speed
#define SPEED_LVL_0 1000
#define SPEED_LVL_1 900
#define SPEED_LVL_2 800
#define SPEED_LVL_3 700
#define SPEED_LVL_4 500
#define SPEED_LVL_5 300
#define SPEED_LVL_6 200
#define SPEED_LVL_7 80

#endif /* CONSTANT */
