#include <mpi.h>
#include <iostream>

using namespace std;

void Mat(double* mat, int size)
{
    srand(100);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            mat[i * size + j] = i + j;
}
void Vec(double** vec, int size)
{
    for (int j = 0; j < size; j++)
        (*vec)[j] = j;
}
int main(int argc, char** argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int n;

    if (rank == 0)
    {
        cout << "Vvedite n" << endl;
        cin >> n;
        if (n % size != 0)
        {
            cout << "n ne delitsa na " << size << endl;
            MPI_Finalize();
            return 0;
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    double* A = new double[n * n];
    double* x = new double[n];
    double* y = new double[n];
    if (rank == 0)
    {
        Vec(&x, n);
        Vec(&A, n * n);
    }

    double* Abuf = new double[n * n / size];
    double* ybuf = new double[n / size];

    for (int i = 0; i < n / size; i++)
        ybuf[i] = 0;

    MPI_Bcast(x, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, n * n / size, MPI_DOUBLE, Abuf, n * n / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n / size; i++)
        for (int j = 0; j < n; j++)
            ybuf[i] += Abuf[i * n + j] * x[j];

    MPI_Gather(ybuf, n / size, MPI_DOUBLE, y, n / size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 0; i < n; i++)
            cout << y[i] << "\n";
    }
    MPI_Finalize();
    return 0;
}
