CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp background.cpp jturallo.cpp dvasquez4.cpp gregpala.cpp mgonzalez3.cpp ssayedmnasim.cpp
	g++ $(CFLAGS) asteroids.cpp mgonzalez3.cpp dvasquez4.cpp gregpala.cpp ssayedmnasim.cpp jturallo.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o NoEscapeGame

personal: asteroids.cpp log.cpp timers.cpp background.cpp dvasquez4.cpp
	g++ asteroids.cpp dvasquez4.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o NoEscapeGame
clean:
	rm -f asteroids
	rm -f *.o

