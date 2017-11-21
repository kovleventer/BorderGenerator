#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "map.h"
#include "linkedlist.h"
#include "voronoi.h"

/**
 * Renders everything into the main screen
 * @param main The main Surface to blit everything to
 * @param map The heightmap's surface
 * @param borders The generated borders
 * @param capitals The list of the capitals
 * @param isGenerated Indicates if the data contained on borders is valid and can be rendered
 */
void render(SDL_Surface* main, SDL_Surface* map, SDL_Surface* borders, List* capitals, bool isGenerated);


/**
 * Checks which capital is clicked
 * @param x The clicks position's x coordinate
 * @param y The clicks position's y coordinate
 * @param l The list of the capitals
 * 
 * @return The pointer to the clicked list item; NULL if none is clicked
 */
List* get_clicked(int x, int y, List* l);


/**
 * Contains the main loop
 * Does all initialization and cleanup stuff
 */
void start(void);

#endif // GRAPHICS_H
