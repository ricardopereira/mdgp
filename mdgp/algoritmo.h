// Algoritmos locais
int trepa_colinas(int sol[], int **mat, int m, int g, int num_iter);
int tc_prob(int sol[], int **mat, int m, int g, int num_iter);
int tc_simulated_annealing(int sol[], int **mat, int m, int g, int num_iter);
int tabu_Search(int sol[], int **mat, int m, int g, int num_iter, int num_Tabu_desc, int flagChangeIter);


// Computação Evolucinaria

// Estrutura para armazenar parametros
struct info {
    int m;
    int g;
    int popsize;
    float pm_swap;
	float pm_ins;
	float pm_inv;
    float pr;
	int t_size;
    int numGenerations;
    int numTabuDescidas;
};

// Individuo (solucao)
typedef struct individual chrom, *pchrom;

struct individual {
    int* sol; //solucao
	int fitness; // distancia (qualidade)
};

void binary_tournament(pchrom pop, struct info d, pchrom parents);
void sized_tournament(pchrom pop, struct info d, pchrom parents);
void genetic_operators(pchrom parents, struct info d, pchrom offspring);
void recombination(pchrom parents, struct info d, pchrom offspring);
void mutation(struct info d, pchrom offspring);
void cx_order(int p1[], int p2[], int d1[], int d2[], struct info d);
void mutation_swap(struct info d, int a[]);
void mutation_ins(struct info d, int a[]);
void mutation_inv(struct info d, int a[]);