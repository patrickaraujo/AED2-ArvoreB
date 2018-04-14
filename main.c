/**
*   @author Patrick Araújo: https://github.com/patrickaraujo
*   Árvore B
*/

#include <stdio.h>
#include <stdlib.h>
#include "TAB.h"
#include "TAB.c"

int main(){
	TAB *main = inicializa();
	int num = 0, from, ordem;
	printf("Digite a ordem\n");
	scanf("%i", &ordem);
	printf("Digite um numero para adicionar. 0 para imprimir. -9 para remover e -1 para sair\n");
	while(num != -1){
		scanf("%i", &num);
		if(num == -9){
		  scanf("%d", &from);
		  main = remocao(main, from, ordem);
		}
		else if(num == -1){
		  printf("\n");
		  imprimir(main, 0);
		  liberaArvore(main);
		  break;
		}
		else if(!num){
		  printf("\n");
		  imprimir(main, 0);
		}
		else
            main = insercao(main, num, ordem);
	}
	return 0;
}
