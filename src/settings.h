#ifndef SETTINGS_H
#define SETTINGS_H

// Limit to number of frames per second
#define FRAMES_PER_SECOND 30

// Max number of simultaneous drops on the window
#define MAX_NR_DROPS 200
// Resolution of the window
#define DROPS_SPREADING_FACTOR 1000
// Percentage of drops having path (approximately)
#define DROPS_PATH_TRACE_PERCENT 20
// The alpha channel of drop's path color (color is black)
#define DROPS_PATH_ALPHA 0.08f
// When drop is below the window, path will fade out each
// frame with specified constant here
#define DROPS_PATH_FADE_OUT 0.001f
// The raindrop can't be bigger than this
#define DROP_MAX_SCALE 0.04f

#endif // SETTINGS_H

