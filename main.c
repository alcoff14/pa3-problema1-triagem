#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#define MAX 100

typedef struct {
    long long cpf;
    char nome[50];
    int dia, mes, ano;
} Paciente;

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

Paciente *cadastros;
int total_cadastros = 0;
int cap_cadastros = 0;

Espera *fila;
int total_fila = 0;
int cap_fila = 0;
Atendido *atendidos;
int total_atendidos = 0;
int cap_atendidos = 0;

int eventos = 0;
int buscar_cadastro(Paciente cadastros[], int total_cadastros, long long cpf) {
    int i;
    for (i = 0; i < total_cadastros; i++) {
        if (cadastros[i].cpf == cpf) {
            return i;
        }
    }
    return -1;
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
    return 1;
}

int proximo_da_fila(void) {
    int i;
    int melhor;

    if (total_fila == 0) {
        return -1;
    }
    melhor = 0;
    for (i = 1; i < total_fila; i++) {
        if (fila[i].prioridade < fila[melhor].prioridade) {
            melhor = i;
        } else if (fila[i].prioridade == fila[melhor].prioridade &&
                   fila[i].chegada < fila[melhor].chegada) {
            melhor = i;
        }
    }
    return melhor;
}

int chamar_proximo(void) {
    int i;
    int melhor;
    int indice;

    melhor = proximo_da_fila();
    if (melhor == -1) {
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

    indice = fila[melhor].indice;
    atendidos[total_atendidos].indice = indice;
    atendidos[total_atendidos].prioridade = fila[melhor].prioridade;
    atendidos[total_atendidos].espera = eventos - fila[melhor].chegada;
    total_atendidos++;

    for (i = melhor; i < total_fila - 1; i++) {
        fila[i] = fila[i + 1];
    }
    total_fila--;
    return indice;
}

int tamanho_fila(void) {
    return total_fila;
}
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
int main(void) {
    cadastros = malloc(MAX * sizeof(Paciente));
    cap_cadastros = MAX;

    fila = malloc(MAX * sizeof(Espera));
    cap_fila = MAX;

    atendidos = malloc(MAX * sizeof(Atendido));
    cap_atendidos = MAX;

    if (cadastros == NULL || fila == NULL || atendidos == NULL) {
        printf("Sem memoria.\n");
        return 1;
    }
    int opcao = -1;
    
    while (opcao != 0) {
        printf ("\n___TRIAGEM___\n");               
        printf ("1. Cadastrar paciente\n");           
        printf ("2. Fila\n");
        printf ("3. Chamar proximo paciente\n");
        printf ("4. Relatorio\n");
        printf ("5. Buscar cadastro\n");
        printf ("6. Desistir da fila\n");
        printf ("0. Sair\n");
        printf ("Escolha: ");
        scanf("%d", &opcao);

        eventos++;

        switch (opcao) {
        case 1: {
            long long cpf;
            int achei = -1;
            int i;

            printf ("informe o CPF: ");
            scanf("%lld", &cpf);

            for (i = 0; i < total_cadastros; i++) {
                if (cadastros[i].cpf == cpf) {
                    achei = i ;
                    break;
                }
            }
            if (achei == -1) {
                               if (total_cadastros == cap_cadastros) {
                    Paciente *novo = realloc(cadastros, cap_cadastros * 2 * sizeof(Paciente));
                    if (novo == NULL) {
                        printf("Sem memoria.\n");
                        break;
                    }
                    cadastros = novo;
                    cap_cadastros = cap_cadastros * 2;
                }
                printf("CPF nao cadastrado. Vamos cadastrar.\n");
                printf("Primeiro nome: ");
                scanf("%49s", cadastros[total_cadastros].nome);
                printf("Nascimento (dia mes ano): ");
                scanf("%d", &cadastros[total_cadastros].dia);
                scanf("%d", &cadastros[total_cadastros].mes);
                scanf("%d", &cadastros[total_cadastros].ano);
            cadastros[total_cadastros].cpf = cpf;
            achei = total_cadastros;
            total_cadastros++;
            printf("Cadastro criado.\n");
            } else { 
                printf("Cadastro Existente: %s\n", cadastros[achei].nome);
            }
            int ja_ta_na_fila = 0; 
            for (i = 0; i < total_fila; i++)  {
                if (fila[i].indice == achei) {
                        ja_ta_na_fila = 1; 
                        break; 
                }
            }
            if (ja_ta_na_fila) {
                printf("Esse paciente ja esta na fila.\n");
                break;
                        }
            if (total_fila == cap_fila) {
                Espera *novo = realloc(fila, cap_fila * 2 * sizeof(Espera));
                if (novo == NULL) {
                    printf("Sem memoria.\n");
                    break;
                }
                fila = novo;
                cap_fila = cap_fila * 2;
            }
            int prioridade = 0; 
                            printf("Nivel de prioridade:\n");
                printf("1 - Emergencia\n");
                printf("2 - Muito urgente\n");
                printf("3 - Urgente\n");
                printf("4 - Pouco urgente\n");
                printf("5 - Nao urgente\n");
                printf("Escolha: ");
                scanf("%d", &prioridade);

                if (prioridade < 1 || prioridade >5) { 
                    printf("Prioridade invalida. Paciente nao entrou na fila.\n");
                    break;
                }
                fila[total_fila].indice = achei;
                fila[total_fila].prioridade = prioridade;
                fila[total_fila].chegada = eventos; 
                total_fila++; 

                printf("%s entrou na fila com prioridade %d. \n",
                    cadastros[achei].nome, prioridade);
                    break; 
                }
                case 2: {
                    int i;
                    printf("\nPacientes aguardando: %d\n", total_fila); 

                    for (i = 0; i < total_fila; i++ ) { 
                        int c = fila [i].indice;
                    printf("%s Prioridade %d Nascimento %02d/%02d/%04d CPF %011lld Espera %d\n",
                        cadastros[c].nome,
                        fila[i].prioridade,
                        cadastros[c].dia, cadastros[c].mes, cadastros[c].ano,
                        cadastros[c].cpf,
                        eventos - fila[i].chegada); 
                }
                    break;
             }
            case 3: {
                int melhor;
                char resposta;

                melhor = proximo_da_fila();
                if (melhor == -1) {
                    printf("A fila esta vazia.\n");
                    break;
                }

                printf("Proximo: %s (prioridade %d). Confirma? (s/n): ",
                       cadastros[fila[melhor].indice].nome, fila[melhor].prioridade);
                scanf(" %c", &resposta);

                if (resposta != 's' && resposta != 'S') {
                    printf("Chamada cancelada.\n");
                    break;
                }

                printf("Chamando %s.\n", cadastros[fila[melhor].indice].nome);
                chamar_proximo();
                break;
            }

                        case 4: {
                int i;
                Atendido *copia;

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

                insertion_sort(copia, total_atendidos);

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
            case 5: {
                long long cpf;
                int achei = -1;

                printf("Informe o CPF: ");
                scanf("%lld", &cpf);

                  achei = buscar_cadastro(cadastros, total_cadastros, cpf);

                if (achei == -1) {
                    printf("Cadastro nao encontrado.\n");
                    break;
                }

                printf("%s Nascimento %02d/%02d/%04d CPF %011lld\n",
                       cadastros[achei].nome,
                       cadastros[achei].dia, cadastros[achei].mes, cadastros[achei].ano,
                       cadastros[achei].cpf);
                break;
            }

            case 6: {
                long long cpf;
                int achei = -1;
                int na_fila = -1;
                int i;

                if (total_fila == 0) {
                    printf("A fila esta vazia.\n");
                    break;
                }

                printf("Informe o CPF de quem desistiu: ");
                scanf("%lld", &cpf);

                for (i = 0; i < total_cadastros; i++) {
                    if (cadastros[i].cpf == cpf) {
                        achei = i;
                        break;
                    }
                }

                if (achei == -1) {
                    printf("Cadastro nao encontrado.\n");
                    break;
                }

                for (i = 0; i < total_fila; i++) {
                    if (fila[i].indice == achei) {
                        na_fila = i;
                        break;
                    }
                }

                if (na_fila == -1) {
                    printf("Esse paciente nao esta na fila.\n");
                    break;
                }

                printf("%s saiu da fila.\n", cadastros[achei].nome);

                for (i = na_fila; i < total_fila - 1; i++) {
                    fila[i] = fila[i + 1];
                }
                total_fila--;
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
                total_fila = 0;
                total_atendidos = 0;

                for (i = 0; i < n; i++) {
                    if (total_cadastros == cap_cadastros) {
                        Paciente *novo = realloc(cadastros, cap_cadastros * 2 * sizeof(Paciente));
                        if (novo == NULL) {
                            printf("Sem memoria.\n");
                            break;
                        }
                        cadastros = novo;
                        cap_cadastros = cap_cadastros * 2;
                    }
                    cadastros[total_cadastros].cpf = 10000000000LL + ((long long)rand() * 32768LL + rand()) % 89999999999LL;
                    sprintf(cadastros[total_cadastros].nome, "P%d", i);
                    cadastros[total_cadastros].dia = 1 + rand() % 28;
                    cadastros[total_cadastros].mes = 1 + rand() % 12;
                    cadastros[total_cadastros].ano = 1950 + rand() % 60;
                    total_cadastros++;
                }
                printf("Cadastros criados: %d\n", total_cadastros);

                quantos = 5000;
                inicio = clock();
                for (i = 0; i < quantos; i++) {
                    buscar_cadastro(cadastros, total_cadastros, cadastros[(int)(((long long)rand() * 32768LL + rand()) % total_cadastros)].cpf);
                }
                tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;
                printf("R2 buscar_cadastro: %d buscas em %.4f s, media %.8f s\n",
                       quantos, tempo, tempo / quantos);

                quantos = 0;
                tempo = 0;
                for (rep = 0; rep < 20; rep++) {
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

            default:
                printf("Opcao invalida.\n");

        }
    }
        free(cadastros);
    free(fila);
    free(atendidos);

    return 0;
}