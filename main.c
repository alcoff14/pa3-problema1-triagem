#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "cadastro.h"
#include "fila.h"
#include "ordenacao.h"

int main(void) {
    if (iniciar_cadastro() == 0 || iniciar_fila() == 0) {
        printf("Sem memoria.\n");
        return 1;
    }

    long long opcao = -1;

    while (opcao != 0) {
           printf("\nTRIAGEM\n");
        printf("1. Chamar proximo paciente\n");
        printf("2. Fila\n");
        printf("3. Relatorio\n");
        printf("0. Sair\n");
        printf("Ou digite um CPF (11 digitos)\n");
        printf("Escolha: ");
        scanf("%lld", &opcao);

        eventos++;

        switch (opcao) {
      
                          case 2: {
                int i;
                int acao;
                long long cpf;
                int saiu;

                printf("\nPacientes aguardando: %d\n", tamanho_fila());

                for (i = 0; i < total_fila; i++) {
                    int c = fila[i].indice;
                    printf("%d) %s Prioridade %d Nascimento %02d/%02d/%04d CPF %011lld Espera %d\n",
                           i + 1,
                           cadastros[c].nome,
                           fila[i].prioridade,
                           cadastros[c].dia, cadastros[c].mes, cadastros[c].ano,
                           cadastros[c].cpf,
                           eventos - fila[i].chegada);
                }

                if (total_fila == 0) {
                    break;
                }

                printf("\n1 - remover alguem da fila   0 - voltar\n");
                printf("Escolha: ");
                scanf("%d", &acao);

                if (acao != 1) {
                    break;
                }

                printf("CPF de quem desistiu: ");
                scanf("%lld", &cpf);

                saiu = desistir(cpf);
                if (saiu == -1) {
                    printf("Cadastro nao encontrado.\n");
                } else if (saiu == -2) {
                    printf("Esse paciente nao esta na fila.\n");
                } else {
                    printf("%s saiu da fila.\n", cadastros[saiu].nome);
                }
                break;
            }
                       case 1: {
                char resposta;

                if (total_fila == 0) {
                    printf("A fila esta vazia.\n");
                    break;
                }

                printf("Proximo: %s (prioridade %d). Confirma? (s/n): ",
                       cadastros[fila[0].indice].nome, fila[0].prioridade);
                scanf(" %c", &resposta);

                if (resposta != 's' && resposta != 'S') {
                    printf("Chamada cancelada.\n");
                    break;
                }

                printf("Chamando %s.\n", cadastros[fila[0].indice].nome);
                chamar_proximo();
                break;
            }

                        case 3: {
                int i;
                Atendido *copia;
                int algoritmo;
                printf("\n=== RELATORIO DO DIA ===\n");
                printf("Atendidos: %d\n", total_atendidos);

                if (total_atendidos == 0) {
                    break;
                }

                copia = malloc(total_atendidos * sizeof(Atendido));
                if (copia == NULL) {
                    printf("Sem memoria.\n");
                    break;
                }
                for (i = 0; i < total_atendidos; i++) {
                    copia[i] = atendidos[i];
                }

                                printf("1 - insertion sort  2 - merge sort\n");
                printf("Escolha: ");
                scanf("%d", &algoritmo);

                if (algoritmo == 2) {
                    merge_sort(copia, 0, total_atendidos - 1);
                } else {
                    insertion_sort(copia, total_atendidos);
                }

                for (i = 0; i < total_atendidos; i++) {
                    int c = copia[i].indice;
                    printf("%s Prioridade %d Nascimento %02d/%02d/%04d CPF %011lld Espera %d\n",
                           cadastros[c].nome,
                           copia[i].prioridade,
                           cadastros[c].dia, cadastros[c].mes, cadastros[c].ano,
                           cadastros[c].cpf,
                           copia[i].espera);
                }

                free(copia);
                break;
            }
                        case 8: {
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
                    break;
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
                break;
            }
            case 9: {                
                
                int n;
                int i;
                int quantos;
                int rep;
                clock_t inicio;
                double tempo;

                printf("Quantos cadastros? ");
                scanf("%d", &n);

                srand(12345);
                total_cadastros = 0;
                limpar_hash();                total_fila = 0;
                total_atendidos = 0;

                                for (i = 0; i < n; i++) {
                    long long cpf;
                    char nome[50];
                    int dia;
                    int mes;
                    int ano;

                    cpf = 10000000000LL + ((long long)rand() * 32768LL + rand()) % 89999999999LL;
                    sprintf(nome, "P%d", i);
                    dia = 1 + rand() % 28;
                    mes = 1 + rand() % 12;
                    ano = 1950 + rand() % 60;
                    cadastrar(cpf, nome, dia, mes, ano);
                }
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
                break;
            }
            case 0:
                printf("Encerrando.\n");
                break;
                            default: {
                int achei;
                int prioridade;
                int quer;
                char nome[50];
                int dia;
                int mes;
                int ano;

                if (opcao < 10000000000LL || opcao > 99999999999LL) {
                    printf("Opcao invalida.\n");
                    break;
                }

                               achei = buscar_cadastro(opcao);

                if (achei == -1) {
                    printf("CPF %011lld nao cadastrado.\n", opcao);
                    printf("1 - cadastrar   0 - voltar\n");
                    printf("Escolha: ");
                    scanf("%d", &quer);

                    if (quer != 1) {
                        break;
                    }

                    printf("Primeiro nome: ");
                    scanf("%49s", nome);
                    printf("Nascimento (dia mes ano): ");
                                       scanf("%d %d %d", &dia, &mes, &ano); 

                    achei = cadastrar(opcao, nome, dia, mes, ano);
                    if (achei == -1) {
                        printf("Nao foi possivel cadastrar.\n");
                        break;
                    }
                    printf("Cadastro criado.\n");
                } else {
                    printf("%s Nascimento %02d/%02d/%04d CPF %011lld\n",
                           cadastros[achei].nome,
                           cadastros[achei].dia, cadastros[achei].mes, cadastros[achei].ano,
                           cadastros[achei].cpf);
                }

                if (esta_na_fila(achei) != -1) {
                    printf("esse paciente ja esta na fila.\n");
                    break;
                }

                                printf("\nNivel de prioridade:\n");
                printf("1 - Emergencia\n");
                printf("2 - Muito urgente\n");
                printf("3 - Urgente\n");
                printf("4 - Pouco urgente\n");
                printf("5 - Nao urgente\n");
                printf("0 - nao entrar na fila\n");
                printf("Escolha: ");
                scanf("%d", &prioridade);

                if (prioridade < 1 || prioridade > 5) {
                    printf("Prioridade invalida. Paciente nao entrou na fila.\n");
                    break;
                }

                if (dar_entrada(achei, prioridade) == 0) {
                    printf("Sem memoria.\n");
                    break;
                }

                printf("%s entrou na fila com prioridade %d.\n",
                       cadastros[achei].nome, prioridade);
                break;
            }
        }
    }
    encerrar_cadastro();
    encerrar_fila();
        return 0;
}