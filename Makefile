CFLAGS = `sdl-config --cflags` -g
LDFLAGS = -lm `sdl-config --libs` -lSDL_gfx
EXEC = BorderGenerator

main:
	gcc main.c map.c linkedlist.c priorityqueue.c voronoi.c graphics.c -o $(EXEC) $(CFLAGS) $(LDFLAGS)
	
clean:
	rm $(EXEC)
