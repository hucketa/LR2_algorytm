#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <random>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <climits>

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

// 1 в 1 Методичка
int Partition(std::vector<int>& data, int leftindex, int rightindex) {
    int leftScanIndex = leftindex - 1;
    int rightScanIndex = rightindex;
    int pivot = data[rightindex];
    while (true) {
        while (data[++leftScanIndex] < pivot);
        while (pivot < data[--rightScanIndex]) {
            if (rightScanIndex == leftindex) break;
        }
        if (leftScanIndex >= rightScanIndex) break;
        std::swap(data[leftScanIndex], data[rightScanIndex]);
    }
    std::swap(data[leftScanIndex], data[rightindex]);
    return leftScanIndex;
}

void QuickSortRecursive(std::vector<int>& data, int leftindex, int rightindex) {
    if (rightindex <= leftindex) return;
    int pivotIndex = Partition(data, leftindex, rightindex);
    QuickSortRecursive(data, leftindex, pivotIndex - 1);
    QuickSortRecursive(data, pivotIndex + 1, rightindex);
}

double QuickSort(std::vector<int>& data) {
    auto start = std::chrono::steady_clock::now();
    if (!data.empty()) {
        QuickSortRecursive(data, 0, static_cast<int>(data.size()) - 1);
    }
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}


/*
//НАДО ТЕСТИТЬ ПОЧЕМУ ОНО ПРИ КРАЙНЕМ УХОДИТ В БЕКСОНЕЧНУЮ РЕКУРСИЮ И МЕДЛЕННЕЙ
// Функція Partition для алгоритму QuickSort з можливістю вибору першого опорного елемента із середнім 
int Partition(std::vector<int>& data, int leftindex, int rightindex, int pivotIndex) {
    int pivot = data[pivotIndex];
    int leftScanIndex = leftindex - 1;
    int rightScanIndex = rightindex + 1;
    while (true) {
        do {
            ++leftScanIndex;
        } while (data[leftScanIndex] < pivot);
        do {
            --rightScanIndex;
        } while (data[rightScanIndex] > pivot);
        if (leftScanIndex >= rightScanIndex) {
            return rightScanIndex;
        }
        std::swap(data[leftScanIndex], data[rightScanIndex]);
    }
}

void QuickSortRecursive(std::vector<int>& data, int leftindex, int rightindex, int firstPivotIndex, bool& firstPartition) {
    if (leftindex >= rightindex) return;
    int pivotIndex;
    if (firstPartition) {
        pivotIndex = firstPivotIndex;
        firstPartition = false;
    } else {
        pivotIndex = leftindex + (rightindex - leftindex) / 2;
        //pivotIndex = rightindex;
    }
    int splitIndex = Partition(data, leftindex, rightindex, pivotIndex);
    QuickSortRecursive(data, leftindex, splitIndex, firstPivotIndex, firstPartition);
    QuickSortRecursive(data, splitIndex + 1, rightindex, firstPivotIndex, firstPartition);
}

double QuickSort(std::vector<int>& data, int firstPivotIndex) {
    if (data.empty()) return 0.0;
    if (firstPivotIndex < 0 || firstPivotIndex >= static_cast<int>(data.size())) return -1.0;
    auto start = std::chrono::steady_clock::now();
    bool firstPartition = true;
    QuickSortRecursive(data, 0, static_cast<int>(data.size()) - 1, firstPivotIndex, firstPartition);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}
*/

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

    size_t n1;
    std::cout << "\nВведіть кількість елементів першого масиву для двошляхового злиття:\n";
    std::cin >> n1;
    DataArray quickArray(n1);
    quickArray.FillArray();
    std::cout << "Початковий масив:\n";
    quickArray.PrintArray();
    double quickSortTimeUs = QuickSort(quickArray.GetData());
    if (quickSortTimeUs >= 0) 
    {
        std::cout << "\nВідсортований масив:\n";
        quickArray.PrintArray();
        PrintSortTime("Quick Sort", quickArray.GetData().size(), quickSortTimeUs);
    } 
    else 
    {
        std::cout << "Помилка: індекс опорного елемента поза межами масиву!\n";
    }
    std::cout << "\nНатисніть Enter для завершення...";
    std::cin.ignore(INT_MAX, '\n');
    std::cin.get();
    return 0;
}