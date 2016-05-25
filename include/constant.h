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

enum direction {
	NORTH = 0,
	SOUTH, // 1
	WEST, // 2
	EAST //3
};

// monster speed
#define SPEED_LVL_0 1000 // The monsters don´t move faster then every 1000 milliseconds.
#define SPEED_LVL_1 900	 // every 900 milliseconds
#define SPEED_LVL_2 800  // etc
#define SPEED_LVL_3 700
#define SPEED_LVL_4 600
#define SPEED_LVL_5 500
#define SPEED_LVL_6 400
#define SPEED_LVL_7 300

#endif /* CONSTANT */
