#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double random_double() {
    return (double)rand() / RAND_MAX;
}

int main(int argc, char** argv) {
    int num_tasks, rank;
    int points_per_task = 10000;
    int points_in_circle = 0;
    int total_points_in_circle;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank);

    for (int i = 0; i < points_per_task; i++) {
        double x = random_double();
        double y = random_double();
        if (x * x + y * y <= 1.0) {
            points_in_circle++;
        }
    }

    MPI_Reduce(&points_in_circle, &total_points_in_circle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi_estimate = 4.0 * total_points_in_circle / (points_per_task * num_tasks);
        printf("Aproximação de Pi: %f\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}