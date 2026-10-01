CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
MODULOS = cadastro.c fila.c ordenacao.c
CABECALHOS = cadastro.h fila.h ordenacao.h

all: triagem.exe gerador.exe

triagem.exe: main.c $(MODULOS) $(CABECALHOS)
	$(CC) $(CFLAGS) main.c $(MODULOS) -o triagem.exe

gerador.exe: gerador.c $(MODULOS) $(CABECALHOS)
	$(CC) $(CFLAGS) gerador.c $(MODULOS) -o gerador.exe
