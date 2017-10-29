#ifndef MAP_H
#define MAP_H

#include <stdio.h>

#include <SDL.h>
#include <SDL_gfxPrimitives.h>

typedef struct Point {
	int x, y;
} Point;

typedef struct Capital {
	Point position;
	SDL_Color borderColor;
} Capital;

typedef struct PixelData {
	short height;
	Point position;
	int tempClosest;
	int distance;
	Capital* closest;
} PixelData;

typedef struct Heightmap {
	int width, height;
	PixelData*** data;
} Heightmap;

typedef struct List List;

#include "linkedlist.h"

Heightmap read_heightmap(const char* filename, int width, int height);
SDL_Surface* heightmap_to_surface(Heightmap heightmap);
List* read_capitals(const char* filename);
void* save_capitals(const char* filename, List* capitals);

#endif // MAP_H
