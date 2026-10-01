#ifndef FILA_H
#define FILA_H

typedef struct {
    int indice;
    int prioridade;
    int chegada;
} Espera;

typedef struct {
    int indice;
    int prioridade;
    int espera;
} Atendido;

extern Espera *fila;
extern int total_fila;

extern Atendido *atendidos;
extern int total_atendidos;

extern int eventos;

int iniciar_fila(void);
void encerrar_fila(void);
int dar_entrada(int indice, int prioridade);
int chamar_proximo(void);
int desistir(long long cpf);
int esta_na_fila(int indice);
int tamanho_fila(void);

#endif
