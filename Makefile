
CFLAGS = -I ./include
##LIB    = ./libggfonts.so
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: polybius

polybius: polybius.cpp log.cpp timers.cpp andrewH.cpp luisM.cpp adolfoV.cpp chrisR.cpp josephG.cpp classes.cpp
	g++ $(CFLAGS) polybius.cpp log.cpp timers.cpp andrewH.cpp luisM.cpp adolfoV.cpp chrisR.cpp josephG.cpp classes.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -opolybius \
	-D USE_OPENAL_SOUND \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so

clean:
	rm -f polybius
	rm -f *.o
