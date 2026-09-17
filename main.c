#include <stdio.h> 
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

int main(void) { 
    Paciente cadastros[MAX];
    int total_cadastros = 0;

    Espera fila[MAX];
    int total_fila = 0;

    Atendido atendidos[MAX];
    int total_atendidos = 0;
    int eventos = 0;
    int opcao = -1;
    
    while (opcao != 0) {
        printf ("\n___TRIAGEM___\n");               
        printf ("1. Cadastrar paciente\n");           
        printf ("2. Fila\n");
        printf ("3. Chamar proximo paciente\n");
        printf ("4. Relatorio\n");
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
                if (total_cadastros == MAX) { 
                    printf("Cadastro cheio.\n");
                    break; 
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
            if (total_fila == MAX) {
                printf("fila cheia\n");
                break;
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
                int i;
                int melhor;
                char resposta;

                if (total_fila == 0) {
                    printf("A fila esta vazia.\n");
                    break;
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

                printf("Proximo: %s (prioridade %d). Confirma? (s/n): ",
                       cadastros[fila[melhor].indice].nome, fila[melhor].prioridade);
                scanf(" %c", &resposta);

                if (resposta != 's' && resposta != 'S') {
                    printf("Chamada cancelada.\n");
                    break;
                }

                atendidos[total_atendidos].indice = fila[melhor].indice;
                atendidos[total_atendidos].prioridade = fila[melhor].prioridade;
                atendidos[total_atendidos].espera = eventos - fila[melhor].chegada;
                total_atendidos++;

                printf("Chamando %s.\n", cadastros[fila[melhor].indice].nome);

                for (i = melhor; i < total_fila - 1; i++) {
                    fila[i] = fila[i + 1];
                }
                total_fila--;
                break;
            }

            case 4: {
                int i;
                printf("\n=== RELATORIO DO DIA ===\n");
                printf("Atendidos: %d\n", total_atendidos);

                for (i = 0; i < total_atendidos; i++) {
                    int c = atendidos[i].indice;
                    printf("%s Prioridade %d Nascimento %02d/%02d/%04d CPF %011lld Espera %d\n",
                           cadastros[c].nome,
                           atendidos[i].prioridade,
                           cadastros[c].dia, cadastros[c].mes, cadastros[c].ano,
                           cadastros[c].cpf,
                           atendidos[i].espera);
                }
                break;
            }

            case 0:
                printf("Encerrando.\n");
                break;

            default:
                printf("Opcao invalida.\n");

        }
    }
    return 0;
}
