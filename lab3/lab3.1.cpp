#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <windows.h>

using namespace std;

#define VEC_N 100000
#define RANDOM_STEP 1024

double* generate_vector(int n) {
    double* vec = new double[n];
#pragma omp parallel for
    for (int i = 0; i < n; i++)
        vec[i] = (static_cast<double>(rand()) / RAND_MAX);
    return vec;
}

double vector_norm_parallel(double* vec, int n, int threads_num)
{
    double mult = 0;
#pragma omp parallel for reduction(+:mult)
    for (int i = 0; i < n; ++i)
        mult += vec[i] * vec[i];
    return sqrt(mult);
}

int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size > 1) {
        if (rank == 0)
        {
            double* curNorm = new double[size - 1];
            MPI_Request* req = new MPI_Request[size - 1];
            MPI_Status status;
            int flag = 1;
            bool* check = new bool[size - 1] { false }, allchecked = false;
            for (int i = 1; i < size; ++i)
                MPI_Irecv(curNorm + i - 1, 1, MPI_DOUBLE, i, i, MPI_COMM_WORLD, req + i - 1);

            while (!allchecked)
            {
                allchecked = true;
                for (int i = 1; i < size; ++i)
                {
                    MPI_Test(req + i - 1, &flag, &status);
                    if (flag && !check[i - 1])
                    {
                        cout << "Process: " << i << " Vector norm: " << curNorm[i - 1] << endl;
                        check[i - 1] = true;
                    }
                    if (!check[i - 1]) allchecked = false;
                }
            }
            delete[] curNorm;
            delete[] req;
            delete[] check;
        }
        else
        {
            if (rank == 1) Sleep(10000);
            unsigned int seed = time(NULL) + rank * RANDOM_STEP;
            srand(seed);
            double* vec = generate_vector(VEC_N);
            double norm = vector_norm_parallel(vec, VEC_N, omp_get_max_threads());

            MPI_Send(&norm, 1, MPI_DOUBLE, 0, rank, MPI_COMM_WORLD);
            delete[] vec;
        }
    }

    MPI_Finalize();
    return 0;
}