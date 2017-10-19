# Makefile
 
CC = gcc -fsanitize=address
CPPFLAGS = -MMD
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS =
LDLIBS = -lm

SRC = net.c net_test.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
 
all: net_test
 
net_test: ${OBJ}
 
.PHONY: clean
 
clean:
	${RM} ${OBJ} ${DEP} net_test
 
-include ${DEP}
 
# END
