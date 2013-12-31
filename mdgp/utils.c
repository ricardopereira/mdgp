#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"


// Leitura do ficheiro de input
// Recebe: nome do ficheiro, numero de vertices (ptr), numero de iteracoes (ptr)
// Devolve a matriz de adjacencias
int** init_dados(char *nome, int *m, int *g, int *iter)
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
	
	// Numero de iteracoes
	fscanf(f, " %d", m);
	
	// Numero de vertices
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


// copia vector b para a (tamanho n)
void substitui(int a[], int b[], int n)
{
    int i;
    for(i=0; i<n; i++)
        a[i]=b[i];
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


