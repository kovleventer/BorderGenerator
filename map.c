#include "map.h"

Heightmap read_heightmap(const char* filename, int width, int height) {
	FILE* f = fopen(filename, "r");
	if (f == NULL) {
		fprintf(stderr, "Error while opening file\n");
		exit(1);
	}
	
	PixelData*** array = (PixelData***)malloc(sizeof(PixelData**) * height);
	for (int y = 0; y < width; y++) {
		array[y] = (PixelData**)malloc(sizeof(PixelData*) * width);
	}
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			unsigned char buf[2];
			fread(&buf, 1, 2, f);
			short current = ((short)buf[1]) | (((short)buf[0]) << 8);
			array[x][y] = (PixelData*)malloc(sizeof(PixelData));
			*(array[x][y]) = (PixelData){ .height = current,
							.position = (Point){ .x = x, .y = y },
							.tempClosest = 0,
							.distance = 0,
							.closest = NULL };
		}
	}
	
	return (Heightmap){ .width = width, .height = height, .data = array };
}

SDL_Surface* heightmap_to_surface(Heightmap heightmap) {
	int w = heightmap.width;
	int h = heightmap.height;
	
	SDL_Surface* newSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	if (newSurface == NULL) {
		fprintf(stderr, "Can not create surface\n");
	}
	
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			unsigned char r = heightmap.data[x][y]->height;
			pixelRGBA(newSurface, x, y, r, 0, 0, 255);
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

void* save_capitals(const char* filename, List* capitals) {
	FILE* f = fopen(filename, "w");
	for (List* iter = capitals; iter != NULL; iter = iter->next) {
		fprintf(f, "%d %d %d %d %d\n", iter->capital.position.x, iter->capital.position.y, iter->capital.borderColor.r, iter->capital.borderColor.g, iter->capital.borderColor.b);
	}
	fclose(f);
}
