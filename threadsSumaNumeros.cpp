#include <iostream>
#include <thread>
#include <random>
#include <mutex>
#include <algorithm>

std::mutex coutMutex;

void SumRandomNumbers(int id, int& result) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000);

    int sum = 0;
    for (int i = 0; i < 100; ++i) {
        sum += dist(gen);
    }

    result = sum;

    std::scoped_lock lock(coutMutex);
    std::cout << "Thread " << id << " suma: " << sum << '\n';
}

int main() {
    int results[10] = {0};

    // Crear hilos
    std::thread t0(SumRandomNumbers, 0, std::ref(results[0]));
    std::thread t1(SumRandomNumbers, 1, std::ref(results[1]));
    std::thread t2(SumRandomNumbers, 2, std::ref(results[2]));
    std::thread t3(SumRandomNumbers, 3, std::ref(results[3]));
    std::thread t4(SumRandomNumbers, 4, std::ref(results[4]));
    std::thread t5(SumRandomNumbers, 5, std::ref(results[5]));
    std::thread t6(SumRandomNumbers, 6, std::ref(results[6]));
    std::thread t7(SumRandomNumbers, 7, std::ref(results[7]));
    std::thread t8(SumRandomNumbers, 8, std::ref(results[8]));
    std::thread t9(SumRandomNumbers, 9, std::ref(results[9]));

    t0.join(); t1.join(); t2.join(); t3.join(); t4.join();
    t5.join(); t6.join(); t7.join(); t8.join(); t9.join();

    // Buscar el índice con mayor resultado
    auto maxIt = std::ranges::max_element(results);
    int maxIndex = std::distance(std::begin(results), maxIt);

    std::cout << "Thread con puntuación más alta: " << maxIndex
              << " con suma: " << *maxIt << '\n';
    return 0;
}
