#include "graphics.h"

#include "functions.h"

void render(SDL_Surface* main, SDL_Surface* map, SDL_Surface* borders, List* capitals, bool isGenerated) {
	// Heightmap
	SDL_BlitSurface(map, NULL, main, NULL);
	
	// Borders
	if (isGenerated) {
		SDL_BlitSurface(borders, NULL, main, NULL);
	}
	
	// Capitals
	for (List* iter = capitals; iter != NULL; iter = iter->next) {
		int x = iter->capital.position.x;
		int y = iter->capital.position.y;
		filledCircleRGBA(main, x, y, 10, 255, 0, 0, 255);
	}
	
	// Information texts
	uint32_t color = 0x0000FFFF;
	stringColor(main, 10, 10, "Press 'r' to render the current borders", color);
	stringColor(main, 10, 30, "Press 's' to save the current capitals", color);
	stringColor(main, 10, 50, "Press 'c' to clear the list of capitals", color);
	stringColor(main, 10, 70, "Press keys 0-4 to change the distance calculation function", color);
	
	SDL_Flip(main);
}

List* get_clicked(int x, int y, List* l) {
	for (List* iter = l; iter != NULL; iter = iter->next) {
		// Implements a search and a distance calculator in one method
		int dx = iter->capital.position.x - x;
		int dy = iter->capital.position.y - y;
		if (dx * dx + dy * dy < 100) {
			return iter;
		}
	}
	return NULL;
}

void start(void) {
	// Declaring variables
	// Screen width and height are aligned to heightmap
	int scrW = 1201 * 8 / 9 + 1;
	int scrH = 1201 * 5 / 9 + 1;
	
	SDL_Surface* mainScreen;
	SDL_Surface* mapScreen;
	SDL_Surface* bordersScreen;
	bool isGenerated = false;
	
	char heightmapPath[] = "data/merged.hgta";
	char capitalsPath[] = "data/capitals.txt";
	
	// Initializing SDL
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	mainScreen = SDL_SetVideoMode(scrW, scrH, 0, SDL_ANYFORMAT);
	if (!mainScreen) {
		fprintf(stderr, "Can not open window\n");
		exit(1);
	}
	SDL_WM_SetCaption("Border Generator", "Border Generator");
	
	srand(time(NULL));
	
	// Initializing variables and data
	Heightmap heightmap = read_heightmap(heightmapPath, scrW, scrH);
	mapScreen = heightmap_to_surface(heightmap);

	List* capitals = read_capitals(capitalsPath);
	
	bordersScreen = SDL_CreateRGBSurface(0, scrW, scrH, 32, 0, 0, 0, 0);
	SDL_SetAlpha(bordersScreen, SDL_SRCALPHA, 127);
	
	
	// Distance calculation functions
	int (*functions[])(int, int) = { no_height, basic_height, delta_height, tanh_height, relu_height };
	int funcIndex = 1;
	
	
	// Mainloop
	render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
	List* clicked = NULL;
	bool quit = false;
	SDL_Event ev;
	while(!quit) {
		SDL_WaitEvent(&ev);
		switch (ev.type) {
			case SDL_QUIT:
				// Quits the program
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == SDL_BUTTON_LEFT) {
					// Left clicking on a point makes it be able to be dragged
					clicked = get_clicked(ev.button.x, ev.button.y, capitals);
					if (clicked == NULL) {
						// If no point is clicked, a new one is inserted with a random color
						capitals = ll_add_item(capitals, (Capital){
							(Point){ .x = ev.button.x, .y = ev.button.y},
							(SDL_Color){rand() % 255, rand() % 255, rand() % 255 }
						});
					}
				} else if (ev.button.button == SDL_BUTTON_RIGHT) {
					// Right clicking on a pont removes it
					capitals = ll_remove_item(capitals, get_clicked(ev.button.x, ev.button.y, capitals));
					// Right clicking nowhere does nothing
				}
				// Clicking possibly alters the capitals and invalidates the generated borders
				isGenerated = false;
				render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
				break;
			case SDL_KEYDOWN:
				switch (ev.key.keysym.sym) {
					case SDLK_s:
						// S saves the capitals to a file
						save_capitals(capitalsPath, capitals);
						break;
					case SDLK_0:
					case SDLK_1:
					case SDLK_2:
					case SDLK_3:
					case SDLK_4:
						isGenerated = isGenerated && ((ev.key.keysym.sym - SDLK_0) == funcIndex);
						funcIndex = ev.key.keysym.sym - SDLK_0;
					case SDLK_r:
						// R generates and renders the borders
						if (!isGenerated) {
							voronoi(heightmap, capitals, bordersScreen, functions[funcIndex]);
							isGenerated = true;
							render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
						}
						break;
					case SDLK_c:
						// C deletes all capitals
						capitals = ll_free_list(capitals);
						isGenerated = false;
						render(mainScreen, mapScreen, bordersScreen, capitals, isGenerated);
				}
				break;
			case SDL_MOUSEMOTION:
				// Drag'n'drop functionality
				// Rendering on mousemotion is rather expensive
				// However since actual rendering is quite fast, it is not a big problem
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
				// The drop part of drag'n'drop
				clicked = NULL;
				break;
		}
	}
	
	// Cleanup
	ll_free_list(capitals);
	free_heightmap(heightmap);
	SDL_FreeSurface(mapScreen);
	SDL_FreeSurface(bordersScreen);
	
	SDL_Quit();
}
