#include "graphics.h"

void render(SDL_Surface* main, SDL_Surface* map, SDL_Surface* borders, List* capitals, bool isGenerated) {
	SDL_BlitSurface(map, NULL, main, NULL);
	
	if (isGenerated) {
		SDL_BlitSurface(borders, NULL, main, NULL);
	}
	
	for (List* iter = capitals; iter != NULL; iter = iter->next) {
		int x = iter->capital.position.x;
		int y = iter->capital.position.y;
		filledCircleRGBA(main, x, y, 10, 255, 255, 255, 255);
	}
	
	SDL_Flip(main);
}

void start(void) {
	int scrW = 1920, scrH = 1080;
	
	SDL_Surface* mainScreen;
	SDL_Surface* mapScreen;
	SDL_Surface* bordersScreen;
	bool isGenerated = false;
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	mainScreen = SDL_SetVideoMode(scrW, scrH, 0, SDL_ANYFORMAT);
	if (!mainScreen) {
		fprintf(stderr, "Can not open window\n");
		exit(1);
	}
	
	Heightmap heightmap = read_heightmap("data/merged.hgta", 1201, 1201);
	mapScreen = heightmap_to_surface(heightmap);

	List* capitals = read_capitals("data/capitals.txt");
	
	bordersScreen = SDL_CreateRGBSurface(0, scrW, scrH, 32, 0, 0, 0, 0);
	SDL_SetAlpha(bordersScreen, SDL_SRCALPHA, 127);
	
	
	render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
	bool quit = false;
	SDL_Event ev;
	while(!quit) {
		SDL_WaitEvent(&ev);
		switch (ev.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				capitals = ll_add_item(capitals, (Capital){ (Point){ .x = ev.button.x, .y = ev.button.y}, (SDL_Color){rand() % 255, rand() % 255, rand() % 255 } });
				isGenerated = false;
				render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
				break;
			case SDL_KEYDOWN:
				switch (ev.key.keysym.sym) {
					case SDLK_s:
						save_capitals("data/capitals.txt", capitals);
						break;
					case SDLK_r:
						voronoi(heightmap, capitals, bordersScreen);
						isGenerated = true;
						render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
				}
		}
	}
}
