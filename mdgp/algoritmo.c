#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "algoritmo.h"
#include "funcao.h"
#include "utils.h"

// Gera um vizinho
// Parametros: solucao actual, vizinho, numero de vertices
//swap two vertices
void gera_vizinhoV1(int a[], int b[], int n)
{
    int i, p1, p2;
	
    for(i=0; i<n; i++)
        b[i]=a[i];

	// Encontra posicao com valor 0
    do
        p1=random_l_h(0, n-1);
    while(b[p1] != 0);

	// Encontra posicao com valor 0
    do
        p2=random_l_h(0, n-1);
    while(b[p2] != 1);

	// Troca
    b[p1] = 1;
    b[p2] = 0;
}

void gera_vizinhoV2(int a[], int b[], int n)
{
    int i, p1, p2;
    
    for(i=0; i<n; i++)
        b[i]=a[i];

    // Encontra posicao com valor 0
    do
        p1=random_l_h(0, n-1);
    while(b[p1] != 0);

    // Encontra posicao com valor 0
    do
        p2=random_l_h(0, n-1);
    while(b[p2] != 1);

    // Troca
    b[p1] = 1;
    b[p2] = 0;

    // Encontra posicao com valor 0
    do
        p1=random_l_h(0, n-1);
    while(b[p1] != 0);

    // Encontra posicao com valor 0
    do
        p2=random_l_h(0, n-1);
    while(b[p2] != 1);

    // Troca
    b[p1] = 1;
    b[p2] = 0;
}

// Trepa colinas first-choice
// Parametros: solucao, matriz de adjacencias, numero de vertices e numero de iteracoes
// Devolve o custo da melhor solucao encontrada

int trepa_colinas(int sol[], int *mat, int vert, int num_iter)
{
    int *nova_sol, custo, custo_viz, i;
    
	nova_sol = malloc(sizeof(int)*vert);
    if(nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

	// Avalia solucao inicial
    custo = calcula_fit(sol, mat, vert);
    
    for(i=0; i<num_iter; i++)
    {
		// Gera vizinho
		gera_vizinhoV1(sol, nova_sol, vert);
        
		// Avalia vizinho
		custo_viz = calcula_fit(nova_sol, mat, vert);
        
		// Aceita vizinho se o custo diminuir (problema de minimizacao)
        //if(custo_viz < custo)
        if(custo_viz <= custo)
        {
			substitui(sol, nova_sol, vert);
			custo = custo_viz;
        }
    }
    free(nova_sol);
	
    return custo;
}

#define PROB 0.001

int tc_prob(int sol[], int *mat, int vert, int num_iter)
{
    int *nova_sol, *best_sol, custo, custo_best, custo_viz, i;
    double r;
    nova_sol = malloc(sizeof(int)*vert);
    best_sol = malloc(sizeof(int)*vert);    
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, vert);
    
    substitui(best_sol, sol, vert);
    custo_best = custo;
    
    for(i=0; i<num_iter; i++){
        // Gera vizinho
        gera_vizinhoV1(sol, nova_sol, vert);    
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, vert);  
        // Aceita vizinho se o custo diminuir (problema de minimizacao)
        if(custo_viz <= custo){
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }
        else{
            r = rand_01();
            if(r < PROB)
            {
                substitui(sol, nova_sol, vert);
                custo = custo_viz;
            }
        }
        if(custo_best > custo){
            substitui(best_sol, sol, vert);
            custo_best = custo;
        }
    }
    
    substitui(sol, best_sol, vert);
    custo = custo_best;
    
    free(nova_sol);
    free(best_sol);
    return custo;
}

#define TMAX 1.0 //Forçar o valor ser real
#define TMIN 0.0001

int tc_simulated_annealing(int sol[], int *mat, int vert, int num_iter)
{
    int *nova_sol, *best_sol, custo, custo_best, custo_viz, i;
    double eprob, temperatura;
    double r;
    nova_sol = malloc(sizeof(int)*vert);
    best_sol = malloc(sizeof(int)*vert);    
    // Avalia solucao inicial
    custo = calcula_fit(sol, mat, vert);
    
    substitui(best_sol, sol, vert);
    custo_best = custo;
    // Inicializa temperatura
    temperatura = TMAX;
    for(i=0; i<num_iter; i++) {
        // Gera vizinho
        gera_vizinhoV1(sol, nova_sol, vert);    
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, vert);  
        // Calcular probabilidade
        eprob = exp((custo-custo_viz)/temperatura);

        // Aceita vizinho se o custo diminuir (problema de minimizacao)
        if(custo_viz <= custo){
            substitui(sol, nova_sol, vert);
            custo = custo_viz;
        }
        else{
            r = rand_01();
            if(r < eprob)
            {
                substitui(sol, nova_sol, vert);
                custo = custo_viz;
            }
        }
        if(custo_best > custo){
            substitui(best_sol, sol, vert);
            custo_best = custo;
        }
        // Arrefecimento
        temperatura -= (TMAX - TMIN) / num_iter;
    }
    
    substitui(sol, best_sol, vert);
    custo = custo_best;
    
    free(nova_sol);
    free(best_sol);
    return custo;
}