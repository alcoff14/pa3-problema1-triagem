#include <stdio.h> 

int main(void) { 
     int opcao = -1;
   
   
    while (opcao != 0) {
    printf ("\n___TRIAGEM___\n");               
    printf ("1. Cadastrar paciente\n");           
    printf ("2. Fila\n");
    printf ("3. Chamar próximo paciente\n");
    printf ("4. Relatório\n");
    printf ("Escolha: \n");
    scanf("%d", &opcao);
    }
return 0;
}