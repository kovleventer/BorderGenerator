#include "map.h"

Heightmap read_heightmap(const char* filename, int width, int height) {
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		// Error handling
		fprintf(stderr, "Error while opening file\n");
		exit(1);
	}
	
	// Memory allocation is done in three parts
	PixelData*** array = (PixelData***)malloc(sizeof(PixelData**) * width);
	for (int x = 0; x < width; x++) {
		array[x] = (PixelData**)malloc(sizeof(PixelData*) * height);
	}
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// File reading, memory allocation and data initializing are done in the same loop
			unsigned char buf[2];
			fread(&buf, 1, 2, f);
			
			// Hegihtmap data in the srtm file is big endian
			short current = ((short)buf[1]) | (((short)buf[0]) << 8);
			
			array[x][y] = (PixelData*)malloc(sizeof(PixelData));
			*(array[x][y]) = (PixelData){ .height = current,
							.position = (Point){ .x = x, .y = y },
							.distance = 0,
							.closest = NULL };
		}
	}
	
	fclose(f);
	return (Heightmap){ .width = width, .height = height, .data = array };
}

void free_heightmap(Heightmap heightmap) {
	// Memory freeing in three parts
	for (int y = 0; y < heightmap.height; y++) {
		for (int x = 0; x < heightmap.width; x++) {
			free(heightmap.data[x][y]);
		}
	}
	for (int x = 0; x < heightmap.width; x++) {
		free(heightmap.data[x]);
	}
	free(heightmap.data);
}

SDL_Surface* heightmap_to_surface(Heightmap heightmap) {
	int w = heightmap.width;
	int h = heightmap.height;
	
	// Palette generation
	// Code from InfoC advent calendar
	// Generates gradients between explicitly given points
	// Max height is limited to 2048
	SDL_Color pal[2048];
	struct {
		int pos;
		SDL_Color col;
	} colors[] = { 
		// Different heights are marked with different colors
		{   0,       {   0, 128,   0 } },
		{ 128,       {   0, 200,   0 } },
		{ 256,       {  64, 200,   0 } },
		{ 512,       { 200, 200,   0 } },
		{ 1024,      { 139,  69,  19 } },
		{ 2048,      { 200, 200, 200 } },
		{ -1 }
	};
	int i, j;
	for (i = 0; colors[i].pos != -1; ++i) {
		int p1 = colors[i].pos, p2 = colors[i+1].pos;
		SDL_Color c1 = colors[i].col, c2 = colors[i+1].col;
		for (j = p1; j <= p2; j++) {
			pal[j].r = c1.r + (c2.r - c1.r) * (j - p1) / (p2 - p1);
			pal[j].g = c1.g + (c2.g - c1.g) * (j - p1) / (p2 - p1);
			pal[j].b = c1.b + (c2.b - c1.b) * (j - p1) / (p2 - p1);
		}
	}
	
	SDL_Surface* newSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	if (newSurface == NULL) {
		// Error handling
		fprintf(stderr, "Can not create surface\n");
	}
	
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int h = heightmap.data[x][y]->height;
			if (h >= 0 && h <= 2048) {
				// Only valid height are converted into pixels, the rest are black
				SDL_Color c = pal[h];
				pixelRGBA(newSurface, x, y, c.r, c.g, c.b, 255);
			}
		}
	}
	
	return newSurface;
}

List* read_capitals(const char* filename) {
	FILE* f = fopen(filename, "r");
	int x, y, r, g, b;
	List* list = NULL;
	while (fscanf(f, "%d %d %d %d %d", &x, &y, &r, &g, &b) != EOF) {
		Point pos = { .x = x, .y = y };
		SDL_Color col = { .r = r, .g = g, .b = b };
		Capital capital = { .position = pos, .borderColor = col };
		list = ll_add_item(list, capital);
	}
	fclose(f);
	return list;
}

void save_capitals(const char* filename, List* capitals) {
	FILE* f = fopen(filename, "w");
	for (List* iter = capitals; iter != NULL; iter = iter->next) {
		fprintf(f, "%d %d %d %d %d\n", 
				iter->capital.position.x,
				iter->capital.position.y,
				iter->capital.borderColor.r,
				iter->capital.borderColor.g,
				iter->capital.borderColor.b);
	}
	fclose(f);
}
