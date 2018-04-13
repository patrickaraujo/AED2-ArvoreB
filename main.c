/**
*   @author Patrick Araújo: https://github.com/patrickaraujo
*   Árvore B
*/

#include <stdio.h>
#include <stdlib.h>
#include "TAB.h"
#include "TAB.c"

int main(){
	TAB *main = init_tree();
	int num = 0, from, ordem;
	printf("Digite a ordem\n");
	scanf("%i", &ordem);
	printf("Digite um numero para adicionar. 0 para imprimir. -9 para remover e -1 para sair\n");
	while(num != -1){
		scanf("%i", &num);
		if(num == -9){
		  scanf("%d", &from);
		  main = remove_key(main, from, ordem);
		}
		else if(num == -1){
		  printf("\n");
		  print_tree(main, 0);
		  free_tree(main);
		  break;
		}
		else if(!num){
		  printf("\n");
		  print_tree(main, 0);
		}
		else main = insert_key(main, num, ordem);
	}
	return 0;
}
