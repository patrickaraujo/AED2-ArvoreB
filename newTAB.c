#include <stdio.h>
#include <stdlib.h>
#include "TAB.h"

void iNP(TAB *main, TAB *pagina, int num ){	//	Insere na Página
	int i = main->n_chaves;
	int nAP = (i > 0);	//	nAP: Não Achou Posição
	while (nAP){
		if (num >= main->chaves[i-1]){
			nAP = 0;
			break;
		}
		main->chaves[i] = main->chaves[i-1];
		main->filhos[i+1] = main->filhos[i];
		i--;
		if (i < 1)
			nAP = 0;
    }
	main->chaves[i] = num;
	main->filhos[i+1] = pagina;
	main->n_chaves++;
}

void efetuaInsercao( TAB **main, TAB **pagina, int *valida, int *nI, int num, int ordem ){	//	nI: Número Inserido
	if(!(*main)){
        *valida = 1;
        *nI = num;
        *pagina = NULL;
	}
	else{
        int i = 1;
        while( (i < (*main)->n_chaves) && (num > (*main)->chaves[i-1]) )
            i++;
        if(num == (*main)->chaves[i-1]){
            printf("Erro: Chave ja inserida\n");
            *valida = 0;
        }
        else{
            if(num < (*main)->chaves[i-1])
                i--;
            efetuaInsercao( &((*main)->filhos[i]), pagina, valida, nI, num, ordem );
            if(*valida){
                if((*main)->n_chaves < (2 * ordem)){    //  Página tem espaço
                    iNP( (*main), *pagina, *nI );
                    *valida = 0;
                }
                else{   //  Overflow: Página tem que ser dividida
                    TAB *temp = novaPagina(ordem);
                    temp->n_chaves = 0;
                    temp->filhos[0] = NULL;
                    if(i < ordem + 1){
                        iNP( temp, (*main)->filhos[2 * ordem], (*main)->chaves[(2 * ordem) - 1] );
                        (*main)->n_chaves--;
                        iNP( (*main), *pagina, *nI );
                    }
                    else
                        iNP( temp, *pagina, *nI );
                    int j;
                    for(j = ordem + 2; j <= (2 * ordem); j++)
                        iNP( temp, (*main)->filhos[j], (*main)->chaves[j-1] );
                    (*main)->n_chaves = ordem;
                    temp->filhos[0] = temp->filhos[ordem+1];
                    *nI = (*main)->chaves[ordem];
                    *pagina = temp;
                }

            }
        }
	}
}

void insercao( TAB **main, int num, int ordem ){
	int* valida, nI;
	TAB *pagina;
	efetuaInsercao( (&(*main)), &pagina, &valida, &nI, num, ordem);
	if(valida){	//	Arvore cresce na altura pela raiz
		TAB *temp = novaPagina(ordem);
		temp->n_chaves = 1;
		temp->chaves[0] = nI;
		temp->filhos[1] = pagina;
		temp->filhos[0] = *main;
		*main = temp;
	}
}

TAB* novaPagina(int ordem){
	TAB* temp = (TAB*) malloc(sizeof(TAB));
	temp->n_chaves = 0;
	temp->chaves = (int*) malloc(sizeof(int) * (2 * ordem));
	temp->filhos = (TAB**) malloc(sizeof(TAB) * ((2 * ordem) + 1));
	int i;
	for(i = 0; i < (2 * ordem); i++)
        temp->filhos[i] = NULL;
	return temp;
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
