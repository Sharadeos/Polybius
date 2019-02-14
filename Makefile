CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: asteroids

asteroids: asteroids.cpp log.cpp timers.cpp andrewH.cpp luisM.cpp
	g++ $(CFLAGS) asteroids.cpp log.cpp timers.cpp andrewH.cpp luisM.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -oasteroids

clean:
	rm -f asteroids
	rm -f *.o

