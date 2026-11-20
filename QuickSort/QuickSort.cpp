#include <windows.h>

#include <fstream>
#include <random>
#include <vector>

#include "quicksort.h"

int main() {
    const int tests = 5000;
    const int max_length = 32;
    const int step = 1;

    std::mt19937 gen(12345);
    std::uniform_int_distribution<int> dist(0, 1000000);

    LARGE_INTEGER frequency;
    LARGE_INTEGER start;
    LARGE_INTEGER stop;
    QueryPerformanceFrequency(&frequency);

    std::ofstream out("result.csv");
    out << "n,insertion_ns,quick_ns\n";

    for (int n = 1; n <= max_length; n += step) {
        std::vector<int> source(tests * n);
        for (int& x : source) {
            x = dist(gen);
        }

        std::vector<int> work = source;
        QueryPerformanceCounter(&start);
        for (int t = 0; t < tests; ++t) {
            int* first = work.data() + t * n;
            algo::insertion_sort(first, first + n, [](int a, int b) { return a < b; });
        }
        QueryPerformanceCounter(&stop);
        double insertion_ms = (stop.QuadPart - start.QuadPart) * 1e3 / frequency.QuadPart / tests;

        work = source;
        QueryPerformanceCounter(&start);
        for (int t = 0; t < tests; ++t) {
            int* first = work.data() + t * n;
            algo::quick_sort(first, first + n, [](int a, int b) { return a < b; }, 2);
        }
        QueryPerformanceCounter(&stop);
        double quick_ms = (stop.QuadPart - start.QuadPart) * 1e3 / frequency.QuadPart / tests;

        out << n << ',' << insertion_ms << ',' << quick_ms << '\n';
    }
    return 0;
}