#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

// Calcula a qualidade de uma solucao
// Recebe: solucao, matriz, nr de elementos, nr de sub-conjuntos
// Devolve o custo
int calcula_fit(int a[], int **mat, int m, int g)
{
    int *sol;
	int total=0;
	int i, idx=0, subc;
    
    // Coloca solucao num vector
    sol = malloc(sizeof(int)*m);
    for (subc=0; subc<g; subc++)
        for(i=0; i<m; i++)
            if(a[i] == subc)
                sol[idx++] = i;

    // qualidade = diversidade(0) + diversidade(1) + ... + diversidade(g)
    for(i=0; i<g; i++)
        total += calcula_div(sol, mat, m, i*(m/g), m/g);
    
    free(sol);
    
	return total;
}

int calcula_div(int sol[], int **mat, int m, int start, int count)
{
    int div=0;
    int i, j;
    
    for (i=start; i<(start+count); i++)
        for (j=i+1; j<(start+count); j++)
            div += obter_distancia(mat, m, sol[i], sol[j]);

    return div;
}