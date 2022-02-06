#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void openmp_hello(void);

int main(int argc, char* argv[]) {
	int thread_count = strtol(argv[1], NULL, 10);

	#pragma omp parallel num_threads(thread_count)
		openmp_hello();

	return 0;
}

void openmp_hello(void) {
	int thread_id = omp_get_thread_num();
	int num_threads = omp_get_num_threads();

	if (thread_id == 0) {
		printf("Cantidad de hilos: %d\n", num_threads);
	}

	if (thread_id % 2 == 0) {
		printf("Saludos del hilo %d\n", thread_id);
	} else {
		printf("Feliz cumpleanos numero %d!\n", thread_id);
	}

}