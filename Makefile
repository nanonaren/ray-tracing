all:
	gcc -lglut -lGLU -o ray_tracing *.c

clean:
	rm -f *.o ray_tracing
