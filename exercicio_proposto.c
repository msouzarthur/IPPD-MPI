#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int n = 1000;
    int matrix[n][n];
    int i, j;
    int min_val, max_val;
    int num_negativos = 0, num_positivos = 0, num_zeros = 0;
    float media = 0;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank);

    if (rank == 0) {
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                matrix[i][j] = rand() % 201 - 100;
                media+=matrix[i][j];
            }
        }

        media = media/(n*n);
        min_val = max_val = matrix[0][0];

        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (matrix[i][j] < 0) {
                    num_negativos++;
                } else if (matrix[i][j] > 0) {
                    num_positivos++;
                }

                if(matrix[i][j] == 0) {
                    num_zeros++;
                }

                if (matrix[i][j] < min_val) {
                    min_val = matrix[i][j];
                }
                if (matrix[i][j] > max_val) {
                    max_val = matrix[i][j];
                }
            }
        }

        MPI_Bcast(&num_negativos, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&num_positivos, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&num_zeros, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&min_val, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&max_val, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&media, 1, MPI_INT, 0, MPI_COMM_WORLD);

    } else {
        MPI_Bcast(&num_negativos, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&num_positivos, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&num_zeros, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&min_val, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&max_val, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&media, 1, MPI_INT, 0, MPI_COMM_WORLD);

        printf("Rank %d:\n", rank);
        printf("> valores negativos: %d\n", num_negativos);
        printf("> valores positivos: %d\n", num_positivos);
        printf("> valores zero: %d\n", num_zeros);
        printf("> menor valor: %d\n", min_val);
        printf("> maior valor: %d\n", max_val);
        printf("> media: %f\n", media);

    }

    MPI_Finalize();
    return 0;
}