#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <string.h>
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

int buscar_cadastro(Paciente cadastros[], int total_cadastros, long long cpf) {
    int pos;
    int passos;

    if (total_cadastros == 0) {
        return -1;
    }

    pos = (int)(cpf % tam_hash);

    for (passos = 0; passos < tam_hash; passos++) {
        if (tabela_hash[pos] == -1) {
            return -1;
        }
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

int esta_na_fila(int indice) {
    int i;
    for (i = 0; i < total_fila; i++) {
        if (fila[i].indice == indice)   {
            return i;
        }
    }
    return -1;
}

int cadastrar(long long cpf, char nome[], int dia, int mes, int ano) {
    if (buscar_cadastro(cadastros, total_cadastros, cpf) != -1) {
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
    inserir_hash(total_cadastros - 1);    return total_cadastros - 1;
}

int desistir(long long cpf) {
    int i;
    int achei;
    int na_fila;

    achei = buscar_cadastro(cadastros, total_cadastros, cpf);
    if (achei == -1) {
        return -1;
    }

    na_fila = esta_na_fila(achei);
    if (na_fila == -1) {
        return -2;
    }

    for (i = na_fila; i < total_fila - 1; i++) {
        fila[i] = fila[i + 1];
    }
    total_fila--;
    return achei;
}
int main(void) {
    cadastros = malloc(MAX * sizeof(Paciente));
    cap_cadastros = MAX;

    tam_hash = proximo_primo(MAX * 2);
    tabela_hash = malloc(tam_hash * sizeof(int));
    if (tabela_hash == NULL) {
        printf("Sem memoria.\n");
        return 1;
    }
    limpar_hash();
    tam_hash = proximo_primo(MAX * 2);
    tabela_hash = malloc(tam_hash * sizeof(int));
    if (tabela_hash == NULL) {
        printf("Sem memoria.\n");
        return 1;
    }
    limpar_hash();
    fila = malloc(MAX * sizeof(Espera));
    cap_fila = MAX;

    atendidos = malloc(MAX * sizeof(Atendido));
    cap_atendidos = MAX;

    if (cadastros == NULL || fila == NULL || atendidos == NULL) {
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
                    if (total_cadastros * 2 >= tam_hash) {
                        if (rehash(tam_hash * 2) == 0) {
                            printf("Sem memoria.\n");
                            break;
                        }
                    }
                    inserir_hash(total_cadastros - 1);
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

                achei = buscar_cadastro(cadastros, total_cadastros, opcao);

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
                    scanf("%d", &dia);
                    scanf("%d", &mes);
                    scanf("%d", &ano);

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
        free(cadastros);
    free(fila);
    free(atendidos);

    return 0;
}