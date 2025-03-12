CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp background.cpp jturallo.cpp dvasquez4.cpp gregpala.cpp mgonzalez3.cpp ssayedmnasim.cpp
	g++ $(CFLAGS) asteroids.cpp mgonzalez3.cpp gregpala.cpp ssayedmnasim.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o NoEscapeGame

clean:
	rm -f asteroids
	rm -f *.o

