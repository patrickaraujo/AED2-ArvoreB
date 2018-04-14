#ifndef TAB_H_INCLUDED
#define TAB_H_INCLUDED

typedef struct AB{
	int n_chaves, folha;
	int *chaves;
	struct AB **filhos;
}TAB;

TAB* inicializa();
TAB* novaPagina(int);
TAB* liberaArvore( TAB* );
void imprimir( TAB*, int );
TAB* busca( TAB*, int );
TAB* dividirPagina( TAB*, TAB*, int, int );
TAB* efetuaInsercao( TAB*, int, int );
TAB* insercao( TAB*, int, int );
TAB* remocao( TAB*, int, int );
TAB* efetuaRemocao( TAB*, int, int );
#endif // TAB_H_INCLUDED
