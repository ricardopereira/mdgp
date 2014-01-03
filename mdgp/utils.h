
int** init_dados(char *nome, int *m, int *g);

void gera_sol_inicial(int *sol, int m, int g);

void escreve_sol(int *sol, int m, int g);

void mostra_matriz(int **dist, int m);

int obter_distancia(int **dist, int m, int a, int b);

void mostra_distancia(int **dist, int m, int a, int b);

void copia(int a[], int b[], int n);

void troca(int *sol, int a, int b);

// Random

void init_rand();

int random_l_h(int min, int max);

float rand_01();


// Computação Evolucinaria

pchrom init_pop(struct info d, int** dist);
pchrom init_parents(struct info d);
void evaluate(pchrom pop, struct info d, int** dist);
chrom get_best(pchrom pop, struct info d, chrom best);

void atribuicao(pchrom a, pchrom b, struct info d);
int flip();