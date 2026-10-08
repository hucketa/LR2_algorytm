#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <random>

class DataArray {
public:
    DataArray();
    DataArray(size_t arraySize);
    void FillArray();
    void PrintArray();
private:
    size_t size;
    std::vector<int> data; 
};

DataArray::DataArray() : size(0) {}

DataArray::DataArray(size_t arraySize) : size(arraySize) {
    for (size_t i = 0; i < size; ++i) {
        data.push_back(0);
    }
}

void DataArray::FillArray() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(1.0, 100.0);
    for (size_t i = 0; i < size; ++i) {
        data[i] =static_cast<int>(dist(gen));
    }
}

#include <iomanip>

void DataArray::PrintArray() {
    const size_t elementsPerLine = 10;
    for (size_t i = 0; i < size; ++i) {
        if (i % elementsPerLine == 0) { std::cout << "[" << i << "]: "; }
        std::cout << std::setw(4) << data[i];
        if ((i + 1) % elementsPerLine == 0) { std::cout << '\n'; }
    }
    if (size % elementsPerLine != 0) {
        std::cout << '\n';
    }
}

int main(){ 
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    std::cout << "Введіть кількість елементів масиву: " << std::endl;
    size_t n;
    std::cin >> n;
    DataArray arr(n);
    arr.FillArray();
    arr.PrintArray();
    return 0;
}