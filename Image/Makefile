## Simple SDL mini code

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c11 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

OBJ= main.o segmentation.o utils.o block.o pixel_operations.o
DEP= ${SRC:.o=.d}

all: main

main: ${OBJ}

clean:
	${RM} ${OBJ} ${DEP} *~
	${RM} main

-include ${DEP}

# END