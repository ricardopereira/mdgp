#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algoritmo.h"
#include "utils.h"

#define DEFAULT_RUNS 10
#define DEFAULT_FILE "RanInt_n010_ss_01.txt"

int main(int argc, char *argv[])
{
    char nome_fich[100];
    int *sol, *best;
    int **dist;
    int m, g, num_iter, k, i, runs, custo, best_custo;
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
        printf("Nome do Ficheiro: ");
        
        //Teste
        /*gets(nome_fich);*/
        strcpy(nome_fich,DEFAULT_FILE);
        num_iter = 10000;
    }
  
	if(runs <= 0)
		return 0;
	
	init_rand();
    
    
    /* Preenche matriz de adjacencias */
    dist = init_dados(nome_fich, &m, &g);
    
    printf("Elementos: %d\n",m);
    printf("Sub-conjuntos: %d\n",g);
    
    mostra_matriz(dist,m);
    printf("\n");
    
  	
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
		custo = trepa_colinas(sol, dist, m, g, num_iter);
    
		// Trepa colinas probabilístico
		//custo = tc_prob(sol, grafo, vert, num_iter);

		// Trepa colinas Recristalização Simulada
		//custo = tc_simulated_annealing(sol, grafo, vert, num_iter);
        
        
		// Escreve resultados da repeticao k
		printf("\nRepeticao %d:", k);
		escreve_sol(sol, m, g);
		printf("Custo final: %2d\n", custo);
		
		mbf += custo;
		if(k==0 || best_custo < custo)
		{
			best_custo = custo;
			copia(best, sol, m);
		}
    } 
	
	// Escreve eresultados globais
	printf("\n\nMBF: %f\n", mbf/k);
	printf("\nMelhor solucao encontrada");
	escreve_sol(best, m, g);
	printf("Custo final: %2d\n", best_custo);
	
    // Libertar memoria
    for (i=0; i<(m-1); i++)
        free(dist[i]);
    free(dist);
    free(sol);    
	free(best);
    return 0;    
}