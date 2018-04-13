#ifndef TAB_H_INCLUDED
#define TAB_H_INCLUDED

typedef struct AB{
	unsigned n_chaves, folha;
	int *chaves;
	struct AB **filhos;
}TAB;

TAB* init_tree();
TAB* make_node(int);
TAB* free_tree( TAB* );
void print_tree( TAB*, unsigned );
TAB* search_key( TAB*, int );
TAB* divide_node( TAB*, int, TAB*, int );
TAB* partial_insert( TAB*, int, int );
TAB* insert_key( TAB*, int, int );
TAB* remove_key( TAB*, int, int );
TAB* _remove_key( TAB*, int, int );
#endif // TAB_H_INCLUDED
