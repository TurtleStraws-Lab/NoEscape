CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -g -lrt -lX11 -lGLU -lGL -lglut -pthread -lm -lopenal -lalut  #-lXrandr

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp background.cpp jturallo.cpp dvasquez4.cpp gregpala.cpp mgonzalez3.cpp ssayedmnasim.cpp
	g++ $(CFLAGS) asteroids.cpp mgonzalez3.cpp dvasquez4.cpp gregpala.cpp ssayedmnasim.cpp jturallo.cpp log.cpp timers.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -o NoEscapeGame

clean:
	rm -f NoEscapeGame
	rm -f *.o

