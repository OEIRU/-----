#include <omp.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>

using namespace std;

int main()
{
    int n = 2500; // Размерность матрицы

    // Инициализация матриц
    vector<double> A(n * n), B(n * n), C(n * n, 0);

    // Инициализация генератора случайных чисел для воспроизводимости
    srand(42);

    // Заполнение матриц случайными числами от 0 до 10
    for (int i = 0; i < n * n; i++) {
        A[i] = rand() % 11;
        B[i] = rand() % 11;
    }

    // Последовательное умножение матриц
    auto start_time = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_seq = end_time - start_time;
    cout << "Время последовательного вычисления: " << duration_seq.count() << " секунд" << endl;

    // Норма матрицы для последовательного умножения
    double norm_seq = 0;
    for (int i = 0; i < n * n; i++) {
        norm_seq += C[i] * C[i];
    }
    cout << "Норма матрицы при использовании последовательного умножения: " << sqrt(norm_seq) << endl;

    // Обнуление матрицы C перед параллельным вычислением
    fill(C.begin(), C.end(), 0);

    // Параллельное умножение матриц с 2 потоками
    start_time = chrono::high_resolution_clock::now();
    #pragma omp parallel for collapse(2) num_threads(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
    end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_par_2 = end_time - start_time;
    cout << "Время параллельного вычисления с 2 потоками: " << duration_par_2.count() << " секунд" << endl;

    // Норма матрицы для параллельного умножения с 2 потоками
    double norm_par_2 = 0;
    for (int i = 0; i < n * n; i++) {
        norm_par_2 += C[i] * C[i];
    }
    cout << "Норма матрицы при использовании 2 потоков: " << sqrt(norm_par_2) << endl;

    // Обнуление матрицы C перед параллельным вычислением с максимальным числом потоков
    fill(C.begin(), C.end(), 0);

    // Получение максимального числа потоков
    int max_threads = omp_get_max_threads();

    // Параллельное умножение матриц с максимальным числом потоков
    start_time = chrono::high_resolution_clock::now();
    #pragma omp parallel for collapse(2) num_threads(max_threads)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A[i * n + k] * B[k * n + j];
            }
            C[i * n + j] = sum;
        }
    }
    end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_par_max = end_time - start_time;
    cout << "Время параллельного вычисления с " << max_threads << " потоками: " << duration_par_max.count() << " секунд" << endl;

    // Норма матрицы для параллельного умножения с максимальным числом потоков
    double norm_par_max = 0;
    for (int i = 0; i < n * n; i++) {
        norm_par_max += C[i] * C[i];
    }
    cout << "Норма матрицы при использовании " << max_threads << " потоков: " << sqrt(norm_par_max) << endl;

    // Проверка корректности путем сравнения норм
    if (abs(sqrt(norm_seq) - sqrt(norm_par_2)) < 1e-6 && abs(sqrt(norm_seq) - sqrt(norm_par_max)) < 1e-6) {
        cout << "Параллельные вычисления корректны." << endl;
    } else {
        cout << "Ошибка в параллельных вычислениях." << endl;
    }

    // Вычисление ускорения
    cout << "Ускорение с 2 потоками: " << duration_seq.count() / duration_par_2.count() << endl;
    cout << "Ускорение с " << max_threads << " потоками: " << duration_seq.count() / duration_par_max.count() << endl;

    return 0;
}