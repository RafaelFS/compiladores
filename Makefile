#
# Makefile
#
# PCS2056 - Linguagens e Compiladores
# Pedro Napolitano Sant’Ana 	8041817
# Rafael Freitas da Silva 	6480585
#
#

DEP=lexico.o sintatico.o token.o
CFLAGS=-Wall -g
LIBS=
CC=gcc

all: main

main: ${DEP} main.o
	${CC} ${CFLAGS} ${LIBS} ${DEP} $@.c -o $@

%.o: %.c
	gcc ${CFLAGS} -c $^

clean:
	rm -f *.o main
