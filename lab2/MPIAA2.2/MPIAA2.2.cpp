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
    srand(42);  // Используйте фиксированное значение для генерации случайных чисел

    // Заполнение матриц случайными числами от 0 до 10
    for (int i = 0; i < n * n; i++) {
        A[i] = rand() % 11;
        B[i] = rand() % 11;
    }

    // Измерение времени последовательного вычисления
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

    // Норма матрицы для одного потока
    double norm = 0;
    for (int i = 0; i < n * n; i++) {
        norm += C[i] * C[i];
    }
    cout << "Норма матрицы при использовании 1 потока: " << fixed << sqrt(norm) << endl;

    // Обнуление матрицы C перед следующим расчетом
    fill(C.begin(), C.end(), 0);

    // Измерение времени параллельного вычисления с 2 потоками
    start_time = chrono::high_resolution_clock::now();
    #pragma omp parallel for num_threads(2)  // Параллельное умножение (2 потока)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
    end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_par_2 = end_time - start_time;
    cout << "Время параллельного двухпоточного вычисления: " << duration_par_2.count() << " секунд" << endl;

    // Норма матрицы для 2 потоков
    norm = 0;
    for (int i = 0; i < n * n; i++) {
        norm += C[i] * C[i];
    }
    cout << "Норма матрицы при использовании 2 потоков: " << fixed << sqrt(norm) << endl;

    // Обнуление матрицы C перед следующим расчетом
    fill(C.begin(), C.end(), 0);

    // Измерение времени параллельного вычисления с максимальным числом потоков
    start_time = chrono::high_resolution_clock::now();
    int max_threads = omp_get_max_threads();
    #pragma omp parallel for num_threads(max_threads)  // Параллельное умножение (макс. потоки)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
    end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_par_max = end_time - start_time;
    cout << "Время параллельного " << max_threads << " поточного вычисления: " << fixed << duration_par_max.count() << " секунд" << endl;

    // Норма матрицы для максимального числа потоков
    norm = 0;
    for (int i = 0; i < n * n; i++) {
        norm += C[i] * C[i];
    }
    cout << "Норма матрицы при использовании " << max_threads << " потоков: " << sqrt(norm) << endl;

    // Вычисление ускорения
    cout << "Ускорение от двух потоков: " << duration_seq.count() / duration_par_2.count() << endl;
    cout << "Ускорение от максимума потоков: " << duration_seq.count() / duration_par_max.count() << endl;

    return 0;
}
