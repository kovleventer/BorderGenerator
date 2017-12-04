#ifndef VORONOI_H
#define VORONOI_H

#include <limits.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "map.h"


/**
 * @struct Neighbors
 * Stores information about a pixels neighbor pixels
 * Contains an inner struct
 * Since the maximum number of neighbors is 8, it is safe to store them directly
 * If the examined pixel is at the edge of the screen, there will be fewer neighbors, member size notes this
 * The difference between diagonal and non-diagonal pixels are marked
 */
typedef struct Neighbors {
	struct Neighbor {
		PixelData* pd;
		bool isDiagonal;
	} n[8];
	int size;
} Neighbors;


/**
 * Resets the pathfinding related members of the heightmap
 * @param heightmap The heightmap to partially reset
 */
void reset_distances(Heightmap heightmap);


/**
 * Retrieves the neighbors of a pixel
 * @param heightmap The heightmap for referece
 * @param current The current pixel
 * 
 * @return The neighbors in a specific struct
 */
Neighbors get_neighbors(Heightmap heightmap, PixelData* current);


/**
 * Generates the distances and the closest capitals to each pixel
 * This information is written back to the heightmap
 * @param heightmap The heightmap on which the pathfinding is based
 * @param capitals The list of the capitals
 * @param function The distance calculation method's function pointer
 */
void generate_distances(Heightmap heightmap, List* capitals, int (*function)(int, int));


/**
 * Generates the voronoi diagram based on the generated distances and closest capitals
 * Calls generate_distances
 * @param heightmap The heightmap
 * @param capitals Only passed to generate_distances
 * @param target The SDL_Surface to render to
 * @param function The distance calculation method's function pointer
 */
void voronoi(Heightmap heightmap, List* capitals, SDL_Surface* target, int (*function)(int, int));

#endif // VORONOI_H
