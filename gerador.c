#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cadastro.h"
#include "fila.h"
#include "ordenacao.h"

void medir_operacoes(void) {
    int n;
    int i;
    int quantos;
    int rep;
    clock_t inicio;
    double tempo;
        FILE *arquivo;

    printf("Quantos cadastros? ");
    scanf("%d", &n);
        arquivo = fopen("cadastros_1000000.csv", "r");
    if (arquivo == NULL) {
        printf("Nao foi possivel abrir cadastros_1000000.csv.\n");
        return;
    }

    srand(12345);
    total_cadastros = 0;
    limpar_hash();
    total_fila = 0;
    total_atendidos = 0;

    for (i = 0; i < n; i++) {
        long long cpf;
        char nome[50];
        int dia;
        int mes;
        int ano;

               if (fscanf(arquivo, "%lld,%49[^,],%d,%d,%d", &cpf, nome, &dia, &mes, &ano) != 5) {
            break;
        }
        cadastrar(cpf, nome, dia, mes, ano);
    }
    fclose(arquivo);
    printf("Cadastros criados: %d\n", total_cadastros);

    quantos = 500000;
    inicio = clock();
    for (i = 0; i < quantos; i++) {
        buscar_cadastro(cadastros[(int)(((long long)rand() * 32768LL + rand()) % total_cadastros)].cpf);
    }
    tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    printf("R2 buscar_cadastro: %d buscas em %.4f s, media %.8f s\n",
           quantos, tempo, tempo / quantos);

    quantos = 0;
    tempo = 0;
    for (rep = 0; rep < 2000; rep++) {
        for (i = 0; i < 500; i++) {
            eventos++;
            dar_entrada(i, 1 + rand() % 5);
        }
        inicio = clock();
        for (i = 0; i < 500; i++) {
            eventos++;
            chamar_proximo();
        }
        tempo = tempo + (double)(clock() - inicio) / CLOCKS_PER_SEC;
        quantos = quantos + 500;
    }
    printf("R4 chamar_proximo: %d chamadas (fila de 500) em %.4f s, media %.8f s\n",
           quantos, tempo, tempo / quantos);
}

void medir_ordenacoes(void) {
    int m;
    int i;
    Atendido *a;
    Atendido *b;
    clock_t inicio;
    double t1;
    double t2;

    printf("Quantos registros de atendimento? ");
    scanf("%d", &m);

    srand(54321);

    a = malloc(m * sizeof(Atendido));
    b = malloc(m * sizeof(Atendido));
    if (a == NULL || b == NULL) {
        printf("Sem memoria.\n");
        return;
    }

    for (i = 0; i < m; i++) {
        a[i].indice = 0;
        a[i].prioridade = 1 + rand() % 5;
        a[i].espera = (int)(((long long)rand() * 32768LL + rand()) % 1000000LL);
        b[i] = a[i];
    }

    inicio = clock();
    insertion_sort(a, m);
    t1 = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    inicio = clock();
    merge_sort(b, 0, m - 1);
    t2 = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    printf("M = %d\n", m);
    printf("Insertion sort: %.4f s\n", t1);
    printf("Merge sort:     %.4f s\n", t2);

    free(a);
    free(b);
}

int main(void) {
    int opcao = -1;

    if (iniciar_cadastro() == 0 || iniciar_fila() == 0) {
        printf("Sem memoria.\n");
        return 1;
    }

    while (opcao != 0) {
        printf("\nGERADOR DE CARGA\n");
        printf("1. Medir operacoes (R2 e R4)\n");
        printf("2. Medir ordenacoes (insertion e merge)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                medir_operacoes();
                break;
            case 2:
                medir_ordenacoes();
                break;
            case 0:
                printf("Encerrando.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    }

    encerrar_cadastro();
    encerrar_fila();
    return 0;
}
