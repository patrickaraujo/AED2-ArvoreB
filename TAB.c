#include <stdio.h>
#include <stdlib.h>
#include "TAB.h"

TAB* init_tree(){
	return NULL;
}

TAB* make_node(int ordem){
	TAB* new = (TAB*) malloc(sizeof(TAB));
	new->n_chaves = 0;
	new->chaves = (int*) malloc(sizeof(int) * ((2*ordem) - 1));
	new->folha = 1;
	new->filhos = (TAB**) malloc(sizeof(TAB) * 2 * ordem);
	int i;
	for(i = 0; i < (2 * ordem); i++) new->filhos[i] = NULL;
	return new;
}

TAB* free_tree( TAB* main ){
	if(!main)
		return NULL;
	if(!main->folha){
		int i;
		for(i = 0; i <= main->n_chaves; i++)
			free_tree(main->filhos[i]);
	}
	free(main->filhos);
	free(main->chaves);
	free(main);
	return NULL;
}

void print_tree( TAB* main, unsigned height ){
	if(!main) return;
	unsigned i, j;
	for(i = 0; i <= main->n_chaves - 1; i++){
		print_tree(main->filhos[i], height + 1);
		for(j = 0; j <= height; j++) printf("    ");
		printf("%d\n", main->chaves[i]);
	}
	print_tree(main->filhos[i], height + 1);
}

TAB* search_key( TAB* main, int key ){
	if(!main)
		return NULL;
	int i = 0;
	while(i < main->n_chaves && key > main->chaves[i]) i++;
	if(i < main->n_chaves && key == main->chaves[i])
		return main;
	if(main->folha)
		return NULL;
	return search_key(main->filhos[i], key);
}

TAB* divide_node( TAB* x, int index, TAB* y, int ordem ){
	// Divide node y into node x->(z,y), where x will be the father,
	// z will take ordem-1 first chaves and y will take the remaining filhos
	TAB* z = make_node(ordem);
	z->n_chaves = ordem - 1;
	z->folha = y->folha;

	// Passing the first ordem-1 chaves to z as well as the ordem first filhos
	unsigned j;
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

TAB* partial_insert( TAB* x, int key, int ordem ){
	int i = x->n_chaves - 1;

	// If the node is a folha, then the key is inserted
	if(x->folha){
		while((i >= 0) && (key < x->chaves[i])){
			x->chaves[i + 1] = x->chaves[i];
			i--;
		}
		x->chaves[i + 1] = key;
		x->n_chaves++;
		return x;
	}

	// Finding the child where key must be inserted
	// If the child has reached chaves limit, then it is divided and the
	//		new key "root" will be added in x
	while((i >= 0) && (key < x->chaves[i])) i--;
	i++;
	if(x->filhos[i]->n_chaves == (2 * ordem) - 1){
		x = divide_node(x, i+1, x->filhos[i], ordem);
		if(key > x->chaves[i]) i++;
	}

	// Try to insert on the correspondent child
	x->filhos[i] = partial_insert(x->filhos[i], key, ordem);

	return x;
}

TAB* insert_key( TAB* main, int key, int ordem ){
	if(search_key(main, key))
		return main; // Element already inserted
	if(!main){ // NULL tree
		main = make_node(ordem);
		main->chaves[0] = key;
		main->n_chaves++;
		return main;
	}
	if(main->n_chaves == (2 * ordem) - 1){ // Root node is full, needs division
		TAB* aux = make_node(ordem);
		aux->folha = 0;
		aux->filhos[0] = main;
		aux = divide_node(aux, 1, main, ordem);
		aux = partial_insert(aux, key, ordem);
		return aux;
	}
	// Insert the key
	main = partial_insert(main, key, ordem);
	return main;
}

TAB* remove_key( TAB* main, int key, int ordem ){
	if(!main || !search_key(main, key))
		return main;
	return _remove_key(main, key, ordem);
}
TAB* _remove_key( TAB* main, int key, int ordem ){
	if(!main) return main;
	unsigned i; // Position of the key on main
	printf("Removendo.. %d...\n", key);

	// Finding the the node, or its child, where key must be on node main
	for(i = 0; i < main->n_chaves && main->chaves[i] < key; i++);

	// The key is in node main
	if((i < main->n_chaves) && (key == main->chaves[i])){ // Cases 1, 2A, 2B & 2C
		if(main->folha){ // Case 1
			printf("Case 1\n");
			unsigned j;
			for(j = i; j < main->n_chaves - 1; j++)
				main->chaves[j] = main->chaves[j+1];
			main->n_chaves--;
			return main;
		}
		if((!main->folha) && (main->filhos[i]->n_chaves >= ordem)){ // Case 2A
			printf("Case 2A\n");

			// Finding the ancestor k' of the left child from key
			TAB* y = main->filhos[i];
			while(!y->folha)
				y = y->filhos[y->n_chaves];

			// Eliminating k' and swaping it for k in main
			int temp = y->chaves[y->n_chaves - 1];
			main->filhos[i] = _remove_key(main->filhos[i], temp, ordem);
			main->chaves[i] = temp;
			return main;
		}
		if((!main->folha) && (main->filhos[i + 1]->n_chaves >= ordem)){ // Case 2B
			printf("Case 2B\n");

			// Finding the successor k' of the right child from key
			TAB* y = main->filhos[i + 1];
			while(!y->folha)
				y = y->filhos[0];

			// Eliminating k' and swaping it for k in main
			int temp = y->chaves[0];
			y = _remove_key(main->filhos[i + 1], temp, ordem);
			main->chaves[i] = temp;
			return main;
		}
		if((!main->folha) && (main->filhos[i + 1]->n_chaves == ordem - 1) && (main->filhos[i]->n_chaves == ordem - 1)){ // Case 2C
			printf("Case 2C\n");

			// Merging the two filhos of key
			TAB* y = main->filhos[i];
			TAB* z = main->filhos[i + 1];

			// Adding key to the end of its left child
			y->chaves[y->n_chaves] = key;

			// Joining the chaves from the left child + key (y) and the chaves from the right child (z)
			unsigned j;
			for(j = 0; j < ordem - 1; j++)
				y->chaves[ordem + j] = z->chaves[j];

			// Joining the filhos from left and right filhos
			for(j = 0; j <= ordem; j++)
				y->filhos[ordem + j] = z->filhos[j];

			// Removing key and its right child from main
			y->n_chaves = 2 * ordem - 1;
			for(j = i; j < main->n_chaves - 1; j++)
				main->chaves[j] = main->chaves[j + 1];
			for(j = i + 1; j <= main->n_chaves; j++)
				main->filhos[j] = main->filhos[j + 1];
			main->filhos[j] = NULL;
			main->n_chaves--;

			// Removing key from the new merged child
			main->filhos[i] = _remove_key(main->filhos[i], key, ordem);
			return main;
		}
	}

	// If the key isn't in the node main
	TAB* y = main->filhos[i]; // Child where key must be
	TAB* z = NULL;
	if(y->n_chaves == ordem - 1){ // Cases 3A & 3B
		if((i < main->n_chaves) && (main->filhos[i + 1]->n_chaves >= main)){ // Case 3A, i < n_chaves
			printf("Case 3A: i less than n_chaves\n");

			// Giving to y the key i from main (father giving a key to its left child)
			z = main->filhos[i + 1];
			y->chaves[ordem - 1] = main->chaves[i];
			y->n_chaves++;

			// Giving to main a key from z (right child giving a key to father) & adjusting child's chaves
			main->chaves[i] = z->chaves[0];
			unsigned j;
			for(j = 0; j < z->n_chaves - 1; j++)
				z->chaves[j] = z->chaves[j + 1];

			// Giving to new key in y the first child of z & adjusting z's filhos
			y->filhos[y->n_chaves] = z->filhos[0];
			for(j = 0; j < z->n_chaves; j++)
				z->filhos[j] = z->filhos[j + 1];
			z->n_chaves--;

			// Removing key in the child i
			main->filhos[i] = _remove_key(main->filhos[i], key, ordem);
			return main;
		}
		if((i > 0) && (!z) && (main->filhos[i - 1]->n_chaves >= ordem)){ // Case 3A, i == n_chaves
			printf("Case 3A: i equals to n_chaves\n");

			// Adjusting chaves & filhos to add new key (father giving a key to the right child)
			z = main->filhos[i - 1];
			unsigned j;
			for(j = y->n_chaves; j > 0; j--)
				y->chaves[j] = y->chaves[j - 1];
			for(j = y->n_chaves + 1; j > 0; j--)
				y->filhos[j] = y->filhos[j - 1];

			// Giving to right child the father's key
			y->chaves[0] = main->chaves[i - 1];
			y->n_chaves++;

			// Father receiving a key from its left child
			main->chaves[i - 1] = z->chaves[z->n_chaves - 1];

			// Right child's new key receive the last child from father's left child
			y->filhos[0] = z->filhos[z->n_chaves];
			z->n_chaves--;

			// Removing key in the child i
			main->filhos[i] = _remove_key(y, key, ordem);
			return main;
		}
		if(!z){
			if((i < main->n_chaves) && (main->filhos[i + 1]->n_chaves == ordem - 1)){ // Case 3B, i < n_chaves
				printf("Case 3B: i less than n_chaves\n");

				z = main->filhos[i + 1];

				// Giving to left child (y) the key i from main (father giving a key to its left child)
				y->chaves[ordem - 1] = main->chaves[i];
				y->n_chaves++;

				// Left child (y) receive right child's chaves & filhos
				unsigned j;
				for(j = 0; j < ordem - 1; j++){
					y->chaves[ordem + j] = z->chaves[j];
					y->n_chaves++;
				}
				if(!y->folha)
					for (j = 0; j < ordem; j++)
						y->filhos[ordem + j] = z->filhos[j];

				// Adjusting chaves & filhos after the moves in main
				for(j = i; j < main->n_chaves - 1; j++){
					main->chaves[j] = main->chaves[j + 1];
					main->filhos[j + 1] = main->filhos[j + 2];
				}
				main->n_chaves--;

				// Removing key in main
				main = _remove_key(main, key, ordem);
				return main;
			}
			if((i > 0) && (main->filhos[i - 1]->n_chaves == ordem - 1)){ // Case 3B, i == n_chaves
				printf("Case 3B: i equals to n_chaves\n");

				// If key is on the last child (extreme right) of main, then main gives the key i-1 to its (i-1)th child
				// 		otherwise T'll give the key i to its (i-1)th child
				z = main->filhos[i - 1];
				if(i == main->n_chaves)
					z->chaves[ordem - 1] = main->chaves[i - 1];
				else
					z->chaves[ordem - 1] = main->chaves[i];
				z->n_chaves++;

				// Giving to the (i-1)th child the chaves & filhos of the i-th child
				unsigned j;
				for(j = 0; j < ordem - 1; j++){
					z->chaves[ordem + j] = y->chaves[j];
					z->n_chaves++;
				}
				if(!z->folha)
					for(j = 0; j < ordem; j++)
						z->filhos[ordem + j] = y->filhos[j];

				// Updating main
				main->n_chaves--;
				main->filhos[i - 1] = z;

				// Removing key in main
				main = _remove_key(main, key, ordem);
				return main;
			}
		}
	}
	// If none of the cases occurs
	main->filhos[i] = _remove_key(main->filhos[i], key, ordem);
	return main;
}
