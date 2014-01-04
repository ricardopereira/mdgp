#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algoritmo.h"
#include "funcao.h"
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
#define DEFAULT_FILE "RanInt_n010_ss_01.txt"

int main(int argc, char *argv[])
{
    enum TipoAlgoritmo algoritmo;
    char nome_fich[100];
    char nome_alg[100];
    int *sol, *best;
    int** dist;
    int m, g, num_iter, k, i, runs, custo, custo_best = 0;
	float mbf = 0.0;
    // Evolutivo
	struct info parameters;
	int gen_actual;
	chrom best_run, best_ever;
    pchrom pop = NULL, parents = NULL;

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
    algoritmo = algGeneticoPorTorneio;
    num_iter = 1000;
    if (argc == 4)
        parameters.numTabuDescidas = atoi(argv[3]);
    else
        parameters.numTabuDescidas = 5;
    parameters.numGenerations = 2100;
    parameters.popsize = 100;
    parameters.pm_swap = 0.025;
    parameters.pr = 0.25;
	parameters.t_size = 3;
    
    // Preenche matriz de distancias
    dist = init_dados(nome_fich, &m, &g);
    
    parameters.m = m; //Nr Elementos
    parameters.g = g; //Nr Sub-conjuntos
    
    printf("Elementos: %d\n",m);
    printf("Sub-conjuntos: %d\n",g);
    
    // Confirmar valores da matriz
    //mostra_matriz(dist,m);
    //printf("\n");
    
    // Maximizacao
    switch (algoritmo)
    {
        case algTrepaColinas:
        case algTrepaColinasProb:
        case algRecristalizacaoSimulada:
        case algTabu:
            sol = calloc(m,sizeof(int));
            best = calloc(m,sizeof(int));
            
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
                        strcpy(nome_alg, "Trepa Colinas");
                        custo = trepa_colinas(sol, dist, m, g, num_iter);
                        break;
                    case algTrepaColinasProb:
                        // Trepa colinas probabilístico
                        strcpy(nome_alg, "Trepa Colinas probabilistico");
                        custo = tc_prob(sol, dist, m, g, num_iter);
                        break;
                    case algRecristalizacaoSimulada:
                        // Trepa colinas Recristalização Simulada
                        strcpy(nome_alg, "Recristalizacao simulada");
                        custo = tc_simulated_annealing(sol, dist, m, g, num_iter);
                        break;
                    case algTabu:
                        // Tabu
                        strcpy(nome_alg, "Tabu");
                        custo = tabu_Search(sol, dist, m, g, num_iter, parameters.numTabuDescidas, 1);
                        break;
                    default:
                        exit(0);
                }
                
                // Escreve resultados da repeticao k
                printf("\nRepeticao %d:", k);
                escreve_sol(sol, m, g);
                printf("Custo final: %2d\n", custo);
                
                mbf += custo;
                if(k == 0 || custo_best < custo)
                {
                    custo_best = custo;
                    copia(best, sol, m);
                }
            } 
            
            // Escreve resultados globais
            printf("\n\nMBF: %f\n", mbf/k);
            printf("\nMelhor solucao encontrada");
            escreve_sol(best, m, g);
            printf("Custo final: %2d\n", custo_best);
            // Escreve resultado globais para ficheiro
            write_to_file(nome_alg,nome_fich,best, m, g,custo_best,mbf/k,parameters,num_iter);
            // Libertar memoria
            free(sol);
            free(best);
            break;
            
        case algGeneticoPorTorneio:
            strcpy(nome_alg, "Genetico por torneio");
            
            best_run.sol = calloc(m,sizeof(int));
            best_ever.sol = calloc(m,sizeof(int));
            
            // Repeticoes
            for (k=0; k<runs; k++)
            {
                // Geracao da populacao inicial
                pop = init_pop(parameters, dist);
                
                gen_actual = 1;
                atribuicao(&best_run, pop[0], parameters);

                // Inicializar a melhor solucao encontrada
                get_best(pop, parameters, &best_run);
                
                // Reservar espaco para os pais
                parents = init_parents(parameters);
                
                // Main evolutionary loop
                while (gen_actual <= parameters.numGenerations)
                {
                    // Torneio binario para encontrar os progenitores (ficam armazenados no vector parents)
                    sized_tournament(pop, parameters, parents);
                    
                    // Aplicar operadores geneticos aos pais (os descendentes ficam armazenados no vector pop)
                    genetic_operators(parents, parameters, pop, dist);
                    
                    // Reavaliar a qualidade da populacao
                    evaluate(pop, parameters, dist);
                    
                    // Actualizar a melhor solucao encontrada
                    get_best(pop, parameters, &best_run);
                    
                    gen_actual++;
                }
                
                // Escreve resultados da repeticao que terminou
                printf("\nRepeticao %d:",k);
                escreve_sol(best_run.sol, m, g);
                printf("Custo: %2d\n", best_run.fitness);
                
                mbf += best_run.fitness;
                if (k == 0 || best_ever.fitness < best_run.fitness)
                {
                    atribuicao(&best_ever,best_run,parameters);
                }
                
                // Libertar memoria
                // Populacao
                for (i=0; i<parameters.popsize; i++)
                    free(pop[i].sol);
                free(pop);
                
                // Pais
                for (i=0; i<parameters.popsize; i++)
                    free(parents[i].sol);
                free(parents);
            }
            
            // Escreve resultados globais
            printf("\n\nMBF: %f\n", mbf/k);
            printf("\nMelhor solucao encontrada");
            escreve_sol(best_ever.sol, m, g);
            printf("Custo final: %2d\n", best_ever.fitness);
            
            // Escreve resultado globais para ficheiro
            //write_to_file(nome_alg,nome_fich,best_ever.sol, m, g,best_ever.fitness,mbf/k,parameters,num_iter);
            
            free(best_run.sol);
            free(best_ever.sol);
            break;
    }

    // Libertar memoria
    for (i=0; i<(m-1); i++)
        free(dist[i]);
    free(dist);
    return 0;
}