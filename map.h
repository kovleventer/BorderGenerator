#ifndef MAP_H
#define MAP_H

#include <stdio.h>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>


/**
 * @struct Point
 * Represents a two dimensional point in an eucledian plane in a descartes coordinate system
 */
typedef struct Point {
	int x, y;
} Point;


/**
 * @struct Capital
 * Represents a Capital (big red dot on the screen)
 * The member 'borderColor' indicates the color of the corresponding territory
 */
typedef struct Capital {
	Point position;
	SDL_Color borderColor;
} Capital;


/**
 * @struct PixelData
 * Represents a pixel
 * Each pixel has a height value, which forms the heightmap
 * Each pixel knows its position, so its neighbors can be referenced easily
 * Members 'distance' and 'closest' are only relevant, when pathfinding runs
 */
typedef struct PixelData {
	short height;
	Point position;
	int distance;
	Capital* closest;
} PixelData;


/**
 * @struct Heightmap
 * Represents the whole heightmap
 * Consists of a two dimensional array to pointers to pixels
 * Width and height are also stored
 */
typedef struct Heightmap {
	int width, height;
	PixelData*** data;
} Heightmap;


// Forward declaration
typedef struct List List;
#include "linkedlist.h"


/**
 * Reads the heightmap from a file
 * Due to the file format (srtm), the file does not contain information about the width or the height
 * @param filename The file's path
 * @param width The width of the heightmap
 * @param height The height of the heightmap (NOTE that the two 'heights' note different dimensions)
 * 
 * @return The heightmap with dynamically allocated data
 */
Heightmap read_heightmap(const char* filename, int width, int height);


/**
 * Frees the dynamically allocated data of the heightmap
 * @param heightmap The heightmap
 */
void free_heightmap(Heightmap heightmap);


/**
 * Converts the heighmap into an SDL_Surface for visual indication
 * @param heighmap The heighmap to convert
 * 
 * @return The resulting (created) SDL_Surface
 */
SDL_Surface* heightmap_to_surface(Heightmap heightmap);


/**
 * Reads all information about capitals from a textfile
 * File pattern: 'x y r g b' where x and y belong to its position and r, g and b to its color
 * @param filename The file's path
 * 
 * @return A dynamically allocated linked list containing all capital info
 */
List* read_capitals(const char* filename);


/**
 * Saves back all capital info to the given file
 * File pattern is the same as above
 * @param filename The file's path to write into
 * @param capitals The list to save
 */
void save_capitals(const char* filename, List* capitals);

#endif // MAP_H
