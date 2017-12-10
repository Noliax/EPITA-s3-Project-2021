## Simple SDL mini code

CC=gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags gtk+-3.0` `sdl-config --cflags` -MMD
CFLAGS= -Wall -Wextra -std=c11 -O0 -g
LDFLAGS=
LDLIBS= -lm `sdl-config --libs` `pkg-config --libs gtk+-3.0` -lSDL_image

OBJ= main.o segmentation.o block.o net.o

DEP= ${SRC:.o=.d}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} *~
	${RM} main

-include ${DEP}

# END
