#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algoritmo.h"
#include "utils.h"

#define DEFAULT_RUNS 10

int main(int argc, char *argv[])
{
    char nome_fich[100];
    int *grafo, *sol, *best, **dist;
    int m, g, num_iter, k, i,j, runs, custo, best_custo;
	float mbf = 0.0;

	if(argc == 3)
	{
		runs = atoi(argv[2]);
		strcpy(nome_fich, argv[1]);
	}
	else if(argc == 2)
	{
		runs = DEFAULT_RUNS;
        strcpy(nome_fich, argv[1]);
    }
	else
    {
		runs = DEFAULT_RUNS;
        /*printf("Nome do Ficheiro: ");*/
        /*gets(nome_fich);*/
    }
  
	if(runs <= 0)
		return 0;
	
	init_rand();
    
    
    
    /* Preenche matriz de adjacencias */
    /*grafo = init_dados(nome_fich, &m, &g, &num_iter);*/
    m = 10;
    g = 2;
    
    /* Linhas */
    if (( dist = (int**)malloc(sizeof(int*) * (m-1))) == NULL )
    {
    
    }
    
    for (i=0; i<(m-1); i++)
    {
        /* Colunas de cada linha */
        if (( dist[i] = (int*)calloc(m,sizeof(int))) == NULL )
        {
        
        }
        
        /* Iniciar linha */

    }
    
    dist[8][9] = 5;
    dist[8][8] = 6;
    
    printf("Mostra distancias:\n");
    for (i=0; i<(m-1); i++)
        for (j=0; j<m; j++)
            if (dist[i][j])
            {
                printf("%d\n",dist[i][j]);
            }
    
    
    for (i=0; i<(m-1); i++)
        free(dist[i]);
    free(dist);
    
    
  	
	sol = malloc(sizeof(int)*m);
	best = malloc(sizeof(int)*m);
	
	if(sol == NULL || best == NULL)
	{
		printf("Erro na alocacao de memoria");
		exit(1);
	}
	
	for(k=0; k<runs; k++)
	{
		// Gerar solucao inicial
		gera_sol_inicial(sol, m, g);
        
        
    
		// Trepa colinas simples
		//custo = trepa_colinas(sol, grafo, vert, num_iter);
    
		// Trepa colinas probabilístico
		//custo = tc_prob(sol, grafo, vert, num_iter);

		// Trepa colinas Recristalização Simulada
		//custo = tc_simulated_annealing(sol, grafo, vert, num_iter);

		// Escreve resultados da repeticao k
		printf("\nRepeticao %d:", k);
		escreve_sol(sol, m, g);
		printf("Custo final: %2d\n", custo);    
		
		mbf += custo;
		if(k==0 || best_custo > custo)
		{
			best_custo = custo;
			substitui(best, sol, m);
		}
    } 
	
	// Escreve eresultados globais
	printf("\n\nMBF: %f\n", mbf/k);
	printf("\nMelhor solucao encontrada");
	escreve_sol(best, m, g);
	printf("Custo final: %2d\n", best_custo);    
	
	free(grafo);
    free(sol);    
	free(best);
    return 0;    
}