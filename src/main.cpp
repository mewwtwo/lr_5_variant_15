// Dmytro Demchuk K-25

#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <mutex>
#include <thread>

using Clock = std::chrono::steady_clock;

std::mutex printMutex;

void work(const std::string& name, int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << "Completed " << name << " (" << seconds << " s)\n";
}

void slow(const std::string& name) {
    work(name, 7);
}

void quick(const std::string& name) {
    work(name, 1);
}

int main() {
    const auto start = Clock::now();

    auto a1 = std::async(std::launch::async, slow, "A1");
    auto a2 = std::async(std::launch::async, slow, "A2");

    quick("A3");
    quick("A4");
    quick("C");

    a1.get();
    a2.get();
    quick("B");
    quick("D");

    const auto end = Clock::now();
    const std::chrono::duration<double> elapsed = end - start;
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << "Time: " << elapsed.count() << " s\nWork is done!\n";
}
