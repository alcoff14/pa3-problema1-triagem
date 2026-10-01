#include <stdlib.h>
#include "cadastro.h"
#include "fila.h"

Espera *fila;
int total_fila = 0;
int cap_fila = 0;

Atendido *atendidos;
int total_atendidos = 0;
int cap_atendidos = 0;

int eventos = 0;

int vem_antes(int a, int b) {
    if (fila[a].prioridade < fila[b].prioridade) {
        return 1;
    }
    if (fila[a].prioridade == fila[b].prioridade &&
        fila[a].chegada < fila[b].chegada) {
        return 1;
    }
    return 0;
}

void trocar(int a, int b) {
    Espera guardado;

    guardado = fila[a];
    fila[a] = fila[b];
    fila[b] = guardado;
}

void subir(int i) {
    int pai;

    while (i > 0) {
        pai = (i - 1) / 2;
        if (vem_antes(i, pai) == 0) {
            break;
        }
        trocar(i, pai);
        i = pai;
    }
}

void descer(int i) {
    int esq;
    int dir;
    int melhor;

    while (1) {
        esq = 2 * i + 1;
        dir = 2 * i + 2;
        melhor = i;

        if (esq < total_fila && vem_antes(esq, melhor) == 1) {
            melhor = esq;
        }
        if (dir < total_fila && vem_antes(dir, melhor) == 1) {
            melhor = dir;
        }
        if (melhor == i) {
            break;
        }
        trocar(i, melhor);
        i = melhor;
    }
}

int dar_entrada(int indice, int prioridade) {
    if (total_fila == cap_fila) {
        Espera *novo = realloc(fila, cap_fila * 2 * sizeof(Espera));
        if (novo == NULL) {
            return 0;
        }
        fila = novo;
        cap_fila = cap_fila * 2;
    }
    fila[total_fila].indice = indice;
    fila[total_fila].prioridade = prioridade;
    fila[total_fila].chegada = eventos;
    total_fila++;
    subir(total_fila - 1);
    return 1;
}

int chamar_proximo(void) {
    int indice;

    if (total_fila == 0) {
        return -1;
    }

    if (total_atendidos == cap_atendidos) {
        Atendido *novo = realloc(atendidos, cap_atendidos * 2 * sizeof(Atendido));
        if (novo == NULL) {
            return -1;
        }
        atendidos = novo;
        cap_atendidos = cap_atendidos * 2;
    }

    indice = fila[0].indice;
    atendidos[total_atendidos].indice = indice;
    atendidos[total_atendidos].prioridade = fila[0].prioridade;
    atendidos[total_atendidos].espera = eventos - fila[0].chegada;
    total_atendidos++;

    total_fila--;
    fila[0] = fila[total_fila];
    descer(0);
    return indice;
}

int tamanho_fila(void) {
    return total_fila;
}

int esta_na_fila(int indice) {
    int i;

    for (i = 0; i < total_fila; i++) {
        if (fila[i].indice == indice) {
            return i;
        }
    }
    return -1;
}

int desistir(long long cpf) {
    int achei;
    int na_fila;

    achei = buscar_cadastro(cpf);
    if (achei == -1) {
        return -1;
    }

    na_fila = esta_na_fila(achei);
    if (na_fila == -1) {
        return -2;
    }

    total_fila--;
    if (na_fila < total_fila) {
        fila[na_fila] = fila[total_fila];
        subir(na_fila);
        descer(na_fila);
    }
    return achei;
}

int iniciar_fila(void) {
    fila = malloc(MAX * sizeof(Espera));
    cap_fila = MAX;

    atendidos = malloc(MAX * sizeof(Atendido));
    cap_atendidos = MAX;

    if (fila == NULL || atendidos == NULL) {
        return 0;
    }
    return 1;
}

void encerrar_fila(void) {
    free(fila);
    free(atendidos);
}
