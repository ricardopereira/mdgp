// Qualidade da solucao
int calcula_fit(int a[], int **mat, int m, int g);
int calcula_div(int sol[], int **mat, int m, int start, int count);


// Computação Evolucinaria

float compute_dist(int a[], struct info d, float mat[][2]);
void evaluate(pchrom pop, struct info d, float a[][2]);