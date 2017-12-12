## Simple SDL mini code

CC=gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags gtk+-3.0` `sdl-config --cflags` -MMD -D_XOPEN_SOURCE=500
CFLAGS= -Wall -Wextra -std=c11 -O0 -g
LDFLAGS=
LDLIBS= -lm `sdl-config --libs` `pkg-config --libs gtk+-3.0` -lSDL_image -lm -export-dynamic -rdynamic

OBJ=  ../merge/segmentation.o ../merge/block.o ../merge/net.o

DEP= ${SRC:.o=.d}

all: main

main: ${OBJ}

clean:
		${RM} ${OBJ} ${DEP}
		${RM} main
		${RM} *.d *.o

-include ${DEP}

# END
