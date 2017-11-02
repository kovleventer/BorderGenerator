#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

#include "map.h"
#include "linkedlist.h"
#include "voronoi.h"

void render(SDL_Surface* main, SDL_Surface* map, SDL_Surface* borders, List* capitals, bool isGenerated);
List* get_clicked(int x, int y, List* l);
void tick(double percent);
void start(void);

#endif // GRAPHICS_H
