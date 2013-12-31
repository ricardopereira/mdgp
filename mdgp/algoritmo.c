#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
void gera_vizinho(int a[], int b[], int n)
{
    int p1, p2;
    
    copia(b,a,n);

	// Elemento a trocar
    p1=random_l_h(0, n-1);

	// Encontra posicao com valor 0
    do
        p2=random_l_h(0, n-1);
    while (b[p2] == a[p1]);

	// Troca
    troca(b, p1, p2);
}

// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada

int trepa_colinas(int sol[], int **mat, int m, int g, int num_iter)
{
    int *nova_sol, custo, custo_viz, i;
    
	nova_sol = malloc(sizeof(int)*m);
    if(nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

	// Avalia solucao inicial
    custo = calcula_fit(sol, mat, m, g);
    
    for (i=0; i<num_iter; i++)
    {
		// Gera vizinho
		gera_vizinho(sol, nova_sol, m);
        
		// Avalia vizinho
		custo_viz = calcula_fit(nova_sol, mat, m, g);
        
		// Aceita vizinho se o custo aumentar (problema de maximizacao)
        if (custo_viz > custo)
        {
			copia(sol, nova_sol, m);
			custo = custo_viz;
        }
    }
    
    free(nova_sol);
    
    return custo;
}

#define PROB 0.001

int tc_prob(int sol[], int **mat, int m, int g, int num_iter)
{
    int *nova_sol, *best_sol, custo, custo_best, custo_viz, i;
    double r;
    nova_sol = malloc(sizeof(int)*m);
    best_sol = malloc(sizeof(int)*m);    
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, m, g);
    
    copia(best_sol, sol, m);
    custo_best = custo;
    
    for(i=0; i<num_iter; i++){
        // Gera vizinho
        gera_vizinho(sol, nova_sol, m);
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, m, g);
        // Aceita vizinho se o custo diminuir (problema de minimizacao)
        if(custo_viz <= custo){
            copia(sol, nova_sol, m);
            custo = custo_viz;
        }
        else{
            r = rand_01();
            if(r < PROB)
            {
                copia(sol, nova_sol, m);
                custo = custo_viz;
            }
        }
        if(custo_best > custo){
            copia(best_sol, sol, m);
            custo_best = custo;
        }
    }
    
    copia(sol, best_sol, m);
    custo = custo_best;
    
    free(nova_sol);
    free(best_sol);
    return custo;
}

#define TMAX 1.0 //Forçar o valor ser real
#define TMIN 0.0001

int tc_simulated_annealing(int sol[], int **mat, int m, int g, int num_iter)
{
    int *nova_sol, *best_sol, custo, custo_best, custo_viz, i;
    double eprob, temperatura;
    double r;
    nova_sol = malloc(sizeof(int)*m);
    best_sol = malloc(sizeof(int)*m);    
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, m, g);
    
    copia(best_sol, sol, m);
    custo_best = custo;
    // Inicializa temperatura
    temperatura = TMAX;
    for(i=0; i<num_iter; i++) {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, m);    
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, m, g);  
        // Calcular probabilidade
        eprob = exp((custo-custo_viz)/temperatura);

        // Aceita vizinho se o custo diminuir (problema de minimizacao)
        if(custo_viz <= custo){
            copia(sol, nova_sol, m);
            custo = custo_viz;
        }
        else{
            r = rand_01();
            if(r < eprob)
            {
                copia(sol, nova_sol, m);
                custo = custo_viz;
            }
        }
        if(custo_best > custo){
            copia(best_sol, sol, m);
            custo_best = custo;
        }
        // Arrefecimento
        temperatura -= (TMAX - TMIN) / num_iter;
    }
    
    copia(sol, best_sol, m);
    custo = custo_best;
    
    free(nova_sol);
    free(best_sol);
    return custo;
}