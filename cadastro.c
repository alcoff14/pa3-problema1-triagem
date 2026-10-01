#include <stdlib.h>
#include <string.h>
#include "cadastro.h"

Paciente *cadastros;
int total_cadastros = 0;
int cap_cadastros = 0;

int *tabela_hash;
int tam_hash = 0;

int eh_primo(int n) {
    int i;

    if (n < 2) {
        return 0;
    }
    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int proximo_primo(int n) {
    while (eh_primo(n) == 0) {
        n++;
    }
    return n;
}

void limpar_hash(void) {
    int i;

    for (i = 0; i < tam_hash; i++) {
        tabela_hash[i] = -1;
    }
}

void inserir_hash(int indice) {
    int pos;

    pos = (int)(cadastros[indice].cpf % tam_hash);
    while (tabela_hash[pos] != -1) {
        pos++;
        if (pos == tam_hash) {
            pos = 0;
        }
    }
    tabela_hash[pos] = indice;
}

int rehash(int novo_tam) {
    int *nova;
    int i;

    novo_tam = proximo_primo(novo_tam);
    nova = malloc(novo_tam * sizeof(int));
    if (nova == NULL) {
        return 0;
    }

    free(tabela_hash);
    tabela_hash = nova;
    tam_hash = novo_tam;
    limpar_hash();

    for (i = 0; i < total_cadastros; i++) {
        inserir_hash(i);
    }
    return 1;
}

int buscar_cadastro(long long cpf) {
    int pos;

    pos = (int)(cpf % tam_hash);
    while (tabela_hash[pos] != -1) {
        if (cadastros[tabela_hash[pos]].cpf == cpf) {
            return tabela_hash[pos];
        }
        pos++;
        if (pos == tam_hash) {
            pos = 0;
        }
    }
    return -1;
}

int cadastrar(long long cpf, char nome[], int dia, int mes, int ano) {
    if (buscar_cadastro(cpf) != -1) {
        return -1;
    }
    if (total_cadastros == cap_cadastros) {
        Paciente *novo = realloc(cadastros, cap_cadastros * 2 * sizeof(Paciente));
        if (novo == NULL) {
            return -1;
        }
        cadastros = novo;
        cap_cadastros = cap_cadastros * 2;
    }
    cadastros[total_cadastros].cpf = cpf;
    strcpy(cadastros[total_cadastros].nome, nome);
    cadastros[total_cadastros].dia = dia;
    cadastros[total_cadastros].mes = mes;
    cadastros[total_cadastros].ano = ano;
    total_cadastros++;

    if (total_cadastros * 2 >= tam_hash) {
        if (rehash(tam_hash * 2) == 0) {
            return -1;
        }
    }
    inserir_hash(total_cadastros - 1);
    return total_cadastros - 1;
}

int iniciar_cadastro(void) {
    cadastros = malloc(MAX * sizeof(Paciente));
    cap_cadastros = MAX;

    tam_hash = proximo_primo(MAX * 2);
    tabela_hash = malloc(tam_hash * sizeof(int));

    if (cadastros == NULL || tabela_hash == NULL) {
        return 0;
    }
    limpar_hash();
    return 1;
}

void encerrar_cadastro(void) {
    free(cadastros);
    free(tabela_hash);
}
