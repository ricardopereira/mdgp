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
		while(x1==x2);
        
		if((pop+x1)->distance < (pop+x2)->distance)		// Problema de minimizacao
			*(parents + i) = *(pop + x1);
		else
			*(parents + i) = *(pop + x2);
	}
}


// Seleccao por torneio binaria (tamanho de torneio: 2)
// Argumentos: populacao actual, parametros, pais
// A funcao preenche o vector pais de acordo com o resultados dos torneios
void sized_tournament(pchrom pop, struct info d, pchrom parents)
{
	int i,j,min;
    
	int* xvect = malloc(sizeof(int)*d.t_size);
	if (!xvect) return;
    
	for(i=0; i<d.popsize; i++)
	{
		for(j=0; j<d.t_size; j++)
		{
			//Deveria se já foi escolhido
			xvect[j] = random_l_h(0, d.popsize-1);
		}
        
		min = xvect[0];
		for(j=1; j<d.t_size; j++)
		{
			// Problema de minimizacao: só sai um pai
			if((pop+xvect[j])->distance < (pop+min)->distance)
				min = xvect[j];
		}
        
		*(parents + i) = *(pop + min);
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
    
	for(i=0; i<d.popsize; i+=2)
	{
		if(rand_01() < d.pr)
		{
			cx_order((parents+i)->chromosome, (parents+i+1)->chromosome, (offspring+i)->chromosome, (offspring+i+1)->chromosome, d);
		}
		else
		{
			*(offspring+i) = *(parents+i);
			*(offspring+i+1) = *(parents+i+1);
		}
		(offspring+i)->distance =  (offspring+i+1)->distance = 0.0;
	}
}


// Recombinacao por ordem
// Argumentos: pai1, pai2, descendente1, descendente2, estrutura com parametros
void cx_order(int p1[], int p2[], int d1[], int d2[], struct info d)
{
	int tab1[MAXPOINTS+1]={0}, tab2[MAXPOINTS+1]={0};
	int point1, point2, index, i;
    
	// seleccao dos pontos de corte
	point1 = random_l_h(0, d.numCities-1);
	do{
		point2 = random_l_h(0, d.numCities-1);
	}while(point1 == point2);
	if(point1 > point2)
	{
		i = point1;
		point1 = point2;
		point2 = i;
	}
    
	//copia das seccoes internas
	for(i = point1; i<=point2; i++)
	{
		d1[i]=p1[i];
		tab1[p1[i]]=1;
		d2[i]=p2[i];
		tab2[p2[i]]=1;
	}
    
	
	// preencher o resto do descendente 1
	index = (point2+1)%d.numCities;
	for(i=point2+1; i<d.numCities; i++)
	{
		if(tab1[p2[i]]==0)
		{
			d1[index]=p2[i];
			index = (index+1)%d.numCities;
		}
	}
	for(i=0; i<=point2; i++)
	{
		if(tab1[p2[i]]==0)
		{
			d1[index]=p2[i];
			index = (index+1)%d.numCities;
		}
	}
    
    
	// preencher o resto do descendente 1
	index = (point2+1)%d.numCities;
	for(i=point2+1; i<d.numCities; i++)
	{
		if(tab2[p1[i]]==0)
		{
			d2[index]=p1[i];
			index = (index+1)%d.numCities;
		}
	}
	for(i=0; i<=point2; i++)
	{
		if(tab2[p1[i]]==0)
		{
			d2[index]=p1[i];
			index = (index+1)%d.numCities;
		}
	}
}

// Chama as funcoes que implementam as operacoes de mutacao (de acordo com as respectivas probabilidades)
// Argumentos: estrutura de parametros e descendentes
// Na versao disponibilizada, apenas a mutacao swap esta implementada
void mutation(struct info d, pchrom offspring)
{
	int i;
    
	for(i=0; i< d.popsize; i++)
	{
		if(rand_01() < d.pm_swap)
			mutation_swap(d, (offspring+i)->chromosome);
        
		// Functions nao implementadas
		if(rand_01() < d.pm_ins)
			mutation_ins(d, (offspring+i)->chromosome);
		if(rand_01() < d.pm_inv)
			mutation_inv(d, (offspring+i)->chromosome);
	}
}


// Mutacao swap
// Argumentos: estrutura de parametros e solucao a alterar
void mutation_swap(struct info d, int a[])
{
	int x, y, z;
    
	x=random_l_h(0,	d.numCities-1);
	do{
		y=random_l_h(0, d.numCities-1);
	}while(x==y);
    
	z=a[x];
	a[x]=a[y];
	a[y]=z;
}


// Insertion mutation
void mutation_ins(struct info d, int a[])
{
	printf("Nao implementada\n");
}

// Inversion mutation
void mutation_inv(struct info d, int a[])
{
	printf("Nao implementada\n");
}