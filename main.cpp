#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <random>
#include <iomanip>
#include <chrono>

class DataArray {
public:
    DataArray();
    DataArray(size_t arraySize);
    void FillArray();
    void PrintArray();
    std::vector<int>& GetData();
private:
    size_t size;
    std::vector<int> data;
};

DataArray::DataArray() : size(0) {}

DataArray::DataArray(size_t arraySize) : size(arraySize) {
    data.resize(size, 0);
}

void DataArray::FillArray() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 100);
    for (size_t i = 0; i < size; ++i) {
        data[i] = dist(gen);
    }
}

void DataArray::PrintArray() {
    const size_t elementsPerLine = 10;
    for (size_t i = 0; i < size; ++i) {
        if (i % elementsPerLine == 0) {
            std::cout << "[" << i << "]: ";
        }
        std::cout << std::setw(4) << data[i];
        if ((i + 1) % elementsPerLine == 0) {
            std::cout << '\n';
        }
    }
    if (size % elementsPerLine != 0) {
        std::cout << '\n';
    }
}

std::vector<int>& DataArray::GetData() {
    return data;
}

size_t FindMinIndex(const std::vector<int>& data, size_t startIndex) {
    size_t minIndex = startIndex;
    for (size_t i = startIndex + 1; i < data.size(); ++i) {
        if (data[i] < data[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}

double SelectSort(std::vector<int>& data) {
    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i + 1 < data.size(); ++i) {
        size_t minIndex = FindMinIndex(data, i);
        std::swap(data[i], data[minIndex]);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

void PrintSortTime(const std::string& sortName, size_t size, double sortTimeUs) {
    std::cout << "\nАлгоритм сортування: " << sortName << '\n';
    std::cout << "Кількість елементів: " << size << '\n';
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Час виконання: " << sortTimeUs << " мкс\n";
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    std::cout << "Введіть кількість елементів масиву:\n";
    size_t n;
    if (!(std::cin >> n)) {
        std::cout << "Помилка введення!\n";
        return 1;
    }
    DataArray arr(n);
    arr.FillArray();
    std::cout << "\nПочатковий масив:\n";
    arr.PrintArray();
    double sortTimeUs = SelectSort(arr.GetData());
    std::cout << "\nВідсортований масив:\n";
    arr.PrintArray();
    PrintSortTime("Сортування вибором (Selection Sort)", n, sortTimeUs);
    return 0;
}