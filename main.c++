#include <iostream>
#include <string>
#include <cmath>
#include <ctime>

using namespace std;

long double count_pi(long long n) {
    long double result = 0;
    for (long long i = 0; i < n; i++) {
        result += 4 * (pow(-1, i) / (2 * i + 1));
    }
    return result;
}

int main() {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    long long n = 820000000; 
    cout.precision(16);
    cout << count_pi(n) << endl;
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("Time taken: %lf sec.\n",
    end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec));
    return 0;
}