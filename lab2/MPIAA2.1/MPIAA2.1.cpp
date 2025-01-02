#include <omp.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <ctime>
#include <vector>
using namespace std;

double sequential_dot_product(const vector<double>& x, const vector<double>& y) {
    double result = 0.0;
    for (size_t i = 0; i < x.size(); i++) {
        result += x[i] * y[i];
    }
    return result;
}

double parallel_dot_product(const vector<double>& x, const vector<double>& y, int num_threads) {
    double result = 0.0;

    // Параллельное вычисление с использованием OpenMP с заданным количеством потоков
    #pragma omp parallel for num_threads(num_threads) reduction(+:result)
    for (size_t i = 0; i < x.size(); i++) {
        result += x[i] * y[i];
    }
    
    return result;
}

int main() {
    // Тестовые данные для разных n
    vector<int> test_sizes = {10, 1000, 100000, 1000000, 100000000};

    // Стартуем тестирование
    for (int n : test_sizes) {
        // Инициализация векторов случайными числами
        vector<double> x(n), y(n);
        srand(42);  // Для воспроизводимости

        for (int i = 0; i < n; i++) {
            x[i] = rand() / (RAND_MAX + 1.0);  // Случайные числа от 0 до 1
            y[i] = rand() / (RAND_MAX + 1.0);  // Случайные числа от 0 до 1
        }

        // Последовательное вычисление
        auto start = chrono::high_resolution_clock::now();
        double sequential_result = sequential_dot_product(x, y);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration_seq = end - start;

        // Параллельное вычисление на 2 потоках
        start = chrono::high_resolution_clock::now();
        double parallel_result_2 = parallel_dot_product(x, y, 2);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration_par_2 = end - start;

        // Параллельное вычисление на максимальном числе потоков
        start = chrono::high_resolution_clock::now();
        int max_threads = omp_get_max_threads();
        double parallel_result_max = parallel_dot_product(x, y, max_threads);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration_par_max = end - start;

        // Ускорение
        double speedup_2 = duration_seq.count() / duration_par_2.count();
        double speedup_max = duration_seq.count() / duration_par_max.count();

        // Вывод результатов
        cout << "Размер вектора: " << n << "\n";
        cout << "Результат последовательного вычисления: " << sequential_result << "\n";
        cout << "Результат параллельного вычисления (2 потока): " << parallel_result_2 << "\n";
        cout << "Результат параллельного вычисления (макс. потоки): " << parallel_result_max << "\n";
        cout << "Время работы последовательной программы: " << duration_seq.count() << " секунд\n";
        cout << "Время работы параллельной программы (2 потока): " << duration_par_2.count() << " секунд\n";
        cout << "Время работы параллельной программы (макс. потоки): " << duration_par_max.count() << " секунд\n";
        cout << "Ускорение (2 потока): " << speedup_2 << "\n";
        cout << "Ускорение (макс. потоки): " << speedup_max << "\n\n";
    }

    return 0;
}
