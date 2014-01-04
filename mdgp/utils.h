
int** init_dados(char *nome, int *m, int *g);

void gera_sol_inicial(int *sol, int m, int g);

void escreve_sol(int *sol, int m, int g);

void escreve_vect(int *sol, int m);

void mostra_matriz(int **dist, int m);

int obter_distancia(int **dist, int m, int a, int b);

void mostra_distancia(int **dist, int m, int a, int b);

void copia(int a[], int b[], int n);

void troca(int *sol, int a, int b);

int igual(int a[], int b[], int m);

// Random

void init_rand();

int random_l_h(int min, int max);

float rand_01();


// Computação Evolucinaria

pchrom init_pop(struct info d, int** dist);
pchrom init_parents(struct info d);
void evaluate(pchrom pop, struct info d, int** dist);
void get_best(pchrom pop, struct info d, pchrom best);

void atribuicao(pchrom a, chrom b, struct info d);
int flip();

//files
int write_to_file(char const *nomealg,char *nomefich,int *sol, int m, int g,int custo,float mbf,struct info param, int numiter,int flagenum,double elapsed);