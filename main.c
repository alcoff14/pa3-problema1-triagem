#include <stdio.h>
#include <stdlib.h>
#include "cadastro.h"
#include "fila.h"
#include "ordenacao.h"

void chamar_paciente(void) {
    char resposta;

    if (total_fila == 0) {
        printf("A fila esta vazia.\n");
        return;
    }

    printf("Proximo: %s (prioridade %d). Confirma? (s/n): ",
           cadastros[fila[0].indice].nome, fila[0].prioridade);
    scanf(" %c", &resposta);

    if (resposta != 's' && resposta != 'S') {
        printf("Chamada cancelada.\n");
        return;
    }

    printf("Chamando %s.\n", cadastros[fila[0].indice].nome);
    chamar_proximo();
}

void mostrar_fila(void) {
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
        return;
    }

    printf("\n1 - remover alguem da fila   0 - voltar\n");
    printf("Escolha: ");
    scanf("%d", &acao);

    if (acao != 1) {
        return;
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
}

void mostrar_relatorio(void) {
    int i;
    Atendido *copia;
    int algoritmo;

    printf("\n=== RELATORIO DO DIA ===\n");
    printf("Atendidos: %d\n", total_atendidos);

    if (total_atendidos == 0) {
        return;
    }

    copia = malloc(total_atendidos * sizeof(Atendido));
    if (copia == NULL) {
        printf("Sem memoria.\n");
        return;
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
}

void atender_cpf(long long cpf) {
    int achei;
    int prioridade;
    int quer;
    char nome[50];
    int dia;
    int mes;
    int ano;

    if (cpf < 10000000000LL || cpf > 99999999999LL) {
        printf("Opcao invalida.\n");
        return;
    }

    achei = buscar_cadastro(cpf);

    if (achei == -1) {
        printf("CPF %011lld nao cadastrado.\n", cpf);
        printf("1 - cadastrar   0 - voltar\n");
        printf("Escolha: ");
        scanf("%d", &quer);

        if (quer != 1) {
            return;
        }

        printf("Primeiro nome: ");
        scanf("%49s", nome);
        printf("Nascimento (dia mes ano): ");
        scanf("%d %d %d", &dia, &mes, &ano);

        achei = cadastrar(cpf, nome, dia, mes, ano);
        if (achei == -1) {
            printf("Nao foi possivel cadastrar.\n");
            return;
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
        return;
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
        return;
    }

    if (dar_entrada(achei, prioridade) == 0) {
        printf("Sem memoria.\n");
        return;
    }

    printf("%s entrou na fila com prioridade %d.\n",
           cadastros[achei].nome, prioridade);
}

int main(void) {
    long long opcao = -1;

    if (iniciar_cadastro() == 0 || iniciar_fila() == 0) {
        printf("Sem memoria.\n");
        return 1;
    }

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
            case 1:
                chamar_paciente();
                break;
            case 2:
                mostrar_fila();
                break;
            case 3:
                mostrar_relatorio();
                break;
            case 0:
                printf("Encerrando.\n");
                break;
            default:
                atender_cpf(opcao);
                break;
        }
    }

    encerrar_cadastro();
    encerrar_fila();
    return 0;
}
