#include <stdlib.h>
#include "ordenacao.h"

void insertion_sort(Atendido v[], int n) {
    int i;
    int j;
    Atendido atual;

    for (i = 1; i < n; i++) {
        atual = v[i];
        j = i - 1;
        while (j >= 0 && v[j].espera < atual.espera) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = atual;
    }
}

void intercalar(Atendido v[], int inicio, int meio, int fim) {
    int i;
    int j;
    int k;
    int tamanho;
    Atendido *temp;

    tamanho = fim - inicio + 1;
    temp = malloc(tamanho * sizeof(Atendido));
    if (temp == NULL) {
        return;
    }

    i = inicio;
    j = meio + 1;
    k = 0;

    while (i <= meio && j <= fim) {
        if (v[i].espera >= v[j].espera) {
            temp[k] = v[i];
            i++;
        } else {
            temp[k] = v[j];
            j++;
        }
        k++;
    }

    while (i <= meio) {
        temp[k] = v[i];
        i++;
        k++;
    }

    while (j <= fim) {
        temp[k] = v[j];
        j++;
        k++;
    }

    for (k = 0; k < tamanho; k++) {
        v[inicio + k] = temp[k];
    }

    free(temp);
}

void merge_sort(Atendido v[], int inicio, int fim) {
    int meio;

    if (inicio >= fim) {
        return;
    }

    meio = (inicio + fim) / 2;
    merge_sort(v, inicio, meio);
    merge_sort(v, meio + 1, fim);
    intercalar(v, inicio, meio, fim);
}
