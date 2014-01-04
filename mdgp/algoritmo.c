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

// Tabu
// Parametros: solucao, matriz de adjacencias, numero de vertices, numero de iteracoes e numero de descidas tabu
// Devolve o custo da melhor solucao encontrada
int tabu_Search(int sol[], int **mat, int m, int g, int num_iter, int num_Tabu_desc, int flagChangeIter)
{
    int *nova_sol, custo, custo_viz, custoGlobal=0,tabidx=0, i;
    int inc=1;
    
	nova_sol = malloc(sizeof(int)*m);
    if(nova_sol == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    
	// Avalia solucao inicial
    custo = calcula_fit(sol, mat, m, g);
    
    for (i=0; i<num_iter;)
    {
		// Gera vizinho
		gera_vizinho(sol, nova_sol, m);
        
		// Avalia vizinho
		custo_viz = calcula_fit(nova_sol, mat, m, g);
        
		// Aceita vizinho se o custo aumentar (problema de maximizacao)
        if (custo_viz > custo)
        {
            tabidx= 0;
			custo = custo_viz;
            if(custo>custoGlobal)
            {
                custoGlobal = custo_viz;
                copia(sol, nova_sol, m);
            }
            inc=1;
        }
        else
        {
            if (num_Tabu_desc>tabidx)
            {
                if (flagChangeIter == 0)
                    inc =1;
                else
                    inc =0;
                
                custo = custo_viz;
                tabidx++;
            }
            else
                inc = 1;
        }
        if (custoGlobal>custo)
            custo = custoGlobal;
        
        if(inc==1)
            i++;
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
    
    for(i=0; i<num_iter; i++)
    {
        // Gera vizinho
        gera_vizinho(sol, nova_sol, m);
        // Avalia vizinho
        custo_viz = calcula_fit(nova_sol, mat, m, g);
        // Aceita vizinho se o custo diminuir (problema de maximizacao)
        if (custo_viz > custo)
        {
            copia(sol, nova_sol, m);
            custo = custo_viz;
        }
        else
        {
            r = rand_01();
            if(r < PROB)
            {
                copia(sol, nova_sol, m);
                custo = custo_viz;
            }
        }
        if (custo_best < custo)
        {
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

#define TMAX 99
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
        // Calcular probabilidade: maximizacao
        eprob = exp((custo-custo_viz)/temperatura);

        // Aceita vizinho se o custo diminuir (problema de maximizacao)
        if (custo_viz >= custo) //= planicie
        {
            copia(sol, nova_sol, m);
            custo = custo_viz;
        }
        else
        {
            r = rand_01();
            if (r < eprob)
            {
                copia(sol, nova_sol, m);
                custo = custo_viz;
            }
        }
        if (custo_best < custo)
        {
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



// Computação Evolucinaria

// Seleccao por torneio binaria (tamanho de torneio: 2)
// Argumentos: populacao actual, parametros, pais
// A funcao preenche o vector pais de acordo com o resultados dos torneios
void binary_tournament(pchrom pop, struct info d, pchrom parents)
{
	int i, x1, x2;
    
	for(i=0; i<d.popsize; i++)
	{
		x1 = random_l_h(0, d.popsize-1);
		do
			x2 = random_l_h(0, d.popsize-1);
		while (x1==x2);
        
        // Problema de maximizacao
		if ((pop+x1)->fitness < (pop+x2)->fitness)
        {
            atribuicao(parents + i,pop + x2,d);
        }
		else
        {
            atribuicao(parents + i,pop + x1,d);
        }
	}
}


// Seleccao por torneio de tamanho t_size
// Argumentos: populacao actual, parametros, pais
// A funcao preenche o vector pais de acordo com o resultados dos torneios
void sized_tournament(pchrom pop, struct info d, pchrom parents)
{
	int i,j,max;
    
	int* xvect = malloc(sizeof(int) * d.t_size);
	if (!xvect) return;
    
	for (i=0; i<d.popsize; i++)
	{
		for(j=0; j<d.t_size; j++)
		{
			//Deveria se já foi escolhido
			xvect[j] = random_l_h(0, d.popsize-1);
		}
        
		max = xvect[0];
		for (j=1; j<d.t_size; j++)
		{
			// Problema de maximizacao: só sai um pai
			if ((pop+xvect[j])->fitness > (pop+max)->fitness)
				max = xvect[j];
		}
        
        atribuicao(parents + i,pop + max,d);
	}
    
	free(xvect);
}


// Operadores geneticos
// Argumentos: pais, estrutura com parametros, descendentes
void genetic_operators(pchrom parents, struct info d, pchrom offspring)
{
	recombination(parents, d, offspring);
	mutation(d, offspring);
}

// Chama a funcao cx_order que implementa a recombinacao por ordem (com probabilidade pr)
// Argumentos: pais, estrutura com parametros, descendentes
// A funcao preenche o vector descendentes com o resultado das operacoes de crossover
void recombination(pchrom parents, struct info d, pchrom offspring)
{
	int i;
    
	for (i=0; i<d.popsize; i+=2)
	{
		if (rand_01() < d.pr)
		{
            // Recombinar
			cx_order((parents+i)->sol, (parents+i+1)->sol, (offspring+i)->sol, (offspring+i+1)->sol, d);
		}
		else
		{
            // Sem recombinacao
            atribuicao(offspring + i,parents + i,d);
            atribuicao(offspring + i +1,parents + i +1,d);
		}
		(offspring+i)->fitness = (offspring+i+1)->fitness = 0;
	}
}

// Recombinacao por ordem
// Argumentos: pai1, pai2, descendente1, descendente2, estrutura com parametros
void cx_order(int p1[], int p2[], int d1[], int d2[], struct info d)
{
    int i, aceites;
	int *tab1, *tab2, *conj;
    double prob = 0.5;
    double r;
    
    tab1 = (int*)calloc(d.m,sizeof(int));
    if (!tab1)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    
    tab2 = (int*)calloc(d.m,sizeof(int));
    if (!tab2)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    
    conj = (int*)calloc(d.g,sizeof(int));
    if (!conj)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }
    
    //Teste
    //escreve_vect(p1, d.m);
    //escreve_vect(p2, d.m);
    
    // Primeiro descendente
    i = 0;
    aceites = 0;
    while (1)
    {
        // Verificar
        if (aceites >= d.m)
            break;
        
        // Ultimo
        if (i >= d.m)
        {
            i = 0;
            continue;
        }
        
        r = rand_01();
        if (r < prob)
        {
            // Verifica se ja ultrapassa do limite
            if (conj[p1[i]] >= (d.m/d.g) || tab1[i] == 1)
            {
                if (conj[p2[i]] >= (d.m/d.g) || tab2[i] == 1)
                {
                    // Ultimo: Proteccao
                    if (i == d.m-1)
                        i = 0;
                    else
                        i++;
                    continue;
                }
                else
                {
                    d1[aceites] = p2[i];
                    tab2[i] = 1;
                }
            }
            else
            {
                d1[aceites] = p1[i];
                tab1[i] = 1;
            }
        }
        else
        {
            // Verifica se ja ultrapassa do limite
            if (conj[p2[i]] >= (d.m/d.g) || tab2[i] == 1)
            {
                if (conj[p1[i]] >= (d.m/d.g) || tab1[i] == 1)
                {
                    // Ultimo: Proteccao
                    if (i == d.m-1)
                        i = 0;
                    else
                        i++;
                    continue;
                }
                else
                {
                    d1[aceites] = p1[i];
                    tab1[i] = 1;
                }
            }
            else
            {
                d1[aceites] = p2[i];
                tab2[i] = 1;
            }
        }
        
        conj[d1[i]]++;
        
        //Teste
        //printf("\nMemory");
        //escreve_vect(conj, d.g);
        //fflush(0);
        
        aceites++;
        i++;
    }
    
    //Teste
    //printf("\n\ndescendente1");
    //escreve_vect(d1, d.m);
    //escreve_sol(d1, d.m, d.g);
    
    //Teste
    //printf("\nTabs");
    //escreve_vect(tab1, d.m);
    //escreve_vect(tab2, d.m);
    //fflush(0);
    
    // Segundo descendente
    i = 0;
    aceites = 0;
    while (i<d.m)
    {
        // Proteccao
        if (aceites >= d.m)
            break;
        
        // Restantes para o descendente 2
        if (tab1[i] == 0)
        {
            d2[aceites++] = p1[i];
        }
        if (tab2[i] == 0)
        {
            d2[aceites++] = p2[i];
        }
        i++;
    }
    
    //Teste
    //printf("\n\ndescendente2");
    //escreve_vect(d2, d.m);
    //escreve_sol(d2, d.m, d.g);
    
    // Liberta memória
    free(conj);
    free(tab1);
    free(tab2);
}

// Chama as funcoes que implementam as operacoes de mutacao (de acordo com as respectivas probabilidades)
// Argumentos: estrutura de parametros e descendentes
// Na versao disponibilizada, apenas a mutacao swap esta implementada
void mutation(struct info d, pchrom offspring)
{
	int i;
    
	for (i=0; i<d.popsize; i++)
	{
		if (rand_01() < d.pm_swap)
			mutation_swap(d, (offspring+i)->sol);
	}
}


// Mutacao swap
// Argumentos: estrutura de parametros e solucao a alterar
void mutation_swap(struct info d, int a[])
{
	int x, y, z;
    
	x=random_l_h(0,	d.m-1);
	do {
		y=random_l_h(0, d.m-1);
    } while (x==y);
    
	z=a[x];
	a[x]=a[y];
	a[y]=z;
}