CFLAGS = `sdl-config --cflags` -g
LDFLAGS = -lm `sdl-config --libs` -lSDL_gfx
EXEC = BorderGenerator

main:
	gcc src/main.c src/map.c src/linkedlist.c src/priorityqueue.c src/voronoi.c src/graphics.c -o $(EXEC) $(CFLAGS) $(LDFLAGS)
	
clean:
	rm $(EXEC)
