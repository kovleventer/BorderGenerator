#include "graphics.h"

SDL_Surface* mainScreen;

void render(SDL_Surface* main, SDL_Surface* map, SDL_Surface* borders, List* capitals, bool isGenerated) {
	SDL_BlitSurface(map, NULL, main, NULL);
	
	if (isGenerated) {
		SDL_BlitSurface(borders, NULL, main, NULL);
	}
	
	for (List* iter = capitals; iter != NULL; iter = iter->next) {
		int x = iter->capital.position.x;
		int y = iter->capital.position.y;
		filledCircleRGBA(main, x, y, 10, 255, 0, 0, 255);
	}
	
	stringRGBA(main, 10, 70, "Press 'r' to render the current borders", 0, 0, 255, 255);
	stringRGBA(main, 10, 90, "Press 's' to save the current capitals", 0, 0, 255, 255);
	stringRGBA(main, 10, 110, "Press 'c' to clear the list of capitals", 0, 0, 255, 255);
	
	SDL_Flip(main);
}

List* get_clicked(int x, int y, List* l) {
	for (List* iter = l; iter != NULL; iter = iter->next) {
		
		int dx = iter->capital.position.x - x;
		int dy = iter->capital.position.y - y;
		if (dx * dx + dy * dy < 100) {
			return iter;
		}
	}
	return NULL;
}

void tick(double percent) {
	boxRGBA(mainScreen, 0, 0, percent * 1201 / 100, 50, 0, 0, 255, 255);
	SDL_Flip(mainScreen);
}

void start(void) {
	int scrW = 1920, scrH = 1080;
	
	/*SDL_Surface* mainScreen;*/
	SDL_Surface* mapScreen;
	SDL_Surface* bordersScreen;
	bool isGenerated = false;
	
	char heightmapPath[] = "data/merged.hgta";
	char capitalsPath[] = "data/capitals.txt";
	
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	mainScreen = SDL_SetVideoMode(scrW, scrH, 0, SDL_ANYFORMAT);
	if (!mainScreen) {
		fprintf(stderr, "Can not open window\n");
		exit(1);
	}
	SDL_WM_SetCaption("Border Generator", "Border Generator");
	
	Heightmap heightmap = read_heightmap(heightmapPath, 1201, 1201);
	mapScreen = heightmap_to_surface(heightmap);

	List* capitals = read_capitals(capitalsPath);
	
	bordersScreen = SDL_CreateRGBSurface(0, scrW, scrH, 32, 0, 0, 0, 0);
	SDL_SetAlpha(bordersScreen, SDL_SRCALPHA, 127);
	
	
	render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
	List* clicked = 0;
	bool quit = false;
	SDL_Event ev;
	while(!quit) {
		SDL_WaitEvent(&ev);
		switch (ev.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == SDL_BUTTON_LEFT) {
					clicked = get_clicked(ev.button.x, ev.button.y, capitals);
					if (clicked == NULL) {
						capitals = ll_add_item(capitals, (Capital){ (Point){ .x = ev.button.x, .y = ev.button.y}, (SDL_Color){rand() % 255, rand() % 255, rand() % 255 } });
					}
				} else if (ev.button.button == SDL_BUTTON_RIGHT) {
					capitals = ll_remove_item(capitals, get_clicked(ev.button.x, ev.button.y, capitals));
				}
				isGenerated = false;
				render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
				break;
			case SDL_KEYDOWN:
				switch (ev.key.keysym.sym) {
					case SDLK_s:
						save_capitals(capitalsPath, capitals);
						break;
					case SDLK_r:
						if (!isGenerated) {
							voronoi(heightmap, capitals, bordersScreen);
							isGenerated = true;
							render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
						}
						break;
					case SDLK_c:
						capitals = ll_free_list(capitals);
						isGenerated = false;
						render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
				}
				break;
			case SDL_MOUSEMOTION:
				if (clicked != NULL) {
					int x = ev.motion.x;
					int y = ev.motion.y;
					clicked->capital.position.x = x;
					clicked->capital.position.y = y;
					isGenerated = false;
					render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				clicked = NULL;
				break;
		}
	}
	
	//Cleanup
	ll_free_list(capitals);
	free_heightmap(heightmap);
	
	SDL_Quit();
}
