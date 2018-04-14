#include <stdio.h>
#include <stdlib.h>
#include "TAB.h"

TAB* inicializa(){
	return NULL;
}

TAB* novaPagina(int ordem){
	TAB* temp = (TAB*) malloc(sizeof(TAB));
	temp->n_chaves = 0;
	temp->chaves = (int*) malloc(sizeof(int) * ((2*ordem) - 1));
	temp->folha = 1;
	temp->filhos = (TAB**) malloc(sizeof(TAB) * 2 * ordem);
	int i;
	for(i = 0; i < (2 * ordem); i++)
        temp->filhos[i] = NULL;
	return temp;
}

TAB* liberaArvore( TAB* main ){
	if(!main)
		return NULL;
	if(!main->folha){
		int i;
		for(i = 0; i <= main->n_chaves; i++)
			liberaArvore(main->filhos[i]);
	}
	free(main->filhos);
	free(main->chaves);
	free(main);
	return NULL;
}

void imprimir( TAB* main, int altura ){
	if(main){
        int i, j;
        for(i = 0; i <= main->n_chaves - 1; i++){
            imprimir(main->filhos[i], altura + 1);
            for(j = 0; j <= altura; j++)
                printf("    ");
            printf("%d\n", main->chaves[i]);
        }
        imprimir(main->filhos[i], altura + 1);
	}
}

TAB* busca( TAB* main, int num ){
	if(!main)
		return NULL;
	int i = 0;
	while(i < main->n_chaves && num > main->chaves[i])
        i++;
	if(i < main->n_chaves && num == main->chaves[i])
		return main;
	if(main->folha)
		return NULL;
	return ( busca(main->filhos[i], num) );
}

TAB* dividirPagina( TAB* x, TAB* y, int index, int ordem ){
	// Divide node y into node x->(z,y), where x will be the father,
	// z will take ordem-1 first chaves and y will take the remaining filhos
	TAB* z = novaPagina(ordem);
	z->n_chaves = ordem - 1;
	z->folha = y->folha;

	// Passing the first ordem-1 chaves to z as well as the ordem first filhos
	int j;
	for(j = 0; j < ordem - 1; j++)
		z->chaves[j] = y->chaves[j + ordem];
	if(!y->folha)
		for(j = 0; j < ordem; j++){
			z->filhos[j] = y->filhos[j+ordem];
			y->filhos[j + ordem] = NULL;
		}

	// Adjusting the filhos from x to add the new child z on index pos
	y->n_chaves = ordem - 1;
	for(j = x->n_chaves; j >= index; j--)
		x->filhos[j + 1] = x->filhos[j];
	x->filhos[index] = z;

	// Adjusting the chaves from x to add the key ordem-1 from y on index-1
	for(j = x->n_chaves; j >= index; j--)
		x->chaves[j] = x->chaves[j - 1];
	x->chaves[index - 1] = y->chaves[ordem - 1];
	x->n_chaves++;

	return x;
}

TAB* efetuaInsercao( TAB* x, int num, int ordem ){
	int i = x->n_chaves - 1;

	// If the node is a folha, then the num is inserted
	if(x->folha){
		while((i >= 0) && (num < x->chaves[i])){
			x->chaves[i + 1] = x->chaves[i];
			i--;
		}
		x->chaves[i + 1] = num;
		x->n_chaves++;
		return x;
	}

	// Finding the child where num must be inserted
	// If the child has reached chaves limit, then it is divided and the
	//		new num "root" will be added in x
	while((i >= 0) && (num < x->chaves[i])) i--;
	i++;
	if(x->filhos[i]->n_chaves == (2 * ordem) - 1){
		x = dividirPagina(x, x->filhos[i], i+1, ordem);
		if(num > x->chaves[i]) i++;
	}

	// Try to insert on the correspondent child
	x->filhos[i] = efetuaInsercao(x->filhos[i], num, ordem);

	return x;
}

TAB* insercao( TAB* main, int num, int ordem ){
	if(busca(main, num))
		return main; // Element already inserted
	if(!main){ // NULL tree
		main = novaPagina(ordem);
		main->chaves[0] = num;
		main->n_chaves++;
		return main;
	}
	if(main->n_chaves == (2 * ordem) - 1){  //  Root node is full, needs division
		TAB* aux = novaPagina(ordem);
		aux->folha = 0;
		aux->filhos[0] = main;
		aux = dividirPagina(aux, main, 1, ordem);
		aux = efetuaInsercao(aux, num, ordem);
		return aux;
	}
	//  Insert the num
	main = efetuaInsercao(main, num, ordem);
	return main;
}

TAB* remocao( TAB* main, int num, int ordem ){
	if(!main || !busca(main, num))
		return main;
	return efetuaRemocao(main, num, ordem);
}

TAB* efetuaRemocao( TAB* main, int num, int ordem ){
	if(!main)
        return main;
	int i;  //  Position of the num on main
	printf("Removendo.. %d...\n", num);

	//  Finding the the node, or its child, where num must be on node main
	for(i = 0; i < main->n_chaves && main->chaves[i] < num; i++);

	//  The num is in node main
	if((i < main->n_chaves) && (num == main->chaves[i])){   //  Cases 1, 2A, 2B & 2C
		if(main->folha){    //  Case 1
			printf("Case 1\n");
			int j;
			for(j = i; j < main->n_chaves - 1; j++)
				main->chaves[j] = main->chaves[j+1];
			main->n_chaves--;
			return main;
		}
		if((!main->folha) && (main->filhos[i]->n_chaves >= ordem)){ //  Case 2A
			printf("Case 2A\n");

			//  Finding the ancestor k' of the left child from num
			TAB* y = main->filhos[i];
			while(!y->folha)
				y = y->filhos[y->n_chaves];

			//  Eliminating k' and swaping it for k in main
			int temp = y->chaves[y->n_chaves - 1];
			main->filhos[i] = efetuaRemocao(main->filhos[i], temp, ordem);
			main->chaves[i] = temp;
			return main;
		}
		if((!main->folha) && (main->filhos[i + 1]->n_chaves >= ordem)){ //  Case 2B
			printf("Case 2B\n");

			//  Finding the successor k' of the right child from num
			TAB* y = main->filhos[i + 1];
			while(!y->folha)
				y = y->filhos[0];

			//  Eliminating k' and swaping it for k in main
			int temp = y->chaves[0];
			y = efetuaRemocao(main->filhos[i + 1], temp, ordem);
			main->chaves[i] = temp;
			return main;
		}
		if((!main->folha) && (main->filhos[i + 1]->n_chaves == ordem - 1) && (main->filhos[i]->n_chaves == ordem - 1)){ //  Case 2C
			printf("Case 2C\n");

			//  Merging the two filhos of num
			TAB* y = main->filhos[i];
			TAB* z = main->filhos[i + 1];

			//  Adding num to the end of its left child
			y->chaves[y->n_chaves] = num;

			//  Joining the chaves from the left child + num (y) and the chaves from the right child (z)
			int j;
			for(j = 0; j < ordem - 1; j++)
				y->chaves[ordem + j] = z->chaves[j];

			//  Joining the filhos from left and right filhos
			for(j = 0; j <= ordem; j++)
				y->filhos[ordem + j] = z->filhos[j];

			//  Removing num and its right child from main
			y->n_chaves = 2 * ordem - 1;
			for(j = i; j < main->n_chaves - 1; j++)
				main->chaves[j] = main->chaves[j + 1];
			for(j = i + 1; j <= main->n_chaves; j++)
				main->filhos[j] = main->filhos[j + 1];
			main->filhos[j] = NULL;
			main->n_chaves--;

			//  Removing num from the new merged child
			main->filhos[i] = efetuaRemocao(main->filhos[i], num, ordem);
			return main;
		}
	}

	//  If the num isn't in the node main
	TAB* y = main->filhos[i];   //  Child where num must be
	TAB* z = NULL;
	if(y->n_chaves == ordem - 1){   //  Cases 3A & 3B
		if((i < main->n_chaves) && (main->filhos[i + 1]->n_chaves >= main)){    //  Case 3A, i < n_chaves
			printf("Case 3A: i less than n_chaves\n");

			//  Giving to y the num i from main (father giving a num to its left child)
			z = main->filhos[i + 1];
			y->chaves[ordem - 1] = main->chaves[i];
			y->n_chaves++;

			//  Giving to main a num from z (right child giving a num to father) & adjusting child's chaves
			main->chaves[i] = z->chaves[0];
			int j;
			for(j = 0; j < z->n_chaves - 1; j++)
				z->chaves[j] = z->chaves[j + 1];

			//  Giving to new num in y the first child of z & adjusting z's filhos
			y->filhos[y->n_chaves] = z->filhos[0];
			for(j = 0; j < z->n_chaves; j++)
				z->filhos[j] = z->filhos[j + 1];
			z->n_chaves--;

			//  Removing num in the child i
			main->filhos[i] = efetuaRemocao(main->filhos[i], num, ordem);
			return main;
		}
		if((i > 0) && (!z) && (main->filhos[i - 1]->n_chaves >= ordem)){    //  Case 3A, i == n_chaves
			printf("Case 3A: i equals to n_chaves\n");

			//  Adjusting chaves & filhos to add new num (father giving a num to the right child)
			z = main->filhos[i - 1];
			int j;
			for(j = y->n_chaves; j > 0; j--)
				y->chaves[j] = y->chaves[j - 1];
			for(j = y->n_chaves + 1; j > 0; j--)
				y->filhos[j] = y->filhos[j - 1];

			//  Giving to right child the father's num
			y->chaves[0] = main->chaves[i - 1];
			y->n_chaves++;

			//  Father receiving a num from its left child
			main->chaves[i - 1] = z->chaves[z->n_chaves - 1];

			//  Right child's new num receive the last child from father's left child
			y->filhos[0] = z->filhos[z->n_chaves];
			z->n_chaves--;

			//  Removing num in the child i
			main->filhos[i] = efetuaRemocao(y, num, ordem);
			return main;
		}
		if(!z){
			if((i < main->n_chaves) && (main->filhos[i + 1]->n_chaves == ordem - 1)){   //  Case 3B, i < n_chaves
				printf("Case 3B: i less than n_chaves\n");

				z = main->filhos[i + 1];

				//  Giving to left child (y) the num i from main (father giving a num to its left child)
				y->chaves[ordem - 1] = main->chaves[i];
				y->n_chaves++;

				// Left child (y) receive right child's chaves & filhos
				int j;
				for(j = 0; j < ordem - 1; j++){
					y->chaves[ordem + j] = z->chaves[j];
					y->n_chaves++;
				}
				if(!y->folha)
					for (j = 0; j < ordem; j++)
						y->filhos[ordem + j] = z->filhos[j];

				//  Adjusting chaves & filhos after the moves in main
				for(j = i; j < main->n_chaves - 1; j++){
					main->chaves[j] = main->chaves[j + 1];
					main->filhos[j + 1] = main->filhos[j + 2];
				}
				main->n_chaves--;

				//  Removing num in main
				main = efetuaRemocao(main, num, ordem);
				return main;
			}
			if((i > 0) && (main->filhos[i - 1]->n_chaves == ordem - 1)){    //  Case 3B, i == n_chaves
				printf("Case 3B: i equals to n_chaves\n");

				//  If num is on the last child (extreme right) of main, then main gives the num i-1 to its (i-1)th child
				//  otherwise T'll give the num i to its (i-1)th child
				z = main->filhos[i - 1];
				if(i == main->n_chaves)
					z->chaves[ordem - 1] = main->chaves[i - 1];
				else
					z->chaves[ordem - 1] = main->chaves[i];
				z->n_chaves++;

				//  Giving to the (i-1)th child the chaves & filhos of the i-th child
				int j;
				for(j = 0; j < ordem - 1; j++){
					z->chaves[ordem + j] = y->chaves[j];
					z->n_chaves++;
				}
				if(!z->folha)
					for(j = 0; j < ordem; j++)
						z->filhos[ordem + j] = y->filhos[j];

				//  Updating main
				main->n_chaves--;
				main->filhos[i - 1] = z;

				//  Removing num in main
				main = efetuaRemocao(main, num, ordem);
				return main;
			}
		}
	}
	//  If none of the cases occurs
	main->filhos[i] = efetuaRemocao(main->filhos[i], num, ordem);
	return main;
}
