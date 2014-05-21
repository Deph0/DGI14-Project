#ifndef SETTINGS_H
#define SETTINGS_H

// Limit to number of frames per second
#define FRAMES_PER_SECOND 30

// Max number of simultaneous drops on the window
#define MAX_NR_DROPS 1000
// Specifying how heavy rain is (integer, 1 ... MAX_NR_DROPS)
#define HEAVINESS_OF_THE_RAIN 5
// Resolution of the window
#define DROPS_SPREADING_FACTOR 1000
// The alpha channel of drop's path color (color is black)
#define DROPS_PATH_ALPHA_MIN 0.02f
#define DROPS_PATH_ALPHA_MAX 0.04f
// When drop is below the window, path will fade out each
// frame with specified constant here
#define DROPS_PATH_FADE_OUT 0.001f
// The raindrop's max and min size
#define DROP_MIN_SIZE 0.01f
#define DROP_MAX_SIZE 0.03f
// The drops with size below that will never move
#define DROP_MOVE_MIN_SIZE 0.02f
// The distance between two drop less than this
// will trigger the collision event
#define COLLISION_DISTANCE 0.03f

#endif // SETTINGS_H

