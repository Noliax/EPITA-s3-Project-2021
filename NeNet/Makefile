# Makefile
 
CC = gcc -fsanitize=address
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS =
LDLIBS = -lm

SRC = net.c 
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
 
all: net
 
net: ${OBJ}
 
.PHONY: clean
 
clean:
	${RM} ${OBJ} ${DEP} net
 
-include ${DEP}
 
# END
