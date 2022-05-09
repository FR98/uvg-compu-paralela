// Quicksort
// Francisco Rosal 18676
// gcc lab2-openmp.cpp -o lab2-openmp -lstdc++ -lm -fopenmp && ./lab2-openmp
// gcc-11 lab2-openmp.cpp -o lab2-openmp -lstdc++ -lm -fopenmp && ./lab2-openmp
// clang -Xpreprocessor -fopenmp -lstdc++ -lm -lomp lab2-openmp.cpp -o lab2-openmp && ./lab2-openmp

#include <cmath>
#include <string>
#include <fstream>
#include <unistd.h>
#include <iostream>
#include <omp.h>

using namespace std;

int compare(const int *a, const int *b);

void par_qsort(int *data, int lo, int hi);

int main(int argc, char *argv[]) {

    int cantidad;
    ifstream leer;
    ofstream escribir;

    printf("Ingrese la cantidad de numeros: ");
    scanf("%d", &cantidad);

    int *lista_nums = new int[cantidad];

    escribir.open("input.csv");

    for (int i = 0; i < cantidad; i++) {
        escribir << rand() % (cantidad * 10) + 1;
        if (i != cantidad - 1) { escribir << ","; };
    };

    escribir.close();


    leer.open("input.csv");
    escribir.open("output.csv");

    string string_num;
    int e = 0;

    #pragma omp parallel num_threads(12) firstprivate(e)
    while (getline(leer, string_num, ',')) {
        int num = stoi(string_num);
        lista_nums[e] = num;
        e++;
    };

    double t_init = omp_get_wtime();

    // #pragma omp parallel num_threads(12)
	// {
    //     #pragma omp single nowait
    //     {
    //     }
    // }
    par_qsort(lista_nums, 0, cantidad - 1);
    // par_qsort(lista_nums, 0, cantidad - 1);

    double t_fin = omp_get_wtime();
	double delta = t_fin - t_init;
	printf("Tiempo: %lf ", delta);

    #pragma omp for schedule(static , 1000)
    for (int i = 0; i < cantidad; i++) {
        escribir << lista_nums[i];
        if (i != cantidad - 1) { escribir << ","; };
    };

    cout << endl;

    printf("Primeros: %d, %d, %d\n", lista_nums[0], lista_nums[1], lista_nums[2]);
    printf("Medios: %d, %d, %d\n", lista_nums[cantidad / 4], lista_nums[cantidad / 4 + 1], lista_nums[cantidad / 4 + 2]);
    printf("Ultimos: %d, %d, %d\n", lista_nums[cantidad - 3], lista_nums[cantidad - 2], lista_nums[cantidad - 1]);

    return 0;
};

int compare(const int *a, const int *b) {
    return (*(int*)a - *(int*)b);
};

void par_qsort(int *data, int lo, int hi) {
    if (lo > hi) return;

    int tmp;
    int l = lo;
    int h = hi;
    int p = data[(h + l) / 2];

    while (l <= h) {
        // #pragma omp for schedule(static , 1000)
        while ((data[l] - p) < 0) { l++; };

        // #pragma omp for schedule(static , 1000)
        while ((data[h] - p) > 0) { h--; };

        if (l <= h) {
            tmp = data[l];
            data[l] = data[h];
            data[h] = tmp;
            h--;
            l++;
        };
    };

    #pragma omp task
    par_qsort(data, lo, h);
    #pragma omp task
    par_qsort(data, l, hi);
};
