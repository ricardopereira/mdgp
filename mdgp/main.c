#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algoritmo.h"
#include "utils.h"

enum TipoAlgoritmo
{
    algTrepaColinas,
    algTrepaColinasProb,
    algRecristalizacaoSimulada,
    algTabu,
    algGeneticoPorTorneio
};

#define DEFAULT_RUNS 10
#define DEFAULT_FILE "RanInt_n012_ss_01.txt"

int main(int argc, char *argv[])
{
    enum TipoAlgoritmo algoritmo;
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
        
        // Teste
        //gets(nome_fich);
        strcpy(nome_fich,DEFAULT_FILE);
    }
    
	if(runs <= 0)
		return 0;
	
	init_rand();
    
    // Configuracao
    algoritmo = algTrepaColinas;
    num_iter = 10000;
    
    // Preenche matriz de distancias
    dist = init_dados(nome_fich, &m, &g);
    
    printf("Elementos: %d\n",m);
    printf("Sub-conjuntos: %d\n",g);
    
    // Confirmar valores da matriz
    mostra_matriz(dist,m);
    printf("\n");
    
    // Maximizacao
    switch (algoritmo)
    {
        case algTrepaColinas:
        case algTrepaColinasProb:
        case algRecristalizacaoSimulada:
        case algTabu:
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
                
                switch (algoritmo)
                {
                    case algTrepaColinas:
                        // Trepa colinas simples
                        custo = trepa_colinas(sol, dist, m, g, num_iter);
                        break;
                    case algTrepaColinasProb:
                        // Trepa colinas probabilístico
                        //custo = tc_prob(sol, grafo, vert, num_iter);
                        break;
                    case algRecristalizacaoSimulada:
                        // Trepa colinas Recristalização Simulada
                        //custo = tc_simulated_annealing(sol, grafo, vert, num_iter);
                        break;
                    case algTabu:
                        
                        break;
                    default:
                        exit(0);
                }
                
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
            
            free(sol);
            free(best);
            break;
            
        case algGeneticoPorTorneio:
            
            break;
    }
  		
    // Libertar memoria
    for (i=0; i<(m-1); i++)
        free(dist[i]);
    free(dist);
    return 0;
}