#ifndef CADASTRO_H
#define CADASTRO_H

#define MAX 100

typedef struct {
    long long cpf;
    char nome[50];
    int dia, mes, ano;
} Paciente;

extern Paciente *cadastros;
extern int total_cadastros;

int iniciar_cadastro(void);
void encerrar_cadastro(void);
void limpar_hash(void);
int cadastrar(long long cpf, char nome[], int dia, int mes, int ano);
int buscar_cadastro(long long cpf);

#endif