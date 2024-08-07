#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int rank, num_tasks;
    int n = 100;
    int sub_size;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    sub_size = n / num_tasks;

    int *sub_A = (int *)malloc(sub_size * sizeof(int));
    int *sub_B = (int *)malloc(sub_size * sizeof(int));
    int *sub_C = (int *)malloc(sub_size * sizeof(int));

    if (rank == 0) {
        int *temp_A = (int *)malloc(n * sizeof(int));
        int *temp_B = (int *)malloc(n * sizeof(int));
        srand(time(NULL));

        for (int i = 0; i < n; i++) {
            temp_A[i] = rand() % 100;
            temp_B[i] = rand() % 100;
        }

        MPI_Scatter(temp_A, sub_size, MPI_INT, sub_A, sub_size, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(temp_B, sub_size, MPI_INT, sub_B, sub_size, MPI_INT, 0, MPI_COMM_WORLD);

        free(temp_A);
        free(temp_B);
    } else {
        MPI_Scatter(NULL, sub_size, MPI_INT, sub_A, sub_size, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, sub_size, MPI_INT, sub_B, sub_size, MPI_INT, 0, MPI_COMM_WORLD);
    }

    for (int i = 0; i < sub_size; i++) {
        sub_C[i] = sub_A[i] + sub_B[i];
    }

    int *recv_C = NULL;
    if (rank == 0) {
        recv_C = (int *)malloc(n * sizeof(int));
    }
    MPI_Gather(sub_C, sub_size, MPI_INT, recv_C, sub_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Vetor C (resultado da soma de A e B): ");
        for (int i = 0; i < n; i++) {
            printf("%d ", recv_C[i]);
        }
        printf("\n");

        free(recv_C);
    }

    free(sub_A);
    free(sub_B);
    free(sub_C);

    MPI_Finalize();
    return 0;
}