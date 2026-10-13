#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <random>
#include <iomanip>
#include <algorithm>
#include <chrono>

class DataArray {
public:
    DataArray();
    DataArray(size_t arraySize);
    DataArray(const std::vector<int>& arrayData);
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

DataArray::DataArray(const std::vector<int>& arrayData)
    : size(arrayData.size()), data(arrayData) {}

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

//Пошук мінімального елемнта в масиві з StartIndex
size_t FindMinIndex(const std::vector<int>& data, size_t startIndex) {
    size_t minIndex = startIndex;
    for (size_t i = startIndex + 1; i < data.size(); ++i) {
        if (data[i] < data[minIndex]) {
            minIndex = i;
        }
    }
    return minIndex;
}

//Сортування вибором
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

//Двошляхове злиття
double TwoWayMerge(const std::vector<int>& first, const std::vector<int>& second, std::vector<int>& result) {
    auto start = std::chrono::steady_clock::now();
    result.resize(first.size() + second.size());
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    while (i < first.size() && j < second.size()) {
        if (first[i] <= second[j]) {
            result[k++] = first[i++];
        } else {
            result[k++] = second[j++];
        }
    }
    while (i < first.size()) {
        result[k++] = first[i++];
    }
    while (j < second.size()) {
        result[k++] = second[j++];
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

// Розбиття масиву відносно опорного елемента
int Partition(std::vector<int>& data, int left, int right, int pivotIndex) {
    int pivot = data[pivotIndex];
    int i = left;
    int j = right;
    while (i <= j) {
        while (data[i] < pivot) {
            ++i;
        }
        while (data[j] > pivot) {
            --j;
        }
        if (i <= j) {
            std::swap(data[i], data[j]);
            ++i;
            --j;
        }
    }
    return j;
}

// Рекурсивне сортування швидким методом
void QuickSortRecursive(std::vector<int>& data, int left, int right) {
    if (left >= right) {
        return;
    }
    int splitIndex = Partition(data, left, right, right);
    QuickSortRecursive(data, left, splitIndex);
    QuickSortRecursive(data, splitIndex + 1, right);
}

// Швидке сортування з вибором першого опорного елемента
double QuickSort(std::vector<int>& data, int firstPivotIndex) {
    auto start = std::chrono::steady_clock::now();
    if (!data.empty()) {
        int right = static_cast<int>(data.size()) - 1;
        int splitIndex = Partition(data, 0, right, firstPivotIndex);

        QuickSortRecursive(data, 0, splitIndex);
        QuickSortRecursive(data, splitIndex + 1, right);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

int main() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    /*
    // Сортування вибором
    size_t n;
    std::cout << "Введіть кількість елементів масиву для сортування вибором:\n";
    std::cin >> n;
    DataArray selectionArray(n);
    selectionArray.FillArray();
    std::cout << "\nПочатковий масив для сортування вибором:\n";
    selectionArray.PrintArray();
    double selectionTimeUs = SelectSort(selectionArray.GetData());
    std::cout << "\nРезультат сортування вибором:\n";
    selectionArray.PrintArray();
    PrintSortTime("Сортування вибором (Selection Sort)", n, selectionTimeUs);

    // Двошляхове злиття
    size_t n1;
    size_t n2;
    std::cout << "\nВведіть кількість елементів першого масиву для двошляхового злиття:\n";
    std::cin >> n1;
    std::cout << "Введіть кількість елементів другого масиву для двошляхового злиття:\n";
    std::cin >> n2;
    DataArray first(n1);
    DataArray second(n2);
    first.FillArray();
    second.FillArray();

    // Сортування масивів перед злиттям
    std::sort(first.GetData().begin(), first.GetData().end());
    std::sort(second.GetData().begin(), second.GetData().end());
    std::cout << "\nПерший відсортований масив:\n";
    first.PrintArray();
    std::cout << "\nДругий відсортований масив:\n";
    second.PrintArray();

    //Двошляхове злиття
    std::vector<int> result;
    double mergeTimeUs = TwoWayMerge(first.GetData(), second.GetData(), result);
    DataArray merged(result.size());
    merged.GetData() = result;
    std::cout << "\nРезультат двошляхового злиття:\n";
    merged.PrintArray();
    PrintSortTime("Двошляхове злиття (Two-Way Merge)", result.size(), mergeTimeUs);
    */
    
    // Швидке сортування
    size_t quickSize;
    std::cout << "\nВведіть кількість елементів масиву для швидкого сортування:\n";
    std::cin >> quickSize;
    DataArray quickArray(quickSize);
    quickArray.FillArray();
    std::cout << "\nПочатковий масив для швидкого сортування:\n";
    quickArray.PrintArray();

    int firstPivotIndex = 0;
    if (quickSize > 1) {
        std::cout << "Введіть індекс першого опорного елемента (0-" << quickSize - 1 << "):\n";
        std::cin >> firstPivotIndex;
        while (firstPivotIndex < 0 || static_cast<size_t>(firstPivotIndex) >= quickSize) {
            std::cout << "Невірний індекс. Введіть число від 0 до " << quickSize - 1 << ":\n";
            std::cin >> firstPivotIndex;
        }
    }

    double quickTimeUs = QuickSort(quickArray.GetData(), firstPivotIndex);
    std::cout << "\nРезультат швидкого сортування:\n";
    quickArray.PrintArray();
    PrintSortTime("Швидке сортування (Quick Sort)", quickSize, quickTimeUs);
    return 0;
}