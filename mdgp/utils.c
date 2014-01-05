#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "algoritmo.h"
#include "utils.h"
#include "funcao.h"

// Leitura do ficheiro de input
// Recebe: nome do ficheiro, numero de vertices (ptr), numero de iteracoes (ptr)
// Devolve a matriz de adjacencias
int** init_dados(char *nome, int *m, int *g)
{
	FILE *f;
	int **dist;
	int i, j, idx, lines;

	f=fopen(nome,"r");
	if (!f)
	{
		printf("Erro no acesso ao ficheiro dos dados\n");
		exit(1);
	}
	
	// Numero de elementos
	fscanf(f, " %d", m);
	
	// Numero de sub-conjuntos
	fscanf(f, " %d", g);
    
    // Linhas
    dist = (int**)malloc(sizeof(int*) * (*m-1));
    if (!dist)
    {
	    printf("Erro na alocacao de memoria\n");
	    exit(1);
    }
    
    for (i=0; i<(*m-1); i++)
    {
        // Colunas de cada linha
        dist[i] = (int*)calloc(*m,sizeof(int));
        if (!dist[i])
        {
            printf("Erro na alocacao de memoria para linha %d\n",i);
            exit(1);
        }
    }
    
    //Número de linhas
    lines=(*m * (*m-1)/2);
    // Preencher matriz
    for (idx=0; idx<lines; idx++)
    {
        //Lado A
        fscanf(f, " %d", &i);
        //Lado B
        fscanf(f, " %d", &j);
        //Valor Distancia
        fscanf(f, " %d", &dist[i][j]);
    }

	fclose(f);
	return dist;
}


// Mostra a matriz das distancias
void mostra_matriz(int **dist, int m)
{
    int i, j;
    printf("\nMostra distancias:\n");
    for (i=0; i<(m-1); i++)
        for (j=0; j<m; j++)
            if (dist[i][j])
            {
                printf("%d\n",dist[i][j]);
            }
}


// Obter o valor da distancia entre dois pontos
int obter_distancia(int **dist, int m, int a, int b)
{
    int i,j;
    if (a > b)
    {
        i = b;
        j = a;
    }
    else
    {
        i = a;
        j = b;
    }
    return dist[i][j];
}


// Mostra a distancia entre dois pontos
void mostra_distancia(int **dist, int m, int a, int b)
{
    printf("%d\n",obter_distancia(dist,m,a,b));
}


// Gera a solucao inicial
// Parametros: solucao, m = nr de elementos, g = nr de sub-conjuntos
void gera_sol_inicial(int *sol, int m, int g)
{
	int i, x, subc;

	// Primeiro sub-conjunto: 0
	for (i=0; i<m; i++)
        sol[i]=0;
    
    // Restante sub-conjuntos: 1,2,3... g
    for (subc=1; subc<g; subc++)
        for (i=0; i<m/g; i++)
        {
            do
                x = random_l_h(0, m-1);
            while (sol[x] != 0);
            sol[x]=subc;
        }
}


// Escreve solucao
// Parametros: solucao, nr de elementos, nr de sub-conjuntos
void escreve_sol(int *sol, int m, int g)
{
	int i, subc;
    
    for (subc=0; subc<g; subc++)
    {
        printf("\nConjunto %d: ",subc+1);
        for(i=0; i<m; i++)
            if(sol[i] == subc)
                printf("%2d  ", i);
    }
    printf("\n");
}


// Escreve o vector
// Parametros: solucao
void escreve_vect(int *sol, int m)
{
	int i;
    printf("\nVector: ");
    for(i=0; i<m; i++)
        printf("%2d ", sol[i]);
}


// Copia vector b para a (tamanho n)
void copia(int a[], int b[], int n)
{
    int i;
    for(i=0; i<n; i++)
        a[i]=b[i];
}


// Troca de
void troca(int *sol, int a, int b)
{
    int aux;
    aux = sol[b];
    sol[b] = sol[a];
    sol[a] = aux;
}


// Inicializa o gerador de numeros aleatorios
void init_rand()
{
	srand((unsigned)time(NULL));
}


// Devolve valor inteiro aleatorio entre min e max
int random_l_h(int min, int max)
{
	return min + rand() % (max-min+1);
}


// Devolve um valor real aleatorio do intervalo [0, 1]  
float rand_01()
{
	return ((float)rand())/RAND_MAX;
}



// Computação Evolucinaria

void swap(int *a, int *b)
{
    int x;
    
    x=*a;
    *a=*b;
    *b=x;
}

int igual(int a[], int b[], int m)
{
    int i;
    for (i=0; i<m; i++)
        if (a[i] != b[i])
            return 0;
    return 1;
}

// Criacao da populacao inicial. O vector e alocado dinamicamente
// Argumento: Estrutura com parametros, Matriz de Distancias
// Devolve o vector com a populacao
pchrom init_pop(struct info d, int** dist)
{
	int i;
    
    // Linhas
    pchrom p = malloc(sizeof(chrom) * d.popsize);
    if (!p)
    {
	    printf("Erro na alocacao de memoria\n");
	    exit(1);
    }
    
    for (i=0; i<d.popsize; i++)
    {
        // Colunas de cada linha
        p[i].sol = (int*)calloc(d.m,sizeof(int));
        if (!p[i].sol)
        {
            printf("Erro na alocacao de memoria para linha %d\n",i);
            exit(1);
        }
        
        gera_sol_inicial(p[i].sol, d.m, d.g);
        
        p[i].fitness = calcula_fit(p[i].sol, dist, d.m, d.g);
    }
        
	return p;
}

// Criacao dos pais. O vector e alocado dinamicamente
// Argumento: Estrutura com parametros
// Devolve o vector com para os pais
pchrom init_parents(struct info d)
{
    int i;
    pchrom parents = malloc(sizeof(chrom) * d.popsize);
    
    if (!parents)
    {
        printf("Erro na alocacao de memoria\n");
        exit(1);
    }
    for (i=0; i<d.popsize; i++)
    {
        // Colunas de cada linha
        parents[i].sol = (int*)calloc(d.m,sizeof(int));
        if (!parents[i].sol)
        {
            printf("Erro na alocacao de memoria para linha %d\n",i);
            exit(1);
        }
        // Inicializa
        parents[i].fitness = 0;
    }
    return parents;
}

// Avaliar cada solucao da populacao.
// Argumento: Populacao, Estrutura com parametros, Matriz de Distancias
void evaluate(pchrom pop, struct info d, int** dist)
{
	int i;
    for (i=0; i<d.popsize; i++)
        pop[i].fitness = calcula_fit(pop[i].sol, dist, d.m, d.g);
}

// Actualiza a melhor solucao encontrada
// Argumentos: populacao actual, estrutura com parametros e melhor solucao encontrada ate a geracao imediatamente anterior
// Devolve a melhor solucao encontrada ate a geracao actual
void get_best(pchrom pop, struct info d, pchrom best)
{
	int i;
	for (i=0; i<d.popsize; i++)
	{
		if(best->fitness < pop[i].fitness)
			atribuicao(best,pop[i],d);
	}
}

// Igualar uma solucao de uma populacao
void atribuicao(pchrom a, chrom b, struct info d)
{
    copia(a->sol, b.sol, d.m);
    a->fitness = b.fitness;
}

// Simula o lancamento de uma moeda
int flip()
{
	if ((((float)rand()) / RAND_MAX) < 0.5)
		return 0;
	
	else
		return 1;
}

int write_to_file(char const *nome_alg,char *nomefich, int *sol, int m, int g,int custo,float mbf,struct info param, int numiter,int flagenum, double elapsed,int flagWriteAlg)
{
    char filename[100];
    int i, subc;
    
    FILE *f;
    strcpy(filename,nome_alg);
    strcat(filename,".csv");
    f= fopen(filename, "a");
    
    
    if (f == NULL) return -1;
    if((numiter==10)||(numiter=2000)||(numiter==1000))
    {
        if (flagWriteAlg==1)
        {
            fprintf(f,"Algoritmo:;%s", nome_alg);
        }
        fprintf(f,"\nFicheiro:;%s\n", nomefich);
    }
    
    //fprintf(f,"\nParametros\n");
    if (flagenum==1||flagenum==0)
    {
        fprintf(f,"num iter:;%d\n", numiter);
    //    fprintf(f,"num descidas TABU:;%d\n",param.numTabuDescidas);
    }
    if (flagenum==2)
    {
        fprintf(f,"num generations:;%d\n",param.numGenerations);
        fprintf(f,"pop size:;%d\n",param.popsize);
        fprintf(f,"pm swap:;%f\n",param.pm_swap);
        fprintf(f,"pr:;%f\n",param.pr);
        fprintf(f,"t size:;%d\n\n",param.t_size);
    }
    
    fprintf(f, "MBF:;%f",mbf);
    
    /*for (subc=0; subc<g; subc++)
    {
        fprintf(f,"\nConjunto %d: ",subc+1);
        for(i=0; i<m; i++)
            if(sol[i] == subc)
                fprintf(f,";%2d  ", i);
    }*/
    fprintf(f,"\n");
    
    fprintf(f, "Custo final:;%d\n", custo);
    
    fprintf(f, "Segundos decorridos:;%f\n", elapsed);
    fclose(f);
    return 0;
}