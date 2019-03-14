CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: polybius

polybius: polybius.cpp log.cpp timers.cpp andrewH.cpp luisM.cpp adolfoV.cpp chrisR.cpp josephG.cpp
	g++ $(CFLAGS) polybius.cpp log.cpp timers.cpp andrewH.cpp luisM.cpp adolfoV.cpp chrisR.cpp josephG.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -opolybius

clean:
	rm -f polybius
	rm -f *.o

